#ifndef text_parser_h__
#define text_parser_h__

#include <QList>
#include <QString>
#include <QStringList>

#include "tasklist_global.h"

INSIGHT_NAME_SPACE
struct TASKLIST_EXPORT TextParser
{
	TextParser() :m_columns(0)
	{
	}

	typedef QStringList  RowData;
	QList<RowData> m_tableData;
	QList<QString> m_fields;
	int m_rowFrom = 0;//从第0行开始
	QString m_txt;
	int m_columns;
	void updateColumnCount();
	// semicolon 分号 ;
	// comma  逗号
	// tab 缩进
	void parse(bool tab, bool semicolon, bool comma, bool space, QString other, bool multiAsSingle);

	
};

INSIGHT_NAME_SPACE_END
#endif // text_parser_h__
