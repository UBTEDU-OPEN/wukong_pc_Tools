/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
* 
* 文件名称：RobotController.h
* 创建时间：2015/10/18 14:39
* 文件标识：
* 文件摘要：机器人控制类
* 
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/10/18 14:39
* 版本摘要：
*/
#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include <QWidget>
#include <QThread>
#include "util.h"
#include "type.h"

#ifdef Q_OS_WIN
//#include <dbt.h>
#include <windows.h>
#endif

#define TIMEDELAY_CONNECTROBOT  2000   //连接机器人延时

//五麦机器人
#define FIVE_ALPHA_ROBOT_VENDOR_ID     8711
#define FIVE_ALPHA_ROBOT_PRODUCT_ID    17

//二麦机器人
#define TWO_ALPHA_ROBOT_VENDOR_ID     6353
#define TWO_ALPHA_ROBOT_PRODUCT_ID    20006

// AlphaMini
#define ALPHA_MINI_VENDOR_ID            3725
#define ALPHA_MINI_PRODUCT_ID           8221

class UsbEventThread;
class UBXROBOTCONTROLLER_API CRobotController :  public QWidget
{
        Q_OBJECT
public:	
        explicit CRobotController(QWidget* parent = NULL);
        ~CRobotController();
//#pragma region
public:

    /**************************************************************************
    * 函数名: StartConnectTimer
    * 功能: 启动连接端口的时钟
    * 时间: 2015/11/01 16:45
    * 作者: ZDJ
    */
    void StartConnectTimer();

    /**************************************************************************
    * 函数名: StartDisconncectTimer
    * 功能: 启动断开端口的时钟
    * 时间: 2015/11/01 16:45
    * 作者: ZDJ
    */
    void StartDisconncectTimer();

    /**************************************************************************
    * 函数名: TestUsbConnectRobot
    * 功能: 检测是否有机器人通过USB连接
    * 返回： true 存在USB连接的机器人
    * 时间: 2017/5/22 15：09
    * 作者: jianjie
    */
    bool TestUsbConnectRobot();

#ifdef Q_OS_WIN
        bool OnDeviceChange(MSG *msg, long *result);
#endif

private:
#if 0
        /**************************************************************************
        * 函数名: OpenComPort
        * 功能: 打开串口
        * 参数:
        *    @[in ] sComName: 串口名称，如COM1
        *    @[in ] nBandrate: 打开端口使用的波特率
        *    @[out] lpszErrStr: 错误信息
        * 返回值: 成功返回TRUE，失败返回FALSE
        * 时间: 2015/07/16 17:09
        * 作者: ZDJ
        */
        BOOL OpenComPort(const QString& sComName, int nBandrate, char* lpszErrStr);
#endif
        /**************************************************************************
        * 函数名: RegisterDeviceNotify
        * 功能: 注册设备通知
        * 时间: 2015/09/08 10:08
        * 作者: ZDJ
        */
        void RegisterDeviceNotify();

#ifdef Q_OS_WIN
        /**************************************************************************
        * 函数名: UpdateDevice
        * 功能: 设备更新通知
        * 时间: 2015/07/22 10:25
        * 作者: ZDJ
        */
        //void UpdateDevice(PDEV_BROADCAST_DEVICEINTERFACE pDevInf, WPARAM wParam);
#endif

    /**************************************************************************
    * 函数名: OnDeviceChange
    * 功能: 设备更改通知
    * 时间: 2015/07/22 17:05
    * 作者: ZDJ
    */
#if 0
    LRESULT OnDeviceChange(WPARAM wParam, LPARAM lParam);
#endif
#ifdef Q_OS_WIN
    //virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#endif
    //virtual void timerEvent(QTimerEvent *event);
    //virtual void customEvent(QEvent *event);

signals:
    // 机器人连接状态信号
    void SigConnectRobotState(bool bConnect);
private:
#ifdef Q_OS_MAC
//    static void OnDeviceArrival(void *context, IOReturn result, void *sender, IOHIDDeviceRef device);
//    static void OnDeviceRemoval(void *context, IOReturn result, void *sender, IOHIDDeviceRef device);
    /**************************************************************************
    * 函数名: hotplug_callback
    * 功能: libusb 响应热插拔事件
    * 时间: 2017/5/22 11：28
    * 作者: jianjie
    */
    //static int LIBUSB_CALL hotplug_callback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data);
#endif
//#pragma endregion
private:
    int                             m_nCurMotorID;   //当前操作舵机ID
    BOOL                            m_bSupportUTF8;  //是否支持UTF8
    UsbEventThread                  *m_pMacUsbEventThread;           // Mac平台USB热插拔监控
    static CRobotController         *m_pControllerStatic;
};
#endif

