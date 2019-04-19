// This file is part of the AliceVision project.
// Copyright (c) 2017 AliceVision contributors.
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include <aliceVision/sfmData/SfMData.hpp>
#include <aliceVision/sfmData/colorize.hpp>
#include <aliceVision/sfmDataIO/sfmDataIO.hpp>
#include <aliceVision/fuseCut/LargeScale.hpp>
#include <aliceVision/fuseCut/ReconstructionPlan.hpp>
#include <aliceVision/fuseCut/DelaunayGraphCut.hpp>
#include <aliceVision/mesh/meshPostProcessing.hpp>
#include <aliceVision/mvsData/Point3d.hpp>
#include <aliceVision/mvsData/StaticVector.hpp>
#include <aliceVision/mvsUtils/common.hpp>
#include <aliceVision/mvsUtils/MultiViewParams.hpp>
#include <aliceVision/mvsUtils/fileIO.hpp>
#include <aliceVision/system/cmdline.hpp>
#include <aliceVision/system/Logger.hpp>
#include <aliceVision/system/Timer.hpp>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include "stlplus3/file_system.hpp"
#include "glog/logging.h"

// These constants define the current software version.
// They must be updated when the command line is changed.
#define ALICEVISION_SOFTWARE_VERSION_MAJOR 3
#define ALICEVISION_SOFTWARE_VERSION_MINOR 0

using namespace aliceVision;

namespace fs = boost::filesystem;
namespace po = boost::program_options;

enum EPartitioningMode
{
    ePartitioningUndefined = 0,
    ePartitioningSingleBlock = 1,
    ePartitioningAuto = 2,
};

EPartitioningMode EPartitioning_stringToEnum(const std::string& s)
{
    if(s == "singleBlock")
        return ePartitioningSingleBlock;
    if(s == "auto")
        return ePartitioningAuto;
    return ePartitioningUndefined;
}

inline std::istream& operator>>(std::istream& in, EPartitioningMode& out_mode)
{
    std::string s;
    in >> s;
    out_mode = EPartitioning_stringToEnum(s);
    return in;
}

enum ERepartitionMode
{
    eRepartitionUndefined = 0,
    eRepartitionMultiResolution = 1,
    eRepartitionRegularGrid = 2,
	eRepartitionExternal = 3,//外部导入点云已经分块好
};

ERepartitionMode ERepartitionMode_stringToEnum(const std::string& s)
{
    if(s == "multiResolution")
        return eRepartitionMultiResolution;
    if(s == "regularGrid")
        return eRepartitionRegularGrid;
	if (s == "external")
		return eRepartitionExternal;
    return eRepartitionUndefined;
}

inline std::istream& operator>>(std::istream& in, ERepartitionMode& out_mode)
{
    std::string s;
    in >> s;
    out_mode = ERepartitionMode_stringToEnum(s);
    return in;
}

void exportPointCloud(const std::string& path,
                      const mvsUtils::MultiViewParams& mp,
                      const sfmData::SfMData& sfmData,
                      const std::vector<Point3d>& vertices,
                      const StaticVector<StaticVector<int>*> cams)
{
  sfmData::SfMData densePointCloud = sfmData;
  densePointCloud.getLandmarks().clear();
  int outputIndex = 0;

  for(int i = 0; i < vertices.size(); ++i)
  {
    const Point3d& point = vertices.at(i);

    if(!cams[i]->empty())
    {
      const Vec3 pt3D(point.x, point.y, point.z);
      sfmData::Landmark landmark(pt3D, feature::EImageDescriberType::UNKNOWN);
      for(int cam : *(cams[i]))
      {
        const sfmData::View& view = sfmData.getView(mp.getViewId(cam));
        const camera::IntrinsicBase* intrinsicPtr = sfmData.getIntrinsicPtr(view.getIntrinsicId());
        const sfmData::Observation observation(intrinsicPtr->project(sfmData.getPose(view).getTransform(), pt3D, true), UndefinedIndexT); // apply distortion
        landmark.observations[view.getViewId()] = observation;
      }
      densePointCloud.getLandmarks()[outputIndex] = landmark;
      ++outputIndex;
    }
  }
  sfmData::colorizeTracks(densePointCloud);
  sfmDataIO::Save(densePointCloud, path, sfmDataIO::ESfMData::ALL_DENSE);
}

struct PtsPair
{
    std::pair<int, int> LR;
    //QFileInfo fileInfo;
	std::string fileInfo;
};

#pragma pack(1)
struct VPointRGB // visit point with color
{
    union {
        struct
        {
            float x, y, z, w;
        };
        float xyzw[4];
    };
    int idx[2];
    uint8_t rgb[3];
};
struct VPoint // visit point with no color
{
    union {
        struct
        {
            float x, y, z, w;
        };
        float xyzw[4];
    };
    int idx[2];
};

struct XYZ
{
    float x, y, z;
    void makeRGB(float r, float g, float b) {}
    void sumRGB(float& sumR, float& sumG, float& sumB) {}
    void setW(float w) {}
    void setID(int i) {}
    void setVis(const std::set<int>& vis) {}

    std::ostream& write(std::ostream& os) const
    {
        os.write((char*)this, sizeof(XYZ));
        return os;
    }
    std::istream& read(std::istream& is)
    {
        is.read((char*)this, sizeof(XYZ));
        return is;
    }
};

struct XYZRGB
{
    float x, y, z;
    uint8_t r, g, b;
    void makeRGB(float _r, float _g, float _b)
    {
        r = static_cast<uint8_t>(_r);
        g = static_cast<uint8_t>(_g);
        b = static_cast<uint8_t>(_b);
    }
    void sumRGB(float& sumR, float& sumG, float& sumB)
    {
        sumR += r;
        sumG += g;
        sumB += b;
    }
    void setW(float w) {}
    void setID(int i) {}
    void setVis(const std::set<int>& vis) {}

    std::ostream& write(std::ostream& os) const
    {
        os.write((char*)this, sizeof(XYZRGB));
        return os;
    }
    std::istream& read(std::istream& is)
    {
        is.read((char*)this, sizeof(XYZRGB));
        return is;
    }
};

struct XYZW
{
    union {
        struct
        {
            float x, y, z, w;
        };
        float xyzw[4];
    };
    // do nothing,just make template function OK
    void makeRGB(float r, float g, float b) {}
    void sumRGB(float& sumR, float& sumG, float& sumB) {}
    void setW(float _w) { w = _w; }
    void setID(int i) {}
    void setVis(const std::set<int>& vis) {}
};

struct XYZWRGB
{
    union {
        struct
        {
            float x, y, z, w;
        };
        float xyzw[4];
    };
    uint8_t rgb[3];
    void makeRGB(float r, float g, float b)
    {
        rgb[0] = static_cast<uint8_t>(r);
        rgb[1] = static_cast<uint8_t>(g);
        rgb[2] = static_cast<uint8_t>(b);
    }
    void sumRGB(float& sumR, float& sumG, float& sumB)
    {
        sumR += rgb[0];
        sumG += rgb[1];
        sumB += rgb[2];
    }
    void setW(float _w) { w = _w; }
    void setID(int i) {}
    void setVis(const std::set<int>& vis) {}
};

struct XYZWID : public XYZW
{
    int id = 0;
    void setID(int i) { id = i; }
};

struct XYZWRGBID : public XYZWRGB
{
    int id = 0;
    void setID(int i) { id = i; }
};

struct XYZVis : public XYZ
{
    std::vector<int> vis;
    void setVis(const std::set<int>& setvis)
    {
        vis.clear();
        vis.assign(setvis.begin(), setvis.end());
    }

    std::ostream& write(std::ostream& os) const
    {
        os.write((char*)&x, sizeof(float));
        os.write((char*)&y, sizeof(float));
        os.write((char*)&z, sizeof(float));
        int n = vis.size();
        if(n < 0)
        {
            printf("________ERR)R_________\n");
        }
        os.write((char*)&n, sizeof(int));
        if(n > 0)
        {
            os.write((char*)vis.data(), sizeof(int) * n);
        }
        return os;
    }
    std::istream& read(std::istream& is)
    {
        is.read((char*)&x, sizeof(float));
        is.read((char*)&y, sizeof(float));
        is.read((char*)&z, sizeof(float));
        int n = 0;
        is.read((char*)&n, sizeof(int));
        vis.resize(n);
        if(n > 0)
        {
            is.read((char*)vis.data(), sizeof(int) * n);
        }
        return is;
    }
};

struct XYZRGBVis : public XYZRGB
{
    std::vector<int> vis;

    void setVis(const std::set<int>& setvis)
    {
        vis.clear();
        vis.assign(setvis.begin(), setvis.end());
    }

    std::ostream& write(std::ostream& os) const
    {
        os.write((char*)&x, sizeof(float));
        os.write((char*)&y, sizeof(float));
        os.write((char*)&z, sizeof(float));
        os.write((char*)&r, 1);
        os.write((char*)&g, 1);
        os.write((char*)&b, 1);
        int n = vis.size();
        os.write((char*)&n, sizeof(int));
        if(n > 0)
        {
            os.write((char*)vis.data(), sizeof(int) * n);
        }
        return os;
    }
    std::istream& read(std::istream& is)
    {
        is.read((char*)&x, sizeof(float));
        is.read((char*)&y, sizeof(float));
        is.read((char*)&z, sizeof(float));
        is.read((char*)&r, 1);
        is.read((char*)&g, 1);
        is.read((char*)&b, 1);
        int n = 0;
        is.read((char*)&n, sizeof(int));
        vis.resize(n);
        if(n > 0)
        {
            is.read((char*)vis.data(), sizeof(int) * n);
        }
        return is;
    }
};

struct Coor
{
    int idxL = 0;
    int d = 0;
    void makeD(float disparity)
    {
        const int SCALE = 1000;
        d = static_cast<int>(disparity * SCALE);
    }
    void getDisparity(float& disparity)
    {
        const float SCALE = 0.001;
        disparity = d * SCALE;
    }
};

#pragma pack()

struct MarginCloudHeader
{
    size_t totalPoints = 0;
    size_t marginCount[9];
    MarginCloudHeader() { memset(this, 0, sizeof(MarginCloudHeader)); }
};

template <typename T>
void readCloudWithMargin(const std::string& readCloud, std::vector<T>& marginCloud)
{
    MarginCloudHeader header;
    std::ifstream ifs(readCloud, std::ios::in | std::ios::binary);
    if(!ifs.is_open())
    {
        LOG(ERROR) << "Can't read file " << readCloud;
    }
    ifs.read((char*)&header, sizeof(MarginCloudHeader));
    ifs.close();

    if(header.totalPoints == 0)
        return;
    marginCloud.resize(header.totalPoints);
    int iTotal = 0;
    for(int i = 0; i < 9; ++i)
    {
        std::stringstream ss;
        ss << readCloud << "_" << i;
        int nPt = header.marginCount[i];
        std::ifstream iifs(ss.str(), std::ios::in | std::ios::binary);
        if(!iifs)
            continue;
        for(int j = 0; j < nPt; ++j)
        {
            marginCloud[iTotal++].read(iifs);
        }
    }
    marginCloud.resize(iTotal);
    return;

}

template <typename T>
void readCloudWithMargin(const std::string& readCloud, 
	const std::vector<int>& marginIds, std::vector<T>& marginCloud)
{
    MarginCloudHeader header;
    std::ifstream ifs(readCloud, std::ios::in | std::ios::binary);
    if(!ifs.is_open())
    {
        LOG(ERROR) << "Can't read file " << readCloud;
    }
    ifs.read((char*)&header, sizeof(MarginCloudHeader));
    ifs.close();
    if(header.totalPoints == 0)
        return;
    marginCloud.resize(header.totalPoints);
    int iTotal = 0;
    for(size_t i = 0; i < marginIds.size(); ++i)
    {
        int id = marginIds[i];
        CHECK(id >= 0 && id <= 8);
        std::stringstream ss;
        ss << readCloud << "_" << id;
        int nPt = header.marginCount[id];
        std::ifstream iifs(ss.str(), std::ios::in | std::ios::binary);
        if(!iifs)
            continue;
        for(int j = 0; j < nPt; ++j)
        {
            marginCloud[iTotal++].read(iifs);
        }
    }
    marginCloud.resize(iTotal);
}

void findFiles(std::vector<PtsPair>& files, const std::string &path)
{
    files.clear();
    std::string ptsNameFilters;

    std::string file_name;
    ptsNameFilters = "*.xyzrgbv";
    file_name = "%d_%d.xyzrgbv";

	std::vector<std::string> find_files = stlplus::folder_files(path);
	std::vector<std::string> ptsFileList;
	for (int i = 0; i < find_files.size(); ++i) {
		if (stlplus::extension_part(find_files[i]) == "xyzrgbv") {
			ptsFileList.push_back(find_files[i]);
		}
	}

    for(int i = 0; i < ptsFileList.size(); ++i)
    {
        std::string ss(stlplus::basename_part(ptsFileList[i]));
        std::pair<int, int> blockKey;
        sscanf_s(ss.c_str(), file_name.c_str(), &blockKey.first, &blockKey.second);
        PtsPair p;
        p.LR = blockKey;
        p.fileInfo = ptsFileList[i];
        files.push_back(p);
    }
}

void read_bin_file(const std::string &file_path, std::vector<Point3d> &pts, std::vector<std::vector<int>> &ptsVis)
{
    clock_t start = clock();
    clock_t finish = clock();
//     std::vector<PtsPair> xyzrgbFiles;
//     findFiles(xyzrgbFiles,path);
    std::string ss(stlplus::basename_part(file_path));
    std::pair<int, int> blockKey;
    std::string file_name;
    file_name = "%d_%d.xyzrgbv";
    sscanf_s(ss.c_str(), file_name.c_str(), &blockKey.first, &blockKey.second);
    PtsPair p;
    p.LR = blockKey;
	p.fileInfo = file_path;
   // finish = clock();
//#pragma omp parallel for
  //  for(int i = 0; i < xyzrgbFiles.size(); ++i)
	std::string path = stlplus::folder_part(file_path);
    {
        int X = p.LR.first;
        int Y = p.LR.second;
        std::array<int, 8> dx = {-1, 0, 1, -1, 1, -1, 0, 1};
        std::array<int, 8> dy = {-1, -1, -1, 0, 0, 1, 1, 1};
        if(X == 1)
        {
            dx = {-2, 0, 1, -2, 1, -2, 0, 1};
        }
        else if(X == -1)
        {
            dx = {-1, 0, 2, -1, 2, -1, 0, 2};
        }
        if(Y == 1)
        {
            dy = {-2, -2, -2, 0, 0, 1, 1, 1};
        }
        else if(Y == -1)
        {
            dy = {-1, -1, -1, 0, 0, 2, 2, 2};
        }

        char buffer[1024];
        std::vector<int> ids[8] = {{8}, {6, 7, 8}, {6}, {2, 5, 8}, {0, 3, 6}, {2}, {0, 1, 2}, {0}};
        std::vector<XYZRGBVis> cloud;
        readCloudWithMargin(p.fileInfo, cloud);
        for(int dd = 0; dd < 8; ++dd)
        {
            int x = X + dx[dd];
            int y = Y + dy[dd];
            sprintf_s(buffer, 1024, "%s/%d_%d.xyzrgbv", path.c_str(), x, y);
            if(stlplus::file_exists(buffer))
            {
                std::vector<int> margIds;
                margIds = ids[dd];
                std::vector<XYZRGBVis> margPoints;
                readCloudWithMargin(buffer, margIds, margPoints);
                for(int j = 0; j < (int)margPoints.size(); ++j)
                {
                    cloud.emplace_back(margPoints[j]);
                }
            }
        }
        //writeLas(LAS_DIR() + "/" + std::to_string(X) + "_" + std::to_string(Y) + ".las", cloud);
		pts.clear();
		ptsVis.clear();
		pts.resize(cloud.size());
		ptsVis.resize(cloud.size());
		for (int i = 0; i < cloud.size(); ++i) {
			pts[i].x = cloud[i].x;
			pts[i].y = cloud[i].y;
			pts[i].z = cloud[i].z;
			ptsVis[i].swap(cloud[i].vis);
		}
    }
    //	findXYZRGBFiles
}
int main(int argc, char* argv[])
{
    system::Timer timer;

    std::string verboseLevel = system::EVerboseLevel_enumToString(system::Logger::getDefaultVerboseLevel());
    std::string sfmDataFilename;
    std::string outputMesh;
    std::string outputDensePointCloud;
    std::string depthMapsFolder;
    std::string depthMapsFilterFolder;
    EPartitioningMode partitioningMode = ePartitioningSingleBlock;
    ERepartitionMode repartitionMode = eRepartitionMultiResolution;
    std::size_t estimateSpaceMinObservations = 3;
    float estimateSpaceMinObservationAngle = 10.0f;
    double universePercentile = 0.999;
    int maxPtsPerVoxel = 6000000;
    bool meshingFromDepthMaps = true;
    bool estimateSpaceFromSfM = true;
	std::string externalCloudPath;
	double minDist = 0.01;
    bool addLandmarksToTheDensePointCloud = false;
    bool saveRawDensePointCloud = false;

    fuseCut::FuseParams fuseParams;

    po::options_description allParams("AliceVision meshing");

    po::options_description requiredParams("Required parameters");
    requiredParams.add_options()
        ("input,i", po::value<std::string>(&sfmDataFilename)->required(),
          "SfMData file.")
        ("output,o", po::value<std::string>(&outputDensePointCloud)->required(),
          "Output Dense SfMData file.")
        ("outputMesh,o", po::value<std::string>(&outputMesh)->required(),
          "Output mesh (OBJ file format).");

    po::options_description optionalParams("Optional parameters");
    optionalParams.add_options()
        ("depthMapsFolder", po::value<std::string>(&depthMapsFolder),
            "Input depth maps folder.")
        ("depthMapsFilterFolder", po::value<std::string>(&depthMapsFilterFolder),
            "Input filtered depth maps folder.")
        ("maxInputPoints", po::value<int>(&fuseParams.maxInputPoints)->default_value(fuseParams.maxInputPoints),
            "Max input points loaded from images.")
        ("maxPoints", po::value<int>(&fuseParams.maxPoints)->default_value(fuseParams.maxPoints),
            "Max points at the end of the depth maps fusion.")
        ("maxPointsPerVoxel", po::value<int>(&maxPtsPerVoxel)->default_value(maxPtsPerVoxel),
            "Max points per voxel.")
        ("minStep", po::value<int>(&fuseParams.minStep)->default_value(fuseParams.minStep),
            "The step used to load depth values from depth maps is computed from maxInputPts. Here we define the minimal value for this step, "
            "so on small datasets we will not spend too much time at the beginning loading all depth values.")
        ("simFactor", po::value<float>(&fuseParams.simFactor)->default_value(fuseParams.simFactor),
            "simFactor")
        ("angleFactor", po::value<float>(&fuseParams.angleFactor)->default_value(fuseParams.angleFactor),
            "angleFactor")
        ("partitioning", po::value<EPartitioningMode>(&partitioningMode)->default_value(partitioningMode),
            "Partitioning: 'singleBlock' or 'auto'.")
        ("repartition", po::value<ERepartitionMode>(&repartitionMode)->default_value(repartitionMode),
            "Repartition: 'multiResolution' , 'regularGrid' or 'external'.")
		("externalCloudPath", po::value<std::string>(&externalCloudPath)->default_value(externalCloudPath),
				"Input external point cloud file")
		("minDist", po::value<double>(&minDist)->default_value(minDist),
				"point distance")
        ("estimateSpaceFromSfM", po::value<bool>(&estimateSpaceFromSfM)->default_value(estimateSpaceFromSfM),
            "Estimate the 3d space from the SfM.")
        ("addLandmarksToTheDensePointCloud", po::value<bool>(&addLandmarksToTheDensePointCloud)->default_value(addLandmarksToTheDensePointCloud),
            "Add SfM Landmarks into the dense point cloud (created from depth maps). If only the SfM is provided in input, SfM landmarks will be used regardless of this option.");

    po::options_description advancedParams("Advanced parameters");
    advancedParams.add_options()
        ("universePercentile", po::value<double>(&universePercentile)->default_value(universePercentile),
            "universe percentile")
        ("estimateSpaceMinObservations", po::value<std::size_t>(&estimateSpaceMinObservations)->default_value(estimateSpaceMinObservations),
            "Minimum number of observations for SfM space estimation.")
        ("estimateSpaceMinObservationAngle", po::value<float>(&estimateSpaceMinObservationAngle)->default_value(estimateSpaceMinObservationAngle),
            "Minimum angle between two observations for SfM space estimation.")
        ("pixSizeMarginInitCoef", po::value<double>(&fuseParams.pixSizeMarginInitCoef)->default_value(fuseParams.pixSizeMarginInitCoef),
            "pixSizeMarginInitCoef")
        ("pixSizeMarginFinalCoef", po::value<double>(&fuseParams.pixSizeMarginFinalCoef)->default_value(fuseParams.pixSizeMarginFinalCoef),
            "pixSizeMarginFinalCoef")
        ("voteMarginFactor", po::value<float>(&fuseParams.voteMarginFactor)->default_value(fuseParams.voteMarginFactor),
            "voteMarginFactor")
        ("contributeMarginFactor", po::value<float>(&fuseParams.contributeMarginFactor)->default_value(fuseParams.contributeMarginFactor),
            "contributeMarginFactor")
        ("simGaussianSizeInit", po::value<float>(&fuseParams.simGaussianSizeInit)->default_value(fuseParams.simGaussianSizeInit),
            "simGaussianSizeInit")
        ("simGaussianSize", po::value<float>(&fuseParams.simGaussianSize)->default_value(fuseParams.simGaussianSize),
            "simGaussianSize")
        ("minAngleThreshold", po::value<double>(&fuseParams.minAngleThreshold)->default_value(fuseParams.minAngleThreshold),
            "minAngleThreshold")
        ("refineFuse", po::value<bool>(&fuseParams.refineFuse)->default_value(fuseParams.refineFuse),
            "refineFuse")
        ("saveRawDensePointCloud", po::value<bool>(&saveRawDensePointCloud)->default_value(saveRawDensePointCloud),
            "Save dense point cloud before cut and filtering.");

    po::options_description logParams("Log parameters");
    logParams.add_options()
      ("verboseLevel,v", po::value<std::string>(&verboseLevel)->default_value(verboseLevel),
        "verbosity level (fatal, error, warning, info, debug, trace).");

    allParams.add(requiredParams).add(optionalParams).add(advancedParams).add(logParams);

    po::variables_map vm;

    try
    {
      po::store(po::parse_command_line(argc, argv, allParams), vm);

      if(vm.count("help") || (argc == 1))
      {
        ALICEVISION_COUT(allParams);
        return EXIT_SUCCESS;
      }

      po::notify(vm);
    }
    catch(boost::program_options::required_option& e)
    {
      ALICEVISION_CERR("ERROR: " << e.what() << std::endl);
      ALICEVISION_COUT("Usage:\n\n" << allParams);
      return EXIT_FAILURE;
    }
    catch(boost::program_options::error& e)
    {
      ALICEVISION_CERR("ERROR: " << e.what() << std::endl);
      ALICEVISION_COUT("Usage:\n\n" << allParams);
      return EXIT_FAILURE;
    }

    ALICEVISION_COUT("Program called with the following parameters:");
    ALICEVISION_COUT(vm);

    // set verbose level
    system::Logger::get()->setLogLevel(verboseLevel);

	if (repartitionMode == eRepartitionExternal) {
		meshingFromDepthMaps = false;
	}
	else {
        if(depthMapsFolder.empty() || depthMapsFilterFolder.empty())
        {
            if(depthMapsFolder.empty() && depthMapsFilterFolder.empty() &&
               repartitionMode == eRepartitionMultiResolution && partitioningMode == ePartitioningSingleBlock)
            {
                meshingFromDepthMaps = false;
            }
            else
            {
                ALICEVISION_LOG_ERROR(
                    "Invalid input options:\n"
                    "- Meshing from depth maps require --depthMapsFolder and --depthMapsFilterFolder options.\n"
                    "- Meshing from SfM require option --partitioning set to 'singleBlock' and option --repartition "
                    "set to 'multiResolution'.");
                return EXIT_FAILURE;
            }
        }

	}

    // read the input SfM scene
    sfmData::SfMData sfmData;
    if(!sfmDataIO::Load(sfmData, sfmDataFilename, sfmDataIO::ESfMData::ALL))
    {
      ALICEVISION_LOG_ERROR("The input SfMData file '" << sfmDataFilename << "' cannot be read.");
      return EXIT_FAILURE;
    }

    // initialization
    mvsUtils::MultiViewParams mp(sfmData, "", depthMapsFolder, depthMapsFilterFolder, meshingFromDepthMaps);

    mp.userParams.put("LargeScale.universePercentile", universePercentile);
#if 1
    mp.userParams.put("global.LabatutCFG09", true);
    mp.userParams.put("global.JancosekCVPR11", false);
    mp.userParams.put("global.JancosekIJCV", false);
#endif

    int ocTreeDim = mp.userParams.get<int>("LargeScale.gridLevel0", 1024);
    const auto baseDir = mp.userParams.get<std::string>("LargeScale.baseDirName", "root01024");

    fs::path outDirectory = fs::path(outputMesh).parent_path();
    if(!fs::is_directory(outDirectory))
        fs::create_directory(outDirectory);

    fs::path tmpDirectory = outDirectory / "tmp";

    ALICEVISION_LOG_WARNING("repartitionMode: " << repartitionMode);
    ALICEVISION_LOG_WARNING("partitioningMode: " << partitioningMode);

    switch(repartitionMode)
    {
        case eRepartitionRegularGrid:
        {
            switch(partitioningMode)
            {
                case ePartitioningAuto:
                {
                    ALICEVISION_LOG_INFO("Meshing mode: regular Grid, partitioning: auto.");
                    fuseCut::LargeScale lsbase(&mp, tmpDirectory.string() + "/");
                    lsbase.generateSpace(maxPtsPerVoxel, ocTreeDim, true);
                    std::string voxelsArrayFileName = lsbase.spaceFolderName + "hexahsToReconstruct.bin";
                    StaticVector<Point3d>* voxelsArray = nullptr;
                    if(fs::exists(voxelsArrayFileName))
                    {
                        // If already computed reload it.
                        ALICEVISION_LOG_INFO("Voxels array already computed, reload from file: " << voxelsArrayFileName);
                        voxelsArray = loadArrayFromFile<Point3d>(voxelsArrayFileName);
                    }
                    else
                    {
                        ALICEVISION_LOG_INFO("Compute voxels array.");
                        fuseCut::ReconstructionPlan rp(lsbase.dimensions, &lsbase.space[0], lsbase.mp, lsbase.spaceVoxelsFolderName);
                        voxelsArray = rp.computeReconstructionPlanBinSearch(fuseParams.maxPoints);
                        saveArrayToFile<Point3d>(voxelsArrayFileName, voxelsArray);
                    }
                    fuseCut::reconstructSpaceAccordingToVoxelsArray(voxelsArrayFileName, &lsbase);
                    // Join meshes
                    mesh::Mesh* mesh = fuseCut::joinMeshes(voxelsArrayFileName, &lsbase);

                    if(mesh->pts->empty() || mesh->tris->empty())
                      throw std::runtime_error("Empty mesh");

                    ALICEVISION_LOG_INFO("Saving joined meshes...");

                    fs::path spaceBinFileName = outDirectory/"denseReconstruction.bin";
                    mesh->saveToBin(spaceBinFileName.string());

                    // Export joined mesh to obj
                    mesh->saveToObj(outputMesh);

                    delete mesh;

                    // Join ptsCams
                    StaticVector<StaticVector<int>*>* ptsCams = fuseCut::loadLargeScalePtsCams(lsbase.getRecsDirs(voxelsArray));
                    saveArrayOfArraysToFile<int>((outDirectory/"meshPtsCamsFromDGC.bin").string(), ptsCams);
                    deleteArrayOfArrays<int>(&ptsCams);
                    break;
                }
                case ePartitioningSingleBlock:
                {
                    ALICEVISION_LOG_INFO("Meshing mode: regular Grid, partitioning: single block.");
                    fuseCut::LargeScale ls0(&mp, tmpDirectory.string() + "/");
                    ls0.generateSpace(maxPtsPerVoxel, ocTreeDim, true);
                    unsigned long ntracks = std::numeric_limits<unsigned long>::max();
                    while(ntracks > fuseParams.maxPoints)
                    {
                        fs::path dirName = outDirectory/("LargeScaleMaxPts" + mvsUtils::num2strFourDecimal(ocTreeDim));
                        fuseCut::LargeScale* ls = ls0.cloneSpaceIfDoesNotExists(ocTreeDim, dirName.string() + "/");
                        fuseCut::VoxelsGrid vg(ls->dimensions, &ls->space[0], ls->mp, ls->spaceVoxelsFolderName);
                        ntracks = vg.getNTracks();
                        delete ls;
                        ALICEVISION_LOG_INFO("Number of track candidates: " << ntracks);
                        if(ntracks > fuseParams.maxPoints)
                        {
                            ALICEVISION_LOG_INFO("ocTreeDim: " << ocTreeDim);
                            double t = (double)ntracks / (double)fuseParams.maxPoints;
                            ALICEVISION_LOG_INFO("downsample: " << ((t < 2.0) ? "slow" : "fast"));
                            ocTreeDim = (t < 2.0) ? ocTreeDim-100 : ocTreeDim*0.5;
                        }
                    }
                    ALICEVISION_LOG_INFO("Number of tracks: " << ntracks);
                    ALICEVISION_LOG_INFO("ocTreeDim: " << ocTreeDim);
                    fs::path dirName = outDirectory/("LargeScaleMaxPts" + mvsUtils::num2strFourDecimal(ocTreeDim));
                    fuseCut::LargeScale lsbase(&mp, dirName.string()+"/");
                    lsbase.loadSpaceFromFile();
                    fuseCut::ReconstructionPlan rp(lsbase.dimensions, &lsbase.space[0], lsbase.mp, lsbase.spaceVoxelsFolderName);

                    StaticVector<int> voxelNeighs;
                    voxelNeighs.resize(rp.voxels->size() / 8);
                    ALICEVISION_LOG_INFO("voxelNeighs.size(): " << voxelNeighs.size());
                    for(int i = 0; i < voxelNeighs.size(); ++i)
                        voxelNeighs[i] = i;

                    fuseCut::DelaunayGraphCut delaunayGC(lsbase.mp);
                    Point3d* hexah = &lsbase.space[0];

                    StaticVector<int> cams;
                    if(hexah)
                    {
                      cams = mp.findCamsWhichIntersectsHexahedron(hexah);
                    }
                    else
                    {
                      cams.resize(mp.getNbCameras());
                      for(int i = 0; i < cams.size(); ++i)
                          cams[i] = i;
                    }

                    if(cams.size() < 1)
                        throw std::logic_error("No camera to make the reconstruction");

                    delaunayGC.createDensePointCloudFromPrecomputedDensePoints(hexah, cams, &voxelNeighs, (fuseCut::VoxelsGrid*)&rp);
                    delaunayGC.createGraphCut(hexah, cams, (fuseCut::VoxelsGrid*)&rp, outDirectory.string()+"/", lsbase.getSpaceCamsTracksDir(), false, lsbase.getSpaceSteps());
                    delaunayGC.graphCutPostProcessing();

                    // Save mesh as .bin and .obj
                    mesh::Mesh* mesh = delaunayGC.createMesh();
                    if(mesh->pts->empty() || mesh->tris->empty())
                      throw std::runtime_error("Empty mesh");

                    StaticVector<StaticVector<int>*>* ptsCams = delaunayGC.createPtsCams();

                    StaticVector<Point3d>* hexahsToExcludeFromResultingMesh = nullptr;
                    mesh::meshPostProcessing(mesh, ptsCams, mp, outDirectory.string()+"/", hexahsToExcludeFromResultingMesh, hexah);
                    mesh->saveToBin((outDirectory/"denseReconstruction.bin").string());

                    saveArrayOfArraysToFile<int>((outDirectory/"meshPtsCamsFromDGC.bin").string(), ptsCams);
                    deleteArrayOfArrays<int>(&ptsCams);

                    mesh->saveToObj(outputMesh);

                    delete mesh;
                    break;
                }
                case ePartitioningUndefined:
                default:
                    throw std::invalid_argument("Partitioning mode is not defined");
            }
            break;
        }
        case eRepartitionMultiResolution:
        {
            switch(partitioningMode)
            {
                case ePartitioningAuto:
                {
                    throw std::invalid_argument("Meshing mode: 'multiResolution', partitioning: 'auto' is not yet implemented.");
                }
                case ePartitioningSingleBlock:
                {
                    ALICEVISION_LOG_INFO("Meshing mode: multi-resolution, partitioning: single block.");
                    fuseCut::DelaunayGraphCut delaunayGC(&mp);
                    std::array<Point3d, 8> hexah;

                    float minPixSize;
                    fuseCut::Fuser fs(&mp);

                    if(meshingFromDepthMaps && !estimateSpaceFromSfM)
                      fs.divideSpaceFromDepthMaps(&hexah[0], minPixSize);
                    else
                      fs.divideSpaceFromSfM(sfmData, &hexah[0], estimateSpaceMinObservations, estimateSpaceMinObservationAngle);

                    Voxel dimensions = fs.estimateDimensions(&hexah[0], &hexah[0], 0, ocTreeDim, (meshingFromDepthMaps && !estimateSpaceFromSfM) ? nullptr : &sfmData);
                    StaticVector<Point3d>* voxels = mvsUtils::computeVoxels(&hexah[0], dimensions);

                    StaticVector<int> voxelNeighs;
                    voxelNeighs.resize(voxels->size() / 8);
                    ALICEVISION_LOG_INFO("voxelNeighs.size(): " << voxelNeighs.size());

                    for(int i = 0; i < voxelNeighs.size(); ++i)
                        voxelNeighs[i] = i;

                    Point3d spaceSteps;
                    {
                        Point3d vx = hexah[1] - hexah[0];
                        Point3d vy = hexah[3] - hexah[0];
                        Point3d vz = hexah[4] - hexah[0];
                        spaceSteps.x = (vx.size() / (double)dimensions.x) / (double)ocTreeDim;
                        spaceSteps.y = (vy.size() / (double)dimensions.y) / (double)ocTreeDim;
                        spaceSteps.z = (vz.size() / (double)dimensions.z) / (double)ocTreeDim;
                    }

                    StaticVector<int> cams;
                    if(meshingFromDepthMaps)
                    {
                      cams = mp.findCamsWhichIntersectsHexahedron(&hexah[0]);
                    }
                    else
                    {
                      cams.resize(mp.getNbCameras());
                      for(int i = 0; i < cams.size(); ++i)
                          cams[i] = i;
                    }

                    if(cams.empty())
                        throw std::logic_error("No camera to make the reconstruction");

                    delaunayGC.createDensePointCloud(&hexah[0], cams, addLandmarksToTheDensePointCloud ? &sfmData : nullptr, meshingFromDepthMaps ? &fuseParams : nullptr);
                    if(saveRawDensePointCloud)
                    {
                      ALICEVISION_LOG_INFO("Save dense point cloud before cut and filtering.");
                      StaticVector<StaticVector<int>*>* ptsCams = delaunayGC.createPtsCams();
                      exportPointCloud((outDirectory/"densePointCloud_raw.abc").string(), mp, sfmData, delaunayGC._verticesCoords, *ptsCams);
                      deleteArrayOfArrays<int>(&ptsCams);
                    }

                    delaunayGC.createGraphCut(&hexah[0], cams, nullptr, outDirectory.string()+"/", outDirectory.string()+"/SpaceCamsTracks/", false, spaceSteps);
                    delaunayGC.graphCutPostProcessing();

                    // Save mesh as .bin and .obj
                    mesh::Mesh* mesh = delaunayGC.createMesh();
                    if(mesh->pts->empty() || mesh->tris->empty())
                        throw std::runtime_error("Empty mesh");

                    StaticVector<StaticVector<int>*>* ptsCams = delaunayGC.createPtsCams();

                    StaticVector<Point3d>* hexahsToExcludeFromResultingMesh = nullptr;
                    mesh::meshPostProcessing(mesh, ptsCams, mp, outDirectory.string()+"/", hexahsToExcludeFromResultingMesh, &hexah[0]);

                    ALICEVISION_LOG_INFO("Save dense point cloud.");
                    exportPointCloud(outputDensePointCloud, mp, sfmData, mesh->pts->getData(), *ptsCams);

                    //mesh->saveToBin((outDirectory/"denseReconstruction.bin").string());

                    //saveArrayOfArraysToFile<int>((outDirectory/"meshPtsCamsFromDGC.bin").string(), ptsCams);
                    deleteArrayOfArrays<int>(&ptsCams);
                    delete voxels;

                    ALICEVISION_LOG_INFO("Save obj mesh file.");
                    mesh->saveToObj(outputMesh);

                    delete mesh;
                    break;
                }
                case ePartitioningUndefined:
                default:
                    throw std::invalid_argument("Partitioning mode is not defined");
            }
            break;
        }
		case eRepartitionExternal:
		{
            ALICEVISION_LOG_INFO("Meshing mode: external, partitioning: single block.");
            fuseCut::DelaunayGraphCut delaunayGC(&mp);
            std::array<Point3d, 8> hexah;
			StaticVector<int> cams;
            cams.resize(mp.getNbCameras());
            for(int i = 0; i < cams.size(); ++i)
                cams[i] = i;
            
            if(cams.empty())
                throw std::logic_error("No camera to make the reconstruction");
			std::vector<Point3d > pts;
			std::vector<std::vector<int>> ptsVis;
			read_bin_file(externalCloudPath, pts, ptsVis);
			fuseCut::Fuser fs(&mp);
			fs.divedeSpaceFromPoints(pts, &hexah[0]);
			//float minDist = 0.1;
			delaunayGC.createDensePointCloudFromExternalPoints(&hexah[0], cams,pts,ptsVis,minDist);
			Point3d spaceSteps(2 * minDist, 2*  minDist,2 * minDist);
            delaunayGC.createGraphCut(&hexah[0], cams, nullptr, outDirectory.string() + "/",
                                      outDirectory.string() + "/SpaceCamsTracks/", false, spaceSteps);
            delaunayGC.graphCutPostProcessing();

            // Save mesh as .bin and .obj
            mesh::Mesh* mesh = delaunayGC.createMesh();
            if(mesh->pts->empty() || mesh->tris->empty())
                throw std::runtime_error("Empty mesh");

            StaticVector<StaticVector<int>*>* ptsCams = delaunayGC.createPtsCams();
            StaticVector<int> usedCams = delaunayGC.getSortedUsedCams();

            StaticVector<Point3d>* hexahsToExcludeFromResultingMesh = nullptr;
            mesh::meshPostProcessing(mesh, ptsCams, usedCams, mp, outDirectory.string() + "/",
                                     hexahsToExcludeFromResultingMesh, &hexah[0]);
            mesh->saveToBin((outDirectory / "denseReconstruction.bin").string());

            saveArrayOfArraysToFile<int>((outDirectory / "meshPtsCamsFromDGC.bin").string(), ptsCams);
            deleteArrayOfArrays<int>(&ptsCams);

            mesh->saveToObj(outputMesh);

            delete mesh;
            break;
		}
        case eRepartitionUndefined:
        default:
            throw std::invalid_argument("Repartition mode is not defined");
    }

    ALICEVISION_LOG_INFO("Task done in (s): " + std::to_string(timer.elapsed()));
    return EXIT_SUCCESS;
}
