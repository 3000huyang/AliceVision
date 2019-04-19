#include <iostream>
#include <fstream>
#include <sstream>
#include "Coordinates.h"
#include "stlplus3/file_system.hpp"
//#include "stl/split.hpp"
#include "string_utils.h"
#include "gdal.h"
#include <ogr_spatialref.h>
#include "log/baselog.h"
#include <sstream>

INSIGHT_NAME_SPACE

bool parseCoordinates(std::vector<Coordinate> &coords, const std::string &dataBse)
{
	if (!stlplus::file_exists(dataBse))
	{
		return false;
	}
	coords.clear();
	std::ifstream ifs(dataBse);

	char *buffer = new char[90000];
	coords.reserve(7000);
	while (ifs.getline(buffer, 90000))
	{
		std::vector<std::string> values;
		split(buffer, ";", values);
		if (values.size() == 3)
		{
			Coordinate c;
			c.EPSGName = values[0];
			c.CoordinateName = values[1];
			c.WKT = values[2];
			coords.push_back(c);
		}
	}
	delete[] buffer;
	return true;
}

bool insight::Coordinate::isOK() const
{
	if (EPSGName.empty())return false;
	OGRSpatialReference oPGS;
	std::stringstream ss;
	ss.str(EPSGName);
	int epsg = 0;
	ss >> epsg;
	int error = oPGS.importFromEPSG(epsg);
	if (error != OGRERR_NONE){
		return false;
	}
	return true;
}

bool Coordinate::isProject(bool *ok) const
{
	OGRSpatialReference oPGS;
	std::stringstream ss;
	ss.str(EPSGName);
	int epsg = 0;
	ss >> epsg;
	int error = oPGS.importFromEPSG(epsg);
	if (error != OGRERR_NONE){
		ILOG(ERROR) << "Can't import epsg " << epsg << IEND;
		LOG(ERROR) << "Can't import epsg " << epsg;
		if(ok) *ok = false;
		return false;
	}
	if (ok) *ok = true;
	if (oPGS.IsProjected()){
		return true;
	}
	else{
		return false;
	}
}


int Coordinate::EPSG(bool *ok) const
{
	if (EPSGName.empty()){
		if (ok != nullptr){
			*ok = false;
		}
		return 0;
	}
	QString s(toqs(EPSGName));
	s = s.trimmed();
	int epsg = 0;
	epsg = s.toInt(ok);
	return epsg;
}
bool Coordinate::coordToSR(const Coordinate &coord, OGRSpatialReference &sr)
{
	bool ok = false;
	int fromEPSG = coord.EPSG(&ok);
	if (ok){
		OGRErr err = sr.importFromEPSG(fromEPSG);
		if (err != OGRERR_NONE){
			printf("Can't import coord with epsg = %d\n", fromEPSG);
			return false;
		}
	}
	else if (!coord.WKT.empty()){
		char buf[8192] = { 0 };
		char *pBuf = buf;
		memcpy(buf, coord.WKT.c_str(), coord.WKT.size());
		OGRErr err = sr.importFromWkt(&pBuf);
		if (err != OGRERR_NONE){
			printf("Can't import coord with wkt = %s\n", coord.WKT);
			return false;
		}
	}
	else{
		printf("Can't import coord \n");
		return false;
	}
	return true;
}


INSIGHT_NAME_SPACE_END


