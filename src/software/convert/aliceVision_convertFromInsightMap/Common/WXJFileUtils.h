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
//fullFilePath ��ʾ�ļ��ľ���·���������ļ���
WXJCOMMON_EXPORT bool wFileExist(const char *fullFilePath);
WXJCOMMON_EXPORT bool wFileExist(const wchar_t *fullFilePath);

//�õ��ļ�����������չ��
WXJCOMMON_EXPORT std::string wFileName(const char *fullFilePath);
WXJCOMMON_EXPORT std::wstring wFileName(const wchar_t *fullFilePath);

//�õ��ļ�������������չ��,abc.de.fg ������ abc
WXJCOMMON_EXPORT std::string wBaseFileName(const char *fullFilePath);
WXJCOMMON_EXPORT std::wstring wBaseFileName(const wchar_t *fullFilePath);

//�õ���ȫ�ļ�������������չ�� abc.de.fg ������ abc.de
WXJCOMMON_EXPORT std::string wBaseCompleteFileName(const char *fullFilePath);
WXJCOMMON_EXPORT std::wstring wBaseCompleteFileName(const wchar_t *fullFilePath);

//�õ��ļ�·�����������ļ���
WXJCOMMON_EXPORT std::string wFilePath(const char *fullFilePath);
WXJCOMMON_EXPORT std::wstring wFilePath(const wchar_t *fullFilePath);

//�õ��ļ���չ�� abc.de.fg ������ fg
WXJCOMMON_EXPORT std::string wFileSuffix(const char *fullFilePath);
WXJCOMMON_EXPORT std::wstring wFileSuffix(const wchar_t *fullFilePath);

//�õ���ǰexe����ȫ·���������ļ���
WXJCOMMON_EXPORT std::string wExeFullFilePathA();
WXJCOMMON_EXPORT std::wstring wExeFullFilePathW();

//�õ���ǰexe·�����������ļ���
WXJCOMMON_EXPORT std::string wExeDirA();
WXJCOMMON_EXPORT std::wstring wExeDirW();

//����Ӧ�ó���ĵ�ǰ·����������óɹ�����true
//@see wCurrentDir
WXJCOMMON_EXPORT bool wSetCurrentDir(const char *fullDir);
WXJCOMMON_EXPORT bool wSetCurrentDir(const wchar_t *fullDir);

//�õ�Ӧ�ó���ĵ�ǰ·������·�������һ��wSetCurrentDir���á�
//�����δ���ù����򷵻�Ӧ�ó�������·��
//@see wSetCurrentDir
WXJCOMMON_EXPORT std::string wCurrentDirA();
WXJCOMMON_EXPORT std::wstring wCurrentDirW();

//�ж��ļ��Ƿ���dll���ǣ�����true������ false
WXJCOMMON_EXPORT bool wFileIsLibrary(const char *fileWithSuffix);
WXJCOMMON_EXPORT bool wFileIsLibrary(const wchar_t *fileWithSuffix);

/*************************************************************************
 @ author : huyang.
 @ date 2014-10-07.
 @ param :
   1��desFile : ������Ŀ���ļ�
   2��srcFile : Դ�ļ� 
   3��failIfExists : ������ʶ; failIfExists == true ��������ļ����ھ�ʧ�ܣ�
                               failIfExists == false��������ļ����ھ͸��ǣ�
 @ return ��true �ɹ�, false ʧ��
*************************************************************************/
WXJCOMMON_EXPORT bool wCopyFile(const char *desFile, const char *srcFile, bool failIfExists);
WXJCOMMON_EXPORT bool wCopyFile(const wchar_t *desFile, const wchar_t *srcFile, bool failIfExists);

/**
 * @ brief : ɾ��ָ���ļ�.����ļ��������򷵻�false��ɾ���ɹ�����true������false��
 * @ author : huyang.
 * @ date : 2014-10-07.
 **/
WXJCOMMON_EXPORT bool wDeleteFile(const char *fileName);
WXJCOMMON_EXPORT bool wDeleteFile(const wchar_t *fileName);

//��ȡ����ϵͳ�ɽ��ܵ��ļ���
//����ʵ��Ϊ����fileName�в��Ϸ����ַ��滻Ϊ'_'
WXJCOMMON_EXPORT std::string wMakeValidFileName(const char *fileName);
WXJCOMMON_EXPORT std::wstring wMakeValidFileName(const wchar_t *fileName);

WXJCOMMON_EXPORT bool wIsNoSpaceFileName(const char *fileName);
WXJCOMMON_EXPORT bool wIsNoSpaceFileName(const wchar_t *fileName);

WXJCOMMON_EXPORT std::string wMakeNoSpaceFileName(const char *fileName);
WXJCOMMON_EXPORT std::wstring wMakeNoSpaceFileName(const wchar_t *fileName);

//����ļ��Ĵ�С
WXJCOMMON_EXPORT wint64 wFileSize(const char *fileName);
WXJCOMMON_EXPORT wint64 wFileSize(const wchar_t *fileName);

//��ȡӦ�ó�������·����AppData·����
WXJCOMMON_EXPORT std::string wAppDataPathA();
WXJCOMMON_EXPORT std::wstring wAppDataPathW();

//��á��ҵ��ĵ���·��
WXJCOMMON_EXPORT std::string wMyDocumentPathA();
WXJCOMMON_EXPORT std::wstring wMyDocumentPathW();

//��õ�ǰ���ڴ����������������ڴ�������ڴ棨��λ �ֽڣ�
WXJCOMMON_EXPORT wuint64 wCurrentTotalMemoryCount();

//��õ�ǰ�����ڴ����� ����λ �ֽڣ�
WXJCOMMON_EXPORT wuint64 wCurrentPhysMemoryCount();

//��õ�ǰ�����ڴ���������λ �ֽڣ�
WXJCOMMON_EXPORT wuint64 wCurrentVirtualMemoryCount();

//��õ�ǰϵͳcpu����
WXJCOMMON_EXPORT int wSystemCPUCount();

//�ж�ĳһ·���Ƿ����
WXJCOMMON_EXPORT bool wDirExist(const std::string &dir);
WXJCOMMON_EXPORT bool wDirExist(const std::wstring &dir);

//����·��������ϼ�Ŀ¼�����ڣ��ú�������д�������������ɹ���·���Ѿ����ڷ���true�����򷵻�false
WXJCOMMON_EXPORT bool wMakeDir(const std::string &dir);
WXJCOMMON_EXPORT bool wMakeDir(const std::wstring &dir);

WXJCOMMON_EXPORT bool wExecute(const std::string &dir,const std::string &exe,const std::string &params);

//start ����handle��Ҫwait��close
typedef void * winhandle;
WXJCOMMON_EXPORT winhandle wStart(const std::string &dir, const std::string &exe, const std::string &params);
#endif
