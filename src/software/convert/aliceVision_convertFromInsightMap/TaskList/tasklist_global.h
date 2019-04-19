#ifndef TASKLIST_GLOBAL_H
#define TASKLIST_GLOBAL_H

//#include <QtCore/qglobal.h>

/*
task list 为基本的文件交互结构的定义和IO，参见insight map 产品计划 wiki file_io_define
之所以叫task list 而不叫文件交互之类的，是因为这个io本身就是为了后续处理任务提供的接口

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
