#include "Project.h"


#include <cereal/archives/json.hpp>
#include <iostream>
#include <fstream>

#include <windows.h>
#include <string>
#include <iostream>
#include <stlplus3/file_system.hpp>
//#include "data_sheet.h"
//#include "error_info.h"
//#include "../ImageIO/GdalUtils.h"
//#include "log/baselog.h"
#include <glog/logging.h>
//#include "common/Coordinates.h"
//#include "common/gps.h"

//#include "ogr_spatialref.h"
//#include <QSettings>

insight::Project::Project()
{
}

INSIGHT_NAME_SPACE

// bool Project::createProject(const ProjectInfomation &info, const QString &path)
// {
// 	infomation = info;
// 	projectFile = path;
// 	resource.reset();
// 	completeDirs();
// 	return saveProject();
// }

bool Project::openProject(const std::string &projectFile)
{
	try{
		imageListGen.clear();

		this->projectFile = projectFile;

		std::ifstream ifs((projectFile));
		if (!ifs.is_open()){
			//SetLastError(5);
			return false;
		}
		cereal::JSONInputArchive ar(ifs);
		ar(cereal::make_nvp("InsightProject", *this));
		completeDirs();
		imageListGen.imageList.readFromASCIIFile(stlplus::create_filespec(projectDataDir, ("image_list.txt")));
		imageListGen.poseList.readFromASCIIFile(stlplus::create_filespec(projectDataDir, ("pose_list.txt")));
		gcpList.readFromAsciiFile(stlplus::create_filespec(projectDataDir, ("gcp.txt")));
		cameraList.readFromJson(stlplus::create_filespec(projectDataDir, ("camera_list.txt")));

		imageListGen.poseATList.readFromASCIIFile(stlplus::create_filespec(atDir, ("pose_at_list.txt")));
		if (_saveBinFlag &eSaveTrackBin){
			trackList.readFromBinFile(stlplus::create_filespec(atDir, ("track_at_list.bin")));
		}
		else{
			trackList.readFromAsciiFile(stlplus::create_filespec(atDir, ("track_at_list.txt")));
		}
		
	//	updateENUCoord();
		return true;
	}
	catch (std::runtime_error  err){
		LOG(ERROR) << err.what();
		//QString info = QString("Can't open file ") + projectFile;
		//prog.msg(info.toLocal8Bit().constData());
		return false;
	}
}

bool Project::getProjectInfomation(const std::string &projectFile)
{
	this->projectFile = projectFile;

	std::ifstream ifs((projectFile));
	if (!ifs.is_open()){
		SetLastError(5);
		return false;
	}

	cereal::JSONInputArchive ar(ifs);
	ar(cereal::make_nvp("InsightProject", *this));
	return true;
}

bool Project::saveProject(SaveFlag flag)
{
	std::ofstream ofs((projectFile));
	if (!ofs.is_open()){
		SetLastError(6);
		return false;
	}
	cereal::JSONOutputArchive ar(ofs);
	ar(cereal::make_nvp("InsightProject", *this));
	if (flag &eSaveImageAndCameras){
		imageListGen.imageList.saveToASCIIFile(stlplus::create_filespec(projectDataDir, ("image_list.txt")));
		cameraList.saveToJson(stlplus::create_filespec(projectDataDir, ("camera_list.txt")));
	}
	
	if (flag & eSaveInitPose){
		imageListGen.poseList.saveToASCIIFile(stlplus::create_filespec(projectDataDir, ("pose_list.txt")));
	}
	
	if (flag &eSaveGCP){
		gcpList.saveToAsciiFile(stlplus::create_filespec(projectDataDir, ("gcp.txt")));
	}
	if (flag & eSaveATResult){
		imageListGen.poseATList.saveToASCIIFile(stlplus::create_filespec(atDir, ("pose_at_list.txt")));
		if (_saveBinFlag &eSaveTrackBin){
			trackList.saveToBinFile(stlplus::create_filespec(atDir, ("track_at_list.bin")));
		}
		else{
			trackList.saveToAsciiFile(stlplus::create_filespec(atDir, ("track_at_list.txt")));
		}
	}
	return true;
}
#if 0
void Project::generateCameraByExif()
{
	std::map<std::string, std::vector<int>> image_groups;
	std::map<uint32_t, DBImage> &imagelist = imageListGen.imageList.Image_list();
	for (auto itr = imagelist.begin(); itr != imagelist.end(); ++itr)
	{
		DBImage &img = itr->second;
		if (!img.exif_valid){
			img.readExif();
		}
		if (img.exif_valid){
			std::string hash_code = img.cameraHashCode();
			image_groups[hash_code].push_back(itr->first);
		}
	}
	for (auto itr = image_groups.begin(); itr != image_groups.end(); ++itr)
	{
		std::vector<int> &img_ids = itr->second;
		if (!img_ids.empty()){
			std::string hash_code = itr->first;
			uint32_t cam_id = UndefinedKey;
			for (auto camItr = cameraList.Camera_list().begin(); camItr != cameraList.Camera_list().end();
				++camItr){
				const DBCamera &cam = camItr->second;
				if (cam.exif_hash_code == hash_code){
					cam_id = cam.id;
					break;
				}
			}
			if (cam_id == UndefinedKey){
				//generate camera
				cam_id = resource.cameraSeed.generate();
				int img_id = img_ids[0];
				DBCamera camera;
				camera.exif_hash_code = hash_code;
				imageListGen.imageList.Image_list().at(img_id).getCameraByExif(camera);
				camera.id = cam_id;
				cameraList.Camera_list()[cam_id] = camera;
			}

			for (int i = 0; i < img_ids.size(); ++i){
				imageListGen.imageList.Image_list().at(img_ids[i]).camera_id = cam_id;
			}
		}
	}
}


void Project::getCameraFromImageWH()
{
	std::map<std::string, std::vector<int>> image_groups;
	std::map<uint32_t, DBImage> &imagelist = imageListGen.imageList.Image_list();
	std::map<uint32_t, uint32_t> mapCameraImage;
	for (auto itr = imagelist.begin(); itr != imagelist.end(); ++itr)
	{
		DBImage &img = itr->second;
		mapCameraImage[img.camera_id] = itr->first;
	}
	GdalUtils::InitGDAL();
	for (auto itr = mapCameraImage.begin(); itr != mapCameraImage.end(); ++itr)
	{
		uint32_t img_id = itr->second;
		uint32_t camera_id = itr->first;

		if (camera_id != UndefinedKey){
			int w = 0, h = 0;
			GdalUtils::GetWidthHeightPixel(imagelist.at(img_id).image_full_path.c_str(), w, h);
			DBCamera &dbcam = cameraList.Camera_list().at(camera_id);
			dbcam.w = w;
			dbcam.h = h;
			dbcam.ppx = (w - 1.0) / 2.0;
			dbcam.ppy = (h - 1.0) / 2.0;
		}
	}
}


void Project::checkConsistency(std::vector<ImageConsistency> &result)
{
	GdalUtils::InitGDAL();
	std::map<uint32_t, DBImage> &imgList = imageListGen.imageList.Image_list();
	for (auto itr = imgList.begin(); itr != imgList.end(); ++itr){
		ImageConsistency consis;
		consis.imageId = itr->first;
		DBImage &img = itr->second;
		consis.cameraId = img.camera_id;
		if (consis.cameraId != UndefinedKey){
			auto camItr = cameraList.Camera_list().find(consis.cameraId);
			if (camItr != cameraList.Camera_list().end()){
				DBCamera cam = camItr->second;
				consis.cameraW = cam.w;
				consis.cameraH = cam.h;
			}
		}
		QFileInfo info(toqs(img.image_full_path));
		if (info.exists()){
			consis.imageExist = true;
			int w = 0, h = 0;
			if (GdalUtils::GetWidthHeightPixel(img.image_full_path.c_str(), w, h)){
				consis.imageW = w;
				consis.imageH = h;
				consis.imageCanRead = true;
			}
			else{
				consis.imageCanRead = false;
			}
		}
		result.push_back(consis);
	}
}


bool Project::updateENUCoord()
{
	bool local = false;
	std::map<uint32_t, DBPose> &poses = imageListGen.poseList.Pose_list();
	std::map<uint32_t, DBGCP> &gcps = gcpList.GCP_List();

	if (infomation.coordinate.localSystem){
		for (auto itr = poses.begin(); itr != poses.end(); ++itr)
		{
			DBPose &dbpose = itr->second;
			dbpose.enuX = dbpose.x;
			dbpose.enuY = dbpose.y;
			dbpose.enuZ = dbpose.z;
		}
		for (auto itr = gcps.begin(); itr != gcps.end(); ++itr)
		{
			DBGCP &dbgcp = itr->second;
			dbgcp.enuX = dbgcp.landmark.x;
			dbgcp.enuY = dbgcp.landmark.y;
			dbgcp.enuZ = dbgcp.landmark.z;
		}
		return true;
	}

	Coordinate coord;
	coord.WKT = infomation.coordinate.wkt;
	coord.EPSGName = infomation.coordinate.epsg;
	coord.CoordinateName = infomation.coordinate.name;
	OGRSpatialReference sr;
	bool	ok = Coordinate::coordToSR(coord, sr);
	bool gps_ok = false;
	if (ok){
		double a = sr.GetSemiMajor();
		double b = sr.GetSemiMinor();
		double f = 1.0 / sr.GetInvFlattening();
		Datum dtum(a, b, f);

		if (sr.IsProjected()){
			//先转换成大地坐标，然后是世界坐标，再转换成ENU坐标

			bool ok = nullptr;
			CoordTransform transform = CoordTransform::generatePrjToGeoTransform(coord, &ok);
			if (ok){

				//transform pose
				std::vector<double >xs, ys, zs;
				for (auto itr = poses.begin(); itr != poses.end(); ++itr)
				{
					DBPose &dbpose = itr->second;
					xs.push_back(dbpose.x);
					ys.push_back(dbpose.y);
					zs.push_back(dbpose.z);
				}
				int n = xs.size();
				if (n > 0){
					transform.beginTransfrom();
					transform.transform(n, xs.data(), ys.data(), zs.data());
					transform.endTransfrom();
					int ipose = 0;
					std::vector<Vec3> ell;//注意是纬度经度高程的顺序
					for (auto itr = poses.begin(); itr != poses.end(); ++itr,++ipose)
					{
						DBPose &dbpose = itr->second;
						dbpose.lon = xs[ipose];
						dbpose.lat = ys[ipose];
						dbpose.alt = zs[ipose];
						ell.push_back(Vec3(dbpose.lat, dbpose.lon, dbpose.alt));
					}
					Vec3 latLonAlt = infomation.coordinate.ENUCenter;

					if (!ell.empty())
					{//transform init pose
						ENUTransform trans(dtum);
						trans.setBaseEll(latLonAlt.x(), latLonAlt.y(), latLonAlt.z());
						//先转换成世界坐标
						GPSTransform gpsTrans(dtum);
						std::vector<Vec3> xyz;
						xyz = gpsTrans.EllToXYZ(ell);
						//再转换成ENU
						std::vector<Vec3> enu = trans.XYZToENU(xyz);
						int iPose = 0;
						for (auto itr = poses.begin(); itr != poses.end(); ++itr, ++iPose)
						{
							DBPose &dbpose = itr->second;
							dbpose.enuX = enu[iPose].x();
							dbpose.enuY = enu[iPose].y();
							dbpose.enuZ = enu[iPose].z();
						}
					}

				}

				//transform gcp
				xs.clear(); ys.clear(); zs.clear();
				std::vector<Vec3> gcpEll;//lat lon, alt
				for (auto itr = gcps.begin(); itr != gcps.end(); ++itr)
				{
					xs.push_back(itr->second.landmark.x);
					ys.push_back(itr->second.landmark.y);
					zs.push_back(itr->second.landmark.z);
				}
				n = xs.size();
				if (n > 0){
					transform.beginTransfrom();
					transform.transform(n, xs.data(), ys.data(), zs.data());
					transform.endTransfrom();
					int igcp = 0;
					std::vector<Vec3> ell;
					for (auto itr = gcps.begin(); itr != gcps.end(); ++itr, ++igcp)
					{
						DBGCP &dbgcp = itr->second;
						dbgcp.lon = xs[igcp];
						dbgcp.lat = ys[igcp];
						dbgcp.alt = zs[igcp];
						ell.push_back(Vec3(dbgcp.lat, dbgcp.lon, dbgcp.alt));
					}

					//to enu;
					Vec3 latLonAlt = infomation.coordinate.ENUCenter;

					if (!ell.empty())
					{//transform init pose
						ENUTransform trans(dtum);
						trans.setBaseEll(latLonAlt.x(), latLonAlt.y(), latLonAlt.z());
						//先转换成世界坐标
						GPSTransform gpsTrans(dtum);
						std::vector<Vec3> xyz;
						xyz = gpsTrans.EllToXYZ(ell);
						//再转换成ENU
						std::vector<Vec3> enu = trans.XYZToENU(xyz);
						igcp = 0;
						for (auto itr = gcps.begin(); itr != gcps.end(); ++itr, ++igcp)
						{
							DBGCP &dbgcp = itr->second;
							dbgcp.enuX = enu[igcp].x();
							dbgcp.enuY = enu[igcp].y();
							dbgcp.enuZ = enu[igcp].z();
						}
					}
				}
			}
			else{
				ILOG(ERROR) << "Can't transform project-coordination to geo-coordination" << IEND;
			}
		}
		else {
			std::vector<Vec3> ell;//lat lon, alt
			for (auto itr = poses.begin(); itr != poses.end(); ++itr)
			{
				//注意是纬度经度高程的顺序
				ell.push_back(Vec3(itr->second.x, itr->second.y, itr->second.z));
				itr->second.lat = itr->second.x;
				itr->second.lon = itr->second.y;
				itr->second.alt = itr->second.z;
			}
			std::vector<Vec3> gcpEll;//lat lon, alt
			for (auto itr = gcps.begin(); itr != gcps.end(); ++itr)
			{
				//注意是纬度经度高程的顺序
				itr->second.lat = itr->second.landmark.x;
				itr->second.lon = itr->second.landmark.y;
				itr->second.alt = itr->second.landmark.z;
				gcpEll.push_back(Vec3(itr->second.lat, itr->second.lon, itr->second.alt));
			}
			Vec3 latLonAlt = infomation.coordinate.ENUCenter;

			if (!ell.empty())
			{//transform init pose
				ENUTransform trans(dtum);
				trans.setBaseEll(latLonAlt.x(), latLonAlt.y(), latLonAlt.z());
				//先转换成世界坐标
				GPSTransform gpsTrans(dtum);
				std::vector<Vec3> xyz;
				xyz = gpsTrans.EllToXYZ(ell);
				//再转换成ENU
				std::vector<Vec3> enu = trans.XYZToENU(xyz);
				int iPose = 0;
				for (auto itr = poses.begin(); itr != poses.end(); ++itr, ++iPose)
				{
					DBPose &dbpose = itr->second;
					dbpose.enuX = enu[iPose].x();
					dbpose.enuY = enu[iPose].y();
					dbpose.enuZ = enu[iPose].z();
				}
			}
			if (!gcpEll.empty())
			{//transform gcp
				ENUTransform trans(dtum);
				trans.setBaseEll(latLonAlt.x(), latLonAlt.y(), latLonAlt.z());
				//先转换成世界坐标
				GPSTransform gpsTrans(dtum);
				std::vector<Vec3> xyz;
				xyz = gpsTrans.EllToXYZ(gcpEll);
				//再转换成ENU
				std::vector<Vec3> enu = trans.XYZToENU(xyz);
				int iGCP = 0;
				for (auto itr = gcps.begin(); itr != gcps.end(); ++itr, ++iGCP)
				{
					DBGCP &dbGCP = itr->second;
					dbGCP.enuX = enu[iGCP].x();
					dbGCP.enuY = enu[iGCP].y();
					dbGCP.enuZ = enu[iGCP].z();
				}
			}
		}
		return true;
	}
	else{
		ILOG(ERROR) << "Can't transform EPSG/WKT to Spatial reference" << IEND;
		return false;
	}
}
#endif

template <>
void Project::serialize(cereal::JSONInputArchive & ar, std::uint32_t const version)
{
	ar(CEREAL_NVP(infomation));
	ar(CEREAL_NVP(resource));
	if (version >= 1){
		ar(CEREAL_NVP(configData));
	}
	if (version >= 2){
		_saveBinFlag |= eSaveTrackBin;
	}
}

template <>
void Project::serialize(cereal::JSONOutputArchive & ar, std::uint32_t const version)
{
	ar(CEREAL_NVP(infomation));
	ar(CEREAL_NVP(resource));
	if (version >= 1){
		ar(CEREAL_NVP(configData));
	}

	if (version >= 2){
		_saveBinFlag |= eSaveTrackBin;
	}
}

void Project::completeDirs()
{
	projectDir = stlplus::folder_part(projectFile);
	std::string  name = stlplus::basename_part(projectFile);
	//projectDir.mkdir(name + ".datas");
	projectDataDir = stlplus::create_filespec(projectDir, name + ".datas");
// 	projectDataDir.mkdir("feats");
//	projectDataDir.mkdir("match");
//	projectDataDir.mkdir("AT");
	featsDir = stlplus::create_filespec(projectDataDir, "feats");// projectDataDir.filePath("feats");
	matchDir = stlplus::create_filespec(projectDataDir, "match");//projectDataDir.filePath("match");
	atDir = stlplus::create_filespec(projectDataDir, "AT");//projectDataDir.filePath("AT");
}

SystemConfig *global_system_config = nullptr;
SystemConfig & SystemConfig::instance()
{
	if (global_system_config == nullptr){
		global_system_config = new SystemConfig;
	}
	return *global_system_config;
}
void SystemConfig::exit()
{
	if (global_system_config) delete global_system_config;
}

std::string SystemConfig::exePath() const
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

std::string SystemConfig::configPath() const
{
	return stlplus::create_filespec(exePath(), "Config");
}

#if 0
bool SystemConfig::readSensorDatabase()
{
	std::string sensorDB = stlplus::create_filespec(configPath(), "camera_sensor_database.txt");
	return parseDatabase(sensorDB, sensorDatabase);
}

bool SystemConfig::readCoordinate()
{
	std::string geoCoord = stlplus::create_filespec(configPath(), "GEOGCS_Database.csv");
	std::string prjCoord = stlplus::create_filespec(configPath(), "PROJCS_Database.csv");
	ProjCoordinate.clear();
	GeoCoordinate.clear();
	if (parseCoordinates(ProjCoordinate, prjCoord) && parseCoordinates(GeoCoordinate, geoCoord)){
		return true;
	}
	else {
		return false;
	}
}
#endif
// bool SystemConfig::initErrorCode()
// {
// 	std::string errorCodeFile = stlplus::create_filespec(configPath(), "error_code.txt");
// 	if (NoError != InitErrorCode(errorCodeFile)){
// 		return false;
// 	}
// 	return true;
// }

// void SystemConfig::readLicenseConfig()
// {
// 	std::string licenseConfig = stlplus::create_filespec(configPath(), "licenseConfig.txt");
// 	QSettings settings(toqs(licenseConfig), QSettings::IniFormat);
// 	licenseIp = settings.value("ip", "localhost").toString();
// 	licensePort = settings.value("port", "4874").toInt();
// }
// 
// void SystemConfig::saveLicenseConfig()
// {
// 	std::string licenseConfig = stlplus::create_filespec(configPath(), "licenseConfig.txt");
// 	QSettings settings(toqs(licenseConfig), QSettings::IniFormat);
// 	settings.setValue("ip", licenseIp);
// 	settings.value("port", licensePort);
// }

// bool CoordInfomation::isProject(bool *ok) const
// {
// 	Coordinate coord;
// 	coord.EPSGName = epsg;
// 	coord.WKT = wkt;
// 	return coord.isProject(ok);
// }

#if 0
bool Project::toENUCoord(std::hash_map<uint32_t, Vec3> &importPoses)
{
	std::map<uint32_t, DBPose> &poses = imageListGen.poseList.Pose_list();
	std::vector<Vec3> ell;//lat lon, alt
	for (auto itr = poses.begin(); itr != poses.end(); ++itr)
	{
		//注意是纬度经度高程的顺序
		ell.push_back(Vec3(itr->second.y, itr->second.x, itr->second.z));
	}
	Coordinate coord;
	coord.WKT = infomation.coordinate.wkt;
	coord.EPSGName = infomation.coordinate.epsg;
	coord.CoordinateName = infomation.coordinate.name;
	OGRSpatialReference sr;
    bool	ok = Coordinate::coordToSR(coord, sr);
	if (!ok){
		ILOG(INFO) << "can't transform Geo-coordinate to ENU coordinate" << IEND;
	}
	else{
		double a = sr.GetSemiMajor();
		double b = sr.GetSemiMinor();
		double f = 1.0 / sr.GetInvFlattening();
		Datum dtum(a, b, f);
		ENUTransform trans(dtum);
		Vec3 longlatalt = infomation.coordinate.ENUCenter;
		trans.setBaseEll(longlatalt.y(), longlatalt.x(), longlatalt.z());
		//先转换成世界坐标
		GPSTransform gpsTrans(dtum);
		std::vector<Vec3> xyz;
		xyz = gpsTrans.EllToXYZ(ell);
		//再转换成ENU
		std::vector<Vec3> enu = trans.XYZToENU(xyz);

		importPoses.clear();
		int i = 0;
		CHECK(poses.size() == enu.size());
		for (auto itr = poses.begin(); itr != poses.end(); ++itr, ++i)
		{
			importPoses[itr->first] = enu[i];
		}
		ILOG(INFO) << "Geo coordinate to ENU coordinate" << IEND;
	}
	return ok;
}
#endif

INSIGHT_NAME_SPACE_END


