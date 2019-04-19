#include "license.h"
#include <QDataStream>

INSIGHT_NAME_SPACE


void License::connectServer()
{
	blockSize = 0;
	tcpSocket->abort();
	tcpSocket->connectToHost(ip, port);
}

void License::readDatas()
{
	bytes.push_back(tcpSocket->readAll());
	if (bytes.size() == 32){
		int imageCount = bytes.toInt();
		qDebug() << " image count  = " << imageCount;
		emit licenseReceived(imageCount);
		bytes.clear();
	}
}

void License::displayError(QAbstractSocket::SocketError socketError)
{
	switch (socketError) {
	case QAbstractSocket::RemoteHostClosedError:
		break;
	case QAbstractSocket::HostNotFoundError:
		qDebug() << tr("The host was not found. Please check the "
			"host name and port settings.");
		break;
	case QAbstractSocket::ConnectionRefusedError:
		qDebug() <<
			tr("The connection was refused by the peer. "
			"Make sure the fortune server is running, "
			"and check that the host name and port "
			"settings are correct.");
		break;
	default:
		qDebug() <<
			tr("The following error occurred: %1.")
			.arg(tcpSocket->errorString());
	}
}

void License::requestLicense()
{
	int n = 100;

	QByteArray nByte = QByteArray::number(n);
	//qDebug() << nByte;
	nByte = nByte.rightJustified(32, '0');
	//qDebug() << nByte;
	for (int n = 0; n < 100; ++n)
	{
		nByte.push_back(n);
	}
	tcpSocket->write(nByte);
}

void License::startCheck()
{
	timer->start(10000);//10s ¼ì²é
}

void License::stopCheck()
{
	timer->stop();
}

INSIGHT_NAME_SPACE_END


