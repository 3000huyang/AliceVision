// aliceVision_convertFromInsightMap.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <cereal/archives/json.hpp>
#include <aliceVision/sfmData/SfMData.hpp>
#include <aliceVision/sfmDataIO/sfmDataIO.hpp>
#include <aliceVision/system/Logger.hpp>
#include <aliceVision/system/cmdline.hpp>
#include <aliceVision/config.hpp>

#include <boost/program_options.hpp>
#include <boost/system/error_code.hpp>
#include <boost/filesystem.hpp>

#include "TaskList/Project.h"
using namespace insight;
namespace po = boost::program_options;
using namespace aliceVision;

#define ALICEVISION_COUT(x) std::cout << x << std::endl
#define ALICEVISION_CERR(x) std::cerr << x << std::endl

#define ALICEVISION_LOG_TRACE_OBJ BOOST_LOG_TRIVIAL(trace)
#define ALICEVISION_LOG_DEBUG_OBJ BOOST_LOG_TRIVIAL(debug)
#define ALICEVISION_LOG_INFO_OBJ BOOST_LOG_TRIVIAL(info)
#define ALICEVISION_LOG_WARNING_OBJ BOOST_LOG_TRIVIAL(warning)
#define ALICEVISION_LOG_ERROR_OBJ BOOST_LOG_TRIVIAL(error)
#define ALICEVISION_LOG_FATAL_OBJ BOOST_LOG_TRIVIAL(fatal)
#define ALICEVISION_LOG(MODE, a) MODE << a

#define ALICEVISION_LOG_TRACE(a) ALICEVISION_LOG(ALICEVISION_LOG_TRACE_OBJ, a)
#define ALICEVISION_LOG_DEBUG(a) ALICEVISION_LOG(ALICEVISION_LOG_DEBUG_OBJ, a)
#define ALICEVISION_LOG_INFO(a) ALICEVISION_LOG(ALICEVISION_LOG_INFO_OBJ, a)
#define ALICEVISION_LOG_WARNING(a) ALICEVISION_LOG(ALICEVISION_LOG_WARNING_OBJ, a)
#define ALICEVISION_LOG_ERROR(a) ALICEVISION_LOG(ALICEVISION_LOG_ERROR_OBJ, a)
#define ALICEVISION_LOG_FATAL(a) ALICEVISION_LOG(ALICEVISION_LOG_FATAL_OBJ, a)


Mat3 EulerAnglesToRotationMatrix(const double rx, const double ry, const double rz)
{
    const Eigen::Matrix3d Rx = Eigen::AngleAxisd(rx, Eigen::Vector3d::UnitX()).toRotationMatrix();
    const Eigen::Matrix3d Ry = Eigen::AngleAxisd(ry, Eigen::Vector3d::UnitY()).toRotationMatrix();
    const Eigen::Matrix3d Rz = Eigen::AngleAxisd(rz, Eigen::Vector3d::UnitZ()).toRotationMatrix();
    return Rx * Ry * Rz;

}

void generateSfmData(
	sfmData::SfMData& data, 
	ImageListGenerator& imageListGen, DBCameraList& cameraList,
                                DBTrackList& trackList, DBGCPList& gcpList, double tx, double ty, double tz)
{
	typedef sfmData::SfMData SfM_Data;
    SfM_Data& _sfm_data = data;
    _sfm_data.views.clear();
	_sfm_data.intrinsics.clear();
    _sfm_data.structure.clear();
	_sfm_data.control_points.clear();
    const std::map<uint32_t, DBImage>& images = imageListGen.imageList.Image_list();
    const std::map<uint32_t, DBPose>& initPoses = imageListGen.poseList.Pose_list();
    const std::map<uint32_t, DBPose>& poses = imageListGen.poseATList.Pose_list();
    const std::map<uint32_t, DBCamera>& cameras = cameraList.Camera_list();
    const std::map<uint32_t, DBTrack>& tracks = trackList.Track_lsit();
    const std::map<uint32_t, DBGCP>& gcps = gcpList.GCP_List();
	sfmData::Views& views = _sfm_data.views;
	sfmData::Intrinsics & cams = _sfm_data.intrinsics;
    for(auto itr = cameras.begin(); itr != cameras.end(); ++itr)
    {
        DBCamera cam = itr->second;
        if(cam.focalpx == 0)
        {
            LOG(ERROR) << "logic error"
                       << "camera focal is zero\n";
        }
		cams[itr->first] = camera::createPinholeIntrinsic(
			camera::EINTRINSIC::PINHOLE_CAMERA, cam.w, cam.h, cam.focalpx, cam.ppx, cam.ppy);

       // cams[itr->first]->dbCamera = cam;
        //cams[itr->first]->updateK();
    }
    for(auto itr = images.begin(); itr != images.end(); ++itr)
    {
       // RefImageFrame* refimg = new RefImageFrame;
		const DBImage &dbimage = itr->second;
		const DBPose &dbPose = poses.at(itr->first);

		sfmData::View *pView = new sfmData::View(dbimage.image_full_path, dbimage.id, dbimage.camera_id, dbimage.id,
			cameras.at(dbimage.camera_id).w, cameras.at(dbimage.camera_id).h);
		views[itr->first].reset(pView);
		Mat3 R; Vec3 C;
		R = EulerAnglesToRotationMatrix(dbPose.omega, dbPose.phi, dbPose.kappa);
		C.x() = dbPose.x + tx;
		C.y() = dbPose.y + ty;
		C.z() = dbPose.z + tz;

		geometry::Pose3 trans(R, C);
		sfmData::CameraPose pose(trans);
		_sfm_data.setAbsolutePose(dbimage.id, pose);
    }

    // 	Structure structure;//稀疏点及track
    // 	GCPs gcps;//控制点
    _sfm_data.structure.clear();
    for(auto itr = tracks.begin(); itr != tracks.end(); ++itr)
    {
        sfmData::Landmark& lm = _sfm_data.structure[itr->first];
        const DBTrack& track = itr->second;
		lm.X(0) = track.landmark.x + tx;
		lm.X(1) = track.landmark.y + ty;
		lm.X(2) = track.landmark.z + tz;
		lm.descType = feature::EImageDescriberType::SIFT;
		lm.rgb.x() = track.landmark.r;
		lm.rgb.y() = track.landmark.g;
		lm.rgb.z() = track.landmark.b;

		for(int ii = 0; ii < track.views.size(); ++ii)
        {
			uint32_t viewId = track.views[ii].image_id;
			sfmData::Observation ob;
            ob.id_feat = UndefinedKey;
            ob.x(0) = track.views[ii].u;
            ob.x(1) = track.views[ii].v;
			lm.observations[viewId] = ob;
        }
    }

	_sfm_data.control_points.clear();

	for(auto itr = gcps.begin(); itr != gcps.end(); ++itr)
    {
        sfmData::Landmark & cp = _sfm_data.control_points[itr->first];
        const DBGCP& track = itr->second;
		cp.X(0) = track.enuX + tx;
		cp.X(1) = track.enuY + ty;
		cp.X(2) = track.enuZ + tz;

        for(auto itr = track.views.begin(); itr != track.views.end(); ++itr)
        {
            sfmData::Observation& ob = cp.observations[itr->second.image_id];
            ob.id_feat = UndefinedKey;
            ob.x(0) = itr->second.u;
            ob.x(1) = itr->second.v;
        }
    }
    // 	Cameras cameras;//相机信息

}

int main(int argc, char *argv[])
{
    std::cout << "Convert sfm data!\n"; 
	std::string insightProjectFile;
    std::string outputSfMDataFilename;
	std::string transformFile;
	po::options_description allParams("AliceVision convertFromInsightMap");

    po::options_description requiredParams("Required parameters");
    requiredParams.add_options()
		("input,i", po::value<std::string>(&insightProjectFile)->required(), "Insight project file.")
		( "output,o", po::value<std::string>(&outputSfMDataFilename)->required(), "Path to the output SfmData file.")
		("transformFile, t", po::value<std::string>( &transformFile)->required(), "pos transform for gravity centered.");


	allParams.add(requiredParams);

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
        ALICEVISION_CERR("ERROR: " << e.what());
        ALICEVISION_COUT("Usage:\n\n" << allParams);
        return EXIT_FAILURE;
    }
    catch(boost::program_options::error& e)
    {
        ALICEVISION_CERR("ERROR: " << e.what());
        ALICEVISION_COUT("Usage:\n\n" << allParams);
        return EXIT_FAILURE;
    }

    ALICEVISION_COUT("Program called with the following parameters:");
    ALICEVISION_COUT(vm);

    // set verbose level

    if(insightProjectFile.empty() || outputSfMDataFilename.empty())
    {
		ALICEVISION_CERR("Invalid input or output filename");
        return EXIT_FAILURE;
    }
	std::ifstream ifs(transformFile);
	std::string s;
	std::getline(ifs,s);
	std::getline(ifs, s);
	std::getline(ifs, s);
	std::stringstream ss(s);
	double tx, ty, tz;
	ss >> tx >> ty >> tz;
	ALICEVISION_LOG_INFO("transform : " << tx << " " << ty << " " << tz);
	tx *= -1;
	ty *= -1;
	tz *= -1;
	insight::Project prj;
	prj.openProject(insightProjectFile);
	//change camera to undistort
	for (int i = 0; i < prj.cameraList.Camera_list().size(); ++i)
	{
		DBCamera &dbCame = prj.cameraList.Camera_list().at(i);
		dbCame.ppx = (dbCame.w-1) * 0.5;
		dbCame.ppy = (dbCame.h - 1) * 0.5;
	}
	sfmData::SfMData sfmData;
	generateSfmData(sfmData, prj.imageListGen, prj.cameraList, prj.trackList, prj.gcpList, tx, ty, tz);
	
    // export the SfMData scene in the expected format
    if(!sfmDataIO::Save(sfmData, outputSfMDataFilename, sfmDataIO::ESfMData( sfmDataIO::ALL)))
    {
        ALICEVISION_LOG_ERROR("An error occured while trying to save '" << outputSfMDataFilename << "'");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
