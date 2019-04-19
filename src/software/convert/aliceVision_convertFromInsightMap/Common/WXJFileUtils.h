/*****************************************************************************************
* All rights reserved.
* author: huyang.
* date: 2014-10-11
*****************************************************************************************/
#ifndef WXJ_COMMON_WXJFILEUTILS_H
#define WXJ_COMMON_WXJFILEUTILS_H
#include <string>
#include "WXJCommonGlobal.h"
#include "WXJTypeDefs.h"
#include "Qt/WQFileUtils.h"
//fullFilePath 表示文件的绝对路径，包括文件名
WXJCOMMON_EXPORT bool wFileExist(const char *fullFilePath);
WXJCOMMON_EXPORT bool wFileExist(const wchar_t *fullFilePath);

//得到文件名，包括扩展名
WXJCOMMON_EXPORT std::string wFileName(const char *fullFilePath);
WXJCOMMON_EXPORT std::wstring wFileName(const wchar_t *fullFilePath);

//得到文件名，不包括扩展名,abc.de.fg 将返回 abc
WXJCOMMON_EXPORT std::string wBaseFileName(const char *fullFilePath);
WXJCOMMON_EXPORT std::wstring wBaseFileName(const wchar_t *fullFilePath);

//得到完全文件名，不包括扩展名 abc.de.fg 将返回 abc.de
WXJCOMMON_EXPORT std::string wBaseCompleteFileName(const char *fullFilePath);
WXJCOMMON_EXPORT std::wstring wBaseCompleteFileName(const wchar_t *fullFilePath);

//得到文件路径，不包括文件名
WXJCOMMON_EXPORT std::string wFilePath(const char *fullFilePath);
WXJCOMMON_EXPORT std::wstring wFilePath(const wchar_t *fullFilePath);

//得到文件扩展名 abc.de.fg 将返回 fg
WXJCOMMON_EXPORT std::string wFileSuffix(const char *fullFilePath);
WXJCOMMON_EXPORT std::wstring wFileSuffix(const wchar_t *fullFilePath);

//得到当前exe所在全路径，包括文件名
WXJCOMMON_EXPORT std::string wExeFullFilePathA();
WXJCOMMON_EXPORT std::wstring wExeFullFilePathW();

//得到当前exe路径，不包括文件名
WXJCOMMON_EXPORT std::string wExeDirA();
WXJCOMMON_EXPORT std::wstring wExeDirW();

//设置应用程序的当前路径，如果设置成功返回true
//@see wCurrentDir
WXJCOMMON_EXPORT bool wSetCurrentDir(const char *fullDir);
WXJCOMMON_EXPORT bool wSetCurrentDir(const wchar_t *fullDir);

//得到应用程序的当前路径，该路径由最后一次wSetCurrentDir设置。
//如果从未设置过，则返回应用程序运行路径
//@see wSetCurrentDir
WXJCOMMON_EXPORT std::string wCurrentDirA();
WXJCOMMON_EXPORT std::wstring wCurrentDirW();

//判断文件是否是dll，是，返回true，否则 false
WXJCOMMON_EXPORT bool wFileIsLibrary(const char *fileWithSuffix);
WXJCOMMON_EXPORT bool wFileIsLibrary(const wchar_t *fileWithSuffix);

/*************************************************************************
 @ author : huyang.
 @ date 2014-10-07.
 @ param :
   1）desFile : 拷贝到目标文件
   2）srcFile : 源文件 
   3）failIfExists : 操作标识; failIfExists == true 则是如果文件存在就失败；
                               failIfExists == false则是如果文件存在就覆盖；
 @ return ：true 成功, false 失败
*************************************************************************/
WXJCOMMON_EXPORT bool wCopyFile(const char *desFile, const char *srcFile, bool failIfExists);
WXJCOMMON_EXPORT bool wCopyFile(const wchar_t *desFile, const wchar_t *srcFile, bool failIfExists);

/**
 * @ brief : 删除指定文件.如果文件不存在则返回false。删除成功返回true，否则false。
 * @ author : huyang.
 * @ date : 2014-10-07.
 **/
WXJCOMMON_EXPORT bool wDeleteFile(const char *fileName);
WXJCOMMON_EXPORT bool wDeleteFile(const wchar_t *fileName);

//获取操作系统可接受的文件名
//具体实现为：将fileName中不合法的字符替换为'_'
WXJCOMMON_EXPORT std::string wMakeValidFileName(const char *fileName);
WXJCOMMON_EXPORT std::wstring wMakeValidFileName(const wchar_t *fileName);

WXJCOMMON_EXPORT bool wIsNoSpaceFileName(const char *fileName);
WXJCOMMON_EXPORT bool wIsNoSpaceFileName(const wchar_t *fileName);

WXJCOMMON_EXPORT std::string wMakeNoSpaceFileName(const char *fileName);
WXJCOMMON_EXPORT std::wstring wMakeNoSpaceFileName(const wchar_t *fileName);

//获得文件的大小
WXJCOMMON_EXPORT wint64 wFileSize(const char *fileName);
WXJCOMMON_EXPORT wint64 wFileSize(const wchar_t *fileName);

//获取应用程序数据路径（AppData路径）
WXJCOMMON_EXPORT std::string wAppDataPathA();
WXJCOMMON_EXPORT std::wstring wAppDataPathW();

//获得“我的文档”路径
WXJCOMMON_EXPORT std::string wMyDocumentPathA();
WXJCOMMON_EXPORT std::wstring wMyDocumentPathW();

//获得当前的内存数量，包括物理内存和虚拟内存（单位 字节）
WXJCOMMON_EXPORT wuint64 wCurrentTotalMemoryCount();

//获得当前物理内存数量 （单位 字节）
WXJCOMMON_EXPORT wuint64 wCurrentPhysMemoryCount();

//获得当前虚拟内存数量（单位 字节）
WXJCOMMON_EXPORT wuint64 wCurrentVirtualMemoryCount();

//获得当前系统cpu数量
WXJCOMMON_EXPORT int wSystemCPUCount();

//判断某一路径是否存在
WXJCOMMON_EXPORT bool wDirExist(const std::string &dir);
WXJCOMMON_EXPORT bool wDirExist(const std::wstring &dir);

//创建路径，如果上级目录不存在，该函数会进行创建。如果创建成功或路径已经存在返回true，否则返回false
WXJCOMMON_EXPORT bool wMakeDir(const std::string &dir);
WXJCOMMON_EXPORT bool wMakeDir(const std::wstring &dir);

WXJCOMMON_EXPORT bool wExecute(const std::string &dir,const std::string &exe,const std::string &params);

//start 返回handle，要wait和close
typedef void * winhandle;
WXJCOMMON_EXPORT winhandle wStart(const std::string &dir, const std::string &exe, const std::string &params);
#endif
