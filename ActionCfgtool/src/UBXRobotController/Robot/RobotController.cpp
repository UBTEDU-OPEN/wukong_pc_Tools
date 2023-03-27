/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
* 
* 文件名称：RobotController.cpp
* 创建时间：2015/10/18 14:39
* 文件标识：
* 文件摘要：机器人控制类
* 
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/10/18 14:39
* 版本摘要：
*/
#include "StdAfx.h"
#include "RobotController.h"
#include "libusb.h"
//#include "formatfunc.h"
//#include "MemoryReportFilter.h"

#ifdef _DEBUG
#include <QDebug>
#endif


#ifdef Q_OS_WIN
extern "C"
{
#include "setupapi.h"
#include "hidsdi.h"
}
#endif

#ifdef Q_OS_WIN
static const GUID GUID_DEVINTERFACE_LIST[] =
{
    // GUID_DEVINTERFACE_USB_DEVICE
    { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },

    // GUID_DEVINTERFACE_DISK
    { 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },

    // GUID_DEVINTERFACE_HID,
    { 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } },

    // GUID_NDIS_LAN_CLASS
    { 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } }
};
#endif

CRobotController* CRobotController::m_pControllerStatic = NULL;
BOOL     g_bFirstDeviceArrival = FALSE; //设备第一次到达

//#pragma endregion

// 监控热插拔事件
class UsbEventThread : public QThread
{
   void run() {
       while (1)
       {
           libusb_handle_events(NULL);
           msleep(200);
//           sleep (10);
       }
   }
};


CRobotController::CRobotController(QWidget *parent) : QWidget(parent)
{
#ifdef Q_OS_WIN
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    m_pControllerStatic = this;

	m_nCurMotorID = -1;
	m_bSupportUTF8 = FALSE;
    m_pMacUsbEventThread    = NULL;

    // USB热插拔事件注册
    libusb_init(NULL);
    RegisterDeviceNotify();
}

CRobotController::~CRobotController()
{
    // libusb 注销
    libusb_exit(NULL);
   if (m_pMacUsbEventThread)
   {
       m_pMacUsbEventThread->quit();
       delete m_pMacUsbEventThread;
   }
}

//#pragma region



// USB热插拔回调函数
#ifdef Q_OS_MAC
int hotplug_callback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data)
{
    //    m_staticMain->m_bIsUsbInsert = true;
    if (NULL == user_data)
    {
        return -1;
    }
    CRobotController* pThis = (CRobotController*)user_data;

    switch(event)
    {
    case LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED:
        pThis->StartConnectTimer();
        
        emit pThis->SigConnectRobotState(true);
        break;
    case LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT:
        pThis->StartDisconncectTimer();

        emit pThis->SigConnectRobotState(false);
        break;
    default:
        break;
    }

    return 0;
}
#endif

/**************************************************************************
* 函数名: RegisterDeviceNotify
* 功能: 注册设备通知
* 时间: 2015/09/08 10:08
* 作者: ZDJ
*/
void CRobotController::RegisterDeviceNotify()
{
#ifdef Q_OS_MAC

    //CMyHidClass::SetArrivalCbk(OnDeviceArrival);
    //CMyHidClass::SetRemovalCbk(OnDeviceRemoval);

    // Modify by jianjie 2017/5/22
    // Mac 平台下改为 libusb 注册热插拔

    //注册usb设备热插拔事件的回调函数(二麦&五麦)
    libusb_hotplug_register_callback (NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED , LIBUSB_HOTPLUG_NO_FLAGS,
        TWO_ALPHA_ROBOT_VENDOR_ID, TWO_ALPHA_ROBOT_PRODUCT_ID, LIBUSB_HOTPLUG_MATCH_ANY, hotplug_callback, this, NULL);

    libusb_hotplug_register_callback (NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, LIBUSB_HOTPLUG_NO_FLAGS, 
        TWO_ALPHA_ROBOT_VENDOR_ID, TWO_ALPHA_ROBOT_PRODUCT_ID, LIBUSB_HOTPLUG_MATCH_ANY, hotplug_callback, this, NULL);

    libusb_hotplug_register_callback (NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED , LIBUSB_HOTPLUG_NO_FLAGS,
        FIVE_ALPHA_ROBOT_VENDOR_ID, FIVE_ALPHA_ROBOT_PRODUCT_ID, LIBUSB_HOTPLUG_MATCH_ANY, hotplug_callback, this, NULL);

    libusb_hotplug_register_callback (NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, LIBUSB_HOTPLUG_NO_FLAGS, 
        FIVE_ALPHA_ROBOT_VENDOR_ID, FIVE_ALPHA_ROBOT_PRODUCT_ID, LIBUSB_HOTPLUG_MATCH_ANY, hotplug_callback, this, NULL);

   m_pMacUsbEventThread = new UsbEventThread();
   m_pMacUsbEventThread->start();

#elif defined(Q_OS_WIN)

    HDEVNOTIFY hDevNotify = NULL;

    //DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
    //ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
    //NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    //NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

    //for(int i = 0; i < sizeof(GUID_DEVINTERFACE_LIST) / sizeof(GUID); i++)
    //{
    //    NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];
    //    hDevNotify = RegisterDeviceNotification((HANDLE)this->winId(), &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
    //    if( !hDevNotify )
    //    {
    //        //AfxMessageBox(CString(_T("Can't register device notification: ")), MB_ICONEXCLAMATION);
    //        return;
    //    }
    //    else
    //    {
    //        qDebug("reg notify success");
    //    }
    //}

#endif
}

/**************************************************************************
* 函数名: TestUsbConnectRobot
* 功能: 检测是否有机器人通过USB连接
* 返回： true 存在USB连接的机器人
* 时间: 2017/5/22 15：09
* 作者: jianjie
*/
bool CRobotController::TestUsbConnectRobot()
{
    bool bRet = false;

    libusb_device **devs;
    libusb_device *dev;
    libusb_get_device_list(NULL, &devs);
    int i = 0;

    while ((dev = devs[i++]) != NULL)
    {
        struct libusb_device_descriptor desc;
        libusb_get_device_descriptor(dev, &desc);

        int nVendor = desc.idVendor;
        int nProduct = desc.idProduct;
        if ((nVendor == TWO_ALPHA_ROBOT_VENDOR_ID) /*&& (nProduct == TWO_ALPHA_ROBOT_PRODUCT_ID)*/)
        {
            bRet = true;
            break;
        }

        if ((nVendor == FIVE_ALPHA_ROBOT_VENDOR_ID) /*&& (nProduct == FIVE_ALPHA_ROBOT_PRODUCT_ID)*/)
        {
            bRet = true;
            break;
        }

        if (nVendor == ALPHA_MINI_VENDOR_ID)
        {
            bRet = true;
            break;
        }
    }

    return bRet;
}

#if 0
/**************************************************************************
* 函数名: OnDeviceChange
* 功能: 设备更改通知
* 时间: 2015/07/22 17:05
* 作者: ZDJ
*/
LRESULT CRobotController::OnDeviceChange(WPARAM wParam, LPARAM lParam)
{

	if ( DBT_DEVICEARRIVAL == wParam || DBT_DEVICEREMOVECOMPLETE == wParam ) 
	{
		PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
		PDEV_BROADCAST_DEVICEINTERFACE pDevInf = NULL;
		//PDEV_BROADCAST_HANDLE pDevHnd;
		//PDEV_BROADCAST_OEM pDevOem;
		//PDEV_BROADCAST_PORT pDevPort;
		//PDEV_BROADCAST_VOLUME pDevVolume;
		switch( pHdr->dbch_devicetype ) 
		{
		case DBT_DEVTYP_DEVICEINTERFACE:
			{
				pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;

				UpdateDevice(pDevInf, wParam);
			}

			break;

		}
	}

	return 0;
}
#endif
//#pragma endregion

#ifdef Q_OS_WIN
/**************************************************************************
* 函数名: UpdateDevice
* 功能: 设备更新通知
* 时间: 2015/07/22 10:25
* 作者: ZDJ
*/
//void CRobotController::UpdateDevice(PDEV_BROADCAST_DEVICEINTERFACE pDevInf, WPARAM wParam)
//{
//    //ASSERT(lstrlen(pDevInf->dbcc_name) > 4);
//    QString szDevId = QString::fromWCharArray(pDevInf->dbcc_name+4);
//    int idx = szDevId.lastIndexOf('#');
//    //ASSERT( -1 != idx );
//    szDevId.truncate(idx);
//    szDevId.replace('#', '\\');
//    szDevId.toUpper();
//
//#ifndef QT_NO_DEBUG
//    qDebug() << "szDevId:" << szDevId;
//#endif
//
//    idx = szDevId.indexOf('\\');
//    //ASSERT(-1 != idx );
//    QString szClass = szDevId.left(idx);
//
//    if("ROOT" == szClass)
//	{
//		return;
//	}
//
//	int nTempVID = 0;
//	int nTempPID = 0;
//	int nTempREV = 0;
//
//    if(szDevId.indexOf("\\VID_") != -1 && szDevId.indexOf("&PID_") != -1)
//    {
//        QString sTemp = szDevId.mid(szDevId.indexOf("VID_"));
//        sscanf(sTemp.toStdString().c_str(), "VID_%04X&PID_%04X", &nTempVID, &nTempPID);
//
//#ifndef QT_NO_DEBUG
//        qDebug() << "sTemp:" << sTemp;
//        qDebug() << "nTempVID:" << nTempVID << " nTempPID:" << nTempPID;
//#endif
//    }
//
//    // Delete by jianjie 2017/5/23 虚拟串口，Alpha2暂未用到
//	//if((nTempVID == g_devidarr[ePortTypeHID].dwVid && nTempPID == g_devidarr[ePortTypeHID].dwPid)
//	//	|| (nTempVID == g_devidarr[ePortTypeVCP].dwVid && nTempPID == g_devidarr[ePortTypeVCP].dwPid)
//	//	)
//	//{
//		if(DBT_DEVICEARRIVAL == wParam )
//		{
//            // Delete by jianjie 2017/5/23 仅用于响应usb时间，无需关注端口状态
////#ifndef QT_NO_DEBUG
////            qDebug() << "FindMySTMDevice 1";
////#endif
////            if(szDevId.left(4) == "USB\\" && !FindMySTMDevice())
////				return;
////
////#ifndef QT_NO_DEBUG
////            qDebug() << "FindMySTMDevice 2";
////#endif
////
////			if(m_pPort && m_pPort->IsOpen())
////				return;
//
//            //SetTimer(g_nConnectPortTimerID, TIMEDELAY_CONNECTROBOT, NULL);
//            StartConnectTimer();
//            qDebug("StartConnectTimer");
//
//            emit SigConnectRobotState(true);
//            //BOOL bRet = Req_AutoOpenPort();
//            //NotifyObservers(WM_MSG_PORT_OPENSTATECHANGE, NULL, NULL, bRet);
//        }
//		else
//		{
//			//if(!m_pPort || (m_pPort && !m_pPort->IsOpen()))
//			//	return;
//
//            //SetTimer(g_nDisconnectPortTimerID, TIMEDELAY_CONNECTROBOT, NULL);
//            StartDisconncectTimer();
//            qDebug("StartDisconncectTimer");
//
//            emit SigConnectRobotState(false);
//            //Req_ClosePort();
//            //NotifyObservers(WM_MSG_PORT_OPENSTATECHANGE, NULL, NULL, FALSE);
//        }
//	//}	
//}
#endif

/**************************************************************************
* 函数名: StartConnectTimer
* 功能: 启动连接端口的时钟
* 时间: 2015/11/01 16:45
* 作者: ZDJ
*/
void CRobotController::StartConnectTimer()
{
    startTimer(TIMEDELAY_CONNECTROBOT, Qt::PreciseTimer);
}

/**************************************************************************
* 函数名: StartDisconncectTimer
* 功能: 启动断开端口的时钟
* 时间: 2015/11/01 16:45
* 作者: ZDJ
*/
void CRobotController::StartDisconncectTimer()
{
    startTimer(TIMEDELAY_CONNECTROBOT, Qt::PreciseTimer);
}
