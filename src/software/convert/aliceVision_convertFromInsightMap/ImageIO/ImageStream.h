/**************************************************************************************

image
支持全部GDAL格式以及各类自定义栅格如下：
BMP;ECW;EIR;ENVI;ERS;GIF;GTIFF;JPEG;PNG;XYZ;AATGrid等;
Transform[0]: topLeft x;
Transform[1]: w-e pixel resolution;
Transform[2]: rotation, 0 if image is "North up";
Transform[3]: topLeft y;
Transform[4]: rotation, 0 if image is "North up";
Transform[5]: n-s pixel resolution;

BOOL ReadRange (int nLevel,int nFileSX,int nFileSY,int nFileXSize,int nFileYSize,void *pBuffer,int nReadXSize,int nReadYSize,int nBufXSize,int nBufYSize,int *pBandList,int nBandNum,BMPFormat eF=BIP);
BOOL WriteRange(int nLevel,int nFileSX,int nFileSY,int nFileXSize,int nFileYSize,void *pBuffer,int nWriteXSize,int nWriteYSize,int nBufXSize,int nBufYSize,int *pBandList,int nBandNum,BMPFormat eF=BIP);
IO接口说明：
nLevel: 当前读写金字塔层数，金字塔从0起算，原始影像为第0层；
nFileSX,nFileSY:需要读入文件的列号和行号；
nFileSY,nFileYSize:需要读入的文件范围；
pBuffer:需要读写内存首地址，如需偏移请在调用前完成偏移计算；
nReadXSize,nReadYSize:与文件交互的内存大小,可与实际内存不相等，也可与文件范围不相等；
nBufXSize,nBufYSize:实际内存大小；
pBandList: 波段索引,索引从1起算；
nBandNum:波段数目。

**************************************************************************************/
#ifndef INSIGHT_IMAGE_STREAM_H
#define INSIGHT_IMAGE_STREAM_H
#include <string>
#include "imageio_global.h"
#include "ImageInfo.h"
/**
 * @brief 影像读写流
 * @attention   只能以一种状态打开流（read or update），当重新打开一个文件时，若
 *              文件处于打开状态则会先关闭。打开模式下只能调用readRange，更新模式
 *              下只能调用writeRange。
 */
INSIGHT_NAME_SPACE

class IMAGEIO_EXPORT ImageStream
{
	typedef void * HImage;
public:
	enum OpenMode{OM_Read = 0,OM_Update = 1};
public:
	/**
	 * @brief 像素存储方式
	 *	BSQ格式：各波段的二维图像数据按波段顺序排列（（像元号顺序），行号顺序），波段顺序）
	 *  BIL格式：对每一行中代表一个波段的光谱值进行排列，然后按波段顺序排列该行，最后对各行进行重复。（（（像元号顺序），波段顺序），行号顺序）
	 *  BIP格式：在一行中，每个像元按光谱波段次序进行排列，然后对该行的全部像元进行这种波段次序排列，最后对各行进行重复。（（波段次序，像元号顺序），行号顺序)
	 */
	enum IMGFormat
	{ 
		IMG_BSQ = 0, // BAND
		IMG_BIP = 1, // PIXEL
		IMG_BIL = 2 // LINE
	};
public:
	static  bool Create(const char* lpstrFilePath,int nCol,int nRow,int nBand,double* dTransform, 
		EnPixelType pixType=PIXEL_Byte,const char* strCode="GTiff",const char* projWKT=NULL, 
		bool bTiled=false,bool bCompress=false,const char* strTieSize="128",const char* strCompress="LZW");	
public:
	ImageStream();
	~ImageStream();

	void setComputeMinMax(bool compute);
	ImageStream(const std::string &filePath, OpenMode enumOpenModes = OM_Read);
    /**
     * @brief 打开一个文件
     * @param filePath 文件全路径
     * @param nOpenMode 打开模式OM_Read/OM_Update
     * @return 是否打开成功
     */
	bool Open(const std::string &filePath, OpenMode nOpenMode = OM_Read);


	bool SimpleOpen(const std::string &filePath, OpenMode openMode = OM_Read);

	bool SimpleInfomation(int &width, int &hight, int &bandCount);
    /**
     * @brief 当前流是否打开
     * @return
     */
	bool IsOpen() const;

    /**
     * @brief 关闭流
     */
	void Close();

	//nLevel: 当前读写金字塔层数，金字塔从0起算，原始影像为第0层；
    //nFileSX,nFileSY:需要读入文件的列号和行号；起始像素（左上角）
    //nFileXSize,nFileYSize:需要读入的文件范围；结束像素（右下角）
	//pBuffer:需要读写内存首地址，如需偏移请在调用前完成偏移计算；
	//nReadXSize,nReadYSize:与文件交互的内存大小,可与实际内存不相等，也可与文件范围不相等；
	//nBufXSize,nBufYSize:实际内存大小；
	//pBandList: 波段索引,索引从1起算；
	//nBandNum:波段数目。
	bool ReadRange (int nLevel,int nFileSX,int nFileSY,int nFileXSize,int nFileYSize,
		void *pBuffer,int nReadXSize,int nReadYSize,int nBufXSize,int nBufYSize,
		int *pBandList,int nBandNum,IMGFormat eF=IMG_BIP);

	bool WriteRange(int nLevel,int nFileSX,int nFileSY,int nFileXSize,int nFileYSize,
		void *pBuffer,int nWriteXSize,int nWriteYSize,int nBufXSize,int nBufYSize,
		int *pBandList,int nBandNum,IMGFormat eF=IMG_BIP);
	ImageInfo ImageInformation() const;

public:
	std::string FilePath() const{return m_strFilePath;}
private:
	void Init();
private:
	static bool m_bInitialized;//false
	bool m_bOpen;
	std::string m_strFilePath;
	OpenMode m_enumOpenMode;
	ImageInfo m_imageInfomation;
	bool m_computeMinMax;//false;
};
INSIGHT_NAME_SPACE_END
#endif
