#include <algorithm>
#include <QTextCodec>
#include <QSettings>
#include <QDir>
#include "WXJStrUtils.h"

//#ifdef WIN32
//#   include <Windows.h>
//#else
//
//#endif
//#include <string>
//
//#include <QLocale>
//#include <QTextCodec>
//
//#include "GLDGlobal.h"
//#include "GLDTypes.h"
//#include "GLDStrUtils.h"
//#include "GLDSysUtils.h"
//#include "GLDMathUtils.h"
//#include "GLDException.h"
////#include "GEPStrings.h"
//#include "GLDStrings.h"
//#include "GLDStream.h"
//
//const int c_HoursPerDay   = 24;
//const int c_MinsPerHour   = 60;
//const int c_SecsPerMin    = 60;
//const int c_MSecsPerSec   = 1000;
//const int c_MinsPerDay    = c_HoursPerDay * c_MinsPerHour;
//const int c_SecsPerDay    = c_MinsPerDay * c_SecsPerMin;
//const int c_MSecsPerDay   = c_SecsPerDay * c_MSecsPerSec;
//
//static GTextCodec *s_gldGBKCodec = NULL;
//static GTextCodec *s_gldUTF8Codec = NULL;
//
//#ifdef WIN32
//    DEFINE_RESOURCESTRING(sLineBreak, "\r\n");
//#else
//    DEFINE_RESOURCESTRING(sLineBreak, "\r");
//#endif
//
//class GLDTextCodec
//{
//public:
//    static GTextCodec *codecForGBK();
//    static GTextCodec *codecForUTF8();
//    static GTextCodec *codecForLocale();
//};
//
///* GLDTextCodec */
//
//GTextCodec *GLDTextCodec::codecForGBK()
//{
//    if (!s_gldGBKCodec)
//    {
//        s_gldGBKCodec = GTextCodec::codecForName("GBK");
//        assert(s_gldGBKCodec);
//    }
//    return s_gldGBKCodec;
//}
//
//GTextCodec *GLDTextCodec::codecForUTF8()
//{
//    if (!s_gldUTF8Codec)
//    {
//        s_gldUTF8Codec = GTextCodec::codecForName("UTF-8");
//        assert(s_gldUTF8Codec);
//    }
//    return s_gldUTF8Codec;
//}
//
//GTextCodec *GLDTextCodec::codecForLocale()
//{
//    return GTextCodec::codecForLocale();
//}
//
//// public
///*
////获取C风格字符串（\0结尾的字符串）的长度
//size_t CStrLength(const char *sz)
//{
//    int i = 0;
//    while (sz[i])
//    {
//        ++i;
//    }
//    return i;
//}
//
//size_t CStrLength(const wchar_t *wsz)
//{
//    return WideCStrLength(wsz);
//}
//
//size_t WideCStrLength(const wchar_t *wsz)
//{
//    int i = 0;
//    while (wsz[i])
//    {
//        i++;
//    }
//    return i;
//}
//
//wchar_t *WideLowerCase(wchar_t **ppWsz)
//{
//    QString s((const GChar *)(*ppWsz));
//    s = s.toLower();
//    wstring ws = s.toLower().toStdWString();
//
//    wchar_t *wCh = new wchar_t[ws.size() + 1];
//    wCh[ws.size()] = '\0';
//    for (int i = 0; i < int(ws.size()); ++i)
//    {
//        wCh[i] = ws[i];
//    }
//    return wCh;
//}
//
////比较两个字符串，大小写敏感
//bool SameStr(const char *szA, const char *szB)
//{
//    return 0 == strcmp(szA, szB);
//}
//
////比较两个字符串，大小写敏感，宽字符版本
//bool WideSameStr(const wchar_t *wszA, const wchar_t *wszB)
//{
//    return 0 == wcscmp(wszA, wszB);
//}
//
////比较两个字符串，大小写不敏感
//bool SameText(const char *szA, const char *szB)
//{
//#ifdef WIN32
//    return 0 == _stricmp(szA, szB);
//#else
//    return 0 == strcasecmp(szA, szB);
//#endif
//}
//
////比较两个字符串，大小写不敏感，宽字符版本
//bool WideSameText(const wchar_t *wszA, const wchar_t *wszB)
//{
//#ifdef WIN32
//    return 0 == _wcsicmp(wszA, wszB);
//#else
//    return 0 == sameText(QString((const GChar *)wszA),
//                         QString((const GChar *)wszB));
//#endif
//}
//
//wchar_t *charToWchar(const char *str)
//{
//    wchar_t* buffer;
//    if(str)
//    {
//        size_t nu = strlen(str);
//        size_t n =(size_t)MultiByteToWideChar(CP_ACP,0,(const char *)str,int(nu),NULL,0);
//        buffer=0;
//        buffer = new wchar_t[n+1];
//        ::MultiByteToWideChar(CP_ACP,0,(const char *)str,int(nu),buffer,int(n));
//        buffer[n] = 0;
//    }
//    return buffer;
//    delete buffer;
//}
//
//char *wcharToChar(const wchar_t *pwstr)
//{
//    int nlength = (int)wcslen(pwstr);
//    //获取转换后的长度
//    int nbytes = WideCharToMultiByte( 0,         // specify the code page used to perform the conversion
//                                      0,         // no special flags to handle unmapped characters
//                                      pwstr,     // wide character string to convert
//                                      nlength,   // the number of wide characters in that string
//                                      NULL,      // no output buffer given, we just want to know how long it needs to be
//                                      0,
//                                      NULL,      // no replacement character given
//                                      NULL );    // we don't want to know if a character didn't make it
//                                                 // through the translation
//    char *pcstr = new char[nbytes + 1];
//    // 通过以上得到的结果，转换unicode 字符为ascii 字符
//    WideCharToMultiByte( 0,         // specify the code page used to perform the conversion
//                         0,         // no special flags to handle unmapped characters
//                         pwstr,     // wide character string to convert
//                         nlength,   // the number of wide characters in that string
//                         pcstr,     // put the output ascii characters at the end of the buffer
//                         nbytes,    // there is at least this much space there
//                         NULL,      // no replacement character given
//                         NULL );
//    pcstr[nbytes] = 0;
//    return pcstr;
//}
//*/
//
//QString utf8ToUnicode(const char *in, int length)
//{
//    return GLDTextCodec::codecForUTF8()->toUnicode(
//                in, (-1 != length) ? length : (int)strlen(in));
//}
//
//QByteArray unicodeToUTF8(const GChar *in, int length)
//{
//    return GLDTextCodec::codecForUTF8()->fromUnicode(
//                in, (-1 != length) ? length : (int)wcslen((const wchar_t *)in));
//}
//
//QString asciiToUnicode(const char *in, int length, const char *name)
//{
//    if (name == NULL)
//    {
//        return GLDTextCodec::codecForLocale()->toUnicode(
//                    in, (-1 != length) ? length : (int)strlen(in));
//    }
//    else
//    {
//        return GTextCodec::codecForName(name)->toUnicode(
//                    in, (-1 != length) ? length : (int)strlen(in));
//    }
//
//}
//
//QByteArray unicodeToAscii(const GChar *in, int length, const char *name)
//{
//    if (name == NULL)
//    {
//        return GLDTextCodec::codecForGBK()->fromUnicode(
//                    in, (-1 != length) ? length : (int)wcslen((const wchar_t *)in));
//    }
//    else
//    {
//        return GTextCodec::codecForName(name)->fromUnicode(
//                    in, (-1 != length) ? length : (int)wcslen((const wchar_t *)in));
//    }
//}
//
//QString gbkToUnicode(const char *in, int length)
//{
//    return GLDTextCodec::codecForGBK()->toUnicode(
//                in, (-1 != length) ? length : (int)strlen(in));
//}
//
//QByteArray unicodeToGBK(const GChar *in, int length)
//{
//    return GLDTextCodec::codecForGBK()->fromUnicode(
//                in, (-1 != length) ? length : (int)wcslen((const wchar_t *)in));
//}
//
//QString ansiUpperCase(const QString &value)
//{
//    return value.toUpper();
//}
//
//QString ansiLowerCase(const QString &value)
//{
//    return value.toLower();
//}
//
//QString upperCase(const QString &value)
//{
//    return value.toUpper();
//}
//
//QString lowerCase(const QString &value)
//{
//    return value.toLower();
//}
//
//char upperCase(const char ch)
//{
//    GChar chStr(ch);
//    return chStr.toUpper().toLatin1();
//}
//
//char lowerCase(const char ch)
//{
//    GChar chStr(ch);
//    return chStr.toLower().toLatin1();
//}
//
//wchar_t upperCase(const wchar_t ch)
//{
//    if ((int)ch > 256)
//    {
//        return ch;
//    }
//    GChar chStr(ch);
//    return chStr.toUpper().toLatin1();
//}
//
//wchar_t lowerCase(const wchar_t ch)
//{
//    if ((int)ch > 256)
//    {
//        return ch;
//    }
//    GChar chStr(ch);
//    return chStr.toLower().toLatin1();
//}
//
//int compareStr(const QStringRef &s1, const QStringRef &s2)
//{
//    return QStringRef::compare(s1, s2, Qt::CaseSensitive);
//}
//
//int compareStr(const QStringRef &s1, const QString &s2)
//{
//    return s1.compare(s2, Qt::CaseSensitive);
//}
//
//// call CompareString : 'A' > 'a' > '9' > '0'
//int compareStr(const QString &s1, const QString &s2)
//{
//    return QString::localeAwareCompare(s1, s2);
//}
//
//int compareStr(const QByteArray &s1, const QByteArray &s2)
//{
//    return strcmp(s1.constData(), s2.constData());
//}
//
//int compareText(const QStringRef &s1, const QStringRef &s2)
//{
//    return s1.compare(s1, s2, Qt::CaseInsensitive);
//}
//
//int compareText(const QStringRef &s1, const QString &s2)
//{
//    return s1.compare(s2, Qt::CaseInsensitive);
//}
//
//int compareText(const QString &s1, const QString &s2)
//{
//    return QString::localeAwareCompare(s1.toUpper(), s2.toUpper());
//}
//
//int compareText(const QByteArray &s1, const QByteArray &s2)
//{
//#ifdef WIN32
//    return _stricmp(s1.constData(), s2.constData());
//#else
//    return strcasecmp(s1.constData(), s2.constData());
//#endif
//}
//
//// 如果类型不匹配则抛异常
//GLDValueRelationship compareGVariant(const GVariant &v1, const GVariant &v2)
//{
//    // todo 空字符比较
//    if (!v1.isValid() || !v2.isValid())
//    {
//        return v1.isValid() == v2.isValid() ? gvrEqualsValue : (v1.isValid() ? gvrGreaterThanValue : gvrLessThanValue);
//    }
//    GVariant::Type t1 = v1.type();
//    GVariant::Type t2 = v2.type();
//    if (t1 == t2)
//    {
//        if (variantTypeIsUnsigned(t1))
//        {
//            unsigned long long n1 = v1.toULongLong();
//            unsigned long long n2 = v2.toULongLong();
//            return compareDigit(n1, n2);
//        }
//        else if (variantTypeIsDigit(t1))
//        {
//            long long ll1 = v1.toLongLong();
//            long long ll2 = v2.toLongLong();
//            return compareDigit(ll1, ll2);
//        }
//        else if (variantTypeIsDateTime(t1))
//        {
//            GDateTime n1 = v1.toDateTime();
//            GDateTime n2 = v2.toDateTime();
//            return compareDigit(n1, n2);
//        }
//        else if (variantTypeIsNumeric(t1) && variantTypeIsNumeric(t2))
//        {
//            double d1 = v1.toDouble();
//            double d2 = v2.toDouble();
//            return compareDigit(d1, d2);
//        }
//        else if (t1 == GVariant::ByteArray)
//        {
//            return GLDValueRelationship(compareStr(v1.toByteArray(), v2.toByteArray())); // todo 优化
//        }
//        else if (t1 == GVariant::String)
//        {
//            return GLDValueRelationship(compareStr(v1.toString(), v2.toString()));
//        }
//        else if (t1 == GVariant::Uuid)
//        {
//            return GLDValueRelationship(compareGUID(variantToGUID(v1.toUuid()), variantToGUID(v2.toUuid())));
//        }
//        else
//        {
//            gldError(getGLDi18nStr(g_InvalidTypeCompare));
//            return gvrEqualsValue;
//        }
//    }
//    else if (variantTypeIsDigit(t1) && variantTypeIsDigit(t2))
//    {
//        if (variantTypeIsUnsigned(t1) || variantTypeIsUnsigned(t2))
//        {
//            unsigned long long n1 = v1.toULongLong();
//            unsigned long long n2 = v2.toULongLong();
//            return compareDigit(n1, n2);
//        }
//        else
//        {
//            long long ll1 = v1.toLongLong();
//            long long ll2 = v2.toLongLong();
//            return compareDigit(ll1, ll2);
//        }
//    }
//    else if (variantTypeIsNumeric(t1) && variantTypeIsNumeric(t2))
//    {
//        double d1 = v1.toDouble();
//        double d2 = v2.toDouble();
//        return compareDigit(d1, d2);
//    }
//    else
//    {
//        gldError(getGLDi18nStr(g_InvalidTypeCompare));
//        return gvrEqualsValue;
//    }
//    gldError(getGLDi18nStr(g_InvalidTypeCompare));
//    return gvrEqualsValue;
//}
//
//bool sameGVariant(const GVariant &v1, const GVariant &v2)
//{
//    return v1 == v2;
//}
//
//// 区分大小写
//bool sameStr(const QString &s1, const QString &s2)
//{
//    return s1 == s2;
//}
//
//int length(const QString &s)
//{
//    return s.length();
//}
//
//int pos(const QString &subs, const QString &s)
//{
//    if ((subs.length() == 0) || (s.length() == 0))
//    {
//        return -1;
//    }
//    return s.indexOf(subs);
//}
//
//int pos(const GChar &subs, const QString &s)
//{
//    if (s.length() == 0)
//    {
//        return -1;
//    }
//    return s.indexOf(subs);
//}
//
//int pos(const QString &subs, const QString &s, int nFrom)
//{
//    if ((subs.length() == 0) || (s.length() == 0))
//    {
//        return -1;
//    }
//    return s.indexOf(subs, nFrom);
//}
//
//int rPos(const QString &subs, const QString &s, int times)
//{
//    if ((subs.length() == 0) || (s.length() == 0))
//    {
//        return -1;
//    }
//    assert(times >= 1);
//    int result = 0;
//    for (int i = 1; i <= times; ++i)
//    {
//        result = s.lastIndexOf(subs, result - 1);
//        if (-1 == result)
//            break;
//    }
//    return result;
//}
//
//int rPos(const GChar &subs, const QString &s, int times)
//{
//    if (s.length() == 0)
//    {
//        return -1;
//    }
//    assert(times >= 1);
//    int result = 0;
//    for (int i = 1; i <= times; ++i)
//    {
//        result = s.lastIndexOf(subs, result - 1);
//        if (-1 == result)
//            break;
//    }
//    return result;
//}
//
//int rPosEx(const GChar &subs, const QString &s, int offset)
//{
//    if (s.length() == 0)
//    {
//        return -1;
//    }
//    return s.lastIndexOf(subs, offset);
//}
//
//QString trim(const QString &s)
//{
//    return s.trimmed();
//}
//
//QString trimRight(const QString &s)
//{
//    int nIter = s.length() - 1;
//    while ((nIter >= 0) && (s[nIter] <= ' '))
//    {
//        nIter--;
//    }
//    return copy(s, 0, nIter + 1);
//}
//
//QString trimLeft(const QString &s)
//{
//    int nLen = s.length() - 1;
//    int nIter = 0;
//    while ((nIter <= nLen) && (s[nIter] <= ' '))
//    {
//        nIter++;
//    }
//    return copy(s, nIter, nLen + 1);
//}
//
//QString copy(const QString &s, int position, int n)
//{
//    if (position < 0)
//    {
//        position = 0;
//    }
//    if (n == MaxInt)
//    {
//        n = -1;
//    }
//    return s.mid(position, n);
//}
//
//QString stringReplace(const QString &s, const QString &before, const QString &after)
//{
//    QString result = s;
//    result.replace(before, after);
//    return result;
//}
//
//bool containsText(const QString &text, const QString subText)
//{
//    return pos(upperCase(subText), upperCase(text)) >= 0;
//}
//
//QString leftStr(const QString &text, int count)
//{
//    return text.left(count);
//}
//
//QString rightStr(const QString &text, int count)
//{
//    return text.right(count);
//}
//
//QStringList split(const QString &s, GChar sep)
//{
//    if (s == "")
//        return QStringList();
//    else
//        return s.split(sep);
//}
//
//QStringList split(const QString &s, const QString &sep)
//{
//    if (s == "")
//        return QStringList();
//    else
//        return s.split(sep);
//}
//
//bool isInt(const QString &s)
//{
//    bool result = false;
//    s.toInt(&result);
//    return result;
//}
//
//bool isInt64(const QString &s)
//{
//    bool result = false;
//    s.toLongLong(&result);
//    return result;
//}
//
//bool isNumeric(const QString &s)
//{
//    bool result = false;
//    s.toDouble(&result);
//    return result;
//}
//
//bool isDateTime(const QString &s)
//{
//    bool result = false;
//    s.toDouble(&result);
//    return result;
//}
//
//bool charIsDigit(const GChar &ch)
//{
//    return ch.isDigit();
//}
//
//bool variantTypeIsByte(const GVariant::Type type)
//{
//    return (type == GVariant::Char)
//            || (type == (GVariant::Type)QMetaType::UChar)
//            || (type == (GVariant::Type)QMetaType::SChar);
//}
//
//bool variantTypeIsShort(const GVariant::Type type)
//{
//    return (type == (GVariant::Type)QMetaType::Short)
//            || (type == (GVariant::Type)QMetaType::UShort);
//}
//
//bool variantTypeIsInt(const GVariant::Type type)
//{
//    return (type == GVariant::Int) || (type == GVariant::UInt)
//            || (type == (GVariant::Type)QMetaType::Long)
//            || (type == (GVariant::Type)QMetaType::ULong);
//}
//
//bool variantTypeIsDigit(const GVariant::Type type)
//{
//    return (type >= QVariant::Int && type <= QVariant::ULongLong)
//            || (type >= (GVariant::Type)QMetaType::Long && type <= (GVariant::Type)QMetaType::UChar)
//            || (type == (GVariant::Type)QMetaType::SChar);
//    //return variantTypeIsByte(type) || variantTypeIsShort(type) ||
//    //        variantTypeIsInt(type) || variantTypeIsInt64(type);
//}
//
//bool variantTypeIsFloat(const GVariant::Type type)
//{
//    return (type == GVariant::Double) || (type == (GVariant::Type)QMetaType::Float);
//}
//
//bool variantTypeIsNumeric(const GVariant::Type type)
//{
//    return (type >= QVariant::Int && type <= QVariant::Double)
//            || (type >= (GVariant::Type)QMetaType::Long && type <= (GVariant::Type)QMetaType::Float)
//            || (type == (GVariant::Type)QMetaType::SChar);
//    //return variantTypeIsDigit(type) || variantTypeIsFloat(type);
//}
//
//bool variantTypeIsDateTime(const GVariant::Type type)
//{
//    return (type == GVariant::Date) || (type == GVariant::Time) || (type == GVariant::DateTime);
//}
//
//bool variantTypeIsUnsigned(const QVariant::Type type)
//{
//    return (type == (GVariant::Type)QMetaType::UChar)
//            || (type == (GVariant::Type)QMetaType::UShort)
//            || (type == (GVariant::Type)QMetaType::ULong)
//            || (type == GVariant::UInt)
//            || (type == GVariant::ULongLong);
//}
//
//#ifdef WIN32
//size_t getBSTRLen(const BSTR & s)
//{
//    return ::SysStringLen(s);
//}
//
//QString BSTRToQString(const BSTR &s)
//{
//#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
//    return QString::fromUtf16((ushort *)s, (int)getBSTRLen(s));
//#else
//    return QString::fromUtf16(s, BSTRLen(s));
//#endif
//}
//
//BSTR QStringToBSTR(const QString & s)
//{
//    BSTR result = NULL;
//    if (length(s) != 0)
//    {
//        result = ::SysAllocString((wchar_t *)s.unicode());
//    }
//    return result;
//}
//
//void freeBSTR(BSTR & s)
//{
//    ::SysFreeString(s);
//}
//
//#endif
//
//void checkFormatDataType(QChar ch, GVariant &v)
//{
//    ch = ch.toLower();
//    GVariant::Type type = v.type();
//    if (ch == QLatin1Char('s'))
//    {
//        if (type != GVariant::String)
//        {
//            gldError("format '%s' invalid or incompatible with argument.");
//        }
//    }
//    else if (ch == QLatin1Char('d'))
//    {
//        if (!variantTypeIsDigit(type))
//        {
//            gldError("format '%d' invalid or incompatible with argument.");
//        }
//    }
//    else if (ch == QLatin1Char('u'))
//    {
//        if (!variantTypeIsDigit(type))
//            gldError("format '%u' invalid or incompatible with argument.");
//        v = GVariant((unsigned int)(v.toInt()));
//    }
//    else if (ch == QLatin1Char('f'))
//    {
//        if (!variantTypeIsDigit(type) && !variantTypeIsFloat(type))
//            gldError("format '%f' invalid or incompatible with argument.");
//        v = QString::number(v.toDouble(), 14, 2);
//    }
//    else if (ch == QLatin1Char('g'))
//    {
//        if (!variantTypeIsDigit(type) && !variantTypeIsFloat(type))
//            gldError("format '%g' invalid or incompatible with argument.");
//    }
//    else if (ch == QLatin1Char('n'))
//    {
//        if (!variantTypeIsDigit(type) && !variantTypeIsFloat(type))
//            gldError("format '%n' invalid or incompatible with argument.");
//    }
//    else if (ch == QLatin1Char('m'))
//    {
//        if (!variantTypeIsDigit(type) && !variantTypeIsFloat(type))
//            gldError("format '%m' invalid or incompatible with argument.");
//    }
//    else if (ch == QLatin1Char('x'))
//    {
//        if (!variantTypeIsDigit(type))
//            gldError("format '%x' invalid or incompatible with argument.");
//        v = QString::number(v.toLongLong(), 16).toUpper();
//    }
//    else if (!ch.isDigit())
//    {
//        gldError("format '%' invalid or incompatible with argument.");
//    }
//}
//
//QString format(const QString &s, const GVariantList &a)
//{
//    QString result;
//    int nLength = s.length();
//    const QChar *data = s.constData();
//    int nBefore = 0;
//    int nIndex = 0;
//    for (int i = 0; i < nLength; ++i)
//    {
//        if (data[i] == QLatin1Char('%'))
//        {
//            result.append(s.mid(nBefore, i - nBefore));
//            ++i;
//            if (i == nLength)
//            {
//                nBefore = nLength;
//                break;
//            }
//
//            nBefore = i + 1;
//            QChar ch = data[i];
//            if (ch == QLatin1Char('%'))
//            {
//                result.append(QLatin1Char('%'));
//            }
//            else
//            {
//                GVariant var = a.at(nIndex);
//                if (nIndex == a.count())
//                    gldError("format '%' invalid or incompatible with argument.");
//                checkFormatDataType(ch, var);
//                result.append(var.toString());
//                ++nIndex;
//            }
//        }
//    }
//    result.append(s.mid(nBefore, nLength - nBefore));
//    return result;
//}
//
//QString format(const QString &s, const QString &after, const QString &before)
//{
//    QString result = s;
//    int nIndex = result.indexOf(before);
//    if (nIndex >= 0)
//    {
//        //result.replace(i, before.length(), after);
//        result.remove(nIndex, length(before));
//        result.insert(nIndex, after);
//    }
//    else
//        result = result.arg(after);
//    return result;
//}
//
//QString format(const QString &s, const GChar &after, const QString &before)
//{
//    return format(s, QString(after), before);
//}
//
//QString format(const QString &s, int a)
//{
//    return format(s, intToStr(a), "%d");
//}
//
//QString format(const QString &s, long a)
//{
//    return format(s, intToStr(a), "%d");
//}
//
//QString format(const QString &s, ULONGLONG a)
//{
//    return format(s, uint64ToStr(a), "%ld");
//}
//
//QString format(const QString &s, long long a)
//{
//    return format(s, int64ToStr(a), "%ld");
//}
//
//QString format(const QString &s, double a)
//{
//    return format(s, floatToStr(a), "%lf");
//}
//
////QString format(const QString &s, const GVariant &a)
////{
////    switch (a.type())
////    {
////    case GVariant::UInt:
////    case GVariant::Int:
////        return format(s, a.toInt());
////    case GVariant::LongLong:
////        return format(s, a.toLongLong());
////    case GVariant::ULongLong:
////        return format(s, a.toULongLong());
////    case GVariant::Double:
////        return format(s, a.toDouble());
////    case GVariant::String:
////        return format(s, a.toString());
////    case GVariant::ByteArray:
////        return format(s, QString(a.toByteArray()));
////    case GVariant::Uuid:
////        return format(s, QString(a.toString()));
////    default:
////        break;
////    }
////    return QString();
////}
//
//QString boolToStr(bool a, bool useBoolStrs)
//{
//    if (useBoolStrs)
//    {
//        if (a)
//            return "True";
//        else
//            return "False";
//    }
//    else
//    {
//        if (a)
//            return "1";
//        else
//            return "0";
//    }
//}
//
//bool strToBool(const QString &s)
//{
//    if (s.isEmpty())
//        return false;
//    else if (s[0].toUpper() == 'T')
//        return true;
//    else
//        return false;
//}
//
//bool strToBoolDef(const QString &s, bool def)
//{
//    if (s.isEmpty())
//        return def;
//    else if (s[0].toUpper() == 'T')
//        return true;
//    else if (s[0].toUpper() == 'F')
//        return false;
//    else
//        return def;
//}
//
//QString intToStr(int a)
//{
//    return QString::number(a, 10);
//}
//
//QString int64ToStr(gint64 a)
//{
//    return QString::number(a, 10);
//}
//
//QString uint64ToStr(guint64 a)
//{
//    return QString::number(a, 10);
//}
//
//int strToInt(const QString &s)
//{
//    bool bOk;
//    int result = s.toInt(&bOk);
//    if (bOk)
//        return result;
//    else
//    {
//        gldError(format(getGLDi18nStr(g_InvalidInteger), s));
//        return 0;
//    }
//}
//
//int strToIntDef(const QString &s, int def)
//{
//    bool bOk;
//    int result = s.toInt(&bOk);
//    if (bOk)
//        return result;
//    else
//        return def;
//}
//
//gint64 strToInt64(const QString &s)
//{
//    bool bOk;
//    gint64 result = s.toLongLong(&bOk);
//    if (bOk)
//        return result;
//    else
//        throw GLDException(format(getGLDi18nStr(g_InvalidInteger), s));
//}
//
//gint64 strToInt64Def(const QString &s, gint64 def)
//{
//    bool bOk;
//    gint64 result = s.toLongLong(&bOk);
//    if (bOk)
//        return result;
//    else
//        return def;
//}
//
//QString floatToStr(double a)
//{
//    // 为了跟fRound匹配, 浮点数显示14位有效数字
//    // 为了跟delphi保持一致, 浮点数显示15位有效数字
//    return QString::number(a, 'g', 15);
//}
//
//double strToFloat(const QString &s)
//{
//    bool bOk;
//    double result = s.toDouble(&bOk);
//    if (bOk)
//        return result;
//    else
//        throw GLDException(format(getGLDi18nStr(g_InvalidFloat), s));
//}
//
//double strToFloatDef(const QString &s, double def)
//{
//    bool bOk;
//    double result = s.toDouble(&bOk);
//    if (bOk)
//        return result;
//    else
//        return def;
//}
//
//QString dateTimeToStr(const GDateTime &datetime, QString format)
//{
//    if (datetime.time() == GTime(0, 0, 0))
//        format = format.mid(0, format.indexOf(' '));//日期为整时 yyyy-MM-dd hh:mm:ss --->yyyy-MM-dd
//    return datetime.toString(format);
//}
//
//GDateTime strToDateTime(const QString &s, QString format)
//{
//    if (s.indexOf(' ') < 0)
//    {
//        int nIndex = format.indexOf(' ');
//        if (nIndex >= 0)
//            format = format.mid(0, nIndex);
//    }
//    GDateTime result = GDateTime::fromString(s, format);
//    if (!result.isValid())
//    {
//        result = GDateTime::fromString(s, Qt::ISODate);
//    }
//    return result;
//}
//
//QString byteArrayToStr(const QByteArray &a)
//{
//    return QString(a);
//}
//
//QByteArray strToByteArray(const QString &s)
//{
//    return s.toLocal8Bit();
//}
//
//QString intToHex(int value, int digits)
//{
//    QString val = QString::number(value, 16).toUpper();
//    if (val.length() < digits)
//        val.push_front('0');
//    return val;
//}
//
//QString stuffString(const QString &text, int nStart, int nLength, const QString &subText)
//{
//    return copy(text, 0, nStart - 1) + subText + copy(text, nStart + nLength - 1, MaxInt);
//}
//
//////////////////////////////////////////////////////////////////////////////////
////创建：Tu Jianhua 2004.1.18
////功能：计算分隔符在字符串中出现的次数
////参数：delimiter -- 分隔符
////     srcStr -- 字符串串
////返回：delimiter在字符串ASrcStr中出现的次数
//////////////////////////////////////////////////////////////////////////////////
//int occurs(const char delimiter, const QString &srcStr)
//{
//    int nCount = 0;
//    for (int i = 0; i < srcStr.length(); ++i)
//    {
//        if (delimiter == srcStr[i].unicode())
//        {
//            nCount++;
//        }
//    }
//    return nCount;
//}
//
//QByteArray base64Encode(const QByteArray &in)
//{
//    return in.toBase64();
//}
//
//GStream* base64Encode(GStream *in)
//{
//    if (!in)
//    {
//        return NULL;
//    }
//    return new GMemoryStream(new QByteArray(in->readAll().toBase64()), true);
//}
//
//QByteArray base64Decode(const QByteArray &in)
//{
//    return QByteArray::fromBase64(in);
//}
//
//GStream* base64Decode(GStream *in)
//{
//    if (!in)
//    {
//        return NULL;
//    }
//    return new GMemoryStream(new QByteArray(QByteArray::fromBase64(in->readAll())), true);
//}
//
//QString quotedStr(const QString &str, const char quote)
//{
//    QString result = str;
//    QString quotes(2, quote);
//    result.replace(quote, quotes);
//    result.push_front(quote);
//    result.push_back(quote);
//    return result;
//}
//
//QByteArray quotedStr(const QByteArray &str, const char quote)
//{
//    QByteArray result = str;
//    QByteArray quotes(2, quote);
//    result.replace(quote, quotes);
//    result.push_front(quote);
//    result.push_back(quote);
//    return result;
//}
//
//QString dequotedStr(const QString &str, const char quote)
//{
//    QString result = str;
//    if (result[0] == quote)
//        result.remove(0, 1);
//    if (result[result.size() - 1] == quote)
//        result.remove(result.size() - 1, 1);
//    QString quotes(2, quote);
//    result.replace(quotes, QString(1, quote));
//    return result;
//}
//
//QString extractQuotedStr(QString &str, const char quote)
//{
//    str = dequotedStr(str, quote);
//    return str;
//}
//
//QByteArray dequotedStr(const QByteArray &str, const char quote)
//{
//    QByteArray result = str;
//    if (result[0] == quote)
//        result.remove(0, 1);
//    if (result[result.size() - 1] == quote)
//        result.remove(result.size() - 1, 1);
//    QByteArray quotes(2, quote);
//    result.replace(quotes, QByteArray(1, quote));
//    return result;
//}
//
//GStream* stringToStream(const QString &in)
//{
//    return new GMemoryStream(new QByteArray(in.toLocal8Bit()), true);
//}
//
//QString streamToString(GStream* in)
//{
//    if (!in)
//    {
//        return QString();
//    }
//    return QString(in->readAll());
//}
//
//GStream* byteArrayToStream(const QByteArray &in)
//{
//    if (0 == in.size())
//    {
//        return NULL;
//    }
//    return new GMemoryStream(new QByteArray(in), true);
//}
//
//QByteArray streamToByteArray(GStream* in)
//{
//    if (!in)
//    {
//        return QByteArray();
//    }
//    return in->readAll();
//}
//
//GDate intToDate(int d)
//{
//    return GDate(1899, 12, 30).addDays(d);
//}
//
//int dateToInt(const GDate &date)
//{
//    return GDate(1899, 12, 30).daysTo(date);
//}
//
//GTime doubleToTime(double t)
//{
//    t = frac(t);
//    int nMsec = round(t * double(c_MSecsPerDay));
//    int nSecond = trunc(double(nMsec) / double(c_MSecsPerSec));
//    nMsec = nMsec % c_MSecsPerSec;
//    int nMinute = trunc(double(nSecond) / double(c_SecsPerMin));
//    nSecond = nSecond % c_SecsPerMin;
//    int nHour = trunc(double(nMinute) / double(c_MinsPerHour));
//    nMinute = nMinute % c_MinsPerHour;
//    return GTime(nHour, nMinute, nSecond, nMsec);
//}
//
//double timeToDouble(const GTime &time)
//{
//    return (0.0 + ((time.hour() * c_MinsPerHour + time.minute()) * c_SecsPerMin + time.second())
//            * c_MSecsPerSec + time.msec()) / c_MSecsPerDay;
//}
//
//GDateTime doubleToDateTime(double d)
//{
//    GDate date = intToDate(trunc(d));
//    GTime time = doubleToTime(d);
//    return GDateTime(date, time);
//}
//
//double dateTimeToDouble(const GDateTime &dateTime)
//{
//    return dateToInt(dateTime.date()) + timeToDouble(dateTime.time());
//}
//
//int yearOf(double d)
//{
//    GDateTime dataTime = doubleToDateTime(d);
//    return dataTime.date().year();
//}
//
//int monthOf(double d)
//{
//    GDateTime dataTime = doubleToDateTime(d);
//    return dataTime.date().month();
//}
//
//int weekOf(double d)
//{
//    GDateTime dataTime = doubleToDateTime(d);
//    return dataTime.date().weekNumber();
//}
//
//int dayOf(double d)
//{
//    GDateTime dataTime = doubleToDateTime(d);
//    return dataTime.date().day();
//}
//
//int hourOf(double d)
//{
//    GTime time = doubleToTime(d);
//    return time.hour();
//}
//
//int minuteOf(double d)
//{
//    GTime time = doubleToTime(d);
//    return time.minute();
//}
//
//int secondOf(double d)
//{
//    GTime time = doubleToTime(d);
//    return time.second();
//}
//
//int milliSecondOf(double d)
//{
//    GTime time = doubleToTime(d);
//    return time.msec();
//}
//
////// 返回中文平台下的时间日期格式
////// XP 的标准样式，和 VISTA 的不同
////GFormatSettings g_DefFormatSettings = {
////    0,
////    2,
////    ',',
////    '.',
////    2,
////    '-',
////    ':',
////    ',',
////    "￥", "yyyy-MM-dd",
////    "yyyy年M月d日",
////    "上午",
////    "下午",
////    "hh:mm",
////    "hh:mm:ss",
////    {
////        "一月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月", "十一月", "十二月"
////    },
////    {
////        "一月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月", "十一月", "十二月"
////    },
////    {
////        "日", "一", "二", "三", "四", "五", "六"
////    },
////    {
////        "星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"
////    },
////    14180
////};
//
////// 非标准样式，使用 / 分隔时间
////GFormatSettings g_DefFormatSettings1 = {
////    0,
////    2,
////    ',',
////    '.',
////    2,
////    '/',
////    ':',
////    ',',
////    "￥",
////    "yyyy/MM/dd",
////    "yyyy年M月d日",
////    "上午",
////    "下午",
////    "hh:mm",
////    "hh:mm:ss",
////    {
////        "一月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月", "十一月", "十二月"
////    },
////    {
////        "一月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月", "十一月", "十二月"
////    },
////    {
////        "日", "一", "二", "三", "四", "五", "六"
////    },
////    {
////        "星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"
////    },
////    14180,
////};
//
////// 非标准样式，使用 \ 分隔时间
////GFormatSettings g_DefFormatSettings2 = {
////    0,
////    2,
////    ',',
////    '.',
////    2,
////    '\\',
////    ':',
////    ',',
////    "￥",
////    "yyyy\\MM\\dd",
////    "yyyy年M月d日",
////    "上午",
////    "下午",
////    "hh:mm",
////    "hh:mm:ss",
////    {
////        "一月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月", "十一月", "十二月"
////    },
////    {
////        "一月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月", "十一月", "十二月"
////    },
////    {
////        "日", "一", "二", "三", "四", "五", "六"
////    },
////    {
////        "星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"
////    },
////    14180,
////};
//
///*///////////////////////////////////////////////////////////////////////////////
////设计: Liuxd 2007.02.02
////功能: 使用中文平台时间日期格式进行类型转换
////参数：
////注意：同时兼容 -、/、\、三种时间分隔符
/////////////////////////////////////////////////////////////////////////////////*/
//double chsStrToDateTime(const QString &s)
//{
////    GDateTime result;
//    // todo
////    if ((!tryStrToDateTime(s, result, g_DefFormatSettings)) &&
////            (!tryStrToDateTime(s, result, g_DefFormatSettings1)) &&
////            (!tryStrToDateTime(s, result, g_DefFormatSettings2)) &&
////            (!tryStrToDateTime(s, result)))
////    {
////        gldErrorFmt(rsInvalidTimeFormat, s);
////    }
//    GDateTime date = strToDateTime(s);
//    return dateTimeToDouble(date);
//}
//
//QString dateTimeToChsStr(double dateTime)
//{
//    GDateTime date = doubleToDateTime(dateTime);
//    return dateTimeToStr(date);
//}
//
///*//////////////////////////////////////////////////////////////////////////////
////设计: Linw 2008.03.19
////功能: 判断是否是使用中文平台时间日期格式
////注意：同时兼容 -、/、\、三种时间分隔符
/////////////////////////////////////////////////////////////////////////////////*/
//bool isChsDateTime(const QString &s)
//{
//    GDateTime dValue;
//    // todo duanb
//    return true;
//    G_UNUSED(s)
////    return tryStrToDateTime(s, dValue, g_DefFormatSettings) ||
////            tryStrToDateTime(s, dValue, g_DefFormatSettings1) ||
////            tryStrToDateTime(s, dValue, g_DefFormatSettings2);
//}
//
//bool tryStrToDateTime(const QString &s, GDateTime &value)
//{
//    Q_UNUSED(s);
//    Q_UNUSED(value);
//    // todo
////    int nPos = 1;
////    GDateTime date, time;
//    bool result = true;
////Time := 0;
////    if not ScanDate(S, Pos, Date) or
////            not ((Pos > Length(S)) or ScanTime(S, Pos, Time)) then
//
////            // Try time only
////            Result := TryStrToTime(S, Value)
////      else
////      if Date >= 0 then
////      Value := Date + Time
////      else
////      Value := Date - Time;
//    return result;
//}
//
//bool tryStrToDateTime(const QString &s, GDateTime &value,
//  const GFormatSettings &formatSettings)
//{
//    Q_UNUSED(s);
//    Q_UNUSED(value);
//    Q_UNUSED(formatSettings);
//    // todo
////    GDateTime date, time;
//    bool result = true;
////    int nPos = 1;
//    //  Time := 0;
//    //  if not ScanDate(S, Pos, Date, FormatSettings) or
//    //     not ((Pos > Length(S)) or ScanTime(S, Pos, Time, FormatSettings)) then
//
//    //    // Try time only
//    //    Result := TryStrToTime(S, Value, FormatSettings)
//    //  else
//    //    if Date >= 0 then
//    //      Value := Date + Time
//    //    else
//    //      Value := Date - Time;
//    return result;
//}
//
//bool sameDateTime(double dateTime1, double dateTime2)
//{
//    double dHoursPerDay   = 24;
//    double dMinsPerHour   = 60;
//    double dSecsPerMin    = 60;
//    double dMSecsPerSec   = 1000;
//    double dMinsPerDay    = dHoursPerDay * dMinsPerHour;
//    double dSecsPerDay    = dMinsPerDay * dSecsPerMin;
//    double dMSecsPerDay   = dSecsPerDay * dMSecsPerSec;
//    double dOneMillisecond = 1.0 / dMSecsPerDay;
//    return fabs(dateTime1 - dateTime2) < dOneMillisecond;
//}
//
//GLDValueRelationship compareDateTime(double dateTime1, double dateTime2)
//{
//    double dHoursPerDay   = 24;
//    double dMinsPerHour   = 60;
//    double dSecsPerMin    = 60;
//    double dMSecsPerSec   = 1000;
//    double dMinsPerDay    = dHoursPerDay * dMinsPerHour;
//    double dSecsPerDay    = dMinsPerDay * dSecsPerMin;
//    double dMSecsPerDay   = dSecsPerDay * dMSecsPerSec;
//    double dOneMillisecond = 1.0 / dMSecsPerDay;
//    if (fabs(dateTime1 - dateTime2) < dOneMillisecond)
//        return gvrEqualsValue;
//    else if (dateTime1 < dateTime2)
//        return gvrLessThanValue;
//    else
//        return gvrGreaterThanValue;
//}
//
//QString getHZPY(const QString &src)
//{
//    unsigned char ucHigh;
//    unsigned char ucLow;
//    int nCode;
//    QString result;
//    QByteArray byteArray = unicodeToGBK(src);
//    const char *str = byteArray.constData();
//    for (int i = 0; i < byteArray.length(); ++i)
//    {
//        ucHigh = str[i];
//        if (ucHigh < 0x80)//英文字母
//        {
//            result.append(str[i]);
//            continue;
//        }
//        ucLow = str[i + 1];
//        if ((ucHigh < 0xa1) || (ucLow < 0xa1))
//        {
//            continue;
//        }
//        else
//        {
//            nCode = (ucHigh - 0xa0) * 100 + ucLow - 0xa0;
//        }
//        result.append(firstLetter(nCode));
//        ++i;
//    }
//    return result;
//}
//
//QString firstLetter(int nCode)
//{
//    if (nCode >= 1601 && nCode < 1637)
//        return "A";
//    if (nCode >= 1637 && nCode < 1833)
//        return "B";
//    if (nCode >= 1833 && nCode < 2078)
//        return "C";
//    if (nCode >= 2078 && nCode < 2274)
//        return "D";
//    if (nCode >= 2274 && nCode < 2302)
//        return "E";
//    if (nCode >= 2302 && nCode < 2433)
//        return "F";
//    if (nCode >= 2433 && nCode < 2594)
//        return "G";
//    if (nCode >= 2594 && nCode < 2787)
//        return "H";
//    if (nCode >= 2787 && nCode < 3106)
//        return "J";
//    if (nCode >= 3106 && nCode < 3212)
//        return "K";
//    if (nCode >= 3212 && nCode < 3472)
//        return "L";
//    if (nCode >= 3472 && nCode < 3635)
//        return "M";
//    if (nCode >= 3635 && nCode < 3722)
//        return "N";
//    if (nCode >= 3722 && nCode < 3730)
//        return "O";
//    if (nCode >= 3730 && nCode < 3858)
//        return "P";
//    if (nCode >= 3858 && nCode < 4027)
//        return "Q";
//    if (nCode >= 4027 && nCode < 4086)
//        return "R";
//    if (nCode >= 4086 && nCode < 4390)
//        return "S";
//    if (nCode >= 4390 && nCode < 4558)
//        return "T";
//    if (nCode >= 4558 && nCode < 4684)
//        return "W";
//    if (nCode >= 4684 && nCode < 4925)
//        return "X";
//    if (nCode >= 4925 && nCode < 5249)
//        return "Y";
//    if (nCode >= 5249 && nCode < 5590)
//        return "Z";
//    return "";
//}
//
//QString reverseString(const QString &s)
//{
//    QString result;
//    for (int i = s.length() - 1; i >= 0; i--)
//    {
//        result.append(s[i]);
//    }
//    return result;
//}
//
///*-------------------------------------------------------------------------------
//设计：zhangjq 2012.10.31
//参数：const AText,   源字符串
//            ARegEx,  正则表达式
//            AReplacement: 用来替换的字符串，支持反向引用 \1,\2??
//功能：正则表达式替换函数
//-------------------------------------------------------------------------------*/
//QString regExprReplace(const QString &text, const QString &regEx, const QString &replacement)
//{
//    QRegExp regExp;
//    regExp.setPattern(regEx);
//    //将匹配设置为最小（短）匹配
//    regExp.setMinimal(true);
//    QString result = text;
//    result.replace(regExp,replacement);
//    return result;
//}
//
//int posN(const QString subStr, const QString srcStr, int times)
//{
//    int result = -1;
//    int nOffSet = 0;
//    int nSubLen = length(subStr);
//    if (nSubLen == 1)
//        result = posN(subStr[1], srcStr, times);
//    else
//    {
//        for (int i = 1; i <= times; i++)
//        {
//            nOffSet = srcStr.indexOf(subStr, nOffSet);
//            if (nOffSet == -1)
//                break;
//            if (i == times)
//                result = nOffSet;
//            else
//                nOffSet += nSubLen;
//        }
//    }
//    return result;
//}
//
//int posN(const char delimiter, const QString srcStr, int times)
//{
//    int result = -1;
//    int nCounter = 0;
//    for (int i = 0; i < length(srcStr); i++)
//    {
//        if (srcStr[i] == delimiter)
//        {
//            nCounter++;
//            if (nCounter == times)
//            {
//                result = i;
//                break;
//            }
//        }
//    }
//    return result;
//}
//
///*-------------------------------------------------------------------------------
//创建：Tu Jianhua 2004.1.18
//功能：根据指定的分隔符取指定项
//参数：ASrcStr -- 字符串
//      ADelimiter -- 分隔符字符
//      AIndex -- 项号
//返回：字符串ASrcStr以Delimiter为分隔符第AIndex项的子串
//-------------------------------------------------------------------------------*/
//QString getSubString(const QString &srcStr, char delimiter, int index)
//{
//    int nP1;
//    int nP2;
//    QString result = "";
//    QString strText = srcStr;
//    if ((index < 1) || (strText.length() == 0))
//        return result;
//    if (index == 1)
//    {
//        nP1 = posN(delimiter, strText, index);
//        if (nP1 == -1)
//            result = strText;
//        else
//            result = copy(strText, 0, nP1);
//    }
//    else
//    {
//        nP1 = posN(delimiter, strText, index - 1);
//        if (nP1 == -1)
//            return result;
//        nP2 = strText.indexOf(delimiter, nP1 + 1);
//        if (nP2 == -1)
//            result = copy(strText, nP1 + 1);
//        else
//            result = copy(strText, nP1 + 1, nP2 - nP1 - 1);
//    }
//    return trim(result);
//}
//
///*-------------------------------------------------------------------------------
//创建：Tu Jianhua 2004.1.18
//功能：根据指定的分隔符取指定项
//参数：ASrcStr -- 字符串
//      ADelimiter -- 分隔符串
//      AIndex -- 项号
//返回：字符串ASrcStr以Delimiter为分隔符第AIndex项的子串
//-------------------------------------------------------------------------------*/
//QString getSubString(const QString &srcStr, const QString &delimiter, int index)
//{
//    int nP1;
//    int nP2;
//    QString result = "";
//    QString strText = srcStr;
//    if ((index < 1) || (strText.length() == 0))
//        return result;
//    int nSize = length(delimiter);
//    if (nSize == 0)
//        return result;
//    else if (nSize == 1)
//        result = getSubString(strText, delimiter.at(0).toLatin1(), index);
//    else
//    {
//        if (index == 1)
//        {
//            nP1 = posN(delimiter, strText, index);
//            if (nP1 == -1)
//                result = strText;
//            else
//                result = copy(strText, 0, nP1);
//        }
//        else
//        {
//            nP1 = posN(delimiter, strText, index - 1);
//            if (nP1 == -1)
//                return result;
//            nP2 = strText.indexOf(delimiter, nP1 + nSize);
//            if (nP2 == -1)
//                result = copy(strText, nP1 + nSize);
//            else
//                result = copy(strText, nP1 + nSize, nP2 - nP1 - nSize);
//        }
//    }
//    return trim(result);
//}
//
//QString stringOfChar(char Char, int count)
//{
//    QString result("");
//
//    for (int i = 0; i < count; ++i)
//    {
//        result = result + Char;
//    }
//    return result;
//}
//
//QString createGuidString()
//{
//    GUuid oUuid;
//    return oUuid.createUuid().toString();
//}
//
//QString boolToXMLString(bool v)
//{
//    if (v)
//        return "True";
//    else
//        return "False";
//}
//
//bool xmlStringToBool(const QString &s)
//{
//    return sameText(s, "True");
//}
//
//QString floatToXMLString(double v)
//{
//    QString result = floatToStr(double(v));
//    //todo Lipl
////    if (CurrentDecimalSeparator != '.')
////    {
////        int nPos = PosXMLString(CurrentDecimalSeparator, result);
////        if (nPos != 0)
////            result[nPos] = '.';
////    }
//    return result;
//}
//
//QString encodeXMLString(const QString &value, bool encodeCrLf)
//{
//    QString result = "";
//    int nLength = length(value);
//    if (nLength == 0)
//        return result;
//    int nJ = 0;
//    int nI = 0;
//    while (nI < nLength)
//    {
//        switch (value[nI].toLatin1())
//        {
//        case '<':
//        case '>':
//            nJ += 4;
//            break;
//        case '&':
//            nJ += 5;
//            break;
//        case '\'':
//        case '"':
//            nJ += 6;
//            break;
//        case '\n':
//        case '\r':
//        {
//            if (encodeCrLf)
//                nJ += 5;
//            else
//                nJ++;
//        }
//            break;
//        default:
//            nJ++;
//            break;
//        }
//        nI++;
//    }
//    if (nJ == nLength)
//        result = value;
//    else
//    {
//        nJ = 0;
//        nI = 0;
//        while (nI < nLength)
//        {
//            switch (value[nI].toLatin1())
//            {
//            case '<':
//            {
//                result += "&lt;";
//                nJ += 4;
//            }
//                break;
//            case '>':
//            {
//                result += "&gt;";
//                nJ += 4;
//            }
//                break;
//            case '&':
//            {
//                result += "&amp;";
//                nJ += 5;
//            }
//                break;
//            case '\'':
//            {
//                result += "&apos;";
//                nJ += 6;
//            }
//                break;
//            case '"':
//            {
//                result += "&quot;";
//                nJ += 6;
//            }
//                break;
//            case '\n':
//            {
//                if (encodeCrLf)
//                {
//                    result += "&#xA;";
//                    nJ += 5;
//                }
//                else
//                {
//                    result[nJ] = value[nI];
//                    nJ++;
//                }
//            }
//                break;
//            case '\r':
//            {
//                if (encodeCrLf)
//                {
//                    result += "&#xD;";
//                    nJ += 5;
//                }
//                else
//                {
//                    result[nJ] = value[nI];
//                    nJ++;
//                }
//            }
//                break;
//            default:
//            {
//                result[nJ] = value[nI];
//                nJ++;
//            }
//                break;
//            }
//            nI++;
//        }
//    }
//    return result;
//}
//
///**
// * GUID 操作函数
// */
//
//GUID strToGUID(const QString &text)
//{
//#if defined(Q_OS_WIN)
//    return GUID(GUuid(text));
//#else
//    return GUuidToGUID(GUuid(text));
//#endif
//}
//
//QString GUIDToStr(const GUID &a)
//{
//#if defined(Q_OS_WIN)
//    return GUuid(a).toString();
//#else
//    return GUIDToGUuid(a).toString();
//#endif
//}
//
//bool isGUID(const QString &s)
//{
//    return !GUuid(s).isNull();
//}
//
//bool variantTypeIsGUID(const QVariant::Type type)
//{
//    return type == GVariant::Uuid;
//}
//
//GUuid GUIDToGUuid(const GUID &value)
//{
//#if defined(Q_OS_WIN)
//    return GUuid(value).toString();
//#else
//    return QUuid(value.Data1, value.Data2, value.Data3, value.Data4[0], value.Data4[1], value.Data4[2], value.Data4[3],
//            value.Data4[4], value.Data4[5], value.Data4[6], value.Data4[7]);
//#endif
//}
//
//GUID GUuidToGUID(const GUuid &value)
//{
//#if defined(Q_OS_WIN)
//    return GUID(value);
//#else
//    GUID result;
//    result.Data1 = value.data1;
//    result.Data2 = value.data2;
//    result.Data3 = value.data3;
//    result.Data4[0] = value.data4[0];
//    result.Data4[1] = value.data4[1];
//    result.Data4[2] = value.data4[2];
//    result.Data4[3] = value.data4[3];
//    result.Data4[4] = value.data4[4];
//    result.Data4[5] = value.data4[5];
//    result.Data4[6] = value.data4[6];
//    result.Data4[7] = value.data4[7];
//    return result;
//#endif
//}
//
//int compareGUID(const GUID &g1, const GUID &g2)
//{
//    GUuid uuid1 = GUIDToGUuid(g1);
//    GUuid uuid2 = GUIDToGUuid(g2);
//    if (uuid1 > uuid2)
//    {
//        return 1;
//    }
//    else if (uuid1 < uuid2)
//    {
//        return -1;
//    }
//    else
//    {
//        return 0;
//    }
//}
//
//GVariant GUIDToVariant(const GUID &a)
//{
//    return GVariant(GUIDToGUuid(a));
//}
//
//GUID variantToGUID(const GVariant &text)
//{
//    return GUuidToGUID(text.toUuid());
//}
//
//GUID byteArrayToGUID(const QByteArray &ba)
//{
//    return GUuidToGUID(GUuid(ba));
//}
//
//QByteArray GUIDToByteArray(const GUID value)
//{
//    return GUIDToGUuid(value).toByteArray();
//}
//
//GUID createGUID()
//{
//    GUuid uuid;
//    return GUuidToGUID(uuid.createUuid());
//}
//
//bool isUInt64(const QString &s)
//{
//    bool result = false;
//    s.toULongLong(&result);
//    return result;
//}
//
//guint64 strToUInt64(const QString &s)
//{
//    bool bOK;
//    guint64 result;
//    result = s.toULongLong(&bOK);
//    if (bOK)
//    {
//        return result;
//    }
//    else
//    {
//        throw GLDException(format(getGLDi18nStr(g_InvalidInteger), s));
//    }
//}
//
//guint64 strToUInt64Def(const QString &s, guint64 def)
//{
//    bool bOk = false;
//    guint64 result;
//    result = s.toULongLong(&bOk);
//    if (bOk)
//    {
//        return result;
//    }
//    else
//    {
//        return def;
//    }
//}
//



inline QString wToQString(const char *str)
{
	return QString::fromLocal8Bit(str);
}

WXJCOMMON_EXPORT void wSetLocaleGB18030_0()
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030-0"));
}

WXJCOMMON_EXPORT void wSetLocaleUTF_8()
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
}

// WXJCOMMON_EXPORT std::string wToStdString(const char *pszStr)
// {
// 	return wToQString(pszStr).toStdString();
// }

WXJCOMMON_EXPORT std::wstring wToStdWString(const char *pszStr)
{
	return wToQString(pszStr).toStdWString();
}

WXJCOMMON_EXPORT bool wSameText(const std::string &s1, const std::string &s2)
{
	return (0 == QString(s1.c_str()).compare(QString(s2.c_str()), Qt::CaseInsensitive));
}

WXJCOMMON_EXPORT bool wSameText(const std::wstring &s1, const std::wstring &s2)
{
	return (0 == QString::fromStdWString(s1).compare(QString::fromStdWString(s2), Qt::CaseInsensitive));
}

WXJCOMMON_EXPORT bool wSameStr(const char *s1, const char *s2)
{
	return (0 == strcmp(s1, s2));
}

WXJCOMMON_EXPORT bool wSameStr(const wchar_t *s1, const wchar_t *s2)
{
	return (0 == wcscmp(s1, s2));
}

WXJCOMMON_EXPORT size_t wCStrLength(const char *sz)
{
	int i = 0;
	while (sz[i])
	{
		++i;
	}
	return i;
}

WXJCOMMON_EXPORT size_t wCStrLength(const wchar_t *wsz)
{
	int i = 0;
	while (wsz[i])
	{
		++i;
	}
	return i;
}

WXJCOMMON_EXPORT std::string wToLower(const std::string &s1)
{
	return QString(s1.c_str()).toLower().toStdString();
}

WXJCOMMON_EXPORT std::wstring wToLower(const std::wstring &s1)
{
	return QString((const QChar *)s1.c_str()).toLower().toStdWString();
}

WXJCOMMON_EXPORT char wToLower(const char ch)
{
	QChar chStr(ch);
	return chStr.toLower().toLatin1();
}

WXJCOMMON_EXPORT wchar_t wToLower(const wchar_t ch)
{
	if ((int)ch > 256)
	{
		return ch;
	}
	QChar chStr(ch);
	return (wchar_t)(chStr.toLower().toLatin1());
}

WXJCOMMON_EXPORT std::string wToUpper(const std::string &s1)
{
	return QString(s1.c_str()).toUpper().toStdString();
}

WXJCOMMON_EXPORT std::wstring wToUpper(const std::wstring &s1)
{
	return QString::fromStdWString(s1).toUpper().toStdWString();
}

WXJCOMMON_EXPORT char wToUpper(const char ch)
{
	QChar chStr(ch);
	return chStr.toUpper().toLatin1();
}

WXJCOMMON_EXPORT wchar_t wToUpper(const wchar_t ch)
{
	if ((int)ch > 256)
	{
		return ch;
	}
	QChar chStr(ch);
	return (wchar_t)(chStr.toUpper().toLatin1());
}

WXJCOMMON_EXPORT std::string wTrim(const std::string &s)
{
	return QString::fromStdString(s).trimmed().toStdString();
}

WXJCOMMON_EXPORT std::wstring wTrim(const std::wstring &s)
{
	return QString((const QChar *)s.c_str()).trimmed().toStdWString();
}

WXJCOMMON_EXPORT std::string wTrimLeft(const std::string &s)
{
	int nLen = s.size() - 1;
	int nIter = 0;
	while ((nIter <= nLen) && (s[nIter] <= ' '))
	{
		nIter++;
	}
	return std::string(s.begin() + nIter,s.end());
}

WXJCOMMON_EXPORT std::wstring wTrimLeft(const std::wstring &s)
{
	int nLen = s.size() - 1;
	int nIter = 0;
	while ((nIter <= nLen) && (s[nIter] <= L' '))
	{
		nIter++;
	}
	return std::wstring(s.begin() + nIter,s.end());
}

WXJCOMMON_EXPORT std::string wTrimRight(const std::string &s)
{
	int nIter = s.size() - 1;
	while ((nIter >= 0) && (s[nIter] <= ' '))
	{
		nIter--;
	}
	return std::string(s.begin(), s.begin() + nIter + 1);
}

WXJCOMMON_EXPORT std::wstring wTrimRight(const std::wstring &s)
{
	int nIter = s.size() - 1;
	while ((nIter >= 0) && (s[nIter] <= ' '))
	{
		nIter--;
	}
	return std::wstring(s.begin(), s.begin() + nIter + 1);
}

WXJCOMMON_EXPORT int wToInt(const char *s, bool *ok /*= NULL*/)
{
	return QString(s).toInt(ok);
}

WXJCOMMON_EXPORT int wToInt(const wchar_t *s, bool *ok /*= NULL*/)
{
	return QString::fromWCharArray(s).toInt(ok);
}

WXJCOMMON_EXPORT int wToInt(const std::string &s, bool *ok /*= NULL*/)
{
	return QString::fromStdString(s).toInt(ok);
}

WXJCOMMON_EXPORT int wToInt(const std::wstring &s, bool *ok /*= NULL*/)
{
	return QString::fromStdWString(s).toInt(ok);
}

WXJCOMMON_EXPORT uint wToUInt(const char *s, bool *ok /*= NULL*/)
{
	return QString(s).toUInt(ok);
}

WXJCOMMON_EXPORT uint wToUInt(const wchar_t *s, bool *ok /*= NULL*/)
{
	return QString::fromWCharArray(s).toUInt(ok);
}

WXJCOMMON_EXPORT uint wToUInt(const std::string &s, bool *ok /*= NULL*/)
{
	return QString::fromStdString(s).toUInt(ok);
}

WXJCOMMON_EXPORT uint wToUInt(const std::wstring &s, bool *ok /*= NULL*/)
{
	return QString::fromStdWString(s).toUInt(ok);
}

WXJCOMMON_EXPORT short wToShort(const char *s, bool *ok /*= NULL*/)
{
	return QString(s).toShort(ok);
}

WXJCOMMON_EXPORT short wToShort(const wchar_t *s, bool *ok /*= NULL*/)
{
	return QString::fromWCharArray(s).toShort(ok);
}

WXJCOMMON_EXPORT short wToShort(const std::string &s, bool *ok /*= NULL*/)
{
	return QString::fromStdString(s).toShort(ok);
}

WXJCOMMON_EXPORT short wToShort(const std::wstring &s, bool *ok /*= NULL*/)
{
	return QString::fromStdWString(s).toShort(ok);
}

WXJCOMMON_EXPORT ushort wToUShort(const char *s, bool *ok /*= NULL*/)
{
	return QString(s).toUShort(ok);
}

WXJCOMMON_EXPORT ushort wToUShort(const wchar_t *s, bool *ok /*= NULL*/)
{
	return QString::fromWCharArray(s).toUShort(ok);
}

WXJCOMMON_EXPORT ushort wToUShort(const std::string &s, bool *ok /*= NULL*/)
{
	return QString::fromStdString(s).toUShort(ok);
}

WXJCOMMON_EXPORT ushort wToUShort(const std::wstring &s, bool *ok /*= NULL*/)
{
	return QString::fromStdWString(s).toUShort(ok);
}

WXJCOMMON_EXPORT long wToLong(const char *s, bool *ok /*= NULL*/)
{
	return QString(s).toLong(ok);
}

WXJCOMMON_EXPORT long wToLong(const wchar_t *s, bool *ok /*= NULL*/)
{
	return QString::fromWCharArray(s).toLong(ok);
}

WXJCOMMON_EXPORT long wToLong(const std::string &s, bool *ok /*= NULL*/)
{
	return QString::fromStdString(s).toLong(ok);
}

WXJCOMMON_EXPORT long wToLong(const std::wstring &s, bool *ok /*= NULL*/)
{
	return QString::fromStdWString(s).toLong(ok);
}

WXJCOMMON_EXPORT ulong wToULong(const char *s, bool *ok /*= NULL*/)
{
	return QString(s).toULong(ok);
}

WXJCOMMON_EXPORT ulong wToULong(const wchar_t *s, bool *ok /*= NULL*/)
{
	return QString::fromWCharArray(s).toULong(ok);
}

WXJCOMMON_EXPORT ulong wToULong(const std::string &s, bool *ok /*= NULL*/)
{
	return QString::fromStdString(s).toULong(ok);
}

WXJCOMMON_EXPORT ulong wToULong(const std::wstring &s, bool *ok /*= NULL*/)
{
	return QString::fromStdWString(s).toULong(ok);
}

WXJCOMMON_EXPORT wlonglong wToLongLong(const char *s, bool *ok /*= NULL*/)
{
	return QString(s).toLongLong(ok);
}

WXJCOMMON_EXPORT wlonglong wToLongLong(const wchar_t *s, bool *ok /*= NULL*/)
{
	return QString::fromWCharArray(s).toLongLong(ok);
}

WXJCOMMON_EXPORT wlonglong wToLongLong(const std::string &s, bool *ok /*= NULL*/)
{
	return QString::fromStdString(s).toLongLong(ok);
}

WXJCOMMON_EXPORT wlonglong wToLongLong(const std::wstring &s, bool *ok /*= NULL*/)
{
	return QString::fromStdWString(s).toLongLong(ok);
}

WXJCOMMON_EXPORT wulonglong wToULongLong(const char *s, bool *ok /*= NULL*/)
{
	return QString(s).toULongLong(ok);
}

WXJCOMMON_EXPORT wulonglong wToULongLong(const wchar_t *s, bool *ok /*= NULL*/)
{
	return QString::fromWCharArray(s).toULongLong(ok);
}

WXJCOMMON_EXPORT wulonglong wToULongLong(const std::string &s, bool *ok /*= NULL*/)
{
	return QString::fromStdString(s).toULongLong(ok);
}

WXJCOMMON_EXPORT wulonglong wToULongLong(const std::wstring &s, bool *ok /*= NULL*/)
{
	return QString::fromStdWString(s).toULongLong(ok);
}

WXJCOMMON_EXPORT double wToDouble(const char *s, bool *ok /*= NULL*/)
{
	return QString(s).toDouble(ok);
}

WXJCOMMON_EXPORT double wToDouble(const wchar_t *s, bool *ok /*= NULL*/)
{
	return QString::fromWCharArray(s).toDouble(ok);
}

WXJCOMMON_EXPORT double wToDouble(const std::string &s, bool *ok /*= NULL*/)
{
	return QString::fromStdString(s).toDouble(ok);
}

WXJCOMMON_EXPORT double wToDouble(const std::wstring &s, bool *ok /*= NULL*/)
{
	return QString::fromStdWString(s).toDouble(ok);
}

WXJCOMMON_EXPORT float wToFloat(const char *s, bool *ok /*= NULL*/)
{
	return QString(s).toFloat(ok);
}

WXJCOMMON_EXPORT float wToFloat(const wchar_t *s, bool *ok /*= NULL*/)
{
	return QString::fromWCharArray(s).toFloat(ok);
}

WXJCOMMON_EXPORT float wToFloat(const std::string &s, bool *ok /*= NULL*/)
{
	return QString::fromStdString(s).toFloat(ok);
}

WXJCOMMON_EXPORT float wToFloat(const std::wstring &s, bool *ok /*= NULL*/)
{
	return QString::fromStdWString(s).toFloat(ok);
}

WXJCOMMON_EXPORT std::string wNumberToStringA(short val,int base/*=10*/)
{
	return QString::number(val,base).toStdString();
}

WXJCOMMON_EXPORT std::string wNumberToStringA(ushort val,int base/*=10*/)
{
	return QString::number(val,base).toStdString();
}

WXJCOMMON_EXPORT std::string wNumberToStringA(int val,int base/*=10*/)
{
	return QString::number(val,base).toStdString();
}

WXJCOMMON_EXPORT std::string wNumberToStringA(uint val,int base/*=10*/)
{
	return QString::number(val,base).toStdString();
}

WXJCOMMON_EXPORT std::string wNumberToStringA(long val,int base/*=10*/)
{
	return QString::number(val,base).toStdString();
}

WXJCOMMON_EXPORT std::string wNumberToStringA(ulong val,int base/*=10*/)
{
	return QString::number(val,base).toStdString();
}

WXJCOMMON_EXPORT std::string wNumberToStringA(wlonglong val,int base/*=10*/)
{
	return QString::number(val,base).toStdString();
}

WXJCOMMON_EXPORT std::string wNumberToStringA(wulonglong val,int base/*=10*/)
{
	return QString::number(val,base).toStdString();
}

WXJCOMMON_EXPORT std::string wNumberToStringA(float val, char f/*='g'*/, int prec/*=6*/)
{
	return QString::number(val,f,prec).toStdString();
}

WXJCOMMON_EXPORT std::string wNumberToStringA(double val, char f/*='g'*/, int prec/*=6*/)
{
	return QString::number(val,f,prec).toStdString();
}

WXJCOMMON_EXPORT bool wContains(const std::string &s, char c,Wxj::CaseSensitivity cs /*= CS_Sensitive*/)
{
	//return (std::count(s.begin(), s.end(), c) != 0);
	return QString::fromStdString(s).contains(c,(Qt::CaseSensitivity)cs);
}

WXJCOMMON_EXPORT bool wContains(const std::wstring &s, wchar_t c, Wxj::CaseSensitivity cs /*= CS_Sensitive*/)
{
	//return (std::count(s.begin(), s.end(), c) != 0);
	return QString::fromStdWString(s).contains(c,(Qt::CaseSensitivity)cs);
}

WXJCOMMON_EXPORT bool wContains(const std::string &s, const std::string &subStr, 
	Wxj::CaseSensitivity cs /*= CS_Sensitive*/)
{
	return QString::fromStdString(s).contains(QString::fromStdString(subStr),(Qt::CaseSensitivity)cs);
}

WXJCOMMON_EXPORT bool wContains(const std::wstring &s, const std::wstring &subStr, Wxj::CaseSensitivity cs /*= Wxj::CS_Sensitive*/)
{
	return QString::fromStdWString(s).contains(QString::fromStdWString(subStr),(Qt::CaseSensitivity)cs);
}

WXJCOMMON_EXPORT std::string wLeftString(const std::string &s, int nCount)
{
	return std::string(s.begin(), s.begin() + nCount);
}

WXJCOMMON_EXPORT std::wstring wLeftString(const std::wstring &s, int nCount)
{
	return std::wstring(s.begin(), s.begin() + nCount);
}

WXJCOMMON_EXPORT std::string wRightString(const std::string &s, int nCount)
{
	return std::string(s.end() - nCount, s.end());
}

WXJCOMMON_EXPORT std::wstring wRightString(const std::wstring &s, int nCount)
{
	return std::wstring(s.end() - nCount, s.end());
}

WXJCOMMON_EXPORT std::wstring wStringToWString(const std::string &s)
{
	return QString::fromStdString(s).toStdWString();
}

WXJCOMMON_EXPORT std::string wWStringToString(const std::wstring &s)
{
	return QString::fromStdWString(s).toStdString();
}

WXJCOMMON_EXPORT std::string wValueFromRegistry(const char *regPath, const char *regKey)
{
	QSettings setting(regPath, QSettings::NativeFormat);
	QString result = setting.value(regKey, "").toString();
	if ((result.length() > 0) && !((result.right(1) == "\\") || result.right(1) == "/"))
	{
		result += "/";
	}
	return QDir::fromNativeSeparators(result).toStdString();
}

WXJCOMMON_EXPORT std::wstring wValueFromRegistry(const wchar_t *regPath, const wchar_t *regKey)
{
	QSettings setting(QString::fromWCharArray(regPath), QSettings::NativeFormat);
	QString result = setting.value(QString::fromWCharArray(regKey), "").toString();
	if ((result.length() > 0) && !((result.right(1) == "\\") || result.right(1) == "/"))
	{
		result += "/";
	}
	return QDir::fromNativeSeparators(result).toStdWString();
}

WXJCOMMON_EXPORT std::wstring wNumberToStringW( short val,int base/*=10*/ )
{
	return QString::number(val, base).toStdWString();
}

WXJCOMMON_EXPORT std::wstring wNumberToStringW( ushort val,int base/*=10*/ )
{
	return QString::number(val, base).toStdWString();
}

WXJCOMMON_EXPORT std::wstring wNumberToStringW( int val,int base/*=10*/ )
{
	return QString::number(val, base).toStdWString();
}

WXJCOMMON_EXPORT std::wstring wNumberToStringW( uint val,int base/*=10*/ )
{
	return QString::number(val, base).toStdWString();
}

WXJCOMMON_EXPORT std::wstring wNumberToStringW( long val,int base/*=10*/ )
{
	return QString::number(val, base).toStdWString();
}

WXJCOMMON_EXPORT std::wstring wNumberToStringW( ulong val,int base/*=10*/ )
{
	return QString::number(val, base).toStdWString();
}

WXJCOMMON_EXPORT std::wstring wNumberToStringW( wlonglong val,int base/*=10*/ )
{
	return QString::number(val, base).toStdWString();
}

WXJCOMMON_EXPORT std::wstring wNumberToStringW( wulonglong val,int base/*=10*/ )
{
	return QString::number(val, base).toStdWString();
}

WXJCOMMON_EXPORT std::wstring wNumberToStringW( float val, char f/*='g'*/, int prec/*=6*/ )
{
	return QString::number(val, f, prec).toStdWString();
}

WXJCOMMON_EXPORT std::wstring wNumberToStringW( double val, char f/*='g'*/, int prec/*=6*/ )
{
	return QString::number(val, f, prec).toStdWString();
}



