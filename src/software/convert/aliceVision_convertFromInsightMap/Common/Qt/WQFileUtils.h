#ifndef WXJ_COMMON_WQFILEUTILS_H
#define WXJ_COMMON_WQFILEUTILS_H
#ifdef WXJ_QT
#include <QString>
#include "WXJCommonGlobal.h"
#include "WXJTypeDefs.h"

WXJCOMMON_EXPORT bool wFileExist(const QString &fullFilePath);

//�õ��ļ�����������չ��
WXJCOMMON_EXPORT QString wFileName(const QString &fullFilePath);

//�õ��ļ�������������չ��,abc.de.fg ������ abc
WXJCOMMON_EXPORT QString wBaseFileName(const QString &fullFilePath);

//�õ���ȫ�ļ�������������չ�� abc.de.fg ������ abc.de
WXJCOMMON_EXPORT QString wBaseCompleteFileName(const QString &fullFilePath);

//�õ��ļ�·�����������ļ���
WXJCOMMON_EXPORT QString wFilePath(const QString &fullFilePath);

//�õ��ļ���չ�� abc.de.fg ������ fg
WXJCOMMON_EXPORT QString wFileSuffix(const QString &fullFilePath);

//�õ���ǰexe����ȫ·���������ļ���
WXJCOMMON_EXPORT QString wExeFullFilePath();

//�õ���ǰexe·�����������ļ���
WXJCOMMON_EXPORT QString wExeDir();

//����Ӧ�ó���ĵ�ǰ·����������óɹ�����true
//@see wCurrentDir
WXJCOMMON_EXPORT bool wSetCurrentDir(const QString &fullDir);

//�õ�Ӧ�ó���ĵ�ǰ·������·�������һ��wSetCurrentDir���á�
//�����δ���ù����򷵻�Ӧ�ó�������·��
//@see wSetCurrentDir
WXJCOMMON_EXPORT QString wCurrentDir();

//�ж��ļ��Ƿ���dll���ǣ�����true������ false
WXJCOMMON_EXPORT bool wFileIsLibrary(const QString &fileWithSuffix);

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
WXJCOMMON_EXPORT bool wCopyFile(const QString &desFile, const QString &srcFile, bool failIfExists);

/**
 * @ brief : ɾ��ָ���ļ�.����ļ��������򷵻�false��ɾ���ɹ�����true������false��
 * @ author : huyang.
 * @ date : 2014-10-07.
 **/
WXJCOMMON_EXPORT bool wDeleteFile(const QString &fileName);

//��ȡ����ϵͳ�ɽ��ܵ��ļ���
//����ʵ��Ϊ����fileName�в��Ϸ����ַ��滻Ϊ'_'
WXJCOMMON_EXPORT std::string wMakeValidFileName(const QString &fileName);

WXJCOMMON_EXPORT bool wIsNoSpaceFileName(const QString &fileName);

WXJCOMMON_EXPORT QString wMakeNoSpaceFileName(const QString &fileName);

//����ļ��Ĵ�С
WXJCOMMON_EXPORT wint64 wFileSize(const QString &fileName);

//��ȡӦ�ó�������·����AppData·����
WXJCOMMON_EXPORT QString wAppDataPath();

//��á��ҵ��ĵ���·��
WXJCOMMON_EXPORT QString wMyDocumentPath();

//�ж�·���Ƿ����
WXJCOMMON_EXPORT bool wDirExist( const QString &dirFullPath );

WXJCOMMON_EXPORT bool wMakeDir(const QString &dir);
#endif//WXJ_QT
#endif