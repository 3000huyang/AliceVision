#ifndef md5_h__
#define md5_h__


#include "common_global.h"
#include <QtCore/qglobal.h>
INSIGHT_NAME_SPACE

QByteArray getFileMd5(const QString &filePath);
QString getFileMd5Hex(const QString &filePath);

INSIGHT_NAME_SPACE_END
#endif // md5_h__
