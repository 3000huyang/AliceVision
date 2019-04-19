#ifndef WXJ_COMMON_WQFILEUTILS_H
#define WXJ_COMMON_WQFILEUTILS_H
#ifdef WXJ_QT
#include <QString>
#include "WXJCommonGlobal.h"
#include "WXJTypeDefs.h"

WXJCOMMON_EXPORT bool wFileExist(const QString &fullFilePath);

//得到文件名，包括扩展名
WXJCOMMON_EXPORT QString wFileName(const QString &fullFilePath);

//得到文件名，不包括扩展名,abc.de.fg 将返回 abc
WXJCOMMON_EXPORT QString wBaseFileName(const QString &fullFilePath);

//得到完全文件名，不包括扩展名 abc.de.fg 将返回 abc.de
WXJCOMMON_EXPORT QString wBaseCompleteFileName(const QString &fullFilePath);

//得到文件路径，不包括文件名
WXJCOMMON_EXPORT QString wFilePath(const QString &fullFilePath);

//得到文件扩展名 abc.de.fg 将返回 fg
WXJCOMMON_EXPORT QString wFileSuffix(const QString &fullFilePath);

//得到当前exe所在全路径，包括文件名
WXJCOMMON_EXPORT QString wExeFullFilePath();

//得到当前exe路径，不包括文件名
WXJCOMMON_EXPORT QString wExeDir();

//设置应用程序的当前路径，如果设置成功返回true
//@see wCurrentDir
WXJCOMMON_EXPORT bool wSetCurrentDir(const QString &fullDir);

//得到应用程序的当前路径，该路径由最后一次wSetCurrentDir设置。
//如果从未设置过，则返回应用程序运行路径
//@see wSetCurrentDir
WXJCOMMON_EXPORT QString wCurrentDir();

//判断文件是否是dll，是，返回true，否则 false
WXJCOMMON_EXPORT bool wFileIsLibrary(const QString &fileWithSuffix);

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
WXJCOMMON_EXPORT bool wCopyFile(const QString &desFile, const QString &srcFile, bool failIfExists);

/**
 * @ brief : 删除指定文件.如果文件不存在则返回false。删除成功返回true，否则false。
 * @ author : huyang.
 * @ date : 2014-10-07.
 **/
WXJCOMMON_EXPORT bool wDeleteFile(const QString &fileName);

//获取操作系统可接受的文件名
//具体实现为：将fileName中不合法的字符替换为'_'
WXJCOMMON_EXPORT std::string wMakeValidFileName(const QString &fileName);

WXJCOMMON_EXPORT bool wIsNoSpaceFileName(const QString &fileName);

WXJCOMMON_EXPORT QString wMakeNoSpaceFileName(const QString &fileName);

//获得文件的大小
WXJCOMMON_EXPORT wint64 wFileSize(const QString &fileName);

//获取应用程序数据路径（AppData路径）
WXJCOMMON_EXPORT QString wAppDataPath();

//获得“我的文档”路径
WXJCOMMON_EXPORT QString wMyDocumentPath();

//判断路径是否存在
WXJCOMMON_EXPORT bool wDirExist( const QString &dirFullPath );

WXJCOMMON_EXPORT bool wMakeDir(const QString &dir);
#endif//WXJ_QT
#endif