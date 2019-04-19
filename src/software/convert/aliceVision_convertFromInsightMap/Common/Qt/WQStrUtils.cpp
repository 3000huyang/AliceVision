#include <QSettings>
#include <QDir>
#include "Qt/WQStrUtils.h"
#include "WXJStrUtils.h"

WXJCOMMON_EXPORT std::string wToStdString(const QString &str)
{
	return std::string(str.toLocal8Bit().constData());
}

WXJCOMMON_EXPORT bool wSameText( const QString &s1, const QString &s2 )
{
	return (0 == s1.compare(s2), Qt::CaseInsensitive);
}

WXJCOMMON_EXPORT bool wSameStr( const QString &s1, const QString &s2 )
{
	return s1 == s2;
}

WXJCOMMON_EXPORT QString wToLower( const QString &s1 )
{
	return s1.toLower();
}

WXJCOMMON_EXPORT QString wToUpper( const QString &s1 )
{
	return s1.toUpper();
}

WXJCOMMON_EXPORT QChar wToUpper( const QChar ch )
{
	return ch.toUpper();
}

WXJCOMMON_EXPORT QString wTrim( const QString &s )
{
	return s.trimmed();
}

WXJCOMMON_EXPORT QString wTrimLeft( const QString &s )
{
	return QString(wTrimLeft(s.toStdString()).c_str());
}

WXJCOMMON_EXPORT QString wTrimRight( const QString &s )
{
	return QString(wTrimRight(s.toStdString()).c_str());
}

WXJCOMMON_EXPORT int wToInt( const QString &s, bool *ok /*= NULL*/ )
{
	return s.toInt(ok);
}

WXJCOMMON_EXPORT uint wToUInt( const QString &s, bool *ok /*= NULL*/ )
{
	return s.toUInt(ok);
}

WXJCOMMON_EXPORT short wToShort( const QString &s, bool *ok /*= NULL*/ )
{
	return s.toShort(ok);
}

WXJCOMMON_EXPORT ushort wToUShort( const QString &s, bool *ok /*= NULL*/ )
{
	return s.toUShort(ok);
}

WXJCOMMON_EXPORT long wToLong( const QString &s, bool *ok /*= NULL*/ )
{
	return s.toLong(ok);
}

WXJCOMMON_EXPORT ulong wToULong( const QString &s, bool *ok /*= NULL*/ )
{
	return s.toULong(ok);
}

WXJCOMMON_EXPORT wlonglong wToLongLong( const QString &s, bool *ok /*= NULL*/ )
{
	return s.toLongLong(ok);
}

WXJCOMMON_EXPORT wulonglong wToULongLong( const QString &s, bool *ok /*= NULL*/ )
{
	return s.toULongLong(ok);
}

WXJCOMMON_EXPORT double wToDouble( const QString &s, bool *ok /*= NULL*/ )
{
	return s.toDouble(ok);
}

WXJCOMMON_EXPORT float wToFloat( const QString &s, bool *ok /*= NULL*/ )
{
	return s.toFloat(ok);
}

WXJCOMMON_EXPORT QString wNumberToString( short val,int base/*=10*/ )
{
	return QString::number(val, base);
}

WXJCOMMON_EXPORT QString wNumberToString( ushort val,int base/*=10*/ )
{
	return QString::number(val, base);
}

WXJCOMMON_EXPORT QString wNumberToString( int val,int base/*=10*/ )
{
	return QString::number(val, base);
}

WXJCOMMON_EXPORT QString wNumberToString( uint val,int base/*=10*/ )
{
	return QString::number(val, base);
}

WXJCOMMON_EXPORT QString wNumberToString( long val,int base/*=10*/ )
{
	return QString::number(val, base);
}

WXJCOMMON_EXPORT QString wNumberToString( ulong val,int base/*=10*/ )
{
	return QString::number(val, base);
}

WXJCOMMON_EXPORT QString wNumberToString( wlonglong val,int base/*=10*/ )
{
	return QString::number(val, base);
}

WXJCOMMON_EXPORT QString wNumberToString( wulonglong val,int base/*=10*/ )
{
	return QString::number(val, base);
}

WXJCOMMON_EXPORT QString wNumberToString( float val, char f/*='g'*/, int prec/*=6*/ )
{
	return QString::number(val, f, prec);
}

WXJCOMMON_EXPORT QString wNumberToString( double val, char f/*='g'*/, int prec/*=6*/ )
{
	return QString::number(val, f, prec);
}

WXJCOMMON_EXPORT bool wContains( const QString &s, QChar c,Wxj::CaseSensitivity cs /*= Wxj::CS_Sensitive*/ )
{
	return s.contains(c,(Qt::CaseSensitivity)cs);
}

WXJCOMMON_EXPORT bool wContains( const QString &s, const QString &subStr, Wxj::CaseSensitivity cs /*= Wxj::CS_Sensitive*/ )
{
	return s.contains(subStr,(Qt::CaseSensitivity)cs);
}

WXJCOMMON_EXPORT QString wLeftString( const QString &s, int nCount )
{
	return s.left(nCount);
}

WXJCOMMON_EXPORT QString wRightString( const QString &s, int nCount )
{
	return s.right(nCount);
}

WXJCOMMON_EXPORT QString wValueFromRegistry( const QString &regPath, const QString regKey )
{
	QSettings setting(regPath, QSettings::NativeFormat);
	QString result = setting.value(regKey, "").toString();
	if ((result.length() > 0) && !((result.right(1) == "\\") || result.right(1) == "/"))
	{
		result += "/";
	}
	return QDir::fromNativeSeparators(result);
}

