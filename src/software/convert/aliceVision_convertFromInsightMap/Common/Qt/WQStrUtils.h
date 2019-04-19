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

//将gbk编码的qstring转为std string
WXJCOMMON_EXPORT std::string wToStdString(const QString &str);

//比较两个字符串，大小写不敏感
WXJCOMMON_EXPORT bool wSameText(const QString &s1, const QString &s2);

//比较两个字符串，大小写敏感
WXJCOMMON_EXPORT bool wSameStr(const QString &s1, const QString &s2);

//转换为小写
WXJCOMMON_EXPORT QString wToLower(const QString &s1);

//转换为大写
WXJCOMMON_EXPORT QString wToUpper(const QString &s1);

//转换为小写
WXJCOMMON_EXPORT QChar wToLower(const QChar ch);

//转换为大写
WXJCOMMON_EXPORT QChar wToUpper(const QChar ch);

//去掉字符串中的空格
WXJCOMMON_EXPORT QString wTrim(const QString &s);

//去掉字符串左边的空格
WXJCOMMON_EXPORT QString wTrimLeft(const QString &s);

//去掉字符串右边的空格
WXJCOMMON_EXPORT QString wTrimRight(const QString &s);


/**
 * 一组将字符串转换为基本类型的函数，ok返回是否能转换
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

//字符串中是否存在某一字符
//cs 是否忽略大小写
WXJCOMMON_EXPORT bool wContains(const QString &s, QChar c,Wxj::CaseSensitivity cs = Wxj::CS_Sensitive);

//字符串中是否存在某一子字符串
//cs 是否忽略大小写
WXJCOMMON_EXPORT bool wContains(const QString &s, const QString &subStr, Wxj::CaseSensitivity cs = Wxj::CS_Sensitive);

//取字符串左边count个子字符串
WXJCOMMON_EXPORT QString wLeftString(const QString &s, int nCount);

//取字符串右边count个子字符串
WXJCOMMON_EXPORT QString wRightString(const QString &s, int nCount);

//获得注册表中键值
//regPath：注册表路径
//regKey：注册表键
//@return: 键对应的值
WXJCOMMON_EXPORT QString wValueFromRegistry(const QString &regPath, const QString regKey);


#endif//WXJ_QT
#endif