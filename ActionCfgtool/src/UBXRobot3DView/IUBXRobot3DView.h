/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* IUBXRobot3DView.h
* 创建时间：2015/12/24
* 文件标识：
* 文件摘要：UBXRobot3D模型视图接口声明文件
*
* 当前版本：1.0.0.0
* 作    者：hels
* 完成时间：2016/01/05
* 版本摘要：
*/


#ifndef IUBXROBOT3DVIEW_H
#define IUBXROBOT3DVIEW_H

#include "ubxrobot3dview_global.h"

class QWidget;

#define POWEROFF    0   //未连接
#define POWERLOST   1   //掉电
#define POWERON     2   //得电

class UBXROBOT3DVIEWSHARED_EXPORT IUBXRobot3DViewEvent
{
public:
    /*
    * brief:  3D FBX模型载入成功后通知实现此接口的实例
    * author: hels
    * date:   2015-12-24
    * param:  void
    * return: void
    */
    virtual void IUBXRobot3DViewEvent_On3DModelLoaded() = 0;

    /*
    * brief:  FBX事件回调接口实现函数，检测模型是否正在仿真
    * author: hels
    * date:   2015-12-24
    * param:  void
    * return: 模型当前的仿真状态
    */
    virtual bool IUBXRobot3DViewEvent_IsRobotDebuging() = 0;

    virtual void IUBXRobot3DViewEvent_SetRotation(int nId, int nAngle, int nTime) = 0;
    virtual void IUBXRobot3DViewEvent_ReadRotation(int nID, bool bPowerDown = true) = 0;

    virtual void IUBXRobot3DViewEvent_SetAnglesToRobot(MOTOR_DATA* pMotor, int nMotorCount, int nRunTime , int nCurveType = 0) = 0;

    virtual void IUBXRobot3DViewEvent_StopActionPlay() = 0;
};


class UBXROBOT3DVIEWSHARED_EXPORT IUBXRobot3DView
{
public:
    /*
    * brief:  初始化
    * author: hels
    * date:   2015-12-24
    * param:  [in] parent 父窗体指针
    * param:  [in] pEvent 事件通知接口指针
    * return: void
    */
    virtual void Connect(QWidget* parent, IUBXRobot3DViewEvent* pEvent) = 0;

    /*
    * brief:  释放资源,注意会销毁对象本身，调用此函数后，对象指针失效
    * author: hels
    * date:   2015-12-24
    * param:  void
    * return: void
    */
    virtual void Disconnect() = 0;

    /************************************
    * 名称: SetSerialPortControl
    * 功能: 设置串口通信实例
    * 访问权限:    public
    * 参数: [in]CUBXSerialPortSesstion *pConnect 串口实例
    * 返回:   bool
    * 时间:   2016/01/05
    * 作者:   hels
    ************************************/
//    virtual bool SetSerialPortControl(CUBXSerialPortSesstion *pConnect) = 0;

    //获取各舵机角度
    virtual bool GetMotorDatas(char** pData, int& nLen, emActionLayerType eActionType) = 0;
    //设置各舵机角度
    virtual void SetMotorDatas(char* pData, int nLen, int runTime) = 0;
    //设置各舵机角度
    virtual void SetMotorDatasManual(char* pData, int nLen, int runTime) = 0;
    virtual void SetMotorAngle(int nID, int nAngle) = 0;
    //设置3D模型的仿真状态
    virtual void Set3DModelPlayState(bool bState) = 0;
    virtual void SetMotorPowerState(int nMotorID, int nState) = 0;
    //设置所有舵机角度
    virtual void SetAllMotorAngle(char* pData, int nLen, int nRunTime, int nAllTime) = 0;
    //获取串口连接状态
    virtual bool GetComConnectState() = 0;
    //启用/禁用控制界面
    virtual void EnableViewCtrl(bool bEnable) = 0;
    //获取舵机数目
    virtual int GetMotorCount() = 0;
    //获取窗体
    virtual QWidget* GetWidget() = 0;

    virtual void SetMotorDatasSmoothly(char* pData, int nLen, int iTime) = 0;

//    //耳朵灯仿真
//    virtual void PlayEarData(int RunTime,int LeftLed,int RightLed,int Bright,int LedLightUpTime,int LedLightDownTime) = 0;
//    //眼睛灯仿真
//    virtual void PlayEyeData(int LeftLed, int RightLed, int Bright, int Color, int LightUpTime, int LightDownTime, int RunTime) = 0;
//    //停止眼睛灯仿真
//    virtual void StopPlayEyeData() = 0;
//    //停止耳朵灯仿真
//    virtual void StopPlayEarData() = 0;
};

bool UBXROBOT3DVIEWSHARED_EXPORT CreateUBXRobot3DView(IUBXRobot3DView ** pUBXRobot3DView);

#endif // IUBXROBOT3DVIEW_H
