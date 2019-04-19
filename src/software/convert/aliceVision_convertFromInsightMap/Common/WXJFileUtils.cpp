#include <QFile>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QStringList>
#define WXJ_QT
#include "WXJFileUtils.h"
#include "WXJStrUtils.h"
#include "WXJMacros.h"

#include <cstdio>
#include <iostream>

WXJCOMMON_EXPORT bool wFileExist(const char *fullFilePath)
{
	return QFile::exists(WQSTR(fullFilePath));
}

WXJCOMMON_EXPORT bool wFileExist(const wchar_t *fullFilePath)
{
	return QFile::exists(QString::fromWCharArray(fullFilePath));
}

WXJCOMMON_EXPORT std::string wFileName(const char *pszFilePath)
{
	return wToStdString(QFileInfo(WQSTR(pszFilePath)).fileName());
}

WXJCOMMON_EXPORT std::wstring wFileName(const wchar_t *fullFilePath)
{
	return QFileInfo(QString::fromWCharArray(fullFilePath)).fileName().toStdWString();
}

WXJCOMMON_EXPORT std::string wBaseFileName(const char *pszFilePath)
{
	return wToStdString(QFileInfo(WQSTR(pszFilePath)).baseName());
}

WXJCOMMON_EXPORT std::wstring wBaseFileName(const wchar_t *fullFilePath)
{
	return QFileInfo(QString::fromWCharArray(fullFilePath)).baseName().toStdWString();
}

WXJCOMMON_EXPORT std::string wBaseCompleteFileName(const char *fullFilePath)
{
	return wToStdString(QFileInfo(WQSTR(fullFilePath)).completeBaseName());
}

WXJCOMMON_EXPORT std::wstring wBaseCompleteFileName(const wchar_t *fullFilePath)
{
	return QFileInfo(QString::fromWCharArray(fullFilePath)).completeBaseName().toStdWString();
}

WXJCOMMON_EXPORT std::string wFilePath(const char *fullFilePath)
{
	return wToStdString(QFileInfo(WQSTR(fullFilePath)).dir().absolutePath());
}

WXJCOMMON_EXPORT std::wstring wFilePath(const wchar_t *fullFilePath)
{
	return QFileInfo(QString::fromWCharArray(fullFilePath)).dir().absolutePath().toStdWString();
}

WXJCOMMON_EXPORT std::string wFileSuffix(const char *fullFilePath)
{
	return wToStdString(QFileInfo(WQSTR(fullFilePath)).suffix());
}

WXJCOMMON_EXPORT std::wstring wFileSuffix(const wchar_t *fullFilePath)
{
	return QFileInfo(QString::fromWCharArray(fullFilePath)).suffix().toStdWString();
}

WXJCOMMON_EXPORT std::string wExeFullFilePathA()
{
#if defined(WIN32) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__)

	wchar_t wszModeulName[261] = {0};
	GetModuleFileNameW(0, wszModeulName, 261);
	QString strPath = QString::fromUtf16((ushort *)wszModeulName);
	return wToStdString(strPath);
#else
	return wToStdString(qApp->applicationName());
#endif
}

WXJCOMMON_EXPORT std::wstring wExeFullFilePathW()
{
#if defined(WIN32) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
	wchar_t wszModeulName[261] = {0};
	GetModuleFileNameW(0, wszModeulName, 261);
	QString strPath = QString::fromUtf16((ushort *)wszModeulName);
	return strPath.toStdWString();
#else
	qApp->applicationName().toStdWString();
#endif
}

WXJCOMMON_EXPORT std::string wExeDirA()
{
	return wFilePath(wExeFullFilePathA().c_str());
}

WXJCOMMON_EXPORT std::wstring wExeDirW()
{
	return wFilePath(wExeFullFilePathW().c_str());
}

WXJCOMMON_EXPORT std::string wCurrentDirA()
{
	return wToStdString(QDir::currentPath());
}

WXJCOMMON_EXPORT std::wstring wCurrentDirW()
{
	return QDir::currentPath().toStdWString();
}

WXJCOMMON_EXPORT bool wFileIsLibrary(const char *fileWithSuffix)
{
	return (QString::compare(wFileSuffix(fileWithSuffix).c_str(), "dll", Qt::CaseInsensitive) == 0);
}

WXJCOMMON_EXPORT bool wFileIsLibrary(const wchar_t *fileWithSuffix)
{
	return (QString::compare(QString::fromStdWString(wFileSuffix(fileWithSuffix)), "dll", Qt::CaseInsensitive) == 0);
}

WXJCOMMON_EXPORT bool wCopyFile(const char *desFile, const char *srcFile, bool failIfExists)
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

    return (QFile::copy(WQSTR(srcFile), WQSTR(desFile)));
}

WXJCOMMON_EXPORT bool wCopyFile(const wchar_t *desFile, const wchar_t *srcFile, bool failIfExists)
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
		QFile::remove(QString::fromWCharArray(desFile));
	}

	return (QFile::copy(QString::fromWCharArray(srcFile), QString::fromWCharArray(desFile)));
}

WXJCOMMON_EXPORT bool wDeleteFile(const char *fileName)
{
    if (wFileExist(fileName))
    {
        return QFile::remove(WQSTR(fileName));
    }
    return false;
}

WXJCOMMON_EXPORT bool wDeleteFile(const wchar_t *fileName)
{
	if (wFileExist(fileName))
	{
		return QFile::remove(QString::fromWCharArray(fileName));
	}
	return false;
}

/*************************************************************************
  
*************************************************************************/
WXJCOMMON_EXPORT std::string wMakeValidFileName(const char *fileName)
{
	QStringList invalidCharsList = (QStringList() << "\\" << "/" << ":" << "\"" << ">" << "<" << "\r\n" << "|" << "?" << "*");
	QString validFileName = WQSTR(fileName);
	for (int i = 0; i < invalidCharsList.count(); ++i)
	{
		validFileName.replace(invalidCharsList[i], "_", Qt::CaseInsensitive);
	}

	return wToStdString(validFileName);
}

WXJCOMMON_EXPORT std::wstring wMakeValidFileName(const wchar_t *fileName)
{
	QStringList invalidCharsList = (QStringList() << "\\" << "/" << ":" << "\"" << ">" << "<" << "\r\n" << "|" << "?" << "*");
	QString validFileName = QString::fromWCharArray(fileName);
	for (int i = 0; i < invalidCharsList.count(); ++i)
	{
		validFileName.replace(invalidCharsList[i], "_", Qt::CaseInsensitive);
	}

	return validFileName.toStdWString();
}

WXJCOMMON_EXPORT bool wIsNoSpaceFileName(const char *fileName)
{
	return wContains(fileName, ' ');
}

WXJCOMMON_EXPORT bool wIsNoSpaceFileName(const wchar_t *fileName)
{
	return wContains(fileName, ' ');
}

WXJCOMMON_EXPORT std::string wMakeNoSpaceFileName(const char *fileName)
{
	if (wIsNoSpaceFileName(fileName))
	{
		return fileName;
	}
	else
	{
		QString noSpaceFileName(WQSTR(fileName));
		noSpaceFileName.replace(' ',"_");
		return noSpaceFileName.toStdString();
	}
}

WXJCOMMON_EXPORT std::wstring wMakeNoSpaceFileName(const wchar_t *fileName)
{
	if (wIsNoSpaceFileName(fileName))
	{
		return fileName;
	}
	else
	{
		QString noSpaceFileName = QString::fromWCharArray(fileName);
		noSpaceFileName.replace(' ',"_");
		return noSpaceFileName.toStdWString();
	}
}

WXJCOMMON_EXPORT wint64 wFileSize(const char *fileName)
{
	return QFileInfo(fileName).size();
}

WXJCOMMON_EXPORT wint64 wFileSize(const wchar_t *fileName)
{
	return QFileInfo(QString::fromWCharArray(fileName)).size();
}

WXJCOMMON_EXPORT std::string wAppDataPathA()
{
#ifdef W_OS_WIN
	char path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA , 0,SHGFP_TYPE_CURRENT, path)))
	{
		return path;
	}
	return "";
#else
	return "";
#endif
}

WXJCOMMON_EXPORT std::wstring wAppDataPathW()
{
#ifdef W_OS_WIN
	wchar_t path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_APPDATA , 0,SHGFP_TYPE_CURRENT, path)))
	{
		return path;
	}
	return L"";
#else
	return L"";
#endif
}

WXJCOMMON_EXPORT std::string wMyDocumentPathA()
{
#ifdef W_OS_WIN
	char path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_COMMON_DOCUMENTS , 0,SHGFP_TYPE_CURRENT, path)))
	{
		return path;
	}
	return "";
#else
	return "";
#endif
}

WXJCOMMON_EXPORT std::wstring wMyDocumentPathW()
{
#ifdef W_OS_WIN
	wchar_t path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_COMMON_DOCUMENTS , 0,SHGFP_TYPE_CURRENT, path)))
	{
		return path;
	}
	return L"";
#else
	return L"";
#endif
}

WXJCOMMON_EXPORT wuint64 wCurrentTotalMemoryCount()
{
#ifdef W_OS_WIN
	MEMORYSTATUSEX memory;
	memory.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memory);
	return memory.ullAvailPhys + memory.ullAvailVirtual;
#else
	return 0;
#endif
}

WXJCOMMON_EXPORT wuint64 wCurrentPhysMemoryCount()
{
#ifdef W_OS_WIN
	MEMORYSTATUSEX memory;
	memory.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memory);
	return memory.ullAvailPhys;
#else
	return 0;
#endif
}

WXJCOMMON_EXPORT wuint64 wCurrentVirtualMemoryCount()
{
#ifdef W_OS_WIN
	MEMORYSTATUSEX memory;
	memory.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memory);
	return memory.ullAvailVirtual;
#else
	return 0;
#endif
}

WXJCOMMON_EXPORT bool wDirExist( const std::string &dir )
{
	return QDir(WQSTR(dir.c_str())).exists();
}

WXJCOMMON_EXPORT bool wDirExist( const std::wstring &dir )
{
	return QDir(QString::fromStdWString(dir)).exists();
}

WXJCOMMON_EXPORT bool wMakeDir( const std::string &dir )
{
	return QDir().mkpath(WQSTR(dir.c_str()));
}

WXJCOMMON_EXPORT bool wMakeDir( const std::wstring &dir )
{
	return QDir().mkpath(QString::fromStdWString(dir));
}

WXJCOMMON_EXPORT int wSystemCPUCount()
{
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	return systemInfo.dwNumberOfProcessors;
}

WXJCOMMON_EXPORT bool wExecute(const std::string &dir, const std::string &exe, const std::string &params)
{
	SHELLEXECUTEINFOA ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = exe.c_str();
	ShExecInfo.lpParameters = params.c_str();
	ShExecInfo.lpDirectory = dir.c_str();
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	if (!ShellExecuteExA(&ShExecInfo))
	{
		std::cout << "execute " << exe << "failed" << std::endl;
		return false;
	}
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	CloseHandle(ShExecInfo.hProcess);
	return true;
}

WXJCOMMON_EXPORT HANDLE wStart(const std::string &dir, const std::string &exe, const std::string &params)
{
	SHELLEXECUTEINFOA ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = exe.c_str();
	ShExecInfo.lpParameters = params.c_str();
	ShExecInfo.lpDirectory = dir.c_str();
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	if (!ShellExecuteExA(&ShExecInfo))
	{
		std::cout << "execute " << exe << "failed" << std::endl;
		return NULL;
	}
	return ShExecInfo.hProcess;
}
