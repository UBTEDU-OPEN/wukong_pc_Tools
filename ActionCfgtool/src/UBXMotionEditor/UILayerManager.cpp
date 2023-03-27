/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称:UILayerManager.cpp
* 创建时间: 2017/1/16
* 文件标识:
* 文件摘要:编辑层管理
*
* 当前版本：1.0.0.0
* 作    者：Joker
* 完成时间：2017/1/16
* 版本摘要：
*/
#include <QFileDialog>
#include <QResource>
#include <QFile>

#include "UILayerManager.h"
#include "ui_UILayerManager.h"
#include "stable.h"
#include "UILayerManager.h"
#include "LayerUI/MotionLayer.h"
// #include "UIEyesLayer.h"
#include "LayerUI/UIMainLayerLayout.h"
#include "LayerUI/MotionDataPlay.h"
#include "LayerUI/EarLedDataPlay.h"
#include "LayerUI/EyeLedDataPlay.h"
#include "LayerUI/MusicLayer.h"
#include "LayerUI/ExpressionLayer.h"
#include "LayerUI/IGroupDataPlay.h"
#include "LayerUI/TimeLayer.h"
#include "LayerUI/EarLedLayer.h"
#include "LayerUI/EyeLedLayer.h"
#include "LayerUI/UILayerCommonContainer.h"
#include "timesettingdialog.h"
#include "LayerUI/LayerControlWidget.h"
#include "IUBXSocketSession.h"
//#include "util.h"
#include "ubxdatabase.h"
#include "baseutil.h"

bool CreateUILayerManager(IUILayerManager** mManager)
{
    CUILayerManager* pProxy = new CUILayerManager();
    if (pProxy)
    {
        *mManager = pProxy;

        return true;
    }

    return false;
}


CUILayerManager::CUILayerManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayerManagerWidget),
    m_AddLayerDlg(this)
    , m_pTreeView(NULL)
    , m_pSocketSession(NULL)
{
    m_pPixmapMem = NULL;
    m_pPixmapGroup = NULL;
    m_pPixmapWave = NULL;
    m_pPixmapBackGround = NULL;

    //    m_CellCount = 10;
    //    m_lineThick = 1;
    //    m_nCellWidth = 99;
    m_scaleValue = 100;
    m_nCurCol = 1;
    m_IsDataPlaying = false;
    m_IsMusicPlaying = false;
    //	m_pChooseDlg = NULL;
    m_bModified = false;
    //    m_bMusicModified = false;
    m_nDebugType = SINGLE_LAYER_DEBUG;
    m_pGroupDataPlay = NULL;
    m_pILayerCommonContainer = NULL;

    m_ppManagerPlay = NULL;
    m_pMotionPlay = NULL;
    m_pEarLedPlay = NULL;
    m_pEyeLedPlay = NULL;
    m_pMusicPlay  = NULL;

    m_pHandPlay     = NULL;
    m_pLegPlay      = NULL;
    m_pHeadPlay     = NULL;

    m_pExpressionPlay = NULL;

    m_bPausePlay = false;
    m_bIsTotalPlaying = false;
    m_bTotalPause = false;

    m_nLastType = ACTION_LAYER_UNKNOWN;

#ifdef Q_OS_WIN
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
}

CUILayerManager::~CUILayerManager()
{
    delete ui;
}

void CUILayerManager::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//void CUILayerManager::ResetHScroll()
//{
// 	CRect rc;
// 	GetClientRect(rc);
//
// 	GetScrollInfo( SB_HORZ, &m_hSi );
// 	m_hSi.cbSize = sizeof( SCROLLINFO );
// 	m_hSi.nMin = 0;
// 	m_hSi.nPage = 1;
// 	m_hSi.nMax = ;
// 	SetScrollInfo( SB_HORZ, &m_hSi );
//}

bool CUILayerManager::Connect(QWidget* pParent, IUILayerManager_Event* pEvent)
{
    //    QRect rect;
    if(pParent && pParent->layout())
    {
        pParent->layout()->addWidget(this);
    }
    else
    {
        setParent(pParent);
    }
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    ui->setupUi(this);

    m_pILayerCommonContainer = new CUILayerCommonContainer(this);
    ui->horizontalLayout_LayerCommon->addWidget(m_pILayerCommonContainer);//这里已经相当于创建了时间层
    m_pILayerCommonWidget = m_pILayerCommonContainer->GetCUILayerCommonWidget();
    m_pILayerCommonWidget->Connect(this, pEvent);
    m_pEvent = pEvent;
    initWidget();
    return true;
}

/**************************************************************************
* 函数名：initWidget
* 功能: 初始化窗体
* 参数: 无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerManager::initWidget()
{
    m_bMotionPlaying = false;
    m_bEarLedPlaying = false;
    m_bEyeLedPlaying = false;
    m_bMusicPlaying = false;
    m_bHandPlaying  = false;
    m_bLegPlaying   = false;
    m_bHeadPlaying  = false;
    m_bExpressionPlaying = false;
    m_bMusicPause = false;
    connect(this, &CUILayerManager::SigPositionChanged, (CUILayerCommonWidget*)m_pILayerCommonWidget->GetWidget(), &CUILayerCommonWidget::OnPositionChanged);

    //    connect(this, &CUILayerManager::SigMotionPlayStop, this, &CUILayerManager::OnMotionPlayStop);
    //    connect(m_pILayerCommonWidget, &CUILa::SigFinishMusicPlaying, this, &CUILayerManager::OnFinishMusicPlaying);
    //    connect(this, &CUILayerManager::SigSetControlState, this, &CUILayerManager::OnSetControlState);
    //    connect(m_pILayerCommonWidget, &MP3Widget::SigNotifyReleasedMusic, this, &CUILayerManager::OnReleaseMusic);
    //    connect(m_pMP3Widget, &MP3Widget::SigNotifyLoadedMusic, this, &CUILayerManager::OnLoadedMusic);

    //    ui->verticalLayout->setAlignment(ui->btnAddMusic, Qt::AlignHCenter);
    //    ui->verticalLayout->setAlignment(ui->btnDeleteMusic, Qt::AlignHCenter);
    //    ui->pauseMP3->hide();
    //    ui->pauseMotion->hide();
    //    ui->pauseEarLed->hide();
    //    ui->pauseEyeLed->hide();

    //    connect(ui->pauseMP3, &QPushButton::clicked, this, &CUILayerManager::onPlayMP3Clicked);
    //    connect(ui->pauseMotion, &QPushButton::clicked, this, &CUILayerManager::onPlayMotionClicked);

    //    ui->btnDeleteMusic->setEnabled(false);
    //    ui->playMP3->setEnabled(false);
    //    ui->stopMP3->setEnabled(false);
    //    ui->playMotion->setEnabled(false);
    //    ui->stopMotion->setEnabled(false);

    //    ui->playEarLed->setEnabled(false);
    //    ui->stopEarLed->setEnabled(false);
    //    ui->playEyeLed->setEnabled(false);
    //    ui->stopEyeLed->setEnabled(false);

    //初始化的时候，所有层的控制窗体都是隐藏的
    //    ui->widgetMotionLayerCtrl->hide();
    //    ui->widgetMusicLayerCtrl->hide();
    //    ui->widgetEarLayerCtrl->hide();
    //    ui->widgetEyeLayerCtrl->hide();
    connect(ui->pBAddLayer, &QPushButton::clicked, this, &CUILayerManager::onAddLayerClicked);
    connect(ui->pbScaleSetting,&QPushButton::clicked, this, &CUILayerManager::onScaleSettingClicked);

    connect(ui->pbAllLayerPlay, &QPushButton::clicked, this, &CUILayerManager::slotAllLayerPlay);
    connect(ui->pbAllLayerPause, &QPushButton::clicked, this, &CUILayerManager::slotAllLayerPause);
    connect(ui->pBAllLayerStop, &QPushButton::clicked, this, &CUILayerManager::slotAllLayerStop);

    connect(this, &CUILayerManager::sigUnitedStop, this, &CUILayerManager::slotUnitedStop);

    ui->pbAllLayerPause->hide();

    if (m_pILayerCommonWidget)
    {
        m_pILayerCommonWidget->SetTimeScale(m_scaleValue);
    }
    //    ui->btnAddMusic->hide();
    //    ui->btnDeleteMusic->hide();

    //与DialgAddLayer建立连接
    //    connect(this, &CUILayerManager::SigInitList, &m_AddLayerDlg, &CDialogAddLayer::InitLayerList);

    m_bHasMotionData = false;
    setDefaultStyle();
}

void CUILayerManager::setDefaultStyle()
{
    QFile styleSheet(":/res/qss/layerdefault.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        setStyleSheet(styleSheet.readAll());
    }
}

/**************************************************************************
* 函数名:ReleaseConnection
* 功能:解除链接
* 参数:
* 返回值:
* 时间:2016/01/06 14：00
* 作者: 周志平
* 改动日志  现在解除链接不删除层，改为删除相应层数据
**************************************************************************/
void CUILayerManager::ReleaseConnection()
{
    if (m_ppManagerPlay)
    {
        m_ppManagerPlay->ReleaseConnection();
        m_ppManagerPlay = NULL;
    }

    if (m_pMotionPlay)
    {
        m_pMotionPlay->ReleaseConnection();
        m_pMotionPlay = NULL;
    }

    if (m_pEyeLedPlay)
    {
        m_pEyeLedPlay->ReleaseConnection();
        m_pEyeLedPlay = NULL;
    }

    if (m_pEarLedPlay)
    {
        m_pEarLedPlay->ReleaseConnection();
        m_pEarLedPlay = NULL;
    }

    if (m_pMusicPlay)
    {
        m_pMusicPlay->ReleaseConnection();
        m_pMusicPlay = NULL;
    }

    if (m_pHeadPlay)
    {
        m_pHeadPlay->ReleaseConnection();
        m_pHeadPlay = NULL;
    }

    if (m_pHandPlay)
    {
        m_pHandPlay->ReleaseConnection();
        m_pHandPlay = NULL;
    }

    if (m_pLegPlay)
    {
        m_pLegPlay->ReleaseConnection();
        m_pLegPlay = NULL;
    }

    if (m_pExpressionPlay)
    {
        m_pExpressionPlay->ReleaseConnection();
        m_pExpressionPlay = NULL;
    }

    if (m_pGroupDataPlay)
    {
        m_pGroupDataPlay->ReleaseConnection();
        m_pGroupDataPlay = NULL;
    }

    if (m_pILayerCommonWidget)
    {
        m_pILayerCommonWidget->ReleaseConnection();
    }

    m_pSocketSession = NULL;

    disconnect();
    DeleteAllLayer();
    delete this;
}

QWidget* CUILayerManager::GetWidget()
{
    return this;
}

void CUILayerManager::OnDraw(/*CDC* pDC*/)
{

}

//ILayerUI* CUILayerManager::GetLastLayer()
//{
////    return m_listActionLayer.takeLast();
//    return m_listActionLayer.last();
//}

//void CUILayerManager::SetScrollPos(int nPos)
//{
//    if (m_pEvent)
//    {
//        m_pEvent->LayerUIEvent_SetScrollBarPos(nPos);
//    }
//}
void CUILayerManager::ClearSelectState()
{
    for (int i=0; i<GetLayerCount(); i++)
    {
        ILayerUI* pLayUI = FindLayerByIndex(i);
        if (pLayUI)
        {
            pLayUI->ClearSelectState();
        }
    }
}
void CUILayerManager::ReleaseMusic()
{
    //	if (m_pChooseDlg)
    //	{
    //		m_pChooseDlg->RemoveSong();
    //	}
}

/**************************************************************************
* 函数名：LoadMusic
* 功能:加载Music
* 参数:
*    @[in ] strFilePath: 音乐的路径
* 返回值: bool 加载成功返回true, 否则返回false
* 时间: 2017/1/16
* 作者: Joker
*/
bool CUILayerManager::LoadMusic(const QString& strFilePath)
{
    bool bRet = false;

    if (m_pILayerCommonWidget)
    {
        bRet = m_pILayerCommonWidget->LoadMusic(strFilePath);
    }

    return bRet;
}

/**************************************************************************
* 函数名:AddAcionLayer
* 功能:根据类型增加动作层界面
* 参数:int nLayerID
* 返回值: bool 如果添加成功，返回真，否知返回假
* 时间:2015/12/25 11:25
* 作者: 周志平
* 改动日志  2015/12/25 11:25  把音乐层、时间层屏蔽，并自动connnect 动作层
**************************************************************************/
bool CUILayerManager::AddAcionLayer(int nLayerID)
{
    ILayerUI* pLayer = NULL;


    if (nLayerID == ACTION_LAYER_TIME)          // 时间层
    {
        pLayer = new CTimeLayer();
    }
    else if (nLayerID == ACTION_LAYER_MOTION)   // 全动作层
    {
        pLayer = new CMotionLayer();
    }
    else if (nLayerID == ACTION_LAYER_EYE)      // 眼睛层
    {
        pLayer = new CEyeLedLayer();
    }
    else if (nLayerID == ACTION_LAYER_EAR)      // 耳朵层
    {
        pLayer = new CEarLedLayer();
    }
    else if (nLayerID == ACTION_LAYER_MUSIC)    // 音乐层
    {
        pLayer = new CMusicLayer();
    }
    else if (nLayerID == ACTION_LAYER_HAND)     // 手部层
    {
        pLayer = new CHandMotionLayer();
    }
    else if (nLayerID == ACTION_LAYER_LEG)      // 腿部层
    {
        pLayer = new CLegMotionLayer();
    }
    else if (nLayerID == ACTION_LAYER_HEAD)     // 头部层
    {
        pLayer = new CHeadMotionLayer();
    } 
    else if (nLayerID == ACTION_LAYER_EXPRESSION)   // 表情层
    {
        pLayer = new CExpressionLayer();
    }
    else
    {
        return false;
    }

    if (pLayer == NULL)
        return false;

    pLayer->SetLayerManager(this);
    //    pLayer->SetLayerCommonWidget(m_pILayerCommonWidget);
    pLayer->Connect(this);//hels Conenct必须在SetLayerManager后面（Connect中会用到LayerManager）
    //    pLayer->Connect(GetQWidget());

    //同步添加UILayer层
    if (m_pILayerCommonWidget && nLayerID != ACTION_LAYER_TIME)
    {
        m_pILayerCommonWidget->AddUILayer((emActionLayerType)nLayerID);
    }
    return AddActionLayer(pLayer);
}

/**************************************************************************
* 函数名：AddLayerControlWidget
* 功能:根据Layer层类型添加Layer控制层窗体
* 参数:
*    @[in ] nType: 层类型
*    @[in ] strName: 层名称
* 返回值: bool
* 时间: 2017/1/16
* 作者: Joker
*/
bool CUILayerManager::AddLayerControlWidget(emActionLayerType nType, QString strName)
{
    CLayerControlWidget* pLayerCtrlWidget = new CLayerControlWidget(NULL, nType, strName);
    if (nType == ACTION_LAYER_MUSIC)
    {
        ui->vLayoutCtrlMusic->addWidget(pLayerCtrlWidget);
        //这里的分隔大小是不是写死的，应该根据层的位置变化，具体后面再修改
        ui->vSpacerMotion->changeSize(20, 40, QSizePolicy::Fixed, QSizePolicy::Fixed);
    }
    else
    {
        ui->vLayoutLayerControl->addWidget(pLayerCtrlWidget);
    }
    //建立连接
    connect(pLayerCtrlWidget, &CLayerControlWidget::SigDeleteLayerClicked, this,&CUILayerManager::onDeleteLayerClicked);
    connect(pLayerCtrlWidget, &CLayerControlWidget::SigPauseLayerClicked, this,&CUILayerManager::onPauseLayerClicked);
    connect(pLayerCtrlWidget, &CLayerControlWidget::SigPlayLayerClicked, this,&CUILayerManager::onPlayLayerClicked);
    connect(pLayerCtrlWidget, &CLayerControlWidget::SigStopLayerClicked, this,&CUILayerManager::onStopLayerClicked);
    connect(pLayerCtrlWidget, &CLayerControlWidget::sigLaberNameClicked, this, &CUILayerManager::onShowLayerClicked);

    connect(this, &CUILayerManager::SigStop, pLayerCtrlWidget, &CLayerControlWidget::OnStop);

    m_listLayerCtrlWidget.append(pLayerCtrlWidget);
    return true;
}

bool CUILayerManager::AddActionLayer(ILayerUI* pLayer)
{
    ILayerUI* itemFound = NULL; /*new IActionLayer();*/

    if (pLayer == NULL)
        return false;

    for (int i=0; i<m_listActionLayer.count(); i++)
    {
        itemFound = m_listActionLayer.at(i);
        if (itemFound && itemFound->GetType() == pLayer->GetType())
        {
            return false;
        }
    }

    if (pLayer->GetType() != ACTION_LAYER_TIME)
    {
        AddLayerControlWidget(pLayer->GetType(), pLayer->GetLayerName());
    }

    EnableLayerControlWidget(pLayer->GetType(), true);

    //leo add 20170228
    //暂时放这里处理 
    pLayer->SetTreeView(m_pTreeView);
    m_listActionLayer.append(pLayer);

    return true;
}

//void CUILayerManager::AdjustLayerPosition()
//{
//    ILayerUI* itemFound = NULL;
//    int x = 0;
//    int y = 0;
//    for (int i=2; i<m_listActionLayer.count(); i++)
//    {
//        itemFound = m_listActionLayer.at(i);
//        itemFound->GetPosition(x, y);
//        itemFound->SetPosition(x, i*(itemFound->GetCellHeight()));
//    }

////    Invalidate(false);
//}

/**************************************************************************
* 函数名:DeleteActionLayer
* 功能:根据ID删除响应层
* 参数: int nTypeID   层ID
* 返回值: bool 当删除成功时，返回真，否则返回假
* 时间:
* 作者: 周志平
* 改动日志
**************************************************************************/
bool CUILayerManager::DeleteActionLayer(int nTypeID)
{
    ILayerUI* itemFound = NULL;
    CLayerControlWidget* pCtrlItemFound;
    for (int i=0; i<m_listLayerCtrlWidget.count(); i++)
    {
        pCtrlItemFound = m_listLayerCtrlWidget.at(i);
        if (pCtrlItemFound && pCtrlItemFound->GetLayerType() == nTypeID)
        {
            SAFE_DELETE(pCtrlItemFound);
            m_listLayerCtrlWidget.removeAt(i);
        }
    }

    for (int i=0; i<m_listActionLayer.count(); i++)
    {
        itemFound = m_listActionLayer.at(i);
        if (itemFound && itemFound->GetType() == nTypeID)
        {
            if (m_pEvent)
            {
                m_pEvent->LayerUIEvent_OnDeletOptionHandler(itemFound->GetOptionHandler());
            }
            itemFound->ReleaseConnection();
            SAFE_DELETE(itemFound);
            //            delete[] itemFound;
            m_listActionLayer.removeAt(i);
            //            AdjustLayerPosition();
            if (m_pILayerCommonWidget)
            {
                m_pILayerCommonWidget->DeleteUILayerByID(nTypeID);
            }
            EnableLayerControlWidget((emActionLayerType)nTypeID, false);

            // Delete by jianjie 2017/10/20 全动作层可能被删除
            // 设置默认的属性处理对象
            //if (m_pEvent)
            //{
            //    ILayerUI* pLayer = FindLayer(ACTION_LAYER_TIME);
            //    if (pLayer)
            //    {
            //        m_pEvent->LayerUIEvent_SetLayOptionHandler(pLayer->GetOptionHandler());
            //        g_IsUpdateUIByData = true;
            //        m_pEvent->LayerUIEvent_SetActionBlockDataManager(pLayer->GetBlockDataManager(), 0);
            //        //这里更新LayerUI
            //        UpdateLayerWidget(pLayer->GetBlockDataManager(), -1, -1, ACTION_LAYER_TIME);
            //        g_IsUpdateUIByData = false;
            //    }
            //}
            return true;
        }
    }
    return false;
}

ILayerUI* CUILayerManager::FindLayer(int nTypeID)
{
    ILayerUI* itemFound = NULL;

    for (int i=0; i<m_listActionLayer.count(); i++)
    {
        itemFound = m_listActionLayer.at(i);
        if (itemFound && itemFound->GetType() == nTypeID)
        {
            return itemFound;
        }
    }
    return NULL;
}
/**************************************************************************
* 函数名:FindLayerByIndex
* 功能:根据层索引查找层
* 参数:int nIndex   索引值
* 返回值: ILayerUI* 返回获取的层
* 时间:2015/12/22 10:57
* 作者: 周志平
* 改动日志：2015/12/22 10:57 修改了查找方法
**************************************************************************/
ILayerUI* CUILayerManager::FindLayerByIndex(int nIndex)
{
    ILayerUI* itemFound = NULL;
    //    int  pos = m_listActionLayer.indexOf(m_listActionLayer.at(nIndex));
    //    if (pos>0)
    //    {
    //        itemFound = m_listActionLayer.at(pos);
    //    }
    itemFound = m_listActionLayer.at(nIndex);
    return itemFound;
}
/**************************************************************************
* 函数名: DeleteAllLayer
* 功能:删除所有动作层
* 参数:
* 返回值:
* 时间:2015/12/25 18:13
* 作者: 周志平
**************************************************************************/
void CUILayerManager::DeleteAllLayer()
{
    //删除层
    for (int i=0; i<m_listActionLayer.count(); i++)
    {
        ILayerUI* pcui = m_listActionLayer.at(i);
        if (pcui)
        {
            if (m_pEvent)
            {
                m_pEvent->LayerUIEvent_OnDeletOptionHandler(pcui->GetOptionHandler());
            }
            pcui->ReleaseConnection();
            delete pcui;
        }
    }
    m_listActionLayer.clear();
    //删除层的控制窗体
    for (int i=0; i<m_listLayerCtrlWidget.count(); i++)
    {
        CLayerControlWidget* pCtrlWidget = m_listLayerCtrlWidget.at(i);
        if (pCtrlWidget)
        {
            SAFE_DELETE(pCtrlWidget)
        }
    }
    m_listLayerCtrlWidget.clear();

    //删除层的UI
    if (m_pILayerCommonWidget)
    {
        m_pILayerCommonWidget->DeleteAllUILayer();
    }
}

int CUILayerManager::GetLayerCount()
{
    return m_listActionLayer.count();
}

/**************************************************************************
* 函数名：GetLayerTypeByIndex
* 功能:根据Layer序列获取层类型
* 参数:
*    @[in ] nIndex: 层序列
* 返回值: 层类型
* 时间: 2017/1/16
* 作者: Joker
*/
emActionLayerType CUILayerManager::GetLayerTypeByIndex(int nIndex)
{
    ILayerUI*  pLayerUI = FindLayerByIndex(nIndex);
    if (pLayerUI)
    {
        return (emActionLayerType)pLayerUI->GetType();
    }
    return ACTION_LAYER_UNKNOWN;
}

// void CUILayerManager::SetMusicSource(QString strMusicFile)
// {
//
// }
//void CUILayerManager::AdjustData()
//{
//    ILayerUI *pMotionLayer = FindLayer(ACTION_LAYER_MOTION);
//    ILayerUI *pMusicLayer = FindLayer(ACTION_LAYER_MUSIC);
//    if (pMotionLayer)
//    {
//        pMotionLayer->AdjustData();
//    }
//    if (pMusicLayer)
//    {
//        pMusicLayer->AdjustData();
//    }

//}

/**************************************************************************
* 函数名: SetData
* 功能:
* 参数:
* 返回值:
* 时间: 2015/12/30 14:26
* 作者: 周志平
* 改动日志    2016/1/6号  设置数据时，暂时不做DeleteAllLayer();改为DeleteLayerData()
             2016年12月14日，去掉版本等信息
**************************************************************************/
bool CUILayerManager::SetData(char* pData, int nLen)
{
    DeleteAllLayer();
    if (pData == NULL || nLen == 0)
    {
        //默认的层里面，必须有时间层和动作层，以满足最基本的编动作需要
        AddAcionLayer(ACTION_LAYER_TIME);
        AddAcionLayer(ACTION_LAYER_MOTION);

        // Mini动作分层
        //AddAcionLayer(ACTION_LAYER_HEAD);
        //AddAcionLayer(ACTION_LAYER_HAND);
        //AddAcionLayer(ACTION_LAYER_LEG);
    }
    else
    {
        ParseRawData(pData, nLen);
    }


    // 设置默认的属性处理对象

    if (m_pEvent)
    {
        // Modify by jianjie 2017/11/29 显示某一动作类型数据
        for (int i = ACTION_LAYER_MOTION; i <= ACTION_LAYER_LEG; ++i)
        {
            ILayerUI* pLayer = FindLayer(i);
            if (pLayer)
            {
                m_pEvent->LayerUIEvent_SetLayOptionHandler(pLayer->GetOptionHandler());
                g_IsUpdateUIByData = true;
                m_pEvent->LayerUIEvent_SetActionBlockDataManager(pLayer->GetBlockDataManager(), 0);
                //这里更新LayerUI
                UpdateLayerWidget(pLayer->GetBlockDataManager(), -1, -1, ACTION_LAYER_MOTION);
                g_IsUpdateUIByData = false;

                break;
            }
        }
    }
    return true;
}


/**************************************************************************
* 函数名:GetData
* 功能:获取动作数据
* 参数:char** ppData    数据
* 参数:int &nLen        数据长度
* 返回值:
* 时间:2015/12/22 10:48
* 作者: 周志平
改动日志     2016年12月14日，去掉版本等信息
**************************************************************************/
bool CUILayerManager::GetData(char** ppData, int &nLen)
{
    // 总的数据长度(int) + 总的页数(int) [单页的数据长度(int) + 单页的数据]
    vector<char> DataSave;

    // 保存总的数据长度，先占位，后面再改为实际的长度
    AddVectorData(&DataSave, (char*)&nLen, sizeof(nLen));
    // 保存总的层数
    int nLayerCount =GetLayerCount();   // 直接调用 layerManager 函数获取,去掉时间层
    AddVectorData(&DataSave, (char*)&nLayerCount, sizeof(nLayerCount));

    for (int i=0; i<nLayerCount; i++)
    {
        char* pRawData = NULL;
        int nRawDataLen = 0;

        ILayerUI* ppLayer = (ILayerUI*)FindLayerByIndex(i);  //

        if (ppLayer == NULL)
            continue;

        // 保存TYPE
        int nType = ppLayer->GetType();
        AddVectorData(&DataSave, (char*)&nType, sizeof(nType));

        ppLayer->GetData(&pRawData, nRawDataLen);
        // 长度信息
        AddVectorData(&DataSave, (char*)&nRawDataLen, sizeof(nRawDataLen));
        if (pRawData && nRawDataLen)
        {
            // 数据
            AddVectorData(&DataSave, pRawData, nRawDataLen);
            delete [] pRawData;
        }
    }

    nLen = DataSave.size();
    ModifyVertorData(DataSave, 0, (char*)&nLen, sizeof(nLen));
    if (nLen)
    {
        *ppData = new char[nLen];
        CopyVertorToPointer(DataSave, *ppData);
    }

    return true;
}


//void CUILayerManager::SetHScrollPos(int nCol)
//{
//    //RETURN_SM
//    PlayEvent_OnSetPos(nCol);
//    return;
////    CTimeLayer* pTimeLayer = (CTimeLayer*)FindLayer(ACTION_LAYER_TIME);
//    CMotionLayer *pMotionLayer = (CMotionLayer*)FindLayer(ACTION_LAYER_MOTION);
//    if (pTimeLayer/* && GetSafeHwnd()*/)
//    {
//        pTimeLayer->SetCurrentSelectCol(nCol);
//        if (m_pEvent)
//        {
//            m_pEvent->LayerUIEvent_SelectChanger(nCol, pTimeLayer->GetScaleValue());
//        }
//    }
//    //检查是否需要自动滚页
//    if (/*GetSafeHwnd() &&*/ pMotionLayer)
//    {
//        int nScreenCell = pTimeLayer->GetScreenCellCount();
//        int nScreenCellCount = nScreenCell - pTimeLayer->GetCellStartIndex();
//        if (nScreenCellCount == 0)
//        {
//            return;
//        }
//        int nGroupIndex = 0;
//        int nActionIndex = 0;
//        int nOffsetTime = 0;
//        pMotionLayer->GetSelectGroupAction(nCol,nGroupIndex,nActionIndex,nOffsetTime);
//        if (nGroupIndex == -1 || nActionIndex == -1)
//        {
//            return;
//        }
//        int nCell = (nCol / (m_lineThick + m_nCellWidth));



//        // 如果光标位置显示不在界面上，自动跳到光标位置处
//        if (pMotionLayer->GetCellStartIndex() <= nCell && nCell <= nScreenCell)
//        {

//        }
//        else
//        {
//            SetHScrollOffset(nCell);   //页数
//            SetScrollPos(nCell);
//        }
//        if ((nScreenCell - nCell) / nScreenCellCount > 1)
//        {
//            // 			SetHScrollOffset(nCol / (m_lineThick + m_nCellWidth));
//            // 			/*UpdateScrollBar();*/
//            // 			SetScrollPos(nCell);
//        }
//        if (nCell == nScreenCell)
//        {
//            SetHScrollOffset(nCol / (m_lineThick + m_nCellWidth));
//            /*UpdateScrollBar();*/
//            SetScrollPos(nCell);
//        }
//    }
//}
//void CUILayerManager::SetHScrollOffset(uint offset)
//{
//	for (int i=0; i<m_listActionLayer.GetCount(); i++)
//	{
//		ILayerUI* pcui = m_listActionLayer.GetAt(m_listActionLayer.FindIndex(i));
//		if (pcui)
//		{
//			pcui->SetHScrollOffset(offset);
//			pcui->SetCellStartIndex(offset);
//		}
//	}

//	if (GetSafeHwnd())
//	{
//        Invalidate(false);
//	}
//}

//void CUILayerManager::OnHScroll(uint nSBCode, uint nPos, CScrollBar* pScrollBar)
//{
//	switch(nSBCode)
//	{
//	case SB_LINEDOWN:
//		if( m_hSi.nPos < m_hSi.nMax )
//			m_hSi.nPos++;
//		break;
//	case SB_LINEUP:
//		if( m_hSi.nPos > m_hSi.nMin )
//			m_hSi.nPos--;
//		break;
//	case SB_PAGEDOWN:
//		m_hSi.nPos += m_hSi.nPage;
//		break;
//	case SB_PAGEUP:
//		m_hSi.nPos -= m_hSi.nPage;
//		break;
//	case SB_THUMBTRACK:
//		m_hSi.nPos = nPos;
//		break;
//	case SB_THUMBPOSITION:
//		m_hSi.nPos = nPos;
//		break;
//	}

//	SetHScrollOffset(m_hSi.nPos);
//    SetScrollInfo(SB_HORZ, &m_hSi, false);
//    Invalidate(false);

//	__super::OnHScroll(nSBCode, nPos, pScrollBar);
//}

//int CUILayerManager::GetColIndexByPosX(int x)
//{
//    if (m_listActionLayer.count() == 0)
//        return -1;

//    // 因为所有层都是同个宽高，所以只取第一层来计算
//    ILayerUI* pLayerUI = FindLayerByIndex(0);
//    if(pLayerUI == NULL)
//        return -1;

//    int xStart, yStart;

//    // 先获取起始位置
//    pLayerUI->GetPosition(xStart, yStart);

//    int xTmp = xStart+pLayerUI->GetLineThick();

//    int nCurrPosition = pLayerUI->GetCellStartIndex()*100 + x;
//    return nCurrPosition;
//// 	for (int i=pLayerUI->GetCellStartIndex(); i<=pLayerUI->GetCellCount(); i++)
//// 	{
//// 		if (x>=xTmp && x<xTmp+pLayerUI->GetCellWidth()+pLayerUI->GetLineThick())
//// 			return i;
////
//// 		xTmp += (pLayerUI->GetCellWidth()+pLayerUI->GetLineThick());
//// 	}
////
//// 	return -1;
//}

// 根据Y坐标获取行号
//int CUILayerManager::GetRowIndexByPosY(int y)
//{
//    if (m_listActionLayer.count() == 0)
//        return -1;

//    for (int i=0; i<m_listActionLayer.count(); i++)
//    {
//        ILayerUI* pcui = m_listActionLayer.at(i);
//        if (pcui == NULL)
//            continue;

//        int xStart, yStart;
//        // 先获取起始位置
//        pcui->GetPosition(xStart, yStart);
//        if (y >= yStart && y<yStart+pcui->GetCellHeight())
//            return i;
//    }

//    return -1;
//}

/**************************************************************************
* 函数名: DeleteLayerData
* 功能:
* 参数:
* 返回值:
* 时间:2015/12/30 14:57
* 作者: 周志平
* 改动日志
**************************************************************************/
void CUILayerManager::DeleteLayerData()
{
    for (int i=0; i<m_listActionLayer.count(); i++)
    {
        ILayerUI* pcui = m_listActionLayer.at(i);
        if (pcui)
        {
            pcui->ReleaseConnection();
        }
    }
}

/**************************************************************************
* 函数名：EnableLayerControlWidget
* 功能: layer控制层窗体使能
* 参数:
*    @[in ] nType: 层类型
*    @[in ] bShow: 是否显示，true是显示，false是隐藏
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerManager::EnableLayerControlWidget(emActionLayerType nType , bool bShow)
{
    //    QWidget* pWidget = NULL;
    //    switch (nType)
    //    {
    //    case ACTION_LAYER_MOTION:
    //        pWidget = ui->widgetMotionLayerCtrl;
    //        break;
    //    case ACTION_LAYER_MUSIC:
    //         pWidget = ui->widgetMusicLayerCtrl;
    //        break;
    //    case ACTION_LAYER_EAR:
    //         pWidget = ui->widgetEarLayerCtrl;
    //        break;
    //    case ACTION_LAYER_EYE:
    //         pWidget = ui->widgetEyeLayerCtrl;
    //        break;
    //    default:
    //        break;
    //    }
    //    if (pWidget)
    //    {
    //        if (bShow)
    //        {
    //            pWidget->show();
    //        }
    //        else
    //        {
    //            pWidget->hide();
    //        }
    //    }
}

/**************************************************************************
* 函数名：UpdateLayerWidget
* 功能: 通知LayerUI更新界面
* 参数:
*    @[in ] pGroupManager: 数据
*    @[in ] nCurrentGroupID: 当前组ID
*    @[in ] nCurrentActionID: 当前块ID
*    @[in ] nLayerType: 层类型
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerManager::UpdateLayerWidget(CActionBlockDataManager* pDataManager, int nCurrentGroupID,int nCurrentActionID, emActionLayerType nLayerType)
{
    //    ILayerUI* pLayer = FindLayer(ACTION_LAYER_MUSIC);
    //    if(NULL == pLayer)
    //        return;
    //    ILayerOptionHandler* pOptionHandler = pLayer->GetOptionHandler();
    //    if(NULL != pOptionHandler)
    //    {
    //        pOptionHandler->SetActionBlockDataManager(pDataManager, nCurrentGroupID, nCurrentActionID);
    //    }
    if (m_pILayerCommonWidget)
    {
        m_pILayerCommonWidget->UpdateLayerWidget(pDataManager, nCurrentGroupID, nCurrentActionID, nLayerType);
    }
}

/**************************************************************************
* 函数名：UpdateLayerAction
* 功能: 根据组ID和总时间更新LayerUI
* 参数:
*    @[in ] nGroupID: 组ID
*    @[in ] nStartTime: 组开始时间
*    @[in ] nType: 层的类型
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerManager::UpdateLayerAction(int nGroupID, int nStartTime, emActionLayerType nType)
{
    //    ILayerUI* pLayer = FindLayer(ACTION_LAYER_MUSIC);
    //    if(NULL == pLayer)
    //        return;
    //    ILayerOptionHandler* pOptionHandler = pLayer->GetOptionHandler();
    //    if(NULL != pOptionHandler)
    //    {
    //        pOptionHandler->UpdateLayerAction(nGroupID, fTime);
    //    }
    if (m_pILayerCommonWidget)
    {
        m_pILayerCommonWidget->UpdateLayerAction(nGroupID, nStartTime, nType);
    }
}

void CUILayerManager::FinishMusicPlaying()
{
    m_IsMusicPlaying = false;
    if(!m_IsDataPlaying)
    {
        //hels 通知Motion模块刷新按钮
        ILayerUI* pLayer = FindLayer(ACTION_LAYER_MOTION);
        if(NULL == pLayer)
            return;
        ILayerOptionHandler* pOptionHandler = pLayer->GetOptionHandler();
        if(NULL != pOptionHandler && !m_IsDataPlaying && !m_IsMusicPlaying)
        {
            pOptionHandler->MotionPlayStop();
        }
    }
}

void CUILayerManager::HasMotionData(bool hasData, int nLayerType)
{
    ILayerUI* pLayer = FindLayer(ACTION_LAYER_MUSIC);
    if(NULL == pLayer)
        return;
    ILayerOptionHandler* pOptionHandler = pLayer->GetOptionHandler();
    if(NULL != pOptionHandler)
    {
        pOptionHandler->HasMotionData(hasData, nLayerType);
    }
}

/**************************************************************************
* 函数名：OnLayerLButtonDown
* 功能:Layer层鼠标左键单击事件
* 参数:
*    @[in ] nLayerType: 层类型
*    @[in ] nCol: 第几列
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerManager::OnLayerLButtonDown(emActionLayerType nLayerType, int nCol)
{
    m_nCurCol = nCol;
    if (nLayerType == ACTION_LAYER_UNKNOWN || nLayerType == ACTION_LAYER_TIME)
        return;
    ClearSelectState();
    for(int i = 0; i < GetLayerCount(); i++ )
    {
        // Modify by jianjie 2017/10/13 动作分层显示同一时刻的所有动作帧
        ILayerUI* pLayer = FindLayerByIndex(i);
        if (pLayer && pLayer->GetType() != nLayerType && pLayer->GetType() != ACTION_LAYER_TIME)
        {
            if (m_pEvent)
            {
                m_pEvent->LayerUIEvent_SetLayOptionHandler(pLayer->GetOptionHandler());
            }
            QPoint tempPoint(0,0);
            pLayer->OnLButtonDown(m_nCurCol, tempPoint, 0);
        }
    }

    ILayerUI* pLayer = FindLayer((int)nLayerType);
    if (pLayer)
    {
        if (m_pEvent)
        {
            m_pEvent->LayerUIEvent_SetLayOptionHandler(pLayer->GetOptionHandler());
        }
        QPoint tempPoint(0,0);
        pLayer->OnLButtonDown(m_nCurCol, tempPoint, 0);
    }

    //重置选中的列
    SetCurrentSelectCol(nCol);
}

/**************************************************************************
* 函数名：OnLayerMouseMove
* 功能:Layer层鼠标移动事件(调整组数据位置)
* 参数:
*    @[in ] nLayerType: 层类型
*    @[in ] nCol: 第几列
* 返回值: 无
* 时间: 2017/10/30
* 作者: jianjie
*/
void CUILayerManager::OnLayerMouseMove(emActionLayerType nLayerType , int nCol)
{
    ILayerUI* pLayer = FindLayer((int)nLayerType);
    if (pLayer)
    {
        pLayer->OnMouseMove(nCol);
    }
}

/**************************************************************************
* 函数名：OnLayerLButtonDown
* 功能:Layer层鼠标右键单击事件
* 参数:
*    @[in ] nLayerType: 层类型
*    @[in ] nCol: 第几列
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerManager::OnLayerRButtonDown(emActionLayerType nLayerType, int nCol)
{
    m_nCurCol = nCol;
    if (nLayerType == ACTION_LAYER_UNKNOWN || nLayerType == ACTION_LAYER_TIME)
        return;
    ClearSelectState();
    //重置选中的列
    SetCurrentSelectCol(nCol);

    //    //更新状态
    //     UpdateScaleAndRealTime(m_scaleValue, m_nCurCol);
    for(int i = 0; i < GetLayerCount(); i++ )
    {
        ILayerUI* pLayer = FindLayerByIndex(i);
        if (pLayer && pLayer->GetType() == nLayerType)
        {
            if (m_pEvent)
            {
                m_pEvent->LayerUIEvent_SetLayOptionHandler(pLayer->GetOptionHandler());
            }
            pLayer->OnRButtonDown(m_nCurCol);
        }
    }
}

void CUILayerManager::IUILayerManager_SetMusicFile(int nLayerType , void *pData , int nDataLen , int nDataID)
{
    ILayerUI* pLayerUI = FindLayer(nLayerType);

    if (pLayerUI == NULL)
        return;

    if (m_pEvent)
    {
        m_pEvent->LayerUIEvent_SetLayOptionHandler(pLayerUI->GetOptionHandler());
        m_pEvent->LayerUIEvent_SetMusicLayerSourceHandler(pLayerUI->GetOptionHandler(),nLayerType,pData , nDataLen, nDataID);
    }

    QPoint tempPoint(0,0);
    pLayerUI->OnLButtonDown(m_nCurCol, tempPoint, 0);
}
void CUILayerManager::IUILayerManager_SingleBlockPlayingStop(int nLayerType)
{
    StopLayerDebug();
    //PlayEvent_OnDebugStop(nLayerType);
}

//void CUILayerManager::IUILayerManager_SetMusicModifiedState(bool bState)
//{
//    m_bMusicModified = bState;
//}

//bool CUILayerManager::IUILayerManager_GetMusicModifiedState()
//{
//    return m_bMusicModified;
//}
void CUILayerManager::IUILayerManager_SingleGroupPlaying(int nLayerType , CActionBlockGroupManager* pGroupManager)
{
    if (m_pGroupDataPlay)
    {
        m_pGroupDataPlay->ReleaseConnection();
        m_pGroupDataPlay = NULL;
    }

    int nGroupStartTime = pGroupManager->GetStartTime();

    if (nLayerType == ACTION_LAYER_MOTION)
    {
        m_pGroupDataPlay = new CMotionDataPlay(this, nGroupStartTime);
    }

    if (nLayerType == ACTION_LAYER_EAR)
    {
        m_pGroupDataPlay = new EarLedDataPlay(this, nGroupStartTime);
    }

    if (nLayerType == ACTION_LAYER_EYE)
    {
        m_pGroupDataPlay = new EyeLedDataPlay(this, nGroupStartTime);
    }

    if (m_pGroupDataPlay)
    {
        int nEndTime = pGroupManager->GetEndTime();
        m_pGroupDataPlay->Connect(pGroupManager, true, m_scaleValue);
        if (nLayerType == ACTION_LAYER_MOTION)
        {
            m_pGroupDataPlay->StartPlay(nEndTime, m_pRobot3dView);
        } else {
            ILayerUI* itemFound = FindLayer(nLayerType);
            m_pGroupDataPlay->StartPlay(nEndTime, itemFound->GetOptionHandler());
        }
    }
}

void CUILayerManager::IUILayerManager_SingleBlockPlaying(int nLayerType ,CActionBlockData *pActionData)
{

}

bool CUILayerManager::IUILayerManager_IsRobotDebuging()
{
    return m_IsDataPlaying;
}

bool CUILayerManager::IUILayerManager_GetModifiedState()
{
    return m_bModified;
}

void CUILayerManager::IUILayerManager_SetModifiedState(bool bState)
{
    m_bModified = bState;
}

void CUILayerManager::IUILayerManager_DeleteMusicFile(int nLayerType)
{
    /*int nLayerType*/
    ILayerUI* pLayerUI = FindLayer(nLayerType);

    if (pLayerUI == NULL)
        return;
    if (m_pEvent)
    {
        m_pEvent->LayerUIEvent_DeleteMusciFile(pLayerUI->GetOptionHandler());
    }
}


void CUILayerManager::IUILayerManager_SwitchLayer(emActionLayerType eActionType)
{
    if (m_pEvent)
    {
        ILayerUI* pLayer = FindLayer(eActionType);
        if (pLayer)
        {
            m_pEvent->LayerUIEvent_SetLayOptionHandler(pLayer->GetOptionHandler());
            g_IsUpdateUIByData = true;
            m_pEvent->LayerUIEvent_SetActionBlockDataManager(pLayer->GetBlockDataManager(), 0);
            //这里更新LayerUI
            UpdateLayerWidget(pLayer->GetBlockDataManager(), -1, -1, eActionType);
            g_IsUpdateUIByData = false;
        }
    }
}

/**************************************************************************
* 函数名:ParseRawData
* 功能:解析数据
* 参数:char* pData 数据
* 参数:int nLen 数据长度
* 返回值: bool 解析成功，返回真，否则返回假
* 时间:2015/12/22 14:10
* 作者: 周志平
* 改动日志: 2015/12/22 14:10 增加无效指针pLayer判断
* 改动日志：2015/12/30 14:51  注释 AddAcionLayer(nType) 因1S各个Layer没有重叠,connect已经加了，不需要删除Layer后再添加

**************************************************************************/
bool CUILayerManager::ParseRawData(char* pData, int nLen)
{
    // 保存总的数据长度，先占位，后面再改为实际的长度
    int nTotalLen = 0;
    UBX_MEMCPY_INC(nTotalLen, pData);
    if (nTotalLen != nLen)
        return false;

    // 保存总的层数
    int nLayerCount = 0;
    UBX_MEMCPY_INC(nLayerCount, pData);
    for (int i=0; i<nLayerCount; i++)
    {
        char* pRawData = NULL;
        int nRawDataLen = 0;

        // 获取TYPE
        int nType = 0;
        UBX_MEMCPY_INC(nType, pData);

        ILayerUI* pLayer = NULL;
        AddAcionLayer(nType);
        pLayer = FindLayer(nType);
        // Raw DATA
        UBX_MEMCPY_INC(nRawDataLen, pData);
        if (nRawDataLen)
        {
            pRawData = new char[nRawDataLen];
            memcpy(pRawData, pData, nRawDataLen);
            pData += nRawDataLen;
            if(pLayer != NULL)
            {
                pLayer->SetData(pRawData, nRawDataLen);
            }
            delete [] pRawData;
        }

        //保存数据到层UI
        if (nType != ACTION_LAYER_TIME)
        {
            if ( m_pILayerCommonWidget)
            {
                m_pILayerCommonWidget->SetActionBlockDataManager(pLayer->GetBlockDataManager(), (emActionLayerType)nType);
            }

            //leo add 20170321
            if (m_pILayerCommonWidget != NULL && nType == ACTION_LAYER_MUSIC)
            {
                m_pILayerCommonWidget->loadData(ACTION_LAYER_MUSIC);
            }
        }
    }
    return true;
}

QString CUILayerManager::GetLayerName(int nLayerID)
{
    //	ILayerUI* pLayer = NULL;
    //    QString strLayerName = L"";

    //	if (nLayerID == ACTION_LAYER_TIME)
    //	{
    //		return L"";
    //	}
    //	else if (nLayerID == ACTION_LAYER_MOTION)
    //	{
    //		strLayerName = language_loadstring(IDS_MOTION_LAYER);
    //	}
    //	else if (nLayerID == ACTION_LAYER_EAR)
    //	{
    //		strLayerName = language_loadstring(IDS_EAR_LAYER);
    //	}
    //	else if (nLayerID == ACTION_LAYER_EYE)
    //	{
    //		strLayerName = language_loadstring(IDS_EYES_LAYER);
    //	}
    //	else if (nLayerID == ACTION_LAYER_MUSIC)
    //	{
    //		strLayerName = language_loadstring(IDS_MUSIC_LAYER);
    //	}

    QString strLayerName = QObject::tr("");
    return strLayerName;
}

int CUILayerManager::GetCurrentSelTime()
{
    //这一段不能开启，因为时间层没有当前时间线的信息，只有mp3proj模块才有
    //    CTimeLayer* pTimeLayer = (CTimeLayer*)FindLayer(ACTION_LAYER_TIME);
    //    if (pTimeLayer)
    //    {
    //        return pTimeLayer->GetCurrentSelTime();
    //    }
    //hels 原windows下 GetCurrentSelTime与GetCurrentCol返回的值是一样的。这里直接返回Col即可
    return GetCurrentSelectCol();
}

int CUILayerManager::SetCurrentSelectCol(int nCol)
{
    if (m_pILayerCommonWidget)
    {
        m_pILayerCommonWidget->SetCurrentSelectCol(nCol);
    }
    return true;
}

///**************************************************************************
//* 函数名：UpdateScaleAndRealTime
//* 功能:更新时间粒度和实际时间到UI
//* 参数:
//*    @[in ] nScaleValue: 时间粒度
//*    @[in ] nCol: 当前时间格数
//* 返回值: 无
//* 时间: 2017/1/16
//* 作者: Joker
//*/
//void CUILayerManager::UpdateScaleAndRealTime(int nScaleValue, int nCol)
//{
//    //更新状态
//    QString strScaleValue;
//    strScaleValue.sprintf("%d ms", nScaleValue);
//    ui->lEScaleTime->setText(strScaleValue);
//    QString strRealTime;
//    strRealTime.sprintf("%0.2lf s", (double)nScaleValue * (double)nCol / 1000.0);
//    ui->lERealTime->setText(strRealTime);
//}
// void CUILayerManager::SetActionToOptions(int nCurrentGroupID , int nCurrentActionID)
// {
// 	if(m_pEvent)
// 	{
// 	    m_pEvent;
// 	}
// }
void CUILayerManager::SetActionDataGroupManagerToOptions(CActionBlockDataManager* pGroupManager, int nCurrentGroupID,int nCurrentActionID, int nOffset)
{
    if (m_pEvent)
    {
        m_pEvent->LayerUIEvent_SetActionBlockDataManager(pGroupManager, nCurrentGroupID,nCurrentActionID, nOffset);
    }
}

//void CUILayerManager::UpdateScrollBar()
//{
//    if (m_pEvent)
//    {
//        m_pEvent->LayerUIEvent_UpdateScrollBar();
//    }
//}

int CUILayerManager::GetNewMaxCellCount(int nMax)
{
    int nNeedTime = 0;
    // 跳过时间层
    for (int i=ACTION_LAYER_MOTION; i<MAX_ACTION_LAYER; i++)
    {
        ILayerUI* pLayer = FindLayer(i);
        if (pLayer)
        {
            if (pLayer->GetMaxTimeCellCount()>nNeedTime)
            {
                nNeedTime = pLayer->GetMaxTimeCellCount();
            }
        }
    }

    return nMax < nNeedTime ? nNeedTime : nMax;
}

void CUILayerManager::Set3DRobotViewInterface(IUBXRobot3DView* pRobotView)
{
    m_pRobot3dView = pRobotView;
}

void CUILayerManager::SetTreeView(ILibTreeNavigation* pView)
{
    m_pTreeView = pView;
    //ILayerUI* pLayer = FindLayer(ACTION_LAYER_MOTION);
    //if (pLayer)
    //{
    //    pLayer->SetTreeView(pView);
    //}
    ////leo add 20170228
    //pLayer = FindLayer(ACTION_LAYER_EYE);
    //if (pLayer)
    //{
    //    pLayer->SetTreeView(pView);
    //}
    //pLayer = FindLayer(ACTION_LAYER_EAR);
    //if (pLayer)
    //{
    //    pLayer->SetTreeView(pView);
    //}
}

void CUILayerManager::SetTimeScaleValues(int timeScale)
{
    m_scaleValue = timeScale;
    if (m_pILayerCommonWidget)
    {
        m_pILayerCommonWidget->SetTimeScale(timeScale);
    }
}
void CUILayerManager::FreshTimeScaleValues(int timeSacle)
{
    if (m_pEvent)
    {
        m_pEvent->LayerUIEvent_SetScaleValue(timeSacle);
    }
    SetTimeScaleValues(timeSacle);
}
int CUILayerManager::GetTimeScaleValues()
{
    return m_scaleValue;
}

int CUILayerManager::GetLineThick()
{
    return m_lineThick;
}

void CUILayerManager::SetLineThick(int nThick)
{
    m_lineThick = nThick;
}

int CUILayerManager::GetCellWidth()
{
    return m_nCellWidth;
}

int CUILayerManager::GetCellCount()
{
    if (m_pILayerCommonWidget)
    {
        return m_pILayerCommonWidget->GetCellCount();
    }
    return 100;
}

void CUILayerManager::SetCellCount(int nCount)
{
    if (m_pILayerCommonWidget)
    {
        m_pILayerCommonWidget->SetCellCount(nCount);
    }
}

void CUILayerManager::SetCellWidth(int nWidth)
{
    m_nCellWidth = nWidth;
}

void CUILayerManager::LayerPlaying(int layerType, ILayerOptionHandler* optionHandle)
{
    ILayerUI* itemFound = FindLayer(layerType);
    m_IsDataPlaying = TRUE;

    m_pILayerCommonWidget->SetPlaying(true);
    if (m_ppManagerPlay)
    {
        m_ppManagerPlay->ReleaseConnection();
        m_ppManagerPlay = NULL;
    }

    CreateIGroupDataManagerPlay(&m_ppManagerPlay);
    //    m_ppManagerPlay->IGroupDataManagerPlay_SetRobot3DView(m_pRobot3dView);
    m_ppManagerPlay->IGroupDataManagerPlay_SetLayerManager(this);

    if (    layerType == ACTION_LAYER_MOTION
        ||  layerType == ACTION_LAYER_HAND
        ||  layerType == ACTION_LAYER_LEG
        ||  layerType == ACTION_LAYER_HEAD)
    {
        m_ppManagerPlay->Connection(itemFound, this, m_pRobot3dView, TRUE);
    } 
    else 
    {
        m_ppManagerPlay->Connection(itemFound, this, itemFound->GetOptionHandler(), TRUE);
    }

    itemFound->GetOptionHandler()->SetControlState(FALSE);
    itemFound->SetLayerMenuState(TRUE);

    m_ppManagerPlay->StartPlay();
}

void CUILayerManager::LayerStop()
{
    //    if (m_pMusicDataPlay && m_IsMusicPlaying)
    //    {
    //        /*m_pMusicDataPlay->StopPlay();*/
    //        m_pMusicDataPlay->ReleaseConnection();
    //        m_pMusicDataPlay = NULL;
    //        m_IsMusicPlaying = false;
    //    }
    //    if (m_pGroupDataPlay && m_IsDataPlaying)
    //    {
    //        m_pGroupDataPlay->StopPlay();
    //        m_IsDataPlaying = false;
    //    }


    if (m_ppManagerPlay && m_IsDataPlaying)
    {
        m_ppManagerPlay->StopPlay();
        m_IsDataPlaying = false;
    }
    PlayEvent_OnSetPos(1);
}

void CUILayerManager::StopLayerDebug()
{
    if (m_ppManagerPlay && m_IsDataPlaying)
    {
        m_ppManagerPlay->StopPlay();
        m_IsDataPlaying =false;

        m_pILayerCommonWidget->SetPlaying(false);
        m_pILayerCommonWidget->UpdateLayerUIByCol(1);
    }

//    CLayerControlWidget* pLayerCtrlWidget;
//    pLayerCtrlWidget = m_listLayerCtrlWidget.at(1);
//    pLayerCtrlWidget->
}

void CUILayerManager::SetOriginPos(int nCol)
{
    PlayEvent_OnSetPos(nCol);
}

void CUILayerManager::PlayEvent_OnSetPos(int nCol)
{
    if (m_pILayerCommonWidget)
    {
        m_pILayerCommonWidget->UpdateLayerUIByCol(nCol);
    }
}

void CUILayerManager::slotUpdateTimeLine(quint64 nPosition, int nType)
{
    if (m_pILayerCommonWidget)
    {
        m_pILayerCommonWidget->UpdateLayerUIByCol(nPosition);
    }
}

void CUILayerManager::LayerDebugStop(int layerType)
{
    m_IsDataPlaying = false;
    m_bPausePlay = false;
    m_bIsTotalPlaying = false;
    if (layerType == ACTION_LAYER_MOTION)
    {
        m_bMotionPlaying = false;
    }

    if (layerType == ACTION_LAYER_HAND)
    {
        m_bHandPlaying = false;
    }

    if (layerType == ACTION_LAYER_LEG)
    {
        m_bLegPlaying = false;
    }

    if (layerType == ACTION_LAYER_HEAD)
    {
        m_bHeadPlaying = false;
    }

    if (layerType == ACTION_LAYER_EAR)
    {
        m_bEarLedPlaying = false;
    }

    if (layerType == ACTION_LAYER_EYE)
    {
        m_bEyeLedPlaying = false;
    }

    if (layerType == ACTION_LAYER_MUSIC)
    {
        m_bMusicPlaying = false;
    }

    if (layerType == ACTION_LAYER_EXPRESSION)
    {
        m_bExpressionPlaying = false;
    }

    ILayerUI* itemFound = FindLayer(layerType);
    itemFound->GetOptionHandler()->SetControlState(true);
    itemFound->SetLayerMenuState(false);

    emit SigStop();

    // 联合仿真时，待所有组仿真结束更新UI
    if (   !m_bMotionPlaying
        && !m_bEarLedPlaying
        && !m_bEyeLedPlaying
        && !m_bMusicPlaying
        && !m_bHandPlaying
        && !m_bLegPlaying
        && !m_bHeadPlaying
        && !m_bExpressionPlaying)
    {
        emit sigUnitedStop();
    }

    if (m_pILayerCommonWidget)
    {
        m_pILayerCommonWidget->SetPlaying(false);
        // Delete by jianjie 2017/11/29
        //m_pILayerCommonWidget->UpdateLayerUIByCol(1);
    }
}

ILayerOptionHandler* CUILayerManager::GetOptionHandler()
{
    return m_pEvent->LayerUIEvent_GetOptionHandler();
}

void CUILayerManager::IUILayerManager_SetSocketSession(CIUBXSocketSession* pSession)
{
    m_pSocketSession = pSession;
}

int CUILayerManager::GetCurrentSelectCol()
{
    int nRet = 0;

    if (m_pILayerCommonWidget)
    {
        nRet = m_pILayerCommonWidget->GetCurrentSelectCol();
    }

    return nRet;
}

// 联合仿真开始
void CUILayerManager::TotalLayerPlaying()
{
    m_bPausePlay = false;

    if (m_bTotalPause)
    {
        if (m_pMotionPlay)
        {
            m_pMotionPlay->OnResumePlay(ACTION_LAYER_MOTION);
        }

        // 动作分层
        if (m_pHandPlay)
        {
            m_pHandPlay->OnResumePlay(ACTION_LAYER_HAND);
        }

        if (m_pLegPlay)
        {
            m_pLegPlay->OnResumePlay(ACTION_LAYER_LEG);
        }

        if (m_pHeadPlay)
        {
            m_pHeadPlay->OnResumePlay(ACTION_LAYER_HEAD);
        }

        if (m_pEyeLedPlay)
        {
            m_pEyeLedPlay->OnResumePlay(ACTION_LAYER_EYE);
        }

        if (m_pEarLedPlay)
        {
            m_pEarLedPlay->OnResumePlay(ACTION_LAYER_EAR);
        }

        if (m_pMusicPlay)
        {
            m_pMusicPlay->OnResumePlay(ACTION_LAYER_MUSIC);
        }

        if (m_pExpressionPlay)
        {
            m_pExpressionPlay->OnResumePlay(ACTION_LAYER_EXPRESSION);
        }

        m_bTotalPause = false;
    } 
    else
    {
        // 先停止正在进行的仿真
        LayerStop();

        m_IsDataPlaying = true;
        m_bIsTotalPlaying = true;
        if (m_pMotionPlay)
        {
            m_pMotionPlay->ReleaseConnection();
            m_pMotionPlay = NULL;
        }

        if (m_pHandPlay)
        {
            m_pHandPlay->ReleaseConnection();
            m_pHandPlay = NULL;
        }

        if (m_pLegPlay)
        {
            m_pLegPlay->ReleaseConnection();
            m_pLegPlay = NULL;
        }

        if (m_pHeadPlay)
        {
            m_pHeadPlay->ReleaseConnection();
            m_pHeadPlay = NULL;
        }

        if (m_pEarLedPlay)
        {
            m_pEarLedPlay->ReleaseConnection();
            m_pEarLedPlay = NULL;
        }

        if (m_pEyeLedPlay)
        {
            m_pEyeLedPlay->ReleaseConnection();
            m_pEyeLedPlay = NULL;
        }

        if (m_pMusicPlay)
        {
            m_pMusicPlay->ReleaseConnection();
            m_pMusicPlay = NULL;
        }

        if (m_pExpressionPlay)
        {
            m_pExpressionPlay->ReleaseConnection();
            m_pExpressionPlay = NULL;
        }

        ILayerUI* itemMotion = FindLayer(ACTION_LAYER_MOTION);
        if (itemMotion)
        {
            CreateIGroupDataManagerPlay(&m_pMotionPlay);
            m_pMotionPlay->IGroupDataManagerPlay_SetLayerManager(this);
            m_pMotionPlay->Connection(itemMotion, this, m_pRobot3dView, TRUE);
            m_pMotionPlay->StartPlay();
            m_bMotionPlaying = true;
        }

        ILayerUI* itemHand = FindLayer(ACTION_LAYER_HAND);
        if (itemHand)
        {
            CreateIGroupDataManagerPlay(&m_pHandPlay);
            m_pHandPlay->IGroupDataManagerPlay_SetLayerManager(this);
            m_pHandPlay->Connection(itemHand , this , m_pRobot3dView , TRUE);
            m_pHandPlay->StartPlay();
            m_bHandPlaying = true;
        }

        ILayerUI* itemLeg = FindLayer(ACTION_LAYER_LEG);
        if (itemLeg)
        {
            CreateIGroupDataManagerPlay(&m_pLegPlay);
            m_pLegPlay->IGroupDataManagerPlay_SetLayerManager(this);
            m_pLegPlay->Connection(itemLeg , this , m_pRobot3dView , TRUE);
            m_pLegPlay->StartPlay();
            m_bLegPlaying = true;
        }

        ILayerUI* itemHead = FindLayer(ACTION_LAYER_HEAD);
        if (itemHead)
        {
            CreateIGroupDataManagerPlay(&m_pHeadPlay);
            m_pHeadPlay->IGroupDataManagerPlay_SetLayerManager(this);
            m_pHeadPlay->Connection(itemHead , this , m_pRobot3dView , TRUE);
            m_pHeadPlay->StartPlay();
            m_bHeadPlaying = true;
        }

        ILayerUI* itemEar = FindLayer(ACTION_LAYER_EAR);
        if (itemEar)
        {
            CreateIGroupDataManagerPlay(&m_pEarLedPlay);
            m_pEarLedPlay->IGroupDataManagerPlay_SetLayerManager(this);
            m_pEarLedPlay->Connection(itemEar, this, itemEar->GetOptionHandler(), TRUE);
            m_pEarLedPlay->StartPlay();
            m_bEarLedPlaying = true;
        }

        ILayerUI* itemEye = FindLayer(ACTION_LAYER_EYE);
        if (itemEye)
        {
            CreateIGroupDataManagerPlay(&m_pEyeLedPlay);
            m_pEyeLedPlay->IGroupDataManagerPlay_SetLayerManager(this);
            m_pEyeLedPlay->Connection(itemEye, this, itemEye->GetOptionHandler(), TRUE);
            m_pEyeLedPlay->StartPlay();
            m_bEyeLedPlaying = true;
        }

        ILayerUI* itemMusic = FindLayer(ACTION_LAYER_MUSIC);
        if (itemMusic)
        {
            CreateIGroupDataManagerPlay(&m_pMusicPlay);
            m_pMusicPlay->IGroupDataManagerPlay_SetLayerManager(this);
            m_pMusicPlay->Connection(itemMusic , this , itemMusic->GetOptionHandler() , TRUE);
            m_pMusicPlay->StartPlay();
            m_bMusicPlaying = true;
        }

        ILayerUI* itemExpression = FindLayer(ACTION_LAYER_EXPRESSION);
        if (itemExpression)
        {
            CreateIGroupDataManagerPlay(&m_pExpressionPlay);
            m_pExpressionPlay->IGroupDataManagerPlay_SetLayerManager(this);
            m_pExpressionPlay->Connection(itemExpression, this, itemExpression->GetOptionHandler(), TRUE);
            m_pExpressionPlay->StartPlay();
            m_bExpressionPlaying = true;
        }
    }
    
}

// 联合仿真暂停
void CUILayerManager::PauseTotalLayerDebug()
{
    if (m_pGroupDataPlay)//&& m_IsDataPlaying)
    {
        m_pGroupDataPlay->StopPlay();
        m_IsDataPlaying = false;
    }

    if (m_pMotionPlay && m_bMotionPlaying)
    {
        m_pMotionPlay->PausePlay(ACTION_LAYER_MOTION);
    }

    if (m_pHandPlay && m_bHandPlaying)
    {
        m_pHandPlay->PausePlay(ACTION_LAYER_HAND);
    }

    if (m_pLegPlay && m_bLegPlaying)
    {
        m_pLegPlay->PausePlay(ACTION_LAYER_LEG);
    }

    if (m_pHeadPlay && m_bHeadPlaying)
    {
        m_pHeadPlay->PausePlay(ACTION_LAYER_HEAD);
    }

    if (m_pEyeLedPlay && m_bEyeLedPlaying)
    {
        m_pEyeLedPlay->PausePlay(ACTION_LAYER_EYE);
    }

    if (m_pEarLedPlay && m_bEarLedPlaying)
    {
        m_pEarLedPlay->PausePlay(ACTION_LAYER_EAR);
    }

    if (m_pMusicPlay && m_bMusicPlaying)
    {
        m_pMusicPlay->PausePlay(ACTION_LAYER_MUSIC);
    }

    if (m_pExpressionPlay && m_bExpressionPlaying)
    {
        m_pExpressionPlay->PausePlay(ACTION_LAYER_EXPRESSION);
    }

    m_bTotalPause = true;

    /*m_bTotalPlaying =false;*/
    //ILayerUI* itemMotion = FindLayer(ACTION_LAYER_MOTION);
    //if (itemMotion)
    //{
    //    itemMotion->CancelBlockPlay();
    //}

    //ILayerUI* itemMusic = FindLayer(ACTION_LAYER_MUSIC);
    //if (itemMusic)
    //{
    //    itemMusic->CancelBlockPlay();
    //    itemMusic->GetOptionHandler()->PlayMusic(0, 0, 1);//1表示暂停
    //}
}

// 联合仿真停止
void CUILayerManager::StopTotalLayerDebug()
{
    m_bTotalPause = false;
    if (m_pMotionPlay && m_bMotionPlaying)
    {
        m_pMotionPlay->StopPlay();
        m_bMotionPlaying = false;
    }

    if (m_pHandPlay && m_bHandPlaying)
    {
        m_pHandPlay->StopPlay();
        m_bHandPlaying = false;
    }

    if (m_pLegPlay && m_bLegPlaying)
    {
        m_pLegPlay->StopPlay();
        m_bLegPlaying = false;
    }

    if (m_pHeadPlay && m_bHeadPlaying)
    {
        m_pHeadPlay->StopPlay();
        m_bHeadPlaying = false;
    }

    if (m_pEarLedPlay && m_bEarLedPlaying)
    {
        m_pEarLedPlay->StopPlay();
        m_bEarLedPlaying = false;
    }

    if (m_pEyeLedPlay && m_bEyeLedPlaying)
    {
        m_pEyeLedPlay->StopPlay();
        m_bEyeLedPlaying = false;
    }

    if (m_pMusicPlay && m_bMusicPlaying)
    {
        m_pMusicPlay->StopPlay();
        m_bMusicPlaying = false;
    }

    if (m_pExpressionPlay && m_bExpressionPlaying)
    {
        m_pExpressionPlay->StopPlay();
        m_bExpressionPlaying = false;
    }

    m_pILayerCommonWidget->UpdateLayerUIByCol(1);

    ui->pbAllLayerPause->hide();
    ui->pbAllLayerPlay->show();
}


void CUILayerManager::LayerManager_OnLButtonDown(int layerType)
{
    ILayerUI* pUILayer = NULL;
    pUILayer = FindLayer(layerType);
    if (pUILayer)
    {
        m_pEvent->LayerUIEvent_SetLayOptionHandler(pUILayer->GetOptionHandler());
    }
}

/**************************************************************************
* 函数名：IUILayerManager_MusicHasLoaded
* 功能: 音乐加载完成事件通知
* 参数:
*    @[in ] strMp3Path: 音乐路径
*    @[in ] strTitle: 音乐标题
*    @[in ] nTime: 音乐总时长
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerManager::IUILayerManager_MusicHasLoaded(const QString &strMp3Path, const QString &strTitle, int nTime)
{
    ILayerUI* pUILayer = NULL;
    pUILayer = FindLayer(ACTION_LAYER_MUSIC);
    if (pUILayer)
    {
        pUILayer->ILayer_onMusicHasLoaded(strMp3Path, strTitle, nTime);
    }
}

/**************************************************************************
* 函数名：OnDoMenuCommand
* 功能:右键菜单响应
* 参数:
*    @[in ] emActionLayerType: 层类型
*    @[in ] nCommond: 命令
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerManager::OnDoMenuCommand(emActionLayerType nLayerType, int nCommond)
{
    if (nLayerType == ACTION_LAYER_UNKNOWN || nLayerType == ACTION_LAYER_TIME)
        return;
    for(int i = 0; i < GetLayerCount(); i++ )
    {
        ILayerUI* pLayer = FindLayerByIndex(i);
        if (pLayer && pLayer->GetType() == nLayerType)
        {
            pLayer->DoMenuCommand(nCommond, m_nCurCol);
        }
    }
}

//耳朵灯仿真
void CUILayerManager::slotEarLedPlay(int RunTime, int LeftLed, int RightLed, int Bright, int LedLightUpTime, int LedLightDownTime , bool bEnd)
{
    if (m_pSocketSession)
    {
        m_pSocketSession->UBXSocketSession_PlayLedData(emEarLed , LeftLed , RightLed , Bright , 3 , RunTime , LedLightUpTime , LedLightDownTime, bEnd);
    }
}

//眼睛灯仿真
void CUILayerManager::slotEyeLedPlay(int LeftLed, int RightLed, int Bright, int Color, int runTime, int LightUpTime, int LightDownTime, bool bEnd)
{
    if (m_pSocketSession)
    {
        m_pSocketSession->UBXSocketSession_PlayLedData(emEyeLed , LeftLed , RightLed , Bright , Color , runTime , LightUpTime , LightDownTime, bEnd);
    }
}

// 眼部表情仿真
void CUILayerManager::slotExpressionPlay(QString strFileName, int nFrameNum)
{
    IUILayerManager_ShowExpression(strFileName, nFrameNum);
}

//停止眼睛灯仿真
void CUILayerManager::IUILayerManager_StopPlayEyeData()
{
    if (m_pSocketSession)
    {
        m_pSocketSession->UBXSocketSession_StopPlayLedData(emEyeLed);
    }
}

//停止耳朵灯仿真
void CUILayerManager::IUILayerManager_StopPlayEarData()
{
    if (m_pSocketSession)
    {
        m_pSocketSession->UBXSocketSession_StopPlayLedData(emEarLed);
    }
}


// 表情帧显示
void CUILayerManager::IUILayerManager_ShowExpression(QString strFileName, int nCurFrame)
{
    if (m_pSocketSession)
    {
        m_pSocketSession->UBXSocketSession_PlayExpression(strFileName, nCurFrame);
    }
}


void CUILayerManager::ReDrawAll()
{
    QSize size = this->size();
    if(NULL != m_pPixmapMem)
    {
        delete m_pPixmapMem;
    }
    m_pPixmapMem = new QPixmap(size.width(), size.height());
    m_pPixmapMem->fill(Qt::transparent);

    if(NULL != m_pPixmapBackGround)
    {
        delete m_pPixmapBackGround;
    }
    m_pPixmapBackGround = new QPixmap(size.width(), size.height());
    m_pPixmapBackGround->fill(Qt::transparent);

    if(NULL != m_pPixmapGroup)
    {
        delete m_pPixmapGroup;
    }
    m_pPixmapGroup = new QPixmap(size.width(), 20);
    m_pPixmapGroup->fill(Qt::transparent);

    if(NULL != m_pPixmapWave)
    {
        delete m_pPixmapWave;
    }
    m_pPixmapWave = new QPixmap(size.width(), 100);
    m_pPixmapWave->fill(Qt::transparent);

    DrawBackGround(m_pPixmapBackGround);
    //    m_actionLayer.OnDrawGroup(m_pPixmapGroup);
    //    m_waveLayer.DrawWavePixmap(m_pPixmapWave);

    DrawPixmapExcludeSelLine(m_pPixmapMem);
    //    DrawTimeSelLine(m_pPixmapMem);
    update();
}

void CUILayerManager::DrawPixmapExcludeSelLine(QPixmap* pixmap)
{
    if(NULL != pixmap)
    {
        QPainter painter;
        painter.begin(pixmap);
        painter.drawPixmap(0, 0, m_pPixmapBackGround->width(), m_pPixmapBackGround->height(), *m_pPixmapBackGround);
        painter.drawPixmap(0, 20, m_pPixmapWave->width(), m_pPixmapWave->height(), *m_pPixmapWave);
        painter.drawPixmap(0, 20 + 80, m_pPixmapGroup->width(), m_pPixmapGroup->height(), *m_pPixmapGroup);
        painter.end();
    }
}

void CUILayerManager::DrawBackGround(QPixmap* pixmap)
{
    if(NULL == pixmap)
    {
        return;
    }
    //int wTmp = (m_nCellWidth + m_nLineThick) * m_nCellCount;//所有cell总宽度
    //QRect rtAll(m_nStartX, m_nStartY, wTmp, m_nCellHeight);//cell所占区域
    QRect rectWidget = this->geometry();
    QRect rectTimeBG = rectWidget;
    rectTimeBG.setHeight(Constants::DEFAULT_CELL_TIME_HEIGHT);
    QBrush bgBrush(Constants::COLOR_MP3_BG);
    QPainter painter;
    painter.begin(pixmap);
    painter.fillRect(rectWidget, bgBrush);
    painter.fillRect(rectTimeBG, QBrush(Constants::COLOR_TIME_BG));
    painter.end();
    //    DrawTimeLayer(pixmap);
    //    DrawBottomCell(pixmap);
}

//void CUILayerManager::paintEvent(QPaintEvent *event)
//{
//    if(NULL != m_pPixmapMem)
//    {
//        QPainter painter(this);
//        QRect rcWnd(0, 0, this->width(), this->height());
//        painter.drawPixmap(rcWnd, *m_pPixmapMem, rcWnd);
//    }
//}

void CUILayerManager::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //    AdjustCellCount();
    //    emitRangeChanged();
    //    emit SigReDrawAll();
    //    ReDrawAll();
}

/**************************************************************************
* 函数名：onScaleSettingClicked
* 功能: 设置单位时间粒度
* 参数: 无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerManager::onScaleSettingClicked()
{
    CTimeSettingDialog dlg;
    dlg.SetTimeScale(m_scaleValue);
    if(dlg.exec() == QDialog::Accepted)
    {
        m_scaleValue = dlg.GetTimeScale();
        if (m_pILayerCommonWidget)
        {
            m_pILayerCommonWidget->SetTimeScale(m_scaleValue);
        }
    }
}

// 添加动作层响应槽
void CUILayerManager::onAddLayerClicked()
{

    //先初始化列表
    int* pLayerID = NULL;

    int nLayerCount = 0;
    vector<int> unUsedLayerID;
    for (int i = ACTION_LAYER_MOTION; i < ACTION_MAX_LAYER_COUNT; i++)
    {
        if (   ACTION_LAYER_EYE     == i
            || ACTION_LAYER_EAR     == i
            || ACTION_LAYER_MOUTH   == i)
        {
            continue;
        }
        ILayerUI* pLayer = FindLayer(i);
        if (pLayer == NULL)
        {
            unUsedLayerID.push_back(i);
        }
    }

    if (unUsedLayerID.size())
    {
        nLayerCount = unUsedLayerID.size();
        pLayerID = new int[nLayerCount];

    }
    else
    {
        pLayerID = NULL;
        nLayerCount = 0;
    }
    int* p = pLayerID;
    for (int i = 0; i < nLayerCount; i++)
    {
        p[i] = unUsedLayerID[i];
    }
    m_AddLayerDlg.InitLayerList(&pLayerID, nLayerCount);

    if (m_AddLayerDlg.exec() == QDialog::Accepted)
    {
        int* pLayerSelectID = NULL;
        int nLayerSelectCount = 0;
        m_AddLayerDlg.GetSelectLayer(&pLayerSelectID, nLayerSelectCount);

        for (int iLayerType = ACTION_LAYER_MOTION; iLayerType < ACTION_MAX_LAYER_COUNT; iLayerType++)
        {
            if (   ACTION_LAYER_EYE     == iLayerType
                || ACTION_LAYER_EAR     == iLayerType
                || ACTION_LAYER_MOUTH   == iLayerType)
            {
                continue;
            }
            bool bIsLayerExist = false;
            bool bIsLayerSelect = false;
            ILayerUI* pLayer  = FindLayer(iLayerType);
            if (pLayer)
            {
                bIsLayerExist = true;//判断Layer层是否存在
            }

            for (int i = 0; i < nLayerSelectCount; i++)
            {
                int nLayerType = pLayerSelectID[i];
                if (nLayerType == iLayerType)
                {
                    bIsLayerSelect = true; //判断该Layer层是否被选中
                    break;
                }
            }

            // 如果该层不存在并且Layer被选中，则添加层
            if (!bIsLayerExist && bIsLayerSelect)
            {
                AddAcionLayer(iLayerType);

                // Delete by jianjie 2017/10/20
                //ILayerUI* pCurLayer = FindLayer(iLayerType);
                //{
                //    if (pCurLayer)
                //    {
                //        //刷新一下层UI
                //        UpdateLayerWidget(pCurLayer->GetBlockDataManager(), -1, -1, (emActionLayerType)iLayerType);
                //        
                //    }
                //}
            }

            //如果该层存在并且Layer没有被选中，则删除层
            if (bIsLayerExist && !bIsLayerSelect)
            {
                onDeleteLayerClicked((emActionLayerType)iLayerType);
            }

            // 以选中层刷新显示
            if (bIsLayerSelect)
            {
                IUILayerManager_SwitchLayer((emActionLayerType)iLayerType);
            }
        }

        if (pLayerSelectID)
        {
            SAFE_DELETE_ARRAY(pLayerSelectID);
        }

        int nCellCount = GetNewMaxCellCount(0) + ADD_OFFSET;
        SetCellCount(nCellCount);

        //这里如果层都加完了，应该禁用添加按钮了
    }
}

// 联合仿真开始
void CUILayerManager::slotAllLayerPlay()
{
    TotalLayerPlaying();

    ui->pbAllLayerPlay->hide();
    ui->pbAllLayerPause->show();
}

// 联合仿真暂停
void CUILayerManager::slotAllLayerPause()
{
    PauseTotalLayerDebug();

    ui->pbAllLayerPause->hide();
    ui->pbAllLayerPlay->show();
}

// 联合仿真停止
void CUILayerManager::slotAllLayerStop()
{
    StopTotalLayerDebug();

    // Modify by jianjie 2017/11/02 ui切换放入StopTotalLayerDebug()函数中
    //ui->pbAllLayerPause->hide();
    //ui->pbAllLayerPlay->show();
}

// 响应停止所有，跨线程操作，使用信号槽
void CUILayerManager::slotUnitedStop()
{
    ui->pbAllLayerPause->hide();
    ui->pbAllLayerPlay->show();
}

/**************************************************************************
* 函数名：onDeleteLayerClicked
* 功能: 根据层ID删除layer
* 参数:
*    @[in ] emActionLayerType: 层类型
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerManager::onDeleteLayerClicked(emActionLayerType nType)
{
    if (nType == ACTION_LAYER_MUSIC)
    {
        ui->vSpacerMotion->changeSize(20, 68, QSizePolicy::Fixed, QSizePolicy::Fixed);
    }
    DeleteActionLayer(nType);
}

/**************************************************************************
* 函数名：onDeleteLayerClicked
* 功能: 根据层ID仿真
* 参数:
*    @[in ] emActionLayerType: 层类型
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerManager::onPlayLayerClicked(emActionLayerType nType)
{ 
    m_bTotalPause = false;

    //点击仿真的时候，先更新UI
    if (m_pEvent)
    {
        ILayerUI* pLayer = FindLayer(nType);
        if (pLayer)
        {
            m_pEvent->LayerUIEvent_SetLayOptionHandler(pLayer->GetOptionHandler());
            g_IsUpdateUIByData = true;
            m_pEvent->LayerUIEvent_SetActionBlockDataManager(pLayer->GetBlockDataManager(), 0);
            //这里更新LayerUI
            UpdateLayerWidget(pLayer->GetBlockDataManager(), -1, -1, nType);
            g_IsUpdateUIByData = false;
        }
    }

    if (m_bPausePlay && m_nLastType == nType)
    {
        if (m_ppManagerPlay)
        {
            m_ppManagerPlay->OnResumePlay(nType);
        }
        m_bPausePlay = false;
    }
    else
    {
        // 先停止正在进行的仿真
        StopTotalLayerDebug();
        LayerStop();

        //这里还要加一个重置的操作，重置Layer层、Option层UI
        LayerPlaying(nType, NULL);
    }

    m_nLastType = nType;
}

/**************************************************************************
* 函数名：onDeleteLayerClicked
* 功能: 根据层ID暂停仿真
* 参数:
*    @[in ] emActionLayerType: 层类型
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerManager::onPauseLayerClicked(emActionLayerType nType)
{
    if (m_ppManagerPlay && m_IsDataPlaying)
    {
        m_bPausePlay = true;
        m_ppManagerPlay->PausePlay(nType);
    }
}

/**************************************************************************
* 函数名：onDeleteLayerClicked
* 功能: 根据层ID停止仿真
* 参数:
*    @[in ] emActionLayerType: 层类型
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerManager::onStopLayerClicked(emActionLayerType nType)
{
    if (m_ppManagerPlay && m_IsDataPlaying)
    {
        m_ppManagerPlay->StopPlay();
        m_IsDataPlaying =false;
        m_bPausePlay = false;

        if (m_pILayerCommonWidget)
        {
            m_pILayerCommonWidget->SetPlaying(false);
            m_pILayerCommonWidget->UpdateLayerUIByCol(1);
        }
    }
}

void CUILayerManager::onShowLayerClicked(emActionLayerType nType)
{
    IUILayerManager_SwitchLayer(nType);
}

void CUILayerManager::OnMp3PositionChange(quint64 nPosition)
{
    emit SigPositionChanged(nPosition);
}

void CUILayerManager::slotSelectLineChanged(quint64 nPosition)
{
    if (m_pILayerCommonWidget)
    {
        m_pILayerCommonWidget->UpdateLayerUIByCol(nPosition);
    }
}

void CUILayerManager::OnPlayMp3()
{

}

void CUILayerManager::PlayEvent_PlayMp3()
{
    //    QString strMusicPath = "/Users/ubt-wan/Downloads/LittleApple.mp3";
    //    m_pMP3Player = new MP3Player();
    //    m_pMP3Player->SetFileName(strMusicPath);
    //    m_pMP3Player->SetLayerManager(this);
    //    m_pMP3Player->Play(0, 78000);
}

//void CUILayerManager::on_pBDeleteLayer_clicked()
//{

//}

//void CUILayerManager::on_pbDeleteMusic_clicked()
//{
//    DeleteActionLayer(ACTION_LAYER_MUSIC);
//}

//void CUILayerManager::on_pbDeleteEar_clicked()
//{
//    DeleteActionLayer(ACTION_LAYER_EAR);
//}

//void CUILayerManager::on_pbDeleteEye_clicked()
//{
//    DeleteActionLayer(ACTION_LAYER_EYE);
//}

/**************************************************************************
* 函数名:   TreeToView
* 功能:     
* 参数:   
*....@[in]  char * pData
*....@[in]  int nLen
*....@[in]  QPoint * nPoint
*....@[in]  QString FileName
*....@[out] void
* 返回值:   
* 时间:     2017/03/03 18:01
* 作者:   Leo
*/
void CUILayerManager::TreeToView( char *pData, int nLen, emActionLayerType eType, int nCur, QString FileName )
{
    ILayerUI* pLayer = FindLayer(eType);
    if (pLayer)
    {
        pLayer->TreeToView(pData, nLen, nCur, FileName);
    } 
}
