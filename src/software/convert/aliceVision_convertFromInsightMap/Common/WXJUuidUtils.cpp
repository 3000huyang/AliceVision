#include <QUuid>
#include "WXJUuidUtils.h"
#include "WXJCommonGlobal.h"
WXJCOMMON_EXPORT std::string wCreateUuidString()
{
	return QUuid::createUuid().toString().toStdString();
}

// WXJCOMMON_EXPORT QUuid wCreateUuid()
// {
// 	return QUuid::createUuid();
// }
