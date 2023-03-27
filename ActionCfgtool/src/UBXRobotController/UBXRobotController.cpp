// UBXRobotController.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "UBXRobotController.h"

CRobotController*   g_pRobotController = NULL;  //机器人通信控制器

/**************************************************************************
* 函数名: CreateRobotController
* 功能: 创建机器人控制器
* 时间: 2015/09/16 15:18
* 作者: ZDJ
*/
UBXROBOTCONTROLLER_API CRobotController* CreateRobotController(QWidget *parent)
{
    if(!g_pRobotController)
        g_pRobotController = new CRobotController(parent);
    return g_pRobotController;
}

/**************************************************************************
* 函数名: ReleaseRobotController
* 功能: 释放机器人控制器
* 时间: 2015/09/16 15:19
* 作者: ZDJ
*/
UBXROBOTCONTROLLER_API void ReleaseRobotController()
{	
    delete g_pRobotController;
    g_pRobotController = NULL;
}
