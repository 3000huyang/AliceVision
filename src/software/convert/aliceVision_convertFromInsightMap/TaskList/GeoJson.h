#pragma once
#include <QString>
#include <QList>
#include <QRegExp>
#include "tasklist_global.h"

INSIGHT_NAME_SPACE
struct TASKLIST_EXPORT PosData{
	QString id;
	double lon = 0;
	double lat = 0;
	double ele = 0;
	QString  quality;
	QString date;
	QString time;
};
class TASKLIST_EXPORT GeoJson
{
public:
	GeoJson();
	~GeoJson();

	bool readPos(const QString &file); 
	bool readPosAll(const QString &file);
	bool writeGeoJson(const QString &geoJsonFile);
	struct Option
	{
		int minZoom = 4;
		int maxZoom = 20;
		QString path;
		QString baseName;
		int oriSample = 4;
		int sample = 2;//2±¶²ÉÑù
		int minPoints = 1000;
	};
	bool writeGeoJsonTile(const Option &opt);
	void setPosData(const QList<PosData> &data);
private:
	QRegExp getRegExp(bool multiAsSingle,
		bool tab,
		bool semicolon,
		bool comma,
		bool space);
	QList<PosData> _datas;
};

INSIGHT_NAME_SPACE_END

