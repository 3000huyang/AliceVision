#ifndef TASKLIST_GLOBAL_H
#define TASKLIST_GLOBAL_H

//#include <QtCore/qglobal.h>

/*
task list Ϊ�������ļ������ṹ�Ķ����IO���μ�insight map ��Ʒ�ƻ� wiki file_io_define
֮���Խ�task list �������ļ�����֮��ģ�����Ϊ���io�������Ϊ�˺������������ṩ�Ľӿ�

*/
#define TASKLIST_EXPORT
// #ifdef TASKLIST_LIB
// # define TASKLIST_EXPORT Q_DECL_EXPORT
// #else
// # define TASKLIST_EXPORT Q_DECL_IMPORT
// #endif

#ifndef INSIGHT_NAME_SPACE
#define INSIGHT_NAME_SPACE namespace insight{
#define INSIGHT_NAME_SPACE_END }
#endif

#endif // TASKLIST_GLOBAL_H
