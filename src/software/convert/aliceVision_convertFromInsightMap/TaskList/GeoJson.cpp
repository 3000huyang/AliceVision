#include "GeoJson.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegExp>

INSIGHT_NAME_SPACE
GeoJson::GeoJson()
{
}


GeoJson::~GeoJson()
{
}

bool GeoJson::readPos(const QString &posFile)
{
	_datas.clear();
	QFile file(posFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Can't open the file!" << endl;
		return false;
	}
	QTextStream is(&file);
	QString line = is.readLine();

	while (!line.isNull())
	{
		line = is.readLine();
		if (line.isEmpty())continue;
		if (line[0] == '#') continue;
		QRegExp exp = getRegExp(true, true, false, false, true);
		QStringList strs = line.split(exp);
		if (strs.size() != 7){
			qDebug() << "File format error!" << endl;
			return false;
		}
		PosData data;
		data.id = strs[0];
		data.lon = strs[1].toDouble();
		data.lat = strs[2].toDouble();
		data.ele = strs[3].toDouble();
		data.quality = strs[4];
		data.date = strs[5];
		data.time = strs[6];
		_datas.push_back(data);
	}
	return true;
}

bool GeoJson::readPosAll(const QString &posAllFile)
{
	_datas.clear();
	QFile file(posAllFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Can't open the file!" << endl;
		return false;
	}
	QTextStream is(&file);
	QString line = is.readLine();
	int id = 0;
	while (!line.isNull())
	{
		line = is.readLine();
		if (line.isEmpty())continue;
		if (line[0] == '#') continue;
		QRegExp exp = getRegExp(true, true, false, false, true);
		QStringList strs = line.split(exp);
		if (strs.size() != 6){
			qDebug() << "File format error!" << endl;
			return false;
		}
		PosData data;
		data.id = id++;
		data.lon = strs[0].toDouble();
		data.lat = strs[1].toDouble();
		data.ele = strs[2].toDouble();
		data.quality = strs[3];
		data.date = strs[4];
		data.time = strs[5];
		_datas.push_back(data);
	}
	return true;
}

bool GeoJson::writeGeoJson(const QString &geoJsonFile)
{
	QFile file(geoJsonFile);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "Can't write file" << geoJsonFile;
		return false;
	}

	QString left = "{\n"
		"\"type\": \"FeatureCollection\",\n"
		"\"features\" :";
	QString right = "\n}\n";
	QTextStream os(&file);
	os.setRealNumberPrecision(12);
	os.setRealNumberNotation(QTextStream::FixedNotation);
	os << left;
	os << "[\n";
	for (int i = 0; i < _datas.size(); ++i){
		os << "\t { \"type\": \"Feature\", \n"
					"\t\"geometry\" : {\"type\": \"Point\", \"coordinates\" : [";
		os << _datas[i].lon << ","
			<< _datas[i].lat << "]},\n"
			"\t\"properties\" : {\"quality\" : " << _datas[i].quality << "}}";
		if (i != _datas.size() - 1){
			os << "\t,\n";
		}
	}
	os << "]";
	os << right;
	return true;
}

bool GeoJson::writeGeoJsonTile(const Option &opt)
{
	//采用sample倍降采样来处理每一层
	QList<PosData> totalDatas = _datas;
	{
		QList<PosData> curData;
		for (int i = 0; i < _datas.size(); i += opt.oriSample){
			curData.push_back(_datas[i]);
		}
		_datas.swap(curData);
		writeGeoJson(opt.path + "/" + opt.baseName + QString::number(opt.maxZoom) + ".geojson");
	}
	
	int curZoom = opt.maxZoom - 1;
	while (curZoom >= opt.minZoom){
		if (_datas.size() > opt.minPoints)
		{
			QList<PosData> curData;
			for (int i = 0; i < _datas.size(); i += opt.sample){
				curData.push_back(_datas[i]);
			}
			_datas.swap(curData);
		}
		writeGeoJson(opt.path + "/" + opt.baseName + QString::number(curZoom) + ".geojson");
		--curZoom;
	}
	totalDatas.swap(_datas);
	return true;
}

void GeoJson::setPosData(const QList<PosData> &data)
{
	_datas = data;
}

QRegExp GeoJson::getRegExp(bool multiAsSingle, bool tab, bool semicolon, bool comma, bool space)
{
	QString left("[");
	QString right("]");
	if (multiAsSingle)
	{
		right += "+";
	}
	QString regString;

	if (tab)
	{
		regString.append("\\t");
	}

	if (semicolon)
	{
		regString.append(";");
	}

	if (comma)
	{
		regString.append(",");
	}

	if (space)
	{
		regString.append("\\s");
	}
	QString regStr = left + regString + right;
	QRegExp reg(regStr);
	return reg;
// 	if (!reg.isValid())
// 	{
// 		return reg;
// 	}
}

INSIGHT_NAME_SPACE_END