
#ifndef license_h__
#define license_h__
#include "common_global.h"
#include <string>
#include <QTimer>
#include<QAbstractSocket>
#include <QTcpSocket>

INSIGHT_NAME_SPACE

class License : public QObject
{
	Q_OBJECT

signals:
	void licenseReceived(int maxImages);
	void connected();
	void disconnected();
public:
	License(QString ip_, int port_, QObject *parent)
		: QObject(parent), blockSize(0)
		, ip(ip_), port(port_)
	{
		tcpSocket = new QTcpSocket(this);

		connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readDatas()));
		connect(tcpSocket, SIGNAL(connected()), this, SLOT(startCheck()));
		connect(tcpSocket, SIGNAL(connected()), this, SIGNAL(connected()));
		connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(stopCheck()));
		connect(tcpSocket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));

		connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
			this, SLOT(displayError(QAbstractSocket::SocketError)));
		timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(requestLicense()));
		
	}

	public slots:
	void connectServer();
	void requestLicense();
	private slots:
	void startCheck();//¿ªÊ¼¼àÊÓ
	void stopCheck();
	void readDatas();
	void displayError(QAbstractSocket::SocketError socketError);
private:
	QTcpSocket *tcpSocket;
	QString currentFortune;
	quint16 blockSize;
	QByteArray bytes;
	QString ip;
	int port;
	QTimer *timer;
};
INSIGHT_NAME_SPACE_END
#endif // license_h__