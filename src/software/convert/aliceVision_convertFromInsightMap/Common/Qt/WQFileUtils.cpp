#include <QFile>
#include <QFileInfo>
#include <QDir>
#define WXJ_QT

#include "Qt/WQFileUtils.h"
#include "Qt/WQStrUtils.h"
#include "WXJMacros.h"
#include "WXJCommonGlobal.h"


WXJCOMMON_EXPORT bool wFileExist( const QString &fullFilePath )
{
	return QFile::exists(fullFilePath);
}

WXJCOMMON_EXPORT QString wExeDir()
{
	return wFilePath(wExeFullFilePath());
}

WXJCOMMON_EXPORT QString wExeFullFilePath()
{
#if defined(WIN32) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
	wchar_t wszModeulName[261] = {0};
	GetModuleFileNameW(0, wszModeulName, 261);
	return QString::fromUtf16((ushort *)wszModeulName);
#else
	qApp->applicationName();
#endif

}

WXJCOMMON_EXPORT QString wFilePath( const QString &fullFilePath )
{
	return QFileInfo(fullFilePath).dir().absolutePath();
}

WXJCOMMON_EXPORT bool wDirExist( const QString &dirFullPath )
{
	return QDir(dirFullPath).exists();
}

WXJCOMMON_EXPORT QString wCurrentDir()
{
	return QDir::currentPath();
}

WXJCOMMON_EXPORT bool wSetCurrentDir( const QString &dir )
{
	return QDir::setCurrent(dir);
}

WXJCOMMON_EXPORT QString wBaseCompleteFileName(const QString &fullPath)
{
	 return QFileInfo(fullPath).completeBaseName();
}

WXJCOMMON_EXPORT QString wFileName(const QString &fullPath)
{
	return QFileInfo(fullPath).fileName();
}

WXJCOMMON_EXPORT QString wBaseFileName( const QString &fullFilePath )
{
	return QFileInfo(fullFilePath).baseName();
}

WXJCOMMON_EXPORT QString wFileSuffix( const QString &fullFilePath )
{
	return  QFileInfo(fullFilePath).suffix();
}

WXJCOMMON_EXPORT bool wFileIsLibrary( const QString &fileWithSuffix )
{
	return (QString::compare(wFileSuffix(fileWithSuffix), "dll", Qt::CaseInsensitive) == 0);
}

WXJCOMMON_EXPORT bool wCopyFile( const QString &desFile, const QString &srcFile, bool failIfExists )
{
	// 如果 failIfExists 为真,且newFile存在,直接返回
	if (failIfExists && wFileExist(desFile))
	{
		return false;
	}

	// 如果 failIfExists 为假,且newFile存在
	if (!failIfExists && wFileExist(desFile))
	{
		//删除newFile
		QFile::remove(desFile);
	}

	return (QFile::copy(srcFile, desFile));
}

WXJCOMMON_EXPORT bool wDeleteFile( const QString &fileName )
{
	if (wFileExist(fileName))
	{
		return QFile::remove(fileName);
	}
	return false;
}

WXJCOMMON_EXPORT std::string wMakeValidFileName( const QString &fileName )
{
	QStringList invalidCharsList = (QStringList() << "\\" << "/" << ":" << "\"" << ">" << "<" << "\r\n" << "|" << "?" << "*");
	QString validFileName(fileName);
	for (int i = 0; i < invalidCharsList.count(); ++i)
	{
		validFileName.replace(invalidCharsList[i], "_", Qt::CaseInsensitive);
	}

	return validFileName.toStdString();
}

WXJCOMMON_EXPORT bool wIsNoSpaceFileName( const QString &fileName )
{
	return wContains(fileName, QChar(' '));
}

WXJCOMMON_EXPORT QString wMakeNoSpaceFileName( const QString &fileName )
{
	if (wIsNoSpaceFileName(fileName))
	{
		return fileName;
	}
	else
	{
		QString noSpaceFileName(fileName);
		noSpaceFileName.replace(' ',"_");
		return noSpaceFileName;
	}
}

WXJCOMMON_EXPORT wint64 wFileSize( const QString &fileName )
{
	return QFileInfo(fileName).size();
}

WXJCOMMON_EXPORT QString wAppDataPath()
{
#ifdef W_OS_WIN
	char path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA , 0,SHGFP_TYPE_CURRENT, path)))
	{
		return QString::fromLocal8Bit(path);
	}
	return "";
#else
	return "";
#endif
}

WXJCOMMON_EXPORT QString wMyDocumentPath()
{
#ifdef W_OS_WIN
	char path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_COMMON_DOCUMENTS , 0,SHGFP_TYPE_CURRENT, path)))
	{
		return QString::fromLocal8Bit(path);
	}
	return "";
#else
	return "";
#endif
}

WXJCOMMON_EXPORT bool wMakeDir(const QString &dir)
{
	return QDir().mkpath(dir);
}
