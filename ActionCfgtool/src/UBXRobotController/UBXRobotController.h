// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 UBXROBOTCONTROLLER_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// UBXROBOTCONTROLLER_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#ifndef UBXROBOTCONTROLLER_H
#define UBXROBOTCONTROLLER_H

#include <QtCore/qglobal.h>

#ifdef UBXROBOTCONTROLLER_EXPORTS
#define UBXROBOTCONTROLLER_API Q_DECL_EXPORT
#else
#define UBXROBOTCONTROLLER_API Q_DECL_IMPORT
#endif

//#include <Dbt.h>
//#include <SetupAPI.h>
#include <QFile>
#include "RobotController.h"

#ifdef Q_OS_WIN
UBXROBOTCONTROLLER_API extern CRobotController*   g_pRobotController;  //机器人通信控制器
#else
extern CRobotController*   g_pRobotController;  //机器人通信控制器
#endif

/**************************************************************************
* 函数名: CreateRobotController
* 功能: 创建机器人控制器
* 时间: 2015/09/16 15:18
* 作者: ZDJ
*/
UBXROBOTCONTROLLER_API CRobotController* CreateRobotController(QWidget* parent = NULL);

/**************************************************************************
* 函数名: ReleaseRobotController
* 功能: 释放机器人控制器
* 时间: 2015/09/16 15:19
* 作者: ZDJ
*/
UBXROBOTCONTROLLER_API void ReleaseRobotController();


#endif
