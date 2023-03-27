/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：UBXUpdateAdministrator.cpp
* 创建时间：2016/07/23 16:17
* 文件标识：
* 文件摘要：更新模块管理
*
* 当前版本：1.0.0.0
* 作    者：hwx
* 完成时间：2016/07/23 16:17
* 版本摘要：
*/

#include "ubxupdateadministrator.h"

/**************************************************************************
* 函数名: getInstance
* 功能: 获取更新模块管理
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
UBXUpdateAdministrator *UBXUpdateAdministrator::getInstance()
{
    static UBXUpdateAdministrator updateAdm;
    return &updateAdm;
}

/**************************************************************************
* 函数名: OnNotify
* 功能: 通知消息
* 参数:
*    @[in ] pSubject: 被观察者
*    @[in ] nMsgID: 消息ID
*    @[in ] dwContext: 消息上下文
*    @[in ] wParam: 参数1
*    @[in ] lParam: 参数2
* 返回值: void
* 时间: 2015/08/22 19:09
* 作者: zdj
*/
void UBXUpdateAdministrator::OnNotify(const zxObserverSubjectBase *pSubject, int nMsgID, DWORD dwContext, WPARAM wParam, LPARAM lParam)
{
    if(nMsgID == WM_MSG_ROBOT_SWITCHMODE_RESULT)
    {
        if (m_pSysEmbedUpdateWidget && m_pSysEmbedUpdateWidget->isForceCopyUpateVersion())
        {
            // 切换U盘模式
           emit sigOnNotifyChangedUdiskResult((bool) wParam);
        }
    }
    else if (nMsgID == WM_MSG_ROBOT_CONNECT)
    {
         AlphaRobotLog::getInstance()->printDebug("Get robot connect in UBXUpdateAdministrator");
         getSysEmbedUpdateWidget();  // 构建对象，为后续使用准备，因为连接上机器人后，软件会自动触发查询嵌入式版本的命令，自动走嵌入式版本更新流程
    }
}

UBXUpdateAdministrator::UBXUpdateAdministrator(QObject *parent) : QObject(parent)
   ,m_pSysEmbedUpdateWidget(NULL)
   ,m_pSystemPCSoftVersionUI(NULL)
   ,m_pVideoTutorialHandle(NULL)
{
    if(g_pRobotController)
    {
        AddSubscribe(g_pRobotController); //添加通信消息订阅
    }
}

UBXUpdateAdministrator::~UBXUpdateAdministrator()
{
    SAFE_DELETE(m_pSysEmbedUpdateWidget);
    SAFE_DELETE(m_pSystemPCSoftVersionUI);
}

CVideoTutorialHandle *UBXUpdateAdministrator::getVideoTutorialHandle()
{
    if(NULL == m_pVideoTutorialHandle)
    {
        m_pVideoTutorialHandle = new CVideoTutorialHandle();
    }

    return m_pVideoTutorialHandle;
}

SystemEmbeddedUpdateWidget *UBXUpdateAdministrator::getSysEmbedUpdateWidget()
{
    if(NULL == m_pSysEmbedUpdateWidget)
    {
        m_pSysEmbedUpdateWidget = new SystemEmbeddedUpdateWidget();

        connect(m_pSysEmbedUpdateWidget, &SystemEmbeddedUpdateWidget::sigSystemEmbedVersionUpdateStatus,
                this, &UBXUpdateAdministrator::sigSystemEmbedVersionUpdateStatus);
        connect(m_pSysEmbedUpdateWidget, &SystemEmbeddedUpdateWidget::sigIsSystemEmbedVersionNeedCopyUpdate,
                this, &UBXUpdateAdministrator::sigIsSystemEmbedVersionNeedCopyUpdate);
        connect(this, &UBXUpdateAdministrator::sigOnNotifyChangedUdiskResult, m_pSysEmbedUpdateWidget, &SystemEmbeddedUpdateWidget::slotOnChangedToUDiskResult);
    }

    return m_pSysEmbedUpdateWidget;
}

void UBXUpdateAdministrator::slotOnTimeout()
{
      getSysEmbedUpdateWidget()->doCheckUpdate(false);
}

systemPCSoftVersionUI *UBXUpdateAdministrator::getSystemPCSoftVersionUI()
{
    if(NULL == m_pSystemPCSoftVersionUI)
    {
        m_pSystemPCSoftVersionUI = new systemPCSoftVersionUI();
    }

    return m_pSystemPCSoftVersionUI;
}


/**************************************************************************
* 函数名: checkUpdateSystemsEmbedded
* 功能: 检查嵌入式版本更新
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
bool UBXUpdateAdministrator::checkUpdateSystemsEmbedded(bool bCanShowMBox)
{
    getSysEmbedUpdateWidget()->doCheckUpdate(bCanShowMBox);
    return true;
}


void UBXUpdateAdministrator::showSytemPCSoftVersionUI()
{
    getSystemPCSoftVersionUI()->initUI();
    getSystemPCSoftVersionUI()->show();
}
