#include "md5.h"

#include <QString>
#include <QByteArray>
#include <QCryptographicHash>
#include <QFile>
#include <QDebug>

INSIGHT_NAME_SPACE
QByteArray getFileMd5(const QString &filePath)
{
	QFile localFile(filePath);

	if (!localFile.open(QFile::ReadOnly))
	{
		qDebug() << "file open error.";
		return 0;
	}

	QCryptographicHash ch(QCryptographicHash::Md5);

	quint64 totalBytes = 0;
	quint64 bytesWritten = 0;
	quint64 bytesToWrite = 0;
	quint64 loadSize = 1024 * 4;
	QByteArray buf;

	totalBytes = localFile.size();
	bytesToWrite = totalBytes;

	while (1)
	{
		if (bytesToWrite > 0)
		{
			buf = localFile.read(qMin(bytesToWrite, loadSize));
			ch.addData(buf);
			bytesWritten += buf.length();
			bytesToWrite -= buf.length();
			buf.resize(0);
		}
		else
		{
			break;
		}

		if (bytesWritten == totalBytes)
		{
			break;
		}
	}

	localFile.close();
	QByteArray md5 = ch.result();
	return md5;
}

QString getFileMd5Hex(const QString &filePath){
	return getFileMd5(filePath).toHex();
}

INSIGHT_NAME_SPACE_END