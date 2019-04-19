#include "db_types.h"

#include <fstream>
#include <glog/logging.h>
#include <cereal/archives/json.hpp>
#include <sstream>
//#include "exif_IO_EasyExif.hpp"
//#include "exif_IO.hpp"
#include "common/hash.h"
#include "project.h"
#include <iomanip>
//#include <ImageIO/GdalUtils.h>
//#include "common/license.h"
//#include "log/baselog.h"

using namespace cereal;
using namespace std;

INSIGHT_NAME_SPACE



bool DBImageList::saveToASCIIFile(const std::string &file) const
{
	ofstream ofs(file);
	if (!ofs.is_open()) return false;
	ofs << "#Insight image list file." << std::endl
		<< "#Version 1.1. Generate by Task" << std::endl
		<< "#Format : " << endl
		<< "#image_id;camera_id;image_name_with_ext;image_full_path_name;pose_valid;pose_t_valid,pose_r_valid" << endl;
	ofs << "1.1" << std::endl;//version
	for (auto itr = Image_list().begin(); itr != Image_list().end(); ++itr)
	{
		ofs << itr->first << ";"
			<< itr->second.camera_id << ";"
			<< itr->second.image_name << ";"
			<< itr->second.image_full_path << ";"
			<< itr->second.pose_valid << ";"
			<< itr->second.init_pose_center_valid << ";"
			<< itr->second.init_pose_rotation_valid << std::endl;
	}
	return true;
}

//version 1.1
bool DBImageList::readFromASCIIFile(const std::string &file)
{
	ifstream ifs(file);
	std::string s;
	int line = 0;
	Image_list().clear();
	bool readVersion = false;
	while (std::getline(ifs, s))
	{
		++line;
		if (s.empty()) continue;
		if (s[0] == '#') continue;//ignore #
		if (!readVersion){
			readVersion = true;
			if (s != "1.1"){
				LOG(ERROR) << "need version 1.1 at file " << file << ".line:" << line;
				return false;
			}
			continue;
		}
		vector<string> datas;
		split(s, ';', datas);
		if (datas.size() != 7)
		{
			LOG(ERROR) << "Must 7 datas but " << datas.size() << " at file " << file << ".line:" << line;
			return false;
		}
		for (int ii = 0; ii < 7; ++ii)
			datas[ii] = trim(datas[ii]);
		DBImage image;

		stringstream imagess(datas[0]);
		stringstream camerass(datas[1]);
		imagess >> image.id;
		camerass >> image.camera_id;
		image.image_name = datas[2];
		image.image_full_path = datas[3];
		stringstream ss(datas[4]);	ss >> image.pose_valid; ss.clear();
		ss.str(datas[5]); 	ss >> image.init_pose_center_valid; ss.clear();
		ss.str(datas[6]); 	ss >> image.init_pose_rotation_valid; ss.clear();
		Image_list()[image.id] = image;
	}
	return true;
}

bool DBImageList::hasImage(uint32_t image_id) const
{
	return Image_list().find(image_id) != Image_list().end();
}

bool DBImageList::hasImage(const std::string &fullPathName) const
{
	std::string path = fullPathName;
	std::transform(path.begin(), path.end(),path.begin(), tolower);

	for (auto itr = Image_list().begin(); itr != Image_list().end(); ++itr)
	{
		string full_path = itr->second.image_full_path;
		std::transform(itr->second.image_full_path.begin(), itr->second.image_full_path.end(), full_path.begin(), tolower);
		if (full_path == path) return true;
	}
	return false;
}

void DBImageList::addImage(const DBImage &dbimage)
{
	Image_list()[dbimage.id] = dbimage;
}

void DBImageList::resetUnregisted()
{
	for (auto itr = _image_list.begin(); itr != _image_list.end(); ++itr){
		itr->second.pose_valid = false;
	}
}

bool DBCameraList::saveToJson(const std::string &file) const
{
	std::ofstream ofs(file);
	JSONOutputArchive ar(ofs);
	ar(cereal::make_nvp("CameraList",_head));
	for (auto itr = _camera_list.begin(); itr != _camera_list.end(); ++itr)
	{
		stringstream ss;
		ss << itr->first;
		ar(cereal::make_nvp(ss.str(), itr->second));
	}
	return ofs.is_open();
}

bool DBCameraList::readFromJson(const std::string &file)
{
	std::ifstream ifs(file);
	JSONInputArchive ar(ifs);
	_camera_list.clear();
	ar(cereal::make_nvp("CameraList", _head));

	auto hint = _camera_list.begin();
	while (true)
	{
		const auto namePtr = ar.getNodeName();

		if (!namePtr)
			break;
		stringstream ss;
		ss.str((std::string(namePtr)));
		uint32_t cam_id;
		DBCamera cam;
		ss >> cam_id;
		ar(cam);
		hint = _camera_list.emplace_hint(hint, cam_id, std::move(cam));
	}
	return ifs.is_open();
}

#if 0
void DBImage::readExif()
{
	std::unique_ptr<openMVG::exif::Exif_IO_EasyExif> exifReader(new openMVG::exif::Exif_IO_EasyExif());
	exifReader->open(image_full_path);
	if (exifReader->doesHaveExifInfo())
	{
		exif_header = openMVG::exif::SimpleExifHeader(exifReader->exifInfo());
		exif_valid = true;
	}
	else{
		exif_valid = false;
	}
}

bool DBImage::getCameraByExif(DBCamera &camera) const
{
	camera.camera_name = exif_header.getBrand() + " " + exif_header.getModel() + " " + std::to_string(exif_header.FocalLength)
		+ "_" + std::to_string(exif_header.FocalLengthIn35mm);
	camera.w = exif_header.width;
	camera.h = exif_header.height;
	if (camera.w == 0 || camera.h == 0){
		GdalUtils::InitGDAL();
		GdalUtils::GetWidthHeightPixel(image_full_path.c_str(), camera.w, camera.h);
	}
	camera.ppx = (camera.w - 1) * 0.5;
	camera.ppy = (camera.h - 1) * 0.5;
	camera.focalmm = exif_header.FocalLength;
	camera.focal35mm = exif_header.FocalLengthIn35mm;
	float w = camera.w;
	float h = camera.h;
	if (exif_header.FocalLengthIn35mm != 0){
		//35mm size: 36 * 24 (mm) 
		//对角线：43.26661530556787mm
		double scale = exif_header.FocalLengthIn35mm / 43.26661530556787;
		double a = sqrt(w * w + h * h);
		camera.focalpx = scale * a;
		if (exif_header.FocalLength != 0.0){
			float pix_size = exif_header.FocalLength / camera.focalpx;
			double sensor_x = pix_size * w;
			double sensor_y = pix_size * h;
			camera.sensor_size_x = sensor_x;
			camera.sensor_size_y = sensor_y;
		}
		else{
			//估计sensor size，focalpx已经估计出
			Datasheet ds;
			if (getInfo(exif_header.getBrand(), exif_header.getModel(), SystemConfig::instance().sensorDatabase,
				ds)){
				float pix_size = ds._sensorSize / std::max(camera.w, camera.h);
				double sensor_x = pix_size * w;
				double sensor_y = pix_size * h;
				camera.sensor_size_x = sensor_x;
				camera.sensor_size_y = sensor_y;
			}
		}
		return true;
	}
	else{
		Datasheet ds;
		if (getInfo(exif_header.getBrand(), exif_header.getModel(), SystemConfig::instance().sensorDatabase,
			ds)){
			float pix_size = ds._sensorSize / std::max(camera.w, camera.h);
			double sensor_x = pix_size * w;
			double sensor_y = pix_size * h;
			camera.sensor_size_x = sensor_x;
			camera.sensor_size_y = sensor_y;
			if (camera.focalmm != 0.0){
				camera.focalpx = camera.focalmm / pix_size;
			}
			return true;
		}
	}
	return true;
}

std::string DBImage::cameraHashCode() const
{
	size_t seed = 0;
	hash_combine(seed, exif_header.width);
	hash_combine(seed, exif_header.height);
	hash_combine(seed, exif_header.FocalLength);
	hash_combine(seed, exif_header.FocalLengthIn35mm);
	std::stringstream ss;
	ss << exif_header.getBrand()
		<< exif_header.getModel()
		<< seed;
	return ss.str();
}
#endif

bool DBPoseList::saveToASCIIFile(const std::string &file) const
{
	ofstream ofs(file);
	ofs << std::fixed << std::setprecision(10);
	if (!ofs.is_open()) return false;
	ofs << "#Insight pose list file." << std::endl
		<< "#Version 1.0. Generate by Task" << std::endl
		<< "#Format : " << endl
		<< "#image_id;x;y;z;omega;phi;kappa;weight_x,weight_y,weight_z" << endl;
	ofs << "version;" << VERSION << std::endl;
	for (auto itr = _pose_list.begin(); itr != _pose_list.end(); ++itr)
	{
		ofs << itr->first << ";"
			<< itr->second.x << ";"
			<< itr->second.y << ";"
			<< itr->second.z << ";"
			<< itr->second.omega << ";"
			<< itr->second.phi << ";"
			<< itr->second.kappa << ";"
			<< itr->second.weight_x << ";"
			<< itr->second.weight_y << ";"
			<< itr->second.weight_z << std::endl;
	}
	return true;
}

bool DBPoseList::readFromASCIIFile(const std::string &file)
{
	ifstream ifs(file);
	std::string s;
	int line = 0;
	_pose_list.clear();
	int version = -1;
	while (std::getline(ifs, s))
	{
		++line;
		if (s.empty()) continue;
		if (s[0] == '#') continue;//ignore #

		vector<string> datas;
		split(s, ';', datas);
		if (version == -1){
			if (datas.size() != 2){
				LOG(ERROR) << "Must 2 datas but " << datas.size() << " at file " << file << ".line:" << line;
				return false;
			}
			for (int ii = 0; ii < 2; ++ii)
				datas[ii] = trim(datas[ii]);
			if (datas[0] != "version"){
				LOG(ERROR) << "Can't find version " << datas.size() << " at file " << file << ".line:" << line;
				return false;
			}
			stringstream ss(datas[1]);
			ss >> version;
			continue;
		}
		if (version == 0){
			if (datas.size() != 7)
			{
				LOG(ERROR) << "Must 7 datas but " << datas.size() << " at file " << file << ".line:" << line;
				return false;
			}
		}
		else if (version == 1){
			if (datas.size() != 10)
			{
				LOG(ERROR) << "Must 10 datas but " << datas.size() << " at file " << file << ".line:" << line;
				return false;
			}
		}

		for (int ii = 0; ii < datas.size(); ++ii)
			datas[ii] = trim(datas[ii]);
		DBPose pose;

		stringstream image_id(datas[0]);
		stringstream x(datas[1]);
		stringstream y(datas[2]);
		stringstream z(datas[3]);
		stringstream roll(datas[4]);
		stringstream pitch(datas[5]);
		stringstream yaw(datas[6]);

		image_id >> pose.image_id;
		x >> pose.x;
		y >> pose.y;
		z >> pose.z;
		roll >> pose.omega;
		pitch >> pose.phi;
		yaw >> pose.kappa;
		if (version == 1){
			stringstream wx(datas[7]);
			stringstream wy(datas[8]);
			stringstream wz(datas[9]);
			wx >> pose.weight_x;
			wy >> pose.weight_y;
			wz >> pose.weight_z;
		}
		_pose_list[pose.image_id] = pose;
	}
	return true;
}

bool DBTrackList::saveToAsciiFile(const std::string &file) const
{
	ofstream ofs(file);
	ofs << std::fixed << std::setprecision(10);
	if (!ofs.is_open()) return false;
	ofs << "#Insight track list file." << std::endl
		<< "#Version 1.0. Generate by Task" << std::endl
		<< "#Format : " << endl
		<< "#track_id;x;y;z" << endl
		<< "#view_id;u;v;view_id;u;v..." << endl;
	ofs << "version;" << VERSION << std::endl;

	for (auto itr = _track_lsit.begin(); itr != _track_lsit.end(); ++itr)
	{
		const DBTrack &track = itr->second;
		ofs << track.track_id << ";" << track.landmark.x << ";" << track.landmark.y << ";"
			<< track.landmark.z;
		if (VERSION == 1){
			ofs << ";" << (int)track.landmark.r
				<< ";" << (int)track.landmark.g
				<< ";" << (int)track.landmark.b << std::endl;
		}
		else {
			ofs << std::endl;
		}
		for (int i = 0; i < track.views.size(); ++i){
			ofs << track.views[i].image_id << ";" <<
				track.views[i].u << ";" << track.views[i].v;
			if (i !=int( track.views.size()) - 1){
				ofs << ";";
			}
		}
		ofs << std::endl;
	}
	return true;
}

bool DBTrackList::readFromAsciiFile(const std::string &file)
{
	ifstream ifs(file);
	std::string s;
	int line = 0;
	_track_lsit.clear();
	int version = -1;
	while (std::getline(ifs, s))
	{
		++line;
		if (s.empty()) continue;
		if (s[0] == '#') continue;//ignore #
		if (version == -1){
			vector<string> data;
			split(s, ';', data);
			if (data.size() != 2){
				LOG(ERROR) << "Must 2 datas but " << data.size() << " at file " << file << ".line:" << line;
				return false;
			}
			for (int ii = 0; ii < 2; ++ii)
				data[ii] = trim(data[ii]);
			if (data[0] != "version"){
				LOG(ERROR) << "No version find at file " << file << ".line:" << line;
				return false;
			}
			stringstream ss;
			ss << data[1];
			ss >> version;
			continue;
		}
		std::string sviews;
		std::getline(ifs, sviews);

		vector<string> Xs; vector<string>Vs;
		split(s, ';', Xs);
		split(sviews, ';', Vs);
		if (version == 1 && Xs.size() != 7)
		{
			LOG(ERROR) << "Must 7 datas but " << Xs.size() << " at file " << file << ".line:" << line;
			return false;
		}
		if (Vs.size() % 3 != 0){
			LOG(ERROR) << "Must 3 data per view but  at file " << file << ".line:" << line;
			return false;
		}
		for (int ii = 0; ii < Xs.size(); ++ii)
			Xs[ii] = trim(Xs[ii]);
		for (int ii = 0; ii < Vs.size(); ++ii)
			Vs[ii] = trim(Vs[ii]);
		DBTrack track;
	
		stringstream ss;
		ss.str(Xs[0]);	ss >> track.track_id;
		ss.clear(); 
		ss.str(Xs[1]); ss >> track.landmark.x;
		ss.clear(); 
		ss.str(Xs[2]); ss >> track.landmark.y; 
		ss.clear(); 
		ss.str(Xs[3]); ss >> track.landmark.z;
		ss.clear();
		if (version == 1){
			int r = 0, g = 0, b = 0;
			ss.str(Xs[4]);	ss >> r;  ss.clear();
			ss.str(Xs[5]);	ss >> g; ss.clear();
			ss.str(Xs[6]);	ss >> b; ss.clear();
			track.landmark.r = r;
			track.landmark.g = g;
			track.landmark.b = b;
		}
		int viewCount = Vs.size() / 3;
		track.views.resize(viewCount);
		for (int ii = 0; ii < viewCount; ++ii){
			DBTrack::V &v = track.views[ii];
			ss.str(Vs[3 * ii]); ss >> v.image_id; ss.clear();
			ss.str(Vs[3 * ii + 1]); ss >> v.u; ss.clear();
			ss.str(Vs[3 * ii + 2]); ss >> v.v; ss.clear();
		}
		_track_lsit[track.track_id] = track;
	}
	return true;
}

bool DBTrackList::readFromBinFile(const std::string &file) 
{
	ifstream ifs(file, std::ios::in | std::ios::binary);
	if (!ifs.is_open()) return false;

	_track_lsit.clear();
	int version = -1;
	ifs.read((char*)(&version), sizeof(int));
	int nTrack = 0;
	ifs.read((char*)(&nTrack), sizeof(int));

	for (int i = 0; i < nTrack; ++i)
	{
		DBTrack track;
		ifs.read((char*)&track.track_id, sizeof(int));
		ifs.read((char*)&track.landmark.x, sizeof(double));
		ifs.read((char*)&track.landmark.y, sizeof(double));
		ifs.read((char*)&track.landmark.z, sizeof(double));
		if (version == 1){
			int R = 0;//(int)
			int G = 0;//(int);
			int B = 0;//(int);

			ifs.read((char*)&R, sizeof(int));
			ifs.read((char*)&G, sizeof(int));
			ifs.read((char*)&B, sizeof(int));
			track.landmark.r = R;
			track.landmark.g = G;
			track.landmark.b = B;
		}
		int nView = 0; 
		ifs.read((char*)&nView, sizeof(int));

		track.views.resize(nView);
		for (int i = 0; i < track.views.size(); ++i){
			ifs.read((char*)&track.views[i].image_id, sizeof(int));
			ifs.read((char*)&track.views[i].u, sizeof(float));
			ifs.read((char*)&track.views[i].v, sizeof(float));
		}
		_track_lsit[track.track_id] = track;
	}
	return true;
}

bool insight::DBTrackList::saveToBinFile(const std::string &file) const
{
	ofstream ofs(file, std::ios::out | std::ios::binary | std::ios::trunc);
	ofs << std::fixed << std::setprecision(10);
	if (!ofs.is_open()) return false;

	int version = VERSION;
	ofs.write((char*)(&version), sizeof(int));
	int nTrack = _track_lsit.size();
	ofs.write((char*)(&nTrack), sizeof(int));

	for (auto itr = _track_lsit.begin(); itr != _track_lsit.end(); ++itr)
	{
		const DBTrack &track = itr->second;
		ofs.write((char*)&track.track_id, sizeof(int));
		ofs.write((char*)&track.landmark.x, sizeof(double));
		ofs.write((char*)&track.landmark.y, sizeof(double));
		ofs.write((char*)&track.landmark.z, sizeof(double));

		if (VERSION == 1){
			int R = (int)track.landmark.r;
			int G = (int)track.landmark.g;
			int B = (int)track.landmark.b;

			ofs.write((char*)&R, sizeof(int));
			ofs.write((char*)&G, sizeof(int));
			ofs.write((char*)&B, sizeof(int));
		}

		int nView = track.views.size();
		ofs.write((char*)&nView, sizeof(int));

		for (int i = 0; i < track.views.size(); ++i){
			ofs.write((char*)&track.views[i].image_id, sizeof(int));
			ofs.write((char*)&track.views[i].u, sizeof(float));
			ofs.write((char*)&track.views[i].v, sizeof(float));
		}
	}
	return true;
}

bool DBGCPList::saveToAsciiFile(const std::string &file) const
{
	ofstream ofs(file);
	ofs << std::fixed << std::setprecision(10);
	if (!ofs.is_open()) return false;
	ofs << "#Insight gcp list file." << std::endl
		<< "#Version 1.1. Generate by Task" << std::endl
		<< "#Format : " << endl
		<< "#name;type;weight;enabled;track_id;x;y;z;" << endl
		<< "#view_id;u;v;enabled;view_id;u;v;enabled..." << endl;
	for (auto itr = _gcp_lsit.begin(); itr != _gcp_lsit.end(); ++itr)
	{
		const DBGCP &track = itr->second;
		ofs << track.name << ";" << track.type << ";" << track.weight << ";" << track.enabled << ";"
			<< track.track_id << "; " << track.landmark.x << "; " << track.landmark.y << "; "
			<< track.landmark.z << std::endl;
		int i = 0;
		for (auto itr = track.views.begin(); itr != track.views.end(); ++itr,++i){
			ofs << itr->second.image_id << ";" <<
				itr->second.u << ";" << itr->second.v << ";" << itr->second.enabled;
			if (i != int(track.views.size()) - 1){
				ofs << ";";
			}
		}
		ofs << std::endl;
	}
	return true;
}

bool DBGCPList::readFromAsciiFile(const std::string &file)
{
	ifstream ifs(file);
	std::string s;
	int line = 0;
	_gcp_lsit.clear();
	while (std::getline(ifs, s))
	{
		++line;
		if (s.empty()) continue;
		if (s[0] == '#') continue;//ignore #
		std::string sviews;
		std::getline(ifs, sviews);

		vector<string> Xs; vector<string>Vs;
		split(s, ';', Xs);
		split(sviews, ';', Vs);
		if (Xs.size() != 8)
		{
			LOG(ERROR) << "Must 8 datas but " << Xs.size() << " at file " << file << ".line:" << line;
			return false;
		}
		if (Vs.size() % 4 != 0){
			LOG(ERROR) << "Must 4 data per view but  at file " << file << ".line:" << line;
			return false;
		}
		for (int ii = 0; ii < 8; ++ii)
			Xs[ii] = trim(Xs[ii]);
		for (int ii = 0; ii < Vs.size(); ++ii)
			Vs[ii] = trim(Vs[ii]);
		DBGCP track;

		stringstream ss;
		track.name = Xs[0];
		ss.str(Xs[1]); ss >> track.type; ss.clear();
		ss.str(Xs[2]); ss >> track.weight; ss.clear();
		ss.str(Xs[3]); ss >> track.enabled; ss.clear();
		ss.str(Xs[4]);	ss >> track.track_id; ss.clear();
		ss.str(Xs[5]); ss >> track.landmark.x; ss.clear(); 
		ss.str(Xs[6]); ss >> track.landmark.y; ss.clear(); 
		ss.str(Xs[7]); ss >> track.landmark.z; ss.clear();
		int viewCount = Vs.size() / 4;
		for (int ii = 0; ii < viewCount; ++ii){
			DBGCP::V v;
			ss.str(Vs[4 * ii]); ss >> v.image_id; ss.clear();
			ss.str(Vs[4 * ii + 1]); ss >> v.u; ss.clear();
			ss.str(Vs[4 * ii + 2]); ss >> v.v; ss.clear();
			ss.str(Vs[4 * ii + 3]); ss >> v.enabled; ss.clear();
			track.views[v.image_id] = v;
		}
		_gcp_lsit[track.track_id] = track;
	}
	return true;
}

#if 0
//TODO 优化速度
int ImageListGenerator::importImages(const std::set<std::string> &imageFiles, const KeyType &cameraKey, Resource *rc)
{
	//check registered
	int nHave = imageList.Image_list().size();
 	int imageCount = 500;
 	imageCount = std::numeric_limits<int>::max();
// 	if (nHave > imageCount) return 0;
	int n = 0;
	for (auto itr = imageFiles.begin(); itr != imageFiles.end(); ++itr){
		if (imageList.hasImage(*itr)) continue;
		if (n + nHave > imageCount) return n;
		DBImage image;
		image.id = rc->imageSeed.generate();
		image.image_full_path = *itr;
		image.camera_id = cameraKey;
		toLowerStr(image.image_full_path);
		image.image_name = stlplus::filename_part(*itr);
		image.readExif();

		poseList.Pose_list()[image.id].image_id = image.id;//add new db pose
		poseATList.Pose_list()[image.id].image_id = image.id;//add new db pose
		if (image.exif_valid){
			DBPose &dbPose = poseList.Pose_list()[image.id];
			if (image.exif_header.GeoLocation.Longitude == std::numeric_limits<double>::infinity()
				|| image.exif_header.GeoLocation.Latitude == std::numeric_limits<double>::infinity()
				|| image.exif_header.GeoLocation.Altitude == std::numeric_limits<double>::infinity()){
			}
			else{
				dbPose.x = image.exif_header.GeoLocation.Longitude;
				dbPose.y = image.exif_header.GeoLocation.Latitude;
				dbPose.z = image.exif_header.GeoLocation.Altitude;
				image.init_pose_center_valid = true;
			}
		}
		imageList.addImage(image);
		++n;
	}
	return n;
}

#endif
INSIGHT_NAME_SPACE_END


