#ifndef TASK_LIST_IMAGE_LIST_H
#define TASK_LIST_IMAGE_LIST_H


#include "tasklist_global.h"
#include "stlplus3/file_system.hpp"
#include "Common/string_utils.h"
#include "cereal/cereal.hpp"
#include <glog/logging.h>
//#include "exif_IO.hpp"

#include <map>
#include <cstdint>
#include <string>
#include <vector>
#include <set>

INSIGHT_NAME_SPACE

typedef uint64_t image_pair_t; //match pair id

struct VersionHead
{
	int version = 1;
	template <class Archive>
	void serialize(Archive & ar, std::uint32_t const ver)
	{
		ar(CEREAL_NVP(version));
	}
};

typedef uint32_t KeyType;

// Each image pair gets a unique ID, see `Database::ImagePairToPairId`.

struct PriKey
{
	KeyType seed = 0;
	KeyType generate(){ return seed++; }
	void reset(){ seed = 0; }
};

static const KeyType UndefinedKey = std::numeric_limits<KeyType>::max();

struct DBCamera;
//simple object for IO
struct TASKLIST_EXPORT DBImage
{
	KeyType id = UndefinedKey;
	KeyType camera_id = UndefinedKey;
	std::string image_name;//影像名，包括扩展名
	std::string image_full_path;//包括影像名

	bool pose_valid = false;//pose 是否有效
	bool init_pose_center_valid = false;//pose初始值是否有效
	bool init_pose_rotation_valid = false;//pose初始值是否有效

	bool exif_valid = false;
	//openMVG::exif::SimpleExifHeader exif_header;//exif 头信息

#if 0
	void readExif();

	std::string cameraHashCode() const;

	bool getCameraByExif(DBCamera &camera) const;

#endif
};

class TASKLIST_EXPORT DBImageList
{
public:
	const std::map<uint32_t, DBImage> &Image_list() const { return _image_list; }
	std::map<uint32_t, DBImage> &Image_list() { return _image_list; }

	bool saveToASCIIFile(const std::string &file) const;
	bool readFromASCIIFile(const std::string &file);
	void clear(){ Image_list().clear(); }
	bool hasImage(uint32_t image_id) const;
	bool hasImage(const std::string &fullPathName)const;
	void addImage(const DBImage &dbimage);
	void resetUnregisted();
private:
	std::map<uint32_t, DBImage> _image_list;
};

struct DBCamera
{
	enum DISTORT_TYPE
	{
		eAddDistort = 0,
		eRemoveDistort = 1//not implement yet
	};
	KeyType id = UndefinedKey;
	std::string camera_name;
	int w = 0;
	int h = 0;
	float focalmm = 0.f;
	float focal35mm = 0.f;
	float sensor_size_x = 0.f;
	float sensor_size_y = 0.f;
	int distort_type = eAddDistort;

	float focalpx = 0.f;
	float ppx = 0.f;
	float ppy = 0.f;
	float k1 = 0.f;
	float k2 = 0.f;
	float k3 = 0.f;
	float p1 = 0.f;
	float p2 = 0.f;
	float b1 = 0.f;
	float b2 = 0.f;
	float gps_offset_x = 0.f;
	float gps_offset_y = 0.f;
	float gps_offset_z = 0.f;

	std::string exif_hash_code;
	struct AdjustFlag
	{
		bool f = false;
		bool ppxy = false;
		bool k1 = false;
		bool k2 = false;
		bool k3 = false;
		bool p1 = false;
		bool p2 = false;
		bool b1 = false;
		bool b2 = false;
		void getParams(std::vector<bool> &params) const{
			params.resize(11, false);
			params[0] = f;
			params[1] = false;// focal ratio always constant
			params[2] = ppxy;
			params[3] = ppxy;
			params[4] = k1;
			params[5] = k2;
			params[6] = k3;
			params[7] = p1;
			params[8] = p2;
			params[9] = b1;
			params[10] = b2;
		}
		void getConstantParams(std::vector<int> &constantParams) const{
			std::vector<bool> flags;
			getParams(flags);
			constantParams.clear();
			for (int i = 0; i < flags.size(); ++i)
			{
				if (!flags[i]){
					constantParams.push_back(i);
				}
			}
		}
		bool adjustAny() const{
			return (f || ppxy || k1 || k2 || k3
				|| p1 || p2 || b1 || b2);
		}

		template <class Archive>
		void serialize(Archive & ar, std::uint32_t const version)
		{
			ar(CEREAL_NVP(f));
			ar(CEREAL_NVP(ppxy));
			ar(CEREAL_NVP(k1));
			ar(CEREAL_NVP(k2));
			ar(CEREAL_NVP(k3));
			ar(CEREAL_NVP(p1));
			ar(CEREAL_NVP(p2));
			ar(CEREAL_NVP(b1));
			ar(CEREAL_NVP(b2));
		}
	};

	struct GPSAdjustFlag
	{
		bool b_adjust = false;
		float x_weight = 1.f;
		float y_weight = 1.f;
		float z_weight = 1.f;
		template <class Archive>
		void serialize(Archive & ar, std::uint32_t const version)
		{
			if (version > 0){
				ar(CEREAL_NVP(b_adjust));
				ar(CEREAL_NVP(x_weight));
				ar(CEREAL_NVP(y_weight));
				ar(CEREAL_NVP(z_weight));
			}
			else{
				// do something else
				CHECK(false) << "not implement version " << version;
			}
		}
	};
	AdjustFlag adjustFlag;
	GPSAdjustFlag gpsAdjustFlag;

	template <class Archive>
	void serialize(Archive & ar, std::uint32_t const version)
	{
		ar(CEREAL_NVP(id));
		ar(CEREAL_NVP(camera_name));
		ar(CEREAL_NVP(w));
		ar(CEREAL_NVP(h));
		ar(CEREAL_NVP(focalmm));
		ar(CEREAL_NVP(focal35mm));
		ar(CEREAL_NVP(sensor_size_x));
		ar(CEREAL_NVP(sensor_size_y));
		ar(CEREAL_NVP(distort_type));
		ar(CEREAL_NVP(focalpx));
		ar(CEREAL_NVP(ppx));
		ar(CEREAL_NVP(ppy));
		ar(CEREAL_NVP(k1));
		ar(CEREAL_NVP(k2));
		ar(CEREAL_NVP(k3));
		ar(CEREAL_NVP(p1));
		ar(CEREAL_NVP(p2));
		ar(CEREAL_NVP(b1));
		ar(CEREAL_NVP(b2));
		ar(CEREAL_NVP(gps_offset_x));
		ar(CEREAL_NVP(gps_offset_y));
		ar(CEREAL_NVP(gps_offset_z));
		ar(CEREAL_NVP(adjustFlag));
		if (version >= 2){
			ar(CEREAL_NVP(exif_hash_code));
		}
	}
};

/*
DBPose 采用的旋转矩阵均是OmegaPhiKappaForward格式，
Omega-x，Phi-y ,Kappa-z,且z轴方向朝着摄像机前面，x一般为图像右方向，y为下方向
*/
struct DBPose
{
	KeyType image_id = UndefinedKey;
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;

	double omega = 0.0;//x omega
	double phi = 0.0;//y phi
	double kappa = 0.0;//z kappa

	//gps 权值，平差使用
	float weight_x = 1.0;
	float weight_y = 1.0;
	float weight_z = 1.0;


	//大地坐标和ENU坐标为实时计算的,如果为本地坐标，则ENU坐标与本地坐标相同，大地坐标未定义
	//大地坐标
	double lat = 0.0;
	double lon = 0.0;
	double alt = 0.0;
	//DNU坐标系下的坐标 
	double enuX = 0.0;
	double enuY = 0.0;
	double enuZ = 0.0;
};

// struct DBInitPose : public DBPose
// {
// 
// };


class TASKLIST_EXPORT DBCameraList
{
public:
	bool saveToJson(const std::string &file) const;
	bool readFromJson(const std::string &file);

	const std::map<uint32_t, DBCamera> &Camera_list() const { return _camera_list; }
	std::map<uint32_t, DBCamera> &Camera_list()  { return _camera_list; }

private:
	std::map<uint32_t, DBCamera> _camera_list;
	VersionHead _head;
};


/*
每一个图像有一个pose，所以pose id就是image id
*/
class TASKLIST_EXPORT DBPoseList
{
public:
	enum {
		VERSION = 1
	};
	std::map<uint32_t, DBPose> &Pose_list() { return _pose_list; }
	const std::map<uint32_t, DBPose> &Pose_list() const { return _pose_list; }

	bool saveToASCIIFile(const std::string &file) const;
	bool readFromASCIIFile(const std::string &file);
private:
	std::map<uint32_t, DBPose> _pose_list;
	
};

struct DBTrack
{
public:
	struct X{
		double x = 0;
		double y = 0;
		double z = 0;
		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
	};

	struct V{
		KeyType image_id = UndefinedKey;
		//u v coordinate of image
		float u = 0;
		float v = 0;
	};
	KeyType track_id;
	X landmark;
	std::vector<V> views;
};

class TASKLIST_EXPORT DBTrackList
{
public:
	enum {
		VERSION = 1,
	};
	const std::map<uint32_t, DBTrack> &Track_lsit() const { return _track_lsit; }
	std::map<uint32_t, DBTrack> &Track_lsit() { return _track_lsit; }

	bool saveToAsciiFile(const std::string &file) const;
	bool readFromAsciiFile(const std::string &file);

	bool saveToBinFile(const std::string &file) const;
	bool readFromBinFile(const std::string &file) ;
private:
	std::map<uint32_t, DBTrack> _track_lsit;
};

struct DBGCP 
{
	enum TYPE
	{
		GCP_CHECK = 0,//检查点
		GCP_CONTROL = 1,//控制点
	};
	struct X{
		double x = 0;
		double y = 0;
		double z = 0;
	};

	struct V{
		KeyType image_id = UndefinedKey;
		//u v coordinate of image
		float u = 0;
		float v = 0;
		bool enabled = false;
	};
	KeyType track_id;
	X landmark;
	//std::vector<V> views;
	std::map<KeyType, V> views;
	std::string name;//控制点名称
	int type;//检查点还是控制点
	double weight = 200;//weights
	int enabled = 1;//是否采用

	//大地坐标和ENU坐标为实时计算的,如果为本地坐标，则ENU坐标与本地坐标相同，大地坐标未定义
	//大地坐标
	double lat = 0.0;
	double lon = 0.0;
	double alt = 0.0;
	//DNU坐标系下的坐标 
	double enuX = 0.0;
	double enuY = 0.0;
	double enuZ = 0.0;
};

class TASKLIST_EXPORT DBGCPList
{
public:
	const std::map<uint32_t, DBGCP> &GCP_List() const { return _gcp_lsit; }
	std::map<uint32_t, DBGCP> &GCP_List() { return _gcp_lsit; }

	bool saveToAsciiFile(const std::string &file) const;
	bool readFromAsciiFile(const std::string &file);
private:
	std::map<uint32_t, DBGCP> _gcp_lsit;
};
struct Resource
{
	PriKey imageSeed;
	PriKey cameraSeed;
	PriKey gcpSeed;
	Resource(){
		reset();
	}
	void reset(){
		cameraSeed.reset();
		imageSeed.reset();
		gcpSeed.reset();
	}

	template <class Archive>
	void serialize(Archive & ar, std::uint32_t const version)
	{
		// You can choose different behaviors depending on the version
		// This is useful if you need to support older variants of your codebase
		// interacting with newer ones
		ar(cereal::make_nvp("image_id", imageSeed.seed));
		ar(cereal::make_nvp("camera_id", cameraSeed.seed));
		if (version >= 2){
			ar(cereal::make_nvp("gcp_id", gcpSeed.seed));
		}
	}

};


class TASKLIST_EXPORT ImageListGenerator
{
public:

#if 0
	int importImages(const std::set<std::string> &imageFiles,const KeyType &cameraKey,Resource *rc);

#endif

	void removeImages(const std::set<KeyType> &imageFiles){
		for (auto itr = imageFiles.begin(); itr != imageFiles.end(); ++itr){
			imageList.Image_list().erase(*itr);
			poseList.Pose_list().erase(*itr);
			poseATList.Pose_list().erase(*itr);
		}
	}
	DBImageList imageList;
	DBPoseList poseList;//init pose imported by user
	DBPoseList poseATList;//pose
	void clear(){
		imageList.clear();
		poseList.Pose_list().clear();
		poseATList.Pose_list().clear();
	}
};


INSIGHT_NAME_SPACE_END

CEREAL_CLASS_VERSION(insight::VersionHead, 1);
CEREAL_CLASS_VERSION(insight::Resource, 2);
CEREAL_CLASS_VERSION(insight::DBCamera::AdjustFlag, 1);
CEREAL_CLASS_VERSION(insight::DBCamera::GPSAdjustFlag, 1);
CEREAL_CLASS_VERSION(insight::DBCamera, 2);
#endif//TASK_LIST_IMAGE_LIST_H
