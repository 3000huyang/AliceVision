/*****************************************************************************************
* Copyright (c) 2013,北京无限界科技有限公司技术部
* All rights reserved.
* author: huyang.
* date: 2014-10-11
*****************************************************************************************/
#ifndef WXJ_COMMON_STRINGUTILS_H
#define WXJ_COMMON_STRINGUTILS_H
#include <string>
#include "WXJCommonGlobal.h"
#include "WXJTypeDefs.h"
#include "WXJMacros.h"
#include "Qt/WQStrUtils.h"
//大小写

#define W_DECLARE_STRING(strName) \
	extern const char *strName

#define W_CST_STRING(strName,strValue) \
	const char *strName = strValue

#define W_IMP_STRING(strName,strValue) W_CST_STRING(strName, strValue)

#define W_SCST_STRING(strName, strValue) \
	static W_CST_STRING(strName, strValue)

//设置本地字符常量编码为国标编码
WXJCOMMON_EXPORT void wSetLocaleGB18030_0();

//设置本地字符常量编码为UTF-8编码
WXJCOMMON_EXPORT void wSetLocaleUTF_8();

//设置本地字符常量编码为GBK编码
inline void wSetLocaleGBK(){wSetLocaleGB18030_0();}

//gbk编码的char 字符串转换为string
//WXJCOMMON_EXPORT std::string wToStdString(const char *pszStr);
//gbk编码的char 字符串转换为wstring
WXJCOMMON_EXPORT std::wstring wToStdWString(const char *pszStr);
//string wstring相互转换
WXJCOMMON_EXPORT std::wstring wStringToWString(const std::string &s);
WXJCOMMON_EXPORT std::string wWStringToString(const std::wstring &s);

//比较两个字符串，大小写不敏感
WXJCOMMON_EXPORT bool wSameText(const std::string &s1, const std::string &s2);
WXJCOMMON_EXPORT bool wSameText(const std::wstring &s1, const std::wstring &s2);

//比较两个字符串，大小写敏感
WXJCOMMON_EXPORT bool wSameStr(const char *s1, const char *s2);
WXJCOMMON_EXPORT bool wSameStr(const wchar_t *s1, const wchar_t *s2);

//获取C风格字符串（\0结尾的字符串）的长度
WXJCOMMON_EXPORT size_t wCStrLength(const char *sz);
WXJCOMMON_EXPORT size_t wCStrLength(const wchar_t *wsz);

//转换为小写
WXJCOMMON_EXPORT std::string wToLower(const std::string &s1);
WXJCOMMON_EXPORT std::wstring wToLower(const std::wstring &s1);
//转换为大写
WXJCOMMON_EXPORT std::string wToUpper(const std::string &s1);
WXJCOMMON_EXPORT std::wstring wToUpper(const std::wstring &s1);

//转换为小写
WXJCOMMON_EXPORT char wToLower(const char ch);
WXJCOMMON_EXPORT wchar_t wToLower(const wchar_t ch);

//转换为大写
WXJCOMMON_EXPORT char wToUpper(const char ch);
WXJCOMMON_EXPORT wchar_t wToUpper(const wchar_t ch);

//去掉字符串中的空格
WXJCOMMON_EXPORT std::string wTrim(const std::string &s);
WXJCOMMON_EXPORT std::wstring wTrim(const std::wstring &s);

//去掉字符串左边的空格
WXJCOMMON_EXPORT std::string wTrimLeft(const std::string &s);
WXJCOMMON_EXPORT std::wstring wTrimLeft(const std::wstring &s);

//去掉字符串右边的空格
WXJCOMMON_EXPORT std::string wTrimRight(const std::string &s);
WXJCOMMON_EXPORT std::wstring wTrimRight(const std::wstring &s);

/**
 * 一组将字符串转换为基本类型的函数，ok返回是否能转换
 **/
WXJCOMMON_EXPORT int wToInt(const char *s, bool *ok = NULL);
WXJCOMMON_EXPORT int wToInt(const wchar_t *s, bool *ok = NULL);
WXJCOMMON_EXPORT int wToInt(const std::string &s, bool *ok = NULL);
WXJCOMMON_EXPORT int wToInt(const std::wstring &s, bool *ok = NULL);

WXJCOMMON_EXPORT uint wToUInt(const char *s, bool *ok = NULL);
WXJCOMMON_EXPORT uint wToUInt(const wchar_t *s, bool *ok = NULL);
WXJCOMMON_EXPORT uint wToUInt(const std::string &s, bool *ok = NULL);
WXJCOMMON_EXPORT uint wToUInt(const std::wstring &s, bool *ok = NULL);

WXJCOMMON_EXPORT short wToShort(const char *s, bool *ok = NULL);
WXJCOMMON_EXPORT short wToShort(const wchar_t *s, bool *ok = NULL);
WXJCOMMON_EXPORT short wToShort(const std::string &s, bool *ok = NULL);
WXJCOMMON_EXPORT short wToShort(const std::wstring &s, bool *ok = NULL);

WXJCOMMON_EXPORT ushort wToUShort(const char *s, bool *ok = NULL);
WXJCOMMON_EXPORT ushort wToUShort(const wchar_t *s, bool *ok = NULL);
WXJCOMMON_EXPORT ushort wToUShort(const std::string &s, bool *ok = NULL);
WXJCOMMON_EXPORT ushort wToUShort(const std::wstring &s, bool *ok = NULL);

WXJCOMMON_EXPORT long wToLong(const char *s, bool *ok = NULL);
WXJCOMMON_EXPORT long wToLong(const wchar_t *s, bool *ok = NULL);
WXJCOMMON_EXPORT long wToLong(const std::string &s, bool *ok = NULL);
WXJCOMMON_EXPORT long wToLong(const std::wstring &s, bool *ok = NULL);

WXJCOMMON_EXPORT ulong wToULong(const char *s, bool *ok = NULL);
WXJCOMMON_EXPORT ulong wToULong(const wchar_t *s, bool *ok = NULL);
WXJCOMMON_EXPORT ulong wToULong(const std::string &s, bool *ok = NULL);
WXJCOMMON_EXPORT ulong wToULong(const std::wstring &s, bool *ok = NULL);

WXJCOMMON_EXPORT wlonglong wToLongLong(const char *s, bool *ok = NULL);
WXJCOMMON_EXPORT wlonglong wToLongLong(const wchar_t *s, bool *ok = NULL);
WXJCOMMON_EXPORT wlonglong wToLongLong(const std::string &s, bool *ok = NULL);
WXJCOMMON_EXPORT wlonglong wToLongLong(const std::wstring &s, bool *ok = NULL);

WXJCOMMON_EXPORT wulonglong wToULongLong(const char *s, bool *ok = NULL);
WXJCOMMON_EXPORT wulonglong wToULongLong(const wchar_t *s, bool *ok = NULL);
WXJCOMMON_EXPORT wulonglong wToULongLong(const std::string &s, bool *ok = NULL);
WXJCOMMON_EXPORT wulonglong wToULongLong(const std::wstring &s, bool *ok = NULL);

WXJCOMMON_EXPORT double wToDouble(const char *s, bool *ok = NULL);
WXJCOMMON_EXPORT double wToDouble(const wchar_t *s, bool *ok = NULL);
WXJCOMMON_EXPORT double wToDouble(const std::string &s, bool *ok = NULL);
WXJCOMMON_EXPORT double wToDouble(const std::wstring &s, bool *ok = NULL);

WXJCOMMON_EXPORT float wToFloat(const char *s, bool *ok = NULL);
WXJCOMMON_EXPORT float wToFloat(const wchar_t *s, bool *ok = NULL);
WXJCOMMON_EXPORT float wToFloat(const std::string &s, bool *ok = NULL);
WXJCOMMON_EXPORT float wToFloat(const std::wstring &s, bool *ok = NULL);

WXJCOMMON_EXPORT std::string wNumberToStringA(short val,int base=10);  
WXJCOMMON_EXPORT std::string wNumberToStringA(ushort val,int base=10);  
WXJCOMMON_EXPORT std::string wNumberToStringA(int val,int base=10);  
WXJCOMMON_EXPORT std::string wNumberToStringA(uint val,int base=10);  
WXJCOMMON_EXPORT std::string wNumberToStringA(long val,int base=10);  
WXJCOMMON_EXPORT std::string wNumberToStringA(ulong val,int base=10);  
WXJCOMMON_EXPORT std::string wNumberToStringA(wlonglong val,int base=10);  
WXJCOMMON_EXPORT std::string wNumberToStringA(wulonglong val,int base=10); 
WXJCOMMON_EXPORT std::string wNumberToStringA(float val, char f='g', int prec=6); 
WXJCOMMON_EXPORT std::string wNumberToStringA(double val, char f='g', int prec=6); 

WXJCOMMON_EXPORT std::wstring wNumberToStringW(short val,int base=10);  
WXJCOMMON_EXPORT std::wstring wNumberToStringW(ushort val,int base=10);  
WXJCOMMON_EXPORT std::wstring wNumberToStringW(int val,int base=10);  
WXJCOMMON_EXPORT std::wstring wNumberToStringW(uint val,int base=10);  
WXJCOMMON_EXPORT std::wstring wNumberToStringW(long val,int base=10);  
WXJCOMMON_EXPORT std::wstring wNumberToStringW(ulong val,int base=10);  
WXJCOMMON_EXPORT std::wstring wNumberToStringW(wlonglong val,int base=10);  
WXJCOMMON_EXPORT std::wstring wNumberToStringW(wulonglong val,int base=10); 
WXJCOMMON_EXPORT std::wstring wNumberToStringW(float val, char f='g', int prec=6); 
WXJCOMMON_EXPORT std::wstring wNumberToStringW(double val, char f='g', int prec=6); 

//字符串中是否存在某一字符
//cs 是否忽略大小写
WXJCOMMON_EXPORT bool wContains(const std::string &s, char c,Wxj::CaseSensitivity cs = Wxj::CS_Sensitive);
WXJCOMMON_EXPORT bool wContains(const std::wstring &s, wchar_t c, Wxj::CaseSensitivity cs = Wxj::CS_Sensitive);

//字符串中是否存在某一子字符串
//cs 是否忽略大小写
WXJCOMMON_EXPORT bool wContains(const std::string &s, const std::string &subStr, Wxj::CaseSensitivity cs = Wxj::CS_Sensitive);
WXJCOMMON_EXPORT bool wContains(const std::wstring &s, const std::wstring &subStr, Wxj::CaseSensitivity cs = Wxj::CS_Sensitive);

//取字符串左边count个子字符串
WXJCOMMON_EXPORT std::string wLeftString(const std::string &s, int nCount);
WXJCOMMON_EXPORT std::wstring wLeftString(const std::wstring &s, int nCount);

//取字符串右边count个子字符串
WXJCOMMON_EXPORT std::string wRightString(const std::string &s, int nCount);
WXJCOMMON_EXPORT std::wstring wRightString(const std::wstring &s, int nCount);

//获得注册表中键值
//regPath：注册表路径
//regKey：注册表键
//@return: 键对应的值
WXJCOMMON_EXPORT std::string wValueFromRegistry(const char *regPath, const char *regKey);
WXJCOMMON_EXPORT std::wstring wValueFromRegistry(const wchar_t *regPath, const wchar_t *regKey);



//
/*


//QString utf8ToUnicode(const char *in, int length = -1);
//QByteArray unicodeToUTF8(const GChar *in, int length = -1);
//inline QByteArray unicodeToUTF8(const QString &in) { return unicodeToUTF8(in.constData(), in.length()); }
//
//QString asciiToUnicode(const char *in, int length = -1, const char *name = NULL);
//QByteArray unicodeToAscii(const GChar *in, int length = -1, const char *name = NULL);
//inline QByteArray unicodeToAscii(const QString &in, const char *name = NULL) { return unicodeToAscii(in.constData(), in.length(), name); }
//
//QString gbkToUnicode(const char *in, int length = -1);
//QByteArray unicodeToGBK(const GChar *in, int length = -1);
//inline QByteArray unicodeToGBK(const QString &in) { return unicodeToGBK(in.constData(), in.length()); }
//
//QString ansiUpperCase(const QString &value);
//QString ansiLowerCase(const QString &value);
//
//QString upperCase(const QString &value);
//QString lowerCase(const QString &value);
//
//char upperCase(const char ch);
//char lowerCase(const char ch);
//
//wchar_t upperCase(const wchar_t ch);
//wchar_t lowerCase(const wchar_t ch);
//
//int compareStr(const QStringRef &s1, const QStringRef &s2);
//int compareStr(const QStringRef &s1, const QString &s2);
//int compareStr(const QString &s1, const QString &s2);
//int compareStr(const QByteArray &s1, const QByteArray &s2);
//
//int compareText(const QStringRef &s1, const QStringRef &s2);
//int compareText(const QStringRef &s1, const QString &s2);
//int compareText(const QString &s1, const QString &s2);
//int compareText(const QByteArray &s1, const QByteArray &s2);
//
//GLDValueRelationship compareGVariant(const GVariant &v1, const GVariant &v2);
//bool sameGVariant(const GVariant &v1, const GVariant &v2);
//
//bool sameStr(const QString &s1, const QString &s2);
//// 不区分大小写
//inline bool sameText(const QString &s1, const QString &s2)
//{
//    return 0 == s1.compare(s2, Qt::CaseInsensitive);
//}
//// 不区分大小写
//inline bool sameText(const QString &s1, const GLatin1String &s2)
//{
//    return 0 == s1.compare(s2, Qt::CaseInsensitive);
//}
//
//// 不区分大小写
//inline bool sameText(const QStringRef &s1, const GLatin1String &s2)
//{
//    return 0 == s1.compare(s2, Qt::CaseInsensitive);
//}
//
//int length(const QString &s);
//int pos(const QString &subs, const QString &s);
//int pos(const GChar &subs, const QString &s);
//int pos(const QString &subs, const QString &s, int nFrom);
//int rPos(const QString &subs, const QString &s, int times = 1);
//int rPos(const GChar &subs, const QString &s, int times = 1);
//int rPosEx(const GChar &subs, const QString &s, int offset);
//QString trim(const QString &s);
//QString trimRight(const QString &s);
//QString trimLeft(const QString &s);
//QString copy(const QString &s, int position, int n = -1);
//QString stringReplace(const QString &s, const QString &before, const QString &after);
//bool containsText(const QString &text, const QString subText);
//QString leftStr(const QString &text, int count);
//QString rightStr(const QString &text, int count);
//QStringList split(const QString &s, GChar sep);
//QStringList split(const QString &s, const QString &sep);
//
//bool isInt(const QString &s);
//bool isInt64(const QString &s);
//bool isUInt64(const QString &s);
//bool isNumeric(const QString &s);
//bool isDateTime(const QString &s);
//
//bool charIsDigit(const GChar &ch);
//
//bool variantTypeIsByte(const GVariant::Type type);
//bool variantTypeIsShort(const GVariant::Type type);
//bool variantTypeIsInt(const GVariant::Type type);
//inline bool variantTypeIsInt64(const GVariant::Type type)
//{ return (type == GVariant::LongLong || type == GVariant::ULongLong); }
//bool variantTypeIsDigit(const GVariant::Type type);
//bool variantTypeIsFloat(const GVariant::Type type);
//bool variantTypeIsNumeric(const GVariant::Type type);
//bool variantTypeIsDateTime(const GVariant::Type type);
//bool variantTypeIsUnsigned(const GVariant::Type type);
//
//#ifdef WIN32
//QString BSTRToQString(const BSTR & s);
//BSTR QStringToBSTR(const QString & s);
//void freeBSTR(BSTR & s);
//#endif
//
//QString format(const QString &s, const GVariantList &a);
//QString format(const QString &s, const QString &a, const QString &before = QString("%s"));
//QString format(const QString &s, int a);
//QString format(const QString &s, long a);
//QString format(const QString &s, long long a);
//QString format(const QString &s, double a);
//QString format(const QString &s, const GChar &a, const QString &before = QString("%s"));
////QString format(const QString &s, const GVariant &a);
//
//QString boolToStr(bool a, bool useBoolStrs = false);
//bool strToBool(const QString &s);
//bool strToBoolDef(const QString &s, bool def = false);
//QString intToStr(int a);
//QString int64ToStr(gint64 a);
//QString uint64ToStr(guint64 a);
//int strToInt(const QString &s);
//int strToIntDef(const QString &s, int def);
//gint64 strToInt64(const QString &s);
//gint64 strToInt64Def(const QString &s, gint64 def);
//guint64 strToUInt64(const QString &s);
//guint64 strToUInt64Def(const QString &s, guint64 def);
//QString floatToStr(double a);
//double strToFloat(const QString &s);
//double strToFloatDef(const QString &s, double def);
//QString dateTimeToStr(const GDateTime &datetime, QString format = "yyyy-MM-dd hh:mm:ss");
//GDateTime strToDateTime(const QString &s, QString format = "yyyy-M-d hh:mm:ss");
//QString byteArrayToStr(const QByteArray &a);
//QByteArray strToByteArray(const QString &s);
//QString intToHex(int value, int digits);
//
//QString stuffString(const QString &text, int nStart, int nLength, const QString &subText);
//int occurs(const char delimiter, const QString &srcStr);
//
//QByteArray base64Encode(const QByteArray &in);
//GStream* base64Encode(GStream *in);
//
//QByteArray base64Decode(const QByteArray &in);
//GStream* base64Decode(GStream *in);
//
//QString quotedStr(const QString &str, const char quote = '\'');
//QByteArray quotedStr(const QByteArray &str, const char quote);
//
//QString dequotedStr(const QString &str, const char quote);
//QByteArray dequotedStr(const QByteArray &str, const char quote);
//
//QString extractQuotedStr(QString &str, const char quote);
//
//GStream* stringToStream(const QString &in);
//QString streamToString(GStream* in);
//
//GStream* byteArrayToStream(const QByteArray &in);
//QByteArray streamToByteArray(GStream* in);
//
//GDate intToDate(int d);
//int dateToInt(const GDate &date);
//
//GTime doubleToTime(double t);
//double timeToDouble(const GTime &time);
//
//GDateTime doubleToDateTime(double d);
//double dateTimeToDouble(const GDateTime &dateTime);
//
//int yearOf(double d);
//int monthOf(double d);
//int weekOf(double d);
//int dayOf(double d);
//int hourOf(double d);
//int minuteOf(double d);
//int secondOf(double d);
//int milliSecondOf(double d);
//
//struct GFormatSettings
//{
//    byte currencyFormat;
//    byte negCurrFormat;
//    char thousandSeparator;
//    char decimalSeparator;
//    byte currencyDecimals;
//    char dateSeparator;
//    char timeSeparator;
//    char listSeparator;
//    QString currencyString;
//    QString shortDateFormat;
//    QString longDateFormat;
//    QString timeAMString;
//    QString timePMString;
//    QString shortTimeFormat;
//    QString longTimeFormat;
//    QString shortMonthNames[12];
//    QString longMonthNames[12];
//    QString shortDayNames[7];
//    QString longDayNames[7];
//    int twoDigitYearCenturyWindow;
//};
//
//double chsStrToDateTime(const QString &s);
//QString dateTimeToChsStr(double dateTime);
//bool isChsDateTime(const QString &s);
//bool tryStrToDateTime(const QString &s, GDateTime &value);
//bool tryStrToDateTime(const QString &s, GDateTime &value,
//  const GFormatSettings &formatSettings);
//
//bool sameDateTime(double dateTime1, double dateTime2);
//GLDValueRelationship compareDateTime(double dateTime1, double dateTime2);
//
//QString getHZPY(const QString &src);
//QString firstLetter(int nCode);
//QString reverseString(const QString &s);
//QString regExprReplace(const QString &text, const QString &regEx, const QString &replacement);
//
//QString getSubString(const QString &srcStr, char delimiter, int index);
//QString getSubString(const QString &srcStr, const QString &delimiter, int index);
//
//int posN(const char delimiter, const QString srcStr, int times = 1);
//int posN(const QString subStr, const QString srcStr, int times = 1);
//
//QString stringOfChar(char Char, int count);
//
//
//QString boolToXMLString(bool v);
//bool xmlStringToBool(const QString &s);
//QString floatToXMLString(double v);
//QString encodeXMLString(const QString &value, bool encodeCrLf);
//
///**
//  GUID 相关处理函数
//*/
//#include "GLDGuidDef.h"
//QString createGuidString();
//GUID createGUID();
//QString GUIDToStr(const GUID &a);
//GUID strToGUID(const QString &text);
//GVariant GUIDToVariant(const GUID &a);
//GUID variantToGUID(const GVariant &text);
//QByteArray GUIDToByteArray(const GUID value);
//GUID byteArrayToGUID(const QByteArray &ba);
//int compareGUID(const GUID &g1, const GUID &g2);
//bool isGUID(const QString &s);
//bool variantTypeIsGUID(const GVariant::Type type);
//GUuid GUIDToGUuid(const GUID &value);
//GUID GUuidToGUID(const GUuid &value);
//
//template <typename Set, typename T>
//inline void include(Set &set, const T t)
//{
//    set |= (byte(1) << byte(t));
//}
//
//template <typename Set, typename T>
//inline void exclude(Set &set, const T t)
//{
//    set &= ~(byte(1) << byte(t));
//}
//
//template <typename Set, typename T>
//inline bool contains(const Set &set, const T t)
//{
//    return set & (byte(1) << byte(t));
//}
//
////type:0(), 1(], 2[), 3[]
//template <typename T>
//Q_DECL_CONSTEXPR inline bool isIntRange2(const T &min, const T &val, const T &max, int type = 3)
//{
//    return ((1 == (type & 1)) ? (val <= max) : (val < max))
//            && ((2 == (type & 2)) ? (val >= min) : (val > min));
//}
#endif // GLDSTRUTILS_H
