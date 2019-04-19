#ifndef WXJ_COMMON_WQSTRUTILS_H
#define WXJ_COMMON_WQSTRUTILS_H
#ifdef WXJ_QT
#include <QString>
#include <string>

#include "WXJCommonGlobal.h"
#include "WXJTypeDefs.h"
#include "WXJMacros.h"
#define WQSTR(str) \
	QString::fromLocal8Bit(str)

// #if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
// #   define WQSTR(str) QString::fromLocal8Bit(str)
// #else
// #   define WQSTR(str) QStringLiteral(str)
// #endif

//��gbk�����qstringתΪstd string
WXJCOMMON_EXPORT std::string wToStdString(const QString &str);

//�Ƚ������ַ�������Сд������
WXJCOMMON_EXPORT bool wSameText(const QString &s1, const QString &s2);

//�Ƚ������ַ�������Сд����
WXJCOMMON_EXPORT bool wSameStr(const QString &s1, const QString &s2);

//ת��ΪСд
WXJCOMMON_EXPORT QString wToLower(const QString &s1);

//ת��Ϊ��д
WXJCOMMON_EXPORT QString wToUpper(const QString &s1);

//ת��ΪСд
WXJCOMMON_EXPORT QChar wToLower(const QChar ch);

//ת��Ϊ��д
WXJCOMMON_EXPORT QChar wToUpper(const QChar ch);

//ȥ���ַ����еĿո�
WXJCOMMON_EXPORT QString wTrim(const QString &s);

//ȥ���ַ�����ߵĿո�
WXJCOMMON_EXPORT QString wTrimLeft(const QString &s);

//ȥ���ַ����ұߵĿո�
WXJCOMMON_EXPORT QString wTrimRight(const QString &s);


/**
 * һ�齫�ַ���ת��Ϊ�������͵ĺ�����ok�����Ƿ���ת��
 **/

WXJCOMMON_EXPORT int wToInt(const QString &s, bool *ok = NULL);
WXJCOMMON_EXPORT uint wToUInt(const QString &s, bool *ok = NULL);
WXJCOMMON_EXPORT short wToShort(const QString &s, bool *ok = NULL);
WXJCOMMON_EXPORT ushort wToUShort(const QString &s, bool *ok = NULL);
WXJCOMMON_EXPORT long wToLong(const QString &s, bool *ok = NULL);
WXJCOMMON_EXPORT ulong wToULong(const QString &s, bool *ok = NULL);
WXJCOMMON_EXPORT wlonglong wToLongLong(const QString &s, bool *ok = NULL);
WXJCOMMON_EXPORT wulonglong wToULongLong(const QString &s, bool *ok = NULL);
WXJCOMMON_EXPORT double wToDouble(const QString &s, bool *ok = NULL);
WXJCOMMON_EXPORT float wToFloat(const QString &s, bool *ok = NULL);

WXJCOMMON_EXPORT QString wNumberToString(short val,int base=10);  
WXJCOMMON_EXPORT QString wNumberToString(ushort val,int base=10);  
WXJCOMMON_EXPORT QString wNumberToString(int val,int base=10);  
WXJCOMMON_EXPORT QString wNumberToString(uint val,int base=10);  
WXJCOMMON_EXPORT QString wNumberToString(long val,int base=10);  
WXJCOMMON_EXPORT QString wNumberToString(ulong val,int base=10);  
WXJCOMMON_EXPORT QString wNumberToString(wlonglong val,int base=10);  
WXJCOMMON_EXPORT QString wNumberToString(wulonglong val,int base=10); 
WXJCOMMON_EXPORT QString wNumberToString(float val, char f='g', int prec=6); 
WXJCOMMON_EXPORT QString wNumberToString(double val, char f='g', int prec=6); 

//�ַ������Ƿ����ĳһ�ַ�
//cs �Ƿ���Դ�Сд
WXJCOMMON_EXPORT bool wContains(const QString &s, QChar c,Wxj::CaseSensitivity cs = Wxj::CS_Sensitive);

//�ַ������Ƿ����ĳһ���ַ���
//cs �Ƿ���Դ�Сд
WXJCOMMON_EXPORT bool wContains(const QString &s, const QString &subStr, Wxj::CaseSensitivity cs = Wxj::CS_Sensitive);

//ȡ�ַ������count�����ַ���
WXJCOMMON_EXPORT QString wLeftString(const QString &s, int nCount);

//ȡ�ַ����ұ�count�����ַ���
WXJCOMMON_EXPORT QString wRightString(const QString &s, int nCount);

//���ע����м�ֵ
//regPath��ע���·��
//regKey��ע����
//@return: ����Ӧ��ֵ
WXJCOMMON_EXPORT QString wValueFromRegistry(const QString &regPath, const QString regKey);


#endif//WXJ_QT
#endif