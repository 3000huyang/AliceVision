/**************************************************************************************

image
֧��ȫ��GDAL��ʽ�Լ������Զ���դ�����£�
BMP;ECW;EIR;ENVI;ERS;GIF;GTIFF;JPEG;PNG;XYZ;AATGrid��;
Transform[0]: topLeft x;
Transform[1]: w-e pixel resolution;
Transform[2]: rotation, 0 if image is "North up";
Transform[3]: topLeft y;
Transform[4]: rotation, 0 if image is "North up";
Transform[5]: n-s pixel resolution;

BOOL ReadRange (int nLevel,int nFileSX,int nFileSY,int nFileXSize,int nFileYSize,void *pBuffer,int nReadXSize,int nReadYSize,int nBufXSize,int nBufYSize,int *pBandList,int nBandNum,BMPFormat eF=BIP);
BOOL WriteRange(int nLevel,int nFileSX,int nFileSY,int nFileXSize,int nFileYSize,void *pBuffer,int nWriteXSize,int nWriteYSize,int nBufXSize,int nBufYSize,int *pBandList,int nBandNum,BMPFormat eF=BIP);
IO�ӿ�˵����
nLevel: ��ǰ��д��������������������0���㣬ԭʼӰ��Ϊ��0�㣻
nFileSX,nFileSY:��Ҫ�����ļ����кź��кţ�
nFileSY,nFileYSize:��Ҫ������ļ���Χ��
pBuffer:��Ҫ��д�ڴ��׵�ַ������ƫ�����ڵ���ǰ���ƫ�Ƽ��㣻
nReadXSize,nReadYSize:���ļ��������ڴ��С,����ʵ���ڴ治��ȣ�Ҳ�����ļ���Χ����ȣ�
nBufXSize,nBufYSize:ʵ���ڴ��С��
pBandList: ��������,������1���㣻
nBandNum:������Ŀ��

**************************************************************************************/
#ifndef INSIGHT_IMAGE_STREAM_H
#define INSIGHT_IMAGE_STREAM_H
#include <string>
#include "imageio_global.h"
#include "ImageInfo.h"
/**
 * @brief Ӱ���д��
 * @attention   ֻ����һ��״̬������read or update���������´�һ���ļ�ʱ����
 *              �ļ����ڴ�״̬����ȹرա���ģʽ��ֻ�ܵ���readRange������ģʽ
 *              ��ֻ�ܵ���writeRange��
 */
INSIGHT_NAME_SPACE

class IMAGEIO_EXPORT ImageStream
{
	typedef void * HImage;
public:
	enum OpenMode{OM_Read = 0,OM_Update = 1};
public:
	/**
	 * @brief ���ش洢��ʽ
	 *	BSQ��ʽ�������εĶ�άͼ�����ݰ�����˳�����У�����Ԫ��˳�򣩣��к�˳�򣩣�����˳��
	 *  BIL��ʽ����ÿһ���д���һ�����εĹ���ֵ�������У�Ȼ�󰴲���˳�����и��У����Ը��н����ظ�����������Ԫ��˳�򣩣�����˳�򣩣��к�˳��
	 *  BIP��ʽ����һ���У�ÿ����Ԫ�����ײ��δ���������У�Ȼ��Ը��е�ȫ����Ԫ�������ֲ��δ������У����Ը��н����ظ����������δ�����Ԫ��˳�򣩣��к�˳��)
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
     * @brief ��һ���ļ�
     * @param filePath �ļ�ȫ·��
     * @param nOpenMode ��ģʽOM_Read/OM_Update
     * @return �Ƿ�򿪳ɹ�
     */
	bool Open(const std::string &filePath, OpenMode nOpenMode = OM_Read);


	bool SimpleOpen(const std::string &filePath, OpenMode openMode = OM_Read);

	bool SimpleInfomation(int &width, int &hight, int &bandCount);
    /**
     * @brief ��ǰ���Ƿ��
     * @return
     */
	bool IsOpen() const;

    /**
     * @brief �ر���
     */
	void Close();

	//nLevel: ��ǰ��д��������������������0���㣬ԭʼӰ��Ϊ��0�㣻
    //nFileSX,nFileSY:��Ҫ�����ļ����кź��кţ���ʼ���أ����Ͻǣ�
    //nFileXSize,nFileYSize:��Ҫ������ļ���Χ���������أ����½ǣ�
	//pBuffer:��Ҫ��д�ڴ��׵�ַ������ƫ�����ڵ���ǰ���ƫ�Ƽ��㣻
	//nReadXSize,nReadYSize:���ļ��������ڴ��С,����ʵ���ڴ治��ȣ�Ҳ�����ļ���Χ����ȣ�
	//nBufXSize,nBufYSize:ʵ���ڴ��С��
	//pBandList: ��������,������1���㣻
	//nBandNum:������Ŀ��
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
