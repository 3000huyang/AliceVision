#include "text_parser.h"



void insight::TextParser::updateColumnCount()
{
	int maxCol = 0;
	for (int i = 0; i < m_tableData.count(); ++i)
	{
		if (m_tableData[i].count() > maxCol)
		{
			maxCol = m_tableData[i].count();
		}
	}
	m_columns = maxCol;
	m_fields.clear();

	for (int i = 0; i < m_columns; ++i)
	{
		m_fields.append("Undefined");
	}
}

void insight::TextParser::parse(bool tab, bool semicolon, bool comma, bool space, QString other, bool multiAsSingle)
{
	m_tableData.clear();
	m_fields.clear();

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

	regString.append(other);
	QString regStr = left + regString + right;
	// 		printf(regStr.toLocal8Bit());
	// 		printf("\n");
	QRegExp reg(regStr);
	if (!reg.isValid())
	{
		return;
	}
	QStringList lines = m_txt.split('\n');

	for (int i = 0; i < lines.count(); ++i)
	{
		if (lines[i].isEmpty())continue;
		QStringList aLine = lines[i].split(reg);
		if (!aLine.isEmpty())
		{
			m_tableData.append(aLine);
		}
	}
	updateColumnCount();
}
