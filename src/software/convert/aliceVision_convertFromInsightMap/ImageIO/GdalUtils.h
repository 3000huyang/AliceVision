/************************************************************************/
/* 
@Author: Jones
@Date: 2018-02-14

*/
/************************************************************************/
#ifndef INSIGHT_IMAGE_IO_H
#define INSIGHT_IMAGE_IO_H
#include "imageio_global.h"
#include <vector>
#include "imageinfo.h"
#include "common/numeric.h"
#include "image_container.hpp"
#include "pixel_types.hpp"

INSIGHT_NAME_SPACE

class IMAGEIO_EXPORT GdalUtils
{
public:
    /**
     * @brief 初始化GDAL
     */
	static void InitGDAL();

	static void DestoryGDAL();

	static void SetDataPath(const std::string &path);
    /**
     * @brief GDAL是否已经初始化
     * @return 初始化返回true，否则false
     */
	static bool IsGDALInitialized();

    /**
     * @brief GDAL版本
     * @return 版本号
     */
	static int GDALVersion();

    /**
     * @brief 配置GDAL选项
     * @param szKey GDAL配置键
     * @param szValue GDAL配置值
     */
	static void ConfigGDALOption(const char *szKey, const char *szValue);

    /**
     * @brief 地理坐标转换为图像坐标（仿射变换）.
     * @param pTransform 仿射变换参数 @see Raster2Geo
     * @param x
     * @param y
     * @see Raster2Geo
     */
	static void Geo2Raster(const double *pTransform,double *x,double *y);

    /**
     * @brief 坐标变换，影像坐标转到地理坐标。（仿射变换）
     * @param[in] pTransform 转换参数 转换公式
     *      x=pTransform[0]+pTransform[1]*tx+pTransform[2]*ty;
     *      y=pTransform[3]+pTransform[4]*tx+pTransform[5]*ty;
     * @param[in,out] x 需要转换的x坐标，返回转换后的坐标
     * @param[in,out] y 需要转换的y坐标，返回转换后的坐标
     */
	static void Raster2Geo(const double *pTransform,double *x,double *y);

    /**
     * @brief 统计影像信息
     * @param 影像全路径
     * @param[out] MinVal 像素最小值
     * @param[out] MaxVal 像素最大值
     * @param[out] MeanVal 像素均值
     * @param[out] StdDev 像素标准差
     */
	static void StatisticRaster(const char* lpstrFilePath,double *MinVal,double *MaxVal,double *MeanVal=NULL,double *StdDev=NULL);

    /**
     * @brief 初始化坐标变换参数
	    左上角为0，0，右下角为w,h的坐标
     * @param[out] pTrasnform double[6] 数组,
     */
	static void Init6Transform(double *pTrasnform);	

	/**
	* @brief 初始化坐标变换参数
	左下角为0，0，右上角为w,h的坐标
	* @param[out] pTrasnform double[6] 数组,
	*/
	static void Init6GeoTransform(double *pTransform, int h);
    /**
     * @brief 初始化颜色表，初始化后，颜色表的每一个颜色值是递增的，r,g,b从0递增至255。
     * @param[in,out] pRgbTab 颜色表数组
     * @param[in] nItemCount 颜色表数组大小
     * @attention 不支持超过256个颜色表
     */
	static void InitColorTable(unsigned char *pRgb, int dim, int nItemCount=256);

    /**
     * @brief 设置影像无效值
     * @param lpstrPath 影像文件全路径名
     * @param dfNodataVal 无效值
     */
	static void SetNodataVal(const char* lpstrPath,double dfNodataVal);

    /**
     * @brief GDAL错误信息
     * @return 错误描述
     */
	static const char *GDALLastError() ;

	/**
	 * @brief 获得图像的宽和高 ，该过程会打开图像，然后关闭
	 * @param w 宽度，h, 高度，单位都是像素
	 */
	static bool GetWidthHeightPixel(const char *lpstrImagePath, int &w, int &h);

	static bool GenerateOverviews(const char *lpstrFileFullPath, const char *resampleMethod, int *pLevels, int numLevel);

	//创建并写图像,float格式的图像,通常可能是一个地形数据文件
	static bool CreateAndWriteTiffFloat(const char *tiffFullPath, float *pImageData, int w, int h, double geoTransform[6],double noDataValue = -9999);

	static int GetOverviewsCount(const char *tiffFullPath);

	//
	/**
	* @brief 强制w按照32bit对齐，即4个字节的倍数，如果图像内存不是按照4个字节对其，这通常会引起一些错误，在某些显示系统中
	* @param w 需要强制的数字，通常为图像的宽
	*/
	static int Force32bit(int w);

	static void Force32bit(std::vector<unsigned char> &rgbImages, int w, int h, int &fillW);
	/**
	* @brief 强制16bit，32bit缩放到8bit的图像
	* @param pt 输入图像的类型
	 */
	static bool RGBForceTo8Bit(EnPixelType pt, const std::vector<uchar> &in, std::vector<uchar> &out);

	//w, h origin image size
	//sw,sh scaled image size
	//scale = sw / w;
	/*
	* @brief 读取的数据类型强制16bit，32bit缩放到8bit的图像
	* @param maxDimension 读取不超过最大分辨率
	*/
	static bool DownScaleReadRGBForce8Bit(const char *lpstrImagePath, int maxDimension, 
		int &w, int &h, int &sw, int &sh, std::vector<uchar> &outRgbImages);
	
	//w, h origin image size
	//sw,sh scaled image size
	//scale = sw / w;
	static bool DownScaleReadRGB(const char *lpstrImagePath, int maxDimension, int &w, int &h,
		int &sw, int &sh, std::vector<uchar> &outRgbImages,int *pix_type = NULL);

	//3 band color rgb to 1 band gray
	static void RgbToGray(const unsigned char *src, int n, unsigned char *des);

	static void ConvertRGBByteToGrayImage(const std::vector<unsigned char> &ptr, int w, int h, Image<unsigned char> * im);

	static void MapRGBByteToImage(const std::vector<unsigned char> &ptr, int w, int h, Image<RGBColor> &rgbColIm);

	static void MapGrayByteToImage(const std::vector<unsigned char> &ptr, int w, int h, Image<uint8_t> &rgbColIm);

	static std::string GetError();
private:
	GdalUtils(){}
};
INSIGHT_NAME_SPACE_END
#endif
