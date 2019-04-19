

#ifndef Project_h__
#define Project_h__
#include "tasklist_global.h"

#include "db_types.h"
#include <string>
#include <cstdint>
#include <cereal/cereal.hpp>
//#include <QDir>
#include <unordered_map>
#include "data_sheet.h"
#include "common/Coordinates.h"

//////////////////////////////////////////////////////////////////////////
INSIGHT_NAME_SPACE

struct TASKLIST_EXPORT CoordInfomation
{
	CoordInfomation(){
		ENUCenter = Vec3(0, 0, 0);
	}
	std::string name;
	std::string epsg;
	std::string wkt;

	bool localSystem = true;//Ĭ��Ϊ�ֲ�����ϵ

	Vec3 ENUCenter;//ENU����ϵ���ģ�γ�ȣ����Ⱥ͸߳�

	template <class Archive>
	void serialize(Archive & ar, std::uint32_t const version)
	{
		ar(CEREAL_NVP(name),
			CEREAL_NVP(epsg),
			CEREAL_NVP(wkt));
		if (version >= 2){
			ar(CEREAL_NVP(localSystem));
		}
		if (version >= 3){
			ar(cereal::make_nvp("ENU_X", ENUCenter.x()));
			ar(cereal::make_nvp("ENU_Y", ENUCenter.y()));
			ar(cereal::make_nvp("ENU_Z", ENUCenter.z()));
		}
	}

	//bool isProject(bool *ok = nullptr) const;
};

struct ProjectInfomation
{
	enum {
		UNKNOWN_ALTITUTE = 0
	};
	enum {
		Aerial = 0,
		Object = 1,
	};
	std::string name;
	std::string description;
	std::string date;
	std::string author;
	int type = Aerial;
	float relativeFlightAltitude = UNKNOWN_ALTITUTE;
	float averageElevationOfGround = UNKNOWN_ALTITUTE;
	CoordInfomation coordinate;

	template <class Archive>
	void serialize(Archive & ar, std::uint32_t const version)
	{
		// You can choose different behaviors depending on the version
		// This is useful if you need to support older variants of your codebase
		// interacting with newer ones
		ar(CEREAL_NVP(name));
		ar(CEREAL_NVP(description));
		ar(CEREAL_NVP(type));
		if (version >=2){
			ar(CEREAL_NVP(date));
			ar(CEREAL_NVP(author));
		}
		if (type == Aerial){
			ar(CEREAL_NVP(relativeFlightAltitude));
		}
		if (version == 3){
			ar(CEREAL_NVP(averageElevationOfGround));
			ar(CEREAL_NVP(coordinate));
		}
	}

};
/*
һЩ������Ϣ
*/
struct ProjectConfigData
{
	float sfmScale = 1.0f;

	template <class Archive>
	void serialize(Archive & ar, std::uint32_t const version)
	{
		ar(CEREAL_NVP(sfmScale));
	}
};
struct ImageConsistency
{
	KeyType imageId = UndefinedKey;
	KeyType cameraId = UndefinedKey;
	int imageW = 0;
	int imageH = 0;
	int cameraW = 0;
	int cameraH = 0;
	bool imageExist = false;
	bool imageCanRead = false;
	inline bool isOK()const {
		if (imageId != UndefinedKey &&
			cameraId != UndefinedKey &&
			imageW != 0 &&
			imageH != 0 &&
			imageW == cameraW&&
			imageH == cameraH &&
			imageExist &&
			imageCanRead){
			return true;
		}
		return false;
	}
};
class TASKLIST_EXPORT Project
{
public:
	enum SaveFlag
	{
		eSaveImageAndCameras = 0x01,
		eSaveInitPose = 0x02,
		eSaveATResult = 0x04,
		eSaveGCP = 0x08,
		eSaveAll = eSaveImageAndCameras | eSaveInitPose | eSaveATResult | eSaveGCP,
	};
	enum SaveBinFlag
	{
		eSaveTrackBin = 0x01,
	};


	Project();
	bool openProject(const std::string &projectFile);
	bool getProjectInfomation(const std::string &projectFile);
//	bool createProject(const ProjectInfomation &info, const QString &path);
	bool saveProject(SaveFlag flag = eSaveAll);
	//����camera id Ϊ undefined ��image����ͨ��exif����ȡcamera��Ϣ
#if 0
	void generateCameraByExif();

	void getCameraFromImageWH();//camera������ͼ���W��H��Ϣ��ʼ�����



		//���ͼ���ߺ͹�����camera���һ���ԣ�ͼ���Ƿ�ɶ���
	void checkConsistency(std::vector<ImageConsistency> &result);
	bool updateENUCoord();
#endif
	void setSaveBinFlag(int flag){ _saveBinFlag = flag; }
#if 0
	bool toENUCoord(std::hash_map<uint32_t, Vec3> &importPoses);
#endif
	ImageListGenerator imageListGen;
	DBCameraList cameraList;
	DBTrackList trackList;//�����������ӵ�
	DBGCPList  gcpList;//���Ƶ�

	ProjectInfomation infomation;
	std::string projectFile;//�����ļ�
	std::string projectDir;//�����ļ�Ŀ¼
	std::string projectDataDir;//��������Ŀ¼
	std::string featsDir;
	std::string matchDir;
	std::string atDir;
	Resource resource;//ȫ����Դ������id��
	ProjectConfigData configData;

	template <class Archive>
	void serialize(Archive & ar, std::uint32_t const version);
private:
	void completeDirs();//��ȫ����·����
	
	int _saveBinFlag = 0;
};

class TASKLIST_EXPORT SystemConfig
{
public:
	static SystemConfig &instance();
	static void exit();

	std::string exePath() const;
	std::string configPath() const;
//	bool readSensorDatabase();
	//bool readCoordinate();
	//bool initErrorCode();
	//void readLicenseConfig();
	//void saveLicenseConfig();
	std::vector<Datasheet> sensorDatabase;
	std::vector<Coordinate> ProjCoordinate;
	std::vector<Coordinate> GeoCoordinate;
	std::string licenseIp = "localhost";
	int licensePort = 4874;
private:
	SystemConfig(){}
	SystemConfig(SystemConfig &);
	SystemConfig &operator=(SystemConfig &);
	
};
INSIGHT_NAME_SPACE_END

CEREAL_CLASS_VERSION(insight::ProjectInfomation, 3);
CEREAL_CLASS_VERSION(insight::CoordInfomation, 3);
CEREAL_CLASS_VERSION(insight::ProjectConfigData, 1);
CEREAL_CLASS_VERSION(insight::Project, 2);

#endif // Project_h__