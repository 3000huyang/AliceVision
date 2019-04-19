#include "ImageStream.h"
#include "gdal.h"
#include "ogr_api.h"
#include "cpl_string.h"
#include "imageinfo.h"
#include <iostream>
#include "gdalutils.h"
#include <cstdint>
INSIGHT_NAME_SPACE


ImageStream::ImageStream()
{
	Init();
}

ImageStream::ImageStream(const std::string &filePath, OpenMode enumOpenModes /*= OM_Read*/)
{
	Init();
	Open(filePath,enumOpenModes);
}

ImageStream::~ImageStream()
{
	if (IsOpen())
	{
		Close();
	}
}
void ImageStream::Init()
{
	GdalUtils::InitGDAL();
	m_bOpen = false;	
	m_enumOpenMode = OM_Read;
	m_computeMinMax = false;
}

bool ImageStream::Open(const std::string &filePath, OpenMode nOpenMode /*= modeREAD*/)
{
	if (IsOpen())
	{
		Close();
	}
	m_strFilePath = filePath;
	m_imageInfomation.Reset();
	m_imageInfomation.m_hImage = GDALOpen(filePath.c_str(),nOpenMode == OM_Read ? GA_ReadOnly : GA_Update);

	if (m_imageInfomation.m_hImage)
	{
		m_enumOpenMode = nOpenMode;
		const char *pszProjectionRef = GDALGetProjectionRef(m_imageInfomation.m_hImage);
		if (pszProjectionRef)
		{
			m_imageInfomation.m_strProjection = pszProjectionRef;
		}


		int nCols = GDALGetRasterXSize(m_imageInfomation.m_hImage);
		int nRows = GDALGetRasterYSize(m_imageInfomation.m_hImage);
		int nBands = GDALGetRasterCount(m_imageInfomation.m_hImage);

		CPLErr error = GDALGetGeoTransform(m_imageInfomation.m_hImage, m_imageInfomation.m_geoTransform);

		if (error != CPLErr::CE_None){
			m_imageInfomation.m_geoTransformValid = false;
		}
		else{
			m_imageInfomation.m_geoTransformValid = true;
		}

		for (int i = 1; i <= nBands; ++i)
		{
			GDALRasterBandH hBand = GDALGetRasterBand(m_imageInfomation.m_hImage, i);
			m_imageInfomation.m_mapBandColorInterp[i] = (EnColorInterp)(int)(GDALGetRasterColorInterpretation(hBand));
		}
		
		GDALDataType dataType = GDALGetRasterDataType(GDALGetRasterBand(m_imageInfomation.m_hImage, 1));
		int nBits = GDALGetDataTypeSize(dataType);
		EnPixelType enumPixelType=(EnPixelType)dataType;
		m_imageInfomation.m_nColumns = nCols;
		m_imageInfomation.m_nRows = nRows;
		m_imageInfomation.m_nBands = nBands;
		m_imageInfomation.m_nPixelBits = nBits;
		m_imageInfomation.m_enumPixelType = enumPixelType;

		//颜色表;
		GDALColorTableH colorTable=GDALGetRasterColorTable(GDALGetRasterBand(m_imageInfomation.m_hImage, 1));
		if(colorTable!=NULL)
		{
			int nCount=GDALGetColorEntryCount(colorTable);
			for(int i = 0; i < nCount && i < 256; ++i)
			{
				const GDALColorEntry* psEntry = GDALGetColorEntry(colorTable,i);
				//TODO:应该支持 cmyk,hsv 模型 by Jones.
				m_imageInfomation.m_rgbaTable[i * 4] = (uchar)psEntry->c1;
				m_imageInfomation.m_rgbaTable[i * 4 + 1] = (uchar)psEntry->c2;
				m_imageInfomation.m_rgbaTable[i * 4 + 2] = (uchar)psEntry->c3;
				m_imageInfomation.m_rgbaTable[i * 4 + 3] = (uchar)psEntry->c4;
			}
		}
		int nBlockXSize;
		int nBlockYSize;
		int nLevels;
		int bHasNoDataValue = 0;
		GDALGetBlockSize(GDALGetRasterBand(m_imageInfomation.m_hImage,1),&nBlockXSize,&nBlockYSize);
		
		nLevels = GDALGetOverviewCount(GDALGetRasterBand(m_imageInfomation.m_hImage,1));
		GDALRasterBandH hBand = GDALGetRasterBand(m_imageInfomation.m_hImage,1);
		if(hBand)
		{
			for (int i = 0; i < nLevels; ++i)
			{
				GDALRasterBandH hBand1= GDALGetOverview(hBand,i);
				if(hBand1)
				{
					m_imageInfomation.m_overviewColumns.push_back(GDALGetRasterBandXSize(hBand1));
					m_imageInfomation.m_overviewRows.push_back(GDALGetRasterBandYSize(hBand1));
					
				}
			}
		}
		m_imageInfomation.m_nLevels += nLevels;
		m_imageInfomation.m_dNoDataVal = GDALGetRasterNoDataValue(hBand,&bHasNoDataValue);

		if (bHasNoDataValue)
		{
			m_imageInfomation.m_bHasNoDataVal = true;
		}
		else
		{
			m_imageInfomation.m_bHasNoDataVal = false;
		}

		m_imageInfomation.m_nBlockXSize = nBlockXSize;
		m_imageInfomation.m_nBlockYSize = nBlockYSize;

		if (m_computeMinMax)
		{
			double dMinMax[2];
			GDALComputeRasterMinMax(hBand, 0, dMinMax);
			m_imageInfomation.m_dMinValue = dMinMax[0];
			m_imageInfomation.m_dMaxValue = dMinMax[1];
		}
		
		m_bOpen = true;
	}
	else
	{
		m_bOpen = false;
	}
	return m_bOpen;
}

bool ImageStream::IsOpen() const
{
	return m_bOpen;
}

void ImageStream::Close()
{
	if (m_imageInfomation.m_hImage)
	{
		GDALClose(m_imageInfomation.m_hImage);
	}
	m_imageInfomation.Reset();
	m_bOpen = false;
	m_strFilePath.clear();
}

bool ImageStream::ReadRange(int nLevel,int nFileSX,int nFileSY,int nFileXSize,int nFileYSize,
	void *pBuffer,int nReadXSize,int nReadYSize,int nBufXSize,
	int nBufYSize, int *pBandList,int nBandNum,IMGFormat eF/*=BIP*/)
{
	if(!IsOpen()){return false;}
	if(nFileXSize<=0||nFileYSize<=0||nReadXSize<=0||nReadYSize<=0){return false;}	
	if(nLevel>=m_imageInfomation.m_nLevels){return false;}	
	//每一个波段的类型应该是一致的
	GDALDataType	eType=GDALGetRasterDataType(GDALGetRasterBand(m_imageInfomation.m_hImage,1));
	int nBands = GDALGetRasterCount(m_imageInfomation.m_hImage);
	if (nBands < nBandNum)
	{
		std::cout << "Error : exceed bands counts! " << __FUNCTION__ << __LINE__ << std::endl;
		return false;
	}
	int				nPixelSpace,nLineSpace,nBandSpace;
	/*
	BSQ（band sequential format）是按波段保存，也就是一个波段保存后接着保存第二个波段。该格式最适于对单个波谱波段中任何部分的空间（X，Y）存取；
	BIL（band interleaved by line format）是按行保存，就是保存第一个波段的第一行后接着保存第二个波段的第一行，依次类推。该格式提供了空间和波谱处理之间一种折衷方式；
	BIP（band interleaved by pixel format）是按像元保存，即先保存第一个波段的第一个像元，之后保存第二波段的第一个像元，依次保存。该格式为图像数据波谱（Z） 的存取提供最佳性能。
		BSQ（波段顺序格式）
		rrrrrrrrr
		rrrrrrrrr
		ggggggggg
		ggggggggg
		bbbbbbbbb
		bbbbbbbbb
		BIL（波段按行交叉格式）
		rrrrrrrrr
		ggggggggg
		bbbbbbbbb
		rrrrrrrrr
		ggggggggg
		bbbbbbbbb
		BIP（波段按像元交叉格式）
		rgbrgbrgb
		rgbrgbrgb
		rgbrgbrgb
		rgbrgbrgb
		rgbrgbrgb
		rgbrgbrgb
	*/

	switch(eF)
	{
	case IMG_BSQ:// BAND 
		nPixelSpace = GDALGetDataTypeSize(eType) / 8;
		nLineSpace	= nPixelSpace* nBufXSize;
		nBandSpace	= nLineSpace * nBufYSize;
		break;
	case IMG_BIP:// PIXEL 
		nPixelSpace = GDALGetDataTypeSize(eType) / 8 * nBandNum;
		nLineSpace	= nPixelSpace * nBufXSize;
		nBandSpace =  GDALGetDataTypeSize(eType) / 8;
		break;
	case IMG_BIL:// LINE; 
		nPixelSpace	= GDALGetDataTypeSize(eType) / 8;
		nLineSpace = nPixelSpace * nBufXSize * nBandNum;
		nBandSpace	= nPixelSpace * nBufXSize;
		break;
	default:
		throw std::runtime_error("unkown gdal data type");
	};

	GDALRasterBandH hBand;
	for(int iBand=0;iBand<nBandNum;iBand++)
	{
		hBand=GDALGetRasterBand(m_imageInfomation.m_hImage,pBandList[iBand]);	
		if(!hBand){continue;}
		if(nLevel>0){hBand=GDALGetOverview(hBand,nLevel-1);}
		CPLErr cplError=GDALRasterIO(hBand,GF_Read,nFileSX,nFileSY,nFileXSize,nFileYSize,
			(uint8_t*)pBuffer + nBandSpace*iBand, nReadXSize, nReadYSize, eType, nPixelSpace, nLineSpace);
		if(cplError==CE_Failure){return false;}			
	}

	return true;
}

bool ImageStream::WriteRange(int nLevel,int nFileSX,int nFileSY,int nFileXSize,int nFileYSize, 
	void *pBuffer,int nWriteXSize,int nWriteYSize,int nBufXSize,int nBufYSize, 
	int *pBandList,int nBandNum,IMGFormat eF/*=BIP*/)
{
	if(!IsOpen()|| m_enumOpenMode != OM_Update){return false;}
	if(nFileXSize <=0 || nFileYSize<=0 || nWriteXSize<=0 || nWriteYSize <= 0){return false;}	
	if(nLevel >= m_imageInfomation.m_nLevels){return false;}	

	GDALDataType	eType=GDALGetRasterDataType(GDALGetRasterBand(m_imageInfomation.m_hImage,1));
	int				nPixelSpace,nLineSpace,nBandSpace;
	switch(eF)
	{
	case IMG_BSQ:// BAND
		nPixelSpace	= GDALGetDataTypeSize(eType) / 8;
		nLineSpace	= nPixelSpace* nBufXSize;
		nBandSpace	= nLineSpace * nBufYSize;
		break;
	case IMG_BIP:// PIXEL
		nPixelSpace	= GDALGetDataTypeSize(eType) / 8 * nBandNum;
		nLineSpace	= nPixelSpace * nBufXSize;
		nBandSpace	= GDALGetDataTypeSize(eType) / 8;
		break;
	case IMG_BIL:// LINE;
		nPixelSpace	= GDALGetDataTypeSize(eType) / 8;
		nLineSpace	= nPixelSpace * nBufXSize * nBandNum;
		nBandSpace	= nPixelSpace * nBufXSize;
		break;
	};

	GDALRasterBandH hBand;
	for(int iBand=0;iBand<nBandNum;iBand++)
	{
		hBand=GDALGetRasterBand(m_imageInfomation.m_hImage,pBandList[iBand]);	
		if(!hBand){continue;}
		if(nLevel>0){hBand=GDALGetOverview(hBand,nLevel-1);}

		CPLErr cplError = GDALRasterIO(hBand, GF_Write, nFileSX, nFileSY, nFileXSize, nFileYSize, (uint8_t*)pBuffer + nBandSpace*iBand, nWriteXSize, nWriteYSize, eType, nPixelSpace, nLineSpace);
		if(cplError==CE_Failure){return false;}			
	}

	return true;
}

ImageInfo ImageStream::ImageInformation() const
{
	return m_imageInfomation;
}

bool ImageStream::Create(const char* lpstrFilePath,int nCol,int nRow,int nBand,double* dTransform, 
	EnPixelType pixType/*=PIXEL_Byte*/,const char* strCode/*="GTiff"*/,
	const char* projWKT/*=NULL*/, bool bTiled/*=false*/,bool bCompress/*=false*/,
	const char* strTieSize/*="128"*/,const char* strCompress/*="LZW"*/)
{
	bool bSuccess = false;
	if(dTransform == NULL)
	{
		return false;
	}

	GDALDriverH hDriver=GDALGetDriverByName(strCode);

	if(hDriver)
	{
		char **papszMetadata = GDALGetMetadata(hDriver, NULL);//
		if (!CSLFetchBoolean(papszMetadata, GDAL_DCAP_CREATE, FALSE))
		{
			printf("Driver %s NOT supports Create() method.\n", strCode);
			return false;
		}

		GDALDataType eType=(GDALDataType)pixType;	

		//设置创建选项;
		char ** ppStr = NULL;
		if(hDriver==GDALGetDriverByName("GTiff"))
		{
			if(bTiled)
			{
				ppStr = CSLAddNameValue(ppStr, "TILED", "TRUE");
				ppStr = CSLAddNameValue(ppStr, "BLOCKXSIZE", strTieSize);
				ppStr = CSLAddNameValue(ppStr, "BLOCKYSIZE", strTieSize);
			}
			if(bCompress)
			{
				ppStr = CSLAddNameValue(ppStr, "COMPRESS", strCompress);
			}
		}
		else if(hDriver==GDALGetDriverByName("HFA"))
		{
			if(bTiled)
			{
				ppStr = CSLAddNameValue(ppStr, "BLOCKSIZE", strTieSize);
			}
			if(bCompress)
			{
				ppStr = CSLAddNameValue(ppStr, "COMPRESSED", "TRUE");
			}
		}

		//创建文件；
		GDALDatasetH hDS = GDALCreate(hDriver,lpstrFilePath,nCol,nRow,nBand,eType,ppStr);
		if(hDS)
		{
			GDALSetGeoTransform(hDS, dTransform);
			if (projWKT)
				GDALSetProjection(hDS, projWKT);
			GDALFlushCache(hDS);
			bSuccess = true;
		}

		GDALClose(hDS);

	}
	else
	{
		printf( "code not exist\n");
	}
	return bSuccess;
}

void ImageStream::setComputeMinMax(bool compute)
{
	m_computeMinMax = compute;
}

bool ImageStream::SimpleOpen(const std::string &filePath, OpenMode nOpenMode /*= OM_Read*/)
{
	if (IsOpen())
	{
		Close();
	}
	m_strFilePath = filePath;
	m_imageInfomation.Reset();
	m_imageInfomation.m_hImage = GDALOpen(filePath.c_str(), nOpenMode == OM_Read ? GA_ReadOnly : GA_Update);

	if (!m_imageInfomation.m_hImage)
	{
		m_bOpen = false;
	}
	else
	{
		m_enumOpenMode = nOpenMode;
		m_bOpen = true;
	}
	return m_bOpen;
}

bool ImageStream::SimpleInfomation(int &width, int &hight, int &bandCount)
{
	if (!IsOpen())
	{
		return false;
	}
	width = GDALGetRasterXSize(m_imageInfomation.m_hImage);
	hight = GDALGetRasterYSize(m_imageInfomation.m_hImage);
	bandCount = GDALGetRasterCount(m_imageInfomation.m_hImage);
	return true;
}

INSIGHT_NAME_SPACE_END



