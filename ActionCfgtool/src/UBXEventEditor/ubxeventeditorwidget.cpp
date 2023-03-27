/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：UBXEventEditorWidget.cpp
* 创建时间：2016/11/07
* 文件标识：
* 文件摘要：事件编辑界面
*
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2016/11/07
* 版本摘要：
*/
#include "stdafx.h"
#include "ubxeventeditorwidget.h"
#include "ui_ubxeventeditorwidget.h"
#include "configs.h"
#include <QMessageBox>
#include <QPainter>
#include <QResource>
#include <QDir>
#include <QDebug>
#include "util.h"

bool CreateEventEditor(IUBXEventEditor** ppEventEditor)
{
    UBXEventEditorWidget* pProxy = new UBXEventEditorWidget();
    if (pProxy)
    {
        *ppEventEditor = pProxy;
        return true;
    }

    return false;
}

UBXEventEditorWidget::UBXEventEditorWidget(QWidget *parent) :
    QWidget(parent)
    , ui(new Ui::UBXEventEditorWidget)
    , m_pNotify(NULL)
    //, m_pFlowControl(NULL)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    QString rccPath = CConfigs::getAppSubDirPath(SUBDIR_RES) + QDir::separator() + "ubxeventeditor.rcc";
    QResource::registerResource(rccPath);
    setDefaultStyle();

    initCtrls();

    connect(ui->pushButton_addlisten1, &QPushButton::clicked, this, &UBXEventEditorWidget::onAddEventBtnClicked);
    connect(ui->pushButton_addlisten2, &QPushButton::clicked, this, &UBXEventEditorWidget::onAddEventBtnClicked);
    connect(ui->tabWidget_eventeditor, &QTabWidget::tabBarClicked, this, &UBXEventEditorWidget::onTabBarClicked);

#if 0
    connect(ui->toolButton_eventeditor1, &QToolButton::clicked, this, &UBXEventEditorWidget::onEventClicked);
    connect(ui->toolButton_eventeditor2, &QToolButton::clicked, this, &UBXEventEditorWidget::onEventClicked);
#endif
}

UBXEventEditorWidget::~UBXEventEditorWidget()
{
    clearListenEvents();
    delete ui;
}

/**************************************************************************
* 函数名: Connect
* 功能: 连接通知地址和事件窗口
* 参数:
*    @[in ] pWnd: 窗口地址
*    @[in ] pNotify: 通知地址
* 返回值: 成功返回true，失败返回false
* 时间: 2016/11/07
* 作者: ZDJ
*/
bool UBXEventEditorWidget::Connect(QWidget* pWnd, IUBXEventEditorNotify *pNotify)
{
    m_pNotify = pNotify;

    return true;
}

/**************************************************************************
* 函数名: ReleaseConnection
* 功能: 释放通知地址和事件窗口
* 参数: void
* 返回值: void
* 时间: 2016/11/07
* 作者: ZDJ
*/
void UBXEventEditorWidget::ReleaseConnection()
{

}

/**************************************************************************
* 函数名: GetWidget
* 功能: 获取窗口
* 参数: void
* 返回值: 窗口地址
* 时间: 2016/11/07
* 作者: ZDJ
*/
QWidget* UBXEventEditorWidget::GetWidget()
{
    return this;
}

/**************************************************************************
* 函数名: GetData
* 功能: 获取数据
* 参数:
*    @[out] ppData: 数据指针地址
*    @[out] nLen: 数据长度
* 返回值: void
* 时间: 2016/11/05
* 作者: ZDJ
*/
void UBXEventEditorWidget::GetData(char** ppData, int& nLen)
{
    int nBufLen = sizeof(int) * 2;//sizeof(事件总长度) ＋ sizeof(事件数量)
    for(int i = 0; i < m_vecListenEvent.size(); i++)
    {
        LISTENEVENTDATA* pListenData = m_vecListenEvent[i];

        char* pFlowData = NULL;
        int nFlowDataLen = 0;
        if(m_pNotify)
            m_pNotify->getEventFlowData(pListenData->eListenEventType, &pFlowData, nFlowDataLen);
        pListenData->bytesFlowData.setRawData(pFlowData, nFlowDataLen);

        //sizeof(事件类型) + sizeof(事件数据长度)*2 + sizseof(触发数据长度) + sizeof(触发距离) + 流程图数据长度
        //nBufLen += sizeof(int) * 5 + pListenData->bytesFlowData.size();
        //sizeof(事件类型) + sizeof(事件数据长度)*3 + sizseof(触发数据长度)*2 + sizeof(触发距离) + 流程图数据长度
        nBufLen += sizeof(int) * 7 + pListenData->bytesFlowData.size();
    }

    nLen = nBufLen; //事件数据缓冲区总长度
    if(nLen > 0)
    {
        *ppData = new char[nLen];
        char* pDataCursor = *ppData;

        int nEventTotalLen = nLen; //事件数据总长度
        MEMCPY_OFFSETDEST(pDataCursor, &nEventTotalLen, sizeof(nEventTotalLen));

        int nEventCount = m_vecListenEvent.size(); //事件数量
        MEMCPY_OFFSETDEST(pDataCursor, &nEventCount, sizeof(nEventCount));

        for(int i = 0; i < m_vecListenEvent.size(); i++)
        {
            LISTENEVENTDATA* pListenData = m_vecListenEvent[i];

            int nEventType = pListenData->eListenEventType;
            //事件类型
            MEMCPY_OFFSETDEST(pDataCursor, &pListenData->eListenEventType, sizeof(pListenData->eListenEventType));

            //事件数据长度
//            int nEventSize = sizeof(int) * 3 + pListenData->bytesFlowData.size();
            int nEventSize = sizeof(int) * 5 + pListenData->bytesFlowData.size();

            //因为MFC老版本，对长度会有二次检测，为了兼容，所以对长度会写入两次
            for(int j = 0; j < 2; j++)
            {
                MEMCPY_OFFSETDEST(pDataCursor, &nEventSize, sizeof(nEventSize));
            }

            //触发数据长度
            int nTrigDataLen = sizeof(int) * 2;
            //因为MFC老版本，对长度会有二次检测，为了兼容，所以对长度会写入两次
            MEMCPY_OFFSETDEST(pDataCursor, &nTrigDataLen, sizeof(nTrigDataLen));
            MEMCPY_OFFSETDEST(pDataCursor, &nTrigDataLen, sizeof(nTrigDataLen));

            //触发距离
            MEMCPY_OFFSETDEST(pDataCursor, &pListenData->nListenDistance, sizeof(pListenData->nListenDistance));

            char* pFlowData = NULL;
            int nFlowDataLen = 0;
            if(m_pNotify)
                m_pNotify->getEventFlowData(pListenData->eListenEventType, &pFlowData, nFlowDataLen);

            pListenData->bytesFlowData.setRawData(pFlowData, nFlowDataLen);

            //流程图数据长度
            MEMCPY_OFFSETDEST(pDataCursor, &nFlowDataLen, sizeof(nFlowDataLen));

            if(pFlowData && nFlowDataLen > 0)
            {
                //流程图数据
//                MEMCPY_OFFSETDEST(pDataCursor, pListenData->bytesFlowData.data(), nFlowDataLen);
                MEMCPY_OFFSETDEST(pDataCursor, pFlowData, nFlowDataLen);
            }
        }
    }
}

/**************************************************************************
* 函数名: SetData
* 功能: 设置数据
* 参数:
*    @[in ] pData: 数据地址
*    @[in ] nLen: 数据长度
* 返回值: void
* 时间: 2016/11/05
* 作者: ZDJ
*/
void UBXEventEditorWidget::SetData(char* pData, int nLen)
{
    clearListenEvents();

    int nEventTotalLen = 0; //事件数据总长度
    MEMCPY_OFFSETSRC(&nEventTotalLen, pData, sizeof(nEventTotalLen));

    if(nEventTotalLen > 0)
    {
        int nEventCount = 0; //事件数量
        MEMCPY_OFFSETSRC(&nEventCount, pData, sizeof(nEventCount));

        for(int i = 0; i < nEventCount; i++)
        {
            int nEventType = 0; //事件类型
            MEMCPY_OFFSETSRC(&nEventType, pData, sizeof(nEventType));

            int nEventLen = 0; //事件数据长度
            MEMCPY_OFFSETSRC(&nEventLen, pData, sizeof(nEventLen));
            pData += sizeof(nEventLen);

            if(nEventLen > 0)
            {
                int nTrigDataLen = 0; //触发数据长度
                MEMCPY_OFFSETSRC(&nTrigDataLen, pData, sizeof(nTrigDataLen));
                pData += sizeof(nTrigDataLen);
                int nDistance = 0; //触发距离
                MEMCPY_OFFSETSRC(&nDistance, pData, sizeof(nDistance));

                QToolButton* pBtn = NULL;
                if(nEventType == eListenEventInfrared)
                {
                    pBtn = ui->toolButton_eventeditor1;
                }
                else if(nEventType == eListenEventSonar)
                {
                    pBtn = ui->toolButton_eventeditor2;
                }

                if(!pBtn)
                {
                    QMessageBox::warning(this, "", tr("event type is invalid"));
                    break;
                }

                QString sDistance;
                sDistance.sprintf("%d", nDistance);
                pBtn->setText(sDistance);
                pBtn->show();

//                int nFlowDataLen = *(int*)pData - sizeof(int); //??

                int nFlowDataLen = 0;
                MEMCPY_OFFSETSRC(&nFlowDataLen, pData, sizeof(nFlowDataLen));

                char* pFlowData = pData;

                if(m_pNotify)
                {
                    m_pNotify->setEventFlowData(nEventType, pFlowData, nFlowDataLen);
                    m_pNotify->enableFlowChart(nEventType);
                }

                LISTENEVENTDATA* pNewListenData = new LISTENEVENTDATA((emListenEventType)nEventType, nDistance);
                pNewListenData->bytesFlowData.setRawData(pFlowData, nFlowDataLen);
                m_vecListenEvent.push_back(pNewListenData); //缓存数据

                pData += nFlowDataLen;
            }
        }
    }
}

/**************************************************************************
* 函数名: initCtrls
* 功能: 初始化控件
* 参数: void
* 返回值: void
* 时间: 2016/11/05
* 作者: ZDJ
*/
void UBXEventEditorWidget::initCtrls()
{
    ui->tabWidget_eventeditor->setTabText(eListenEventInfrared, tr("Infrared Event"));
    ui->tabWidget_eventeditor->setTabText(eListenEventSonar, tr("Sonar Event"));
    ui->tabWidget_eventeditor->setCurrentIndex(eListenEventInfrared);

    ui->comboBox_distance1->addItem("200cm", 200);
    ui->comboBox_distance1->addItem("600cm", 600);
    ui->comboBox_distance1->addItem("1000cm", 1000);

    ui->comboBox_distance2->addItem("200cm", 200);
    ui->comboBox_distance2->addItem("600cm", 600);
    ui->comboBox_distance2->addItem("1000cm", 1000);

    ui->toolButton_eventeditor1->hide();
    ui->toolButton_eventeditor2->hide();
}

/**************************************************************************
* 函数名: onAddEventBtnClicked
* 功能: 添加事件按钮点击事件槽
* 参数: void
* 返回值: void
* 时间: 2016/11/05
* 作者: ZDJ
*/
void UBXEventEditorWidget::onAddEventBtnClicked()
{
    emListenEventType eListenType = (emListenEventType)ui->tabWidget_eventeditor->currentIndex();
    if(existEvent(eListenType))
    {
        QMessageBox::warning(this, "", tr("event type existing"));
        return;
    }

    QComboBox* pCombo = NULL;
    QToolButton* pBtn = NULL;

    if(eListenType == eListenEventInfrared)
    {
        pCombo = ui->comboBox_distance1;
        pBtn = ui->toolButton_eventeditor1;
    }
    else if(eListenType == eListenEventSonar)
    {
        pCombo = ui->comboBox_distance2;
        pBtn = ui->toolButton_eventeditor2;
    }

    if(!pCombo || !pBtn)
    {
        QMessageBox::warning(this, "", tr("page index is null"));
        return;
    }

    int nDistance = pCombo->currentData().toInt();
    m_vecListenEvent.push_back(new LISTENEVENTDATA(eListenType, nDistance)); //缓存数据

    //添加控件
    QString sDistance;
    sDistance.sprintf("%d", nDistance);
    pBtn->setText(sDistance);

    pBtn->show();
    m_pNotify->enableFlowChart(eListenType);
}

/**************************************************************************
* 函数名: clearListenEvents
* 功能: 清空监听事件列表
* 参数: void
* 返回值: void
* 时间: 2016/11/05
* 作者: ZDJ
*/
void UBXEventEditorWidget::clearListenEvents()
{
    VecListenEventType::iterator itor = m_vecListenEvent.begin();
    while(m_vecListenEvent.size() > 0)
    {        
        LISTENEVENTDATA* pEvent = *itor;
        itor = m_vecListenEvent.erase(itor);
        SAFE_DELETE(pEvent);
    }
}

/**************************************************************************
* 函数名: existEvent
* 功能: 是否存在指定类型的事件
* 参数:
*    @[in ] eEventType: 事件类型
* 返回值: 成功返回true，失败返回false
* 时间: 2016/11/05
* 作者: ZDJ
*/
bool UBXEventEditorWidget::existEvent(emListenEventType eEventType)
{
    return findEventData(eEventType) != NULL;
}

/**************************************************************************
* 函数名: findEventData
* 功能: 查找指定类型的事件
* 参数:
*    @[in ] eEventType: 事件类型
* 返回值: 成功返回事件地址，失败返回NULL
* 时间: 2016/11/05
* 作者: ZDJ
*/
LISTENEVENTDATA* UBXEventEditorWidget::findEventData(emListenEventType eEventType)
{
    LISTENEVENTDATA* pRet = NULL;
    VecListenEventType::iterator itor = m_vecListenEvent.begin();
    for(; itor != m_vecListenEvent.end(); itor++)
    {
        LISTENEVENTDATA* pEvent = *itor;
        if(pEvent->eListenEventType == eEventType)
        {
            pRet = pEvent;
            break;
        }
    }
    return pRet;
}

/**************************************************************************
* 函数名: onTabBarClicked
* 功能: 标签页切换事件槽
* 参数:
*    @[in ] nIndex: tab页索引
* 返回值: void
* 时间: 2016/11/05
* 作者: ZDJ
*/
void UBXEventEditorWidget::onTabBarClicked(int nIndex)
{
//    emListenEventType eListenType = (emListenEventType)nIndex;
//    QToolButton* pBtn = NULL;

//    if(eListenType == eListenEventInfrared)
//    {
//        pBtn = ui->toolButton_eventeditor1;
//    }
//    else if(eListenType == eListenEventSonar)
//    {
//        pBtn = ui->toolButton_eventeditor2;
//    }

//    if(!pBtn)
//    {
//        QMessageBox::warning(this, "", tr("page index is null"));
//        return;
//    }

//    LISTENEVENTDATA* pEventData = findEventData((emListenEventType)nIndex);
//    if(pEventData)
//    {
//        QString sTemp;
//        sTemp.sprintf("%d", pEventData->nListenDistance);
//        pBtn->setText(sTemp);
//    }
//    else
//    {
//        pBtn->setText(NULLSTR);
//    }
    if(m_pNotify)
        m_pNotify->showEventPage(nIndex);
}

/**************************************************************************
* 函数名: setDefaultStyle
* 功能: 设置默认界面
* 参数: void
* 返回值: void
* 时间: 2016/11/08
* 作者: ZDJ
*/
void UBXEventEditorWidget::setDefaultStyle()
{
    QFile styleSheet(":/res/qss/ubxeventeditor.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        setStyleSheet(styleSheet.readAll());
        styleSheet.close();
    }
}

void UBXEventEditorWidget::paintEvent(QPaintEvent* pEvent)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

#if 0
/**************************************************************************
* 函数名: onEventClicked
* 功能: 事件点击槽
* 参数: void
* 返回值: void
* 时间: 2016/11/10
* 作者: ZDJ
*/
void UBXEventEditorWidget::onEventClicked()
{
    emListenEventType eEventType = eListenEventInfrared;
    QObject* pSender = sender();
    if(pSender == ui->toolButton_eventeditor1)
    {
        qDebug() << "click btn1";
        eEventType = eListenEventInfrared;
    }
    else if(pSender == ui->toolButton_eventeditor2)
    {
        qDebug() << "click btn2";
        eEventType = eListenEventSonar;
    }

    LISTENEVENTDATA* pListenData = findEventData(eEventType);
    if(pListenData && m_pNotify)
        m_pNotify->setFlowChartData(pListenData->bytesFlowData.data(), pListenData->bytesFlowData.size());
}

#endif

////设置流程图
//void UBXEventEditorWidget::setFlowControl(IUBXFlowControl* pFlowControl)
//{
//    m_pFlowControl = pFlowControl;
//}
