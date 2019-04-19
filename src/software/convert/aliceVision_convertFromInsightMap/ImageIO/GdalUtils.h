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
     * @brief ��ʼ��GDAL
     */
	static void InitGDAL();

	static void DestoryGDAL();

	static void SetDataPath(const std::string &path);
    /**
     * @brief GDAL�Ƿ��Ѿ���ʼ��
     * @return ��ʼ������true������false
     */
	static bool IsGDALInitialized();

    /**
     * @brief GDAL�汾
     * @return �汾��
     */
	static int GDALVersion();

    /**
     * @brief ����GDALѡ��
     * @param szKey GDAL���ü�
     * @param szValue GDAL����ֵ
     */
	static void ConfigGDALOption(const char *szKey, const char *szValue);

    /**
     * @brief ��������ת��Ϊͼ�����꣨����任��.
     * @param pTransform ����任���� @see Raster2Geo
     * @param x
     * @param y
     * @see Raster2Geo
     */
	static void Geo2Raster(const double *pTransform,double *x,double *y);

    /**
     * @brief ����任��Ӱ������ת���������ꡣ������任��
     * @param[in] pTransform ת������ ת����ʽ
     *      x=pTransform[0]+pTransform[1]*tx+pTransform[2]*ty;
     *      y=pTransform[3]+pTransform[4]*tx+pTransform[5]*ty;
     * @param[in,out] x ��Ҫת����x���꣬����ת���������
     * @param[in,out] y ��Ҫת����y���꣬����ת���������
     */
	static void Raster2Geo(const double *pTransform,double *x,double *y);

    /**
     * @brief ͳ��Ӱ����Ϣ
     * @param Ӱ��ȫ·��
     * @param[out] MinVal ������Сֵ
     * @param[out] MaxVal �������ֵ
     * @param[out] MeanVal ���ؾ�ֵ
     * @param[out] StdDev ���ر�׼��
     */
	static void StatisticRaster(const char* lpstrFilePath,double *MinVal,double *MaxVal,double *MeanVal=NULL,double *StdDev=NULL);

    /**
     * @brief ��ʼ������任����
	    ���Ͻ�Ϊ0��0�����½�Ϊw,h������
     * @param[out] pTrasnform double[6] ����,
     */
	static void Init6Transform(double *pTrasnform);	

	/**
	* @brief ��ʼ������任����
	���½�Ϊ0��0�����Ͻ�Ϊw,h������
	* @param[out] pTrasnform double[6] ����,
	*/
	static void Init6GeoTransform(double *pTransform, int h);
    /**
     * @brief ��ʼ����ɫ����ʼ������ɫ���ÿһ����ɫֵ�ǵ����ģ�r,g,b��0������255��
     * @param[in,out] pRgbTab ��ɫ������
     * @param[in] nItemCount ��ɫ�������С
     * @attention ��֧�ֳ���256����ɫ��
     */
	static void InitColorTable(unsigned char *pRgb, int dim, int nItemCount=256);

    /**
     * @brief ����Ӱ����Чֵ
     * @param lpstrPath Ӱ���ļ�ȫ·����
     * @param dfNodataVal ��Чֵ
     */
	static void SetNodataVal(const char* lpstrPath,double dfNodataVal);

    /**
     * @brief GDAL������Ϣ
     * @return ��������
     */
	static const char *GDALLastError() ;

	/**
	 * @brief ���ͼ��Ŀ�͸� ���ù��̻��ͼ��Ȼ��ر�
	 * @param w ��ȣ�h, �߶ȣ���λ��������
	 */
	static bool GetWidthHeightPixel(const char *lpstrImagePath, int &w, int &h);

	static bool GenerateOverviews(const char *lpstrFileFullPath, const char *resampleMethod, int *pLevels, int numLevel);

	//������дͼ��,float��ʽ��ͼ��,ͨ��������һ�����������ļ�
	static bool CreateAndWriteTiffFloat(const char *tiffFullPath, float *pImageData, int w, int h, double geoTransform[6],double noDataValue = -9999);

	static int GetOverviewsCount(const char *tiffFullPath);

	//
	/**
	* @brief ǿ��w����32bit���룬��4���ֽڵı��������ͼ���ڴ治�ǰ���4���ֽڶ��䣬��ͨ��������һЩ������ĳЩ��ʾϵͳ��
	* @param w ��Ҫǿ�Ƶ����֣�ͨ��Ϊͼ��Ŀ�
	*/
	static int Force32bit(int w);

	static void Force32bit(std::vector<unsigned char> &rgbImages, int w, int h, int &fillW);
	/**
	* @brief ǿ��16bit��32bit���ŵ�8bit��ͼ��
	* @param pt ����ͼ�������
	 */
	static bool RGBForceTo8Bit(EnPixelType pt, const std::vector<uchar> &in, std::vector<uchar> &out);

	//w, h origin image size
	//sw,sh scaled image size
	//scale = sw / w;
	/*
	* @brief ��ȡ����������ǿ��16bit��32bit���ŵ�8bit��ͼ��
	* @param maxDimension ��ȡ���������ֱ���
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
