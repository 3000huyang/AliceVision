//#ifndef qp_image_utils_h__
//#define qp_image_utils_h__
//
//#include <string>
//#include <vector>
//
//#include "qp/qp_global.h"
//#include "qp/core/types.h"
//#include "qp/imgutils/data_sheet.h"
//#include "easyexif/exif.h"
//#include "qp/core/simple_exif_header.h"
//
//namespace qp
//{
//	namespace image_utils
//	{
//		enum EExif
//		{
//			eNoExif = 0,
//			eFocal = 1,
//			eFocal_Sensor = 2
//		};
//
//
//		struct ImageBlock
//		{
//			Eigen::Vector2i leftTop;
//			int width;
//			int height;
//			std::vector<unsigned char> vec_imageData;
//		};
//
//		struct ImageInfo
//		{
//			ImageInfo(const std::string &s_image_file = ""){
//				ui_width = 0;
//				ui_height = 0;
//				haveExif = false;
//				haveSensorSize = false;
//				sennsor_size = -1.0;
//			}
//
//			bool readExif() ;
//
//			void detectSensorSize(const std::vector<qp::image_utils::Datasheet> &vec_database);
//
//			std::string brand() const;
//			std::string model() const;
//			double focal() const;
//			double focal35mm() const;
//			
//			std::string s_image_file;
//			IndexT ui_width ;
//			IndexT ui_height ;
//			EXIFInfo exifInfo;
//			bool haveExif;
//			bool haveSensorSize;
//			double sennsor_size;
//		};
//
//		QP_API bool readWidthAndHeightGDAL(const std::string &s_img, IndexT &w, IndexT &h);
//
//// 		QP_API bool readRGBImageGDAL(const std::string &s_img, int &width, int &height, std::vector<unsigned char> &vec_imageData);
//// 
//// 		QP_API bool readRGBImageGDALBlocks(const std::string &s_img, int maxBlockSize, std::vector<ImageBlock> &blocks,int &w, int &h);
//
//		QP_API bool parseCameraDatabase(const std::string &sfileDatabase, std::vector<Datasheet> &vec_database);
//
//		QP_API EExif readExif(const std::vector<qp::image_utils::Datasheet> &vec_database, const std::string &s_img, double &focal_length, double &sensor_size, std::string *pCamName = NULL);
//
//		QP_API bool parseCameraDatabase(const std::string &sfileDatabase, std::vector<Datasheet> &vec_database);
//
//		QP_API std::string combineBrandModel(const EXIFInfo &info);
//		
//		QP_API bool readExif(const std::string &s_img, EXIFInfo&info);
//
//		QP_API bool readExif(const std::string &s_img, qp::SimpleExifHeader &exifHeader);
//
//		QP_API void makeBlocks(int width, int height, int blockSize, std::vector<ImageBlock> &blocks, const int edgePixel = 10);
//
//		QP_API void copyToBlocks(int width, int height, const std::vector<unsigned char> &src_vec_imageData, std::vector<ImageBlock> &desBlocks);
//	}
//}
//#endif // qp_image_utils_h__