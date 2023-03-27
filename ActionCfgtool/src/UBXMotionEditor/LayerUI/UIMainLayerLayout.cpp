
#include "UIMainLayerLayout.h"
#include "ui_UIMainLayerLayout.h"
#include "UBXMotionEditorLayout.h"
#include <QPainter>

bool CreateLayerManager(ILayerManager** ppManager)
{
    CUIMainLayerLayout* pProxy =new CUIMainLayerLayout();
    if(pProxy)
    {
        *ppManager = pProxy;
        return true;
    }
    return false;
}

CUIMainLayerLayout::CUIMainLayerLayout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CUIMainLayerLayout)
{
        ui->setupUi(this);
}

CUIMainLayerLayout::~CUIMainLayerLayout()
{
    delete ui;
}

bool CUIMainLayerLayout::Connect(QWidget* pParent, ILayerManager_Event* pEvent)
{
//    if (Create(CUIMainLayerLayout::IDD, widget))
//    {
//        ShowWindow(SW_SHOW);

//        m_pEvent = pEvent;
//        return true;
//    }
    if(pParent && pParent->layout())
    {
        pParent->layout()->addWidget(this);
    }
    else
    {
        setParent(pParent);
    }
    m_pEvent = pEvent;


    CreateUILayerManager(&m_pUILayerManager);
    m_pUILayerManager->Connect(this,this);
//    return true;
//    if(CreateMotionLayOptionHandler(&m_pMotionWidget))
//    {
//        m_pMotionWidget->Connect(this,this);
//    }
        return true;
}

// 释放资源
void CUIMainLayerLayout::ReleaseConnection()
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->ReleaseConnection();
        m_pUILayerManager = NULL;
    }

    delete this;
}

QWidget* CUIMainLayerLayout::GetWidget()
{
    return this;
}
//void CUIMainLayerLayout::EnableCtrlPlay(int nLayerType , bool bState)
//{
//    if (m_pEvent)
//    {
//        //m_pEvent->ILayerManager_EnablePlay()
//    }
//}
//void CUIMainLayerLayout::EnablePlay(int nLayerType ,bool bState)
//{
//    if (m_pEvent)
//    {
//        m_pEvent->ILayerManager_EnablePlay(nLayerType , bState);//20160519万向峰合并代码时注释掉
//    }
//}
bool CUIMainLayerLayout::GetData(char** ppData, int &nLen)
{
    if (m_pUILayerManager)
    {
        return m_pUILayerManager->GetData(ppData, nLen);
    }

    return false;
}
//void CUIMainLayerLayout::AdjustData()
//{
//    if (m_pUILayerManager)
//    {
//        m_pUILayerManager->AdjustData();
//    }
//}
bool CUIMainLayerLayout::SetData(char* pData, int nLen)
{
    if (m_pUILayerManager)
    {
        return m_pUILayerManager->SetData(pData, nLen);
    }

    return false;
}


//void CUIMainLayerLayout::OnSrcollChange(int nPos)
//{
//    if (m_pUILayerManager)
//    {
//        m_pUILayerManager->SetHScrollOffset(nPos);//20160519万向峰合并代码时注释掉
//    }
//}

void CUIMainLayerLayout::LayerUIEvent_SelectChanger(int nCurrentCol, int nScale)
{
//    if (m_dlgTools.GetSafeHwnd())
//    {
//        m_dlgTools.ShowStatus(nCurrentCol, nScale);
    //    }
}

void CUIMainLayerLayout::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CUIMainLayerLayout::LayerManager_GetUnUseLayers(int **pLayerID, int& nLayerCount)
{
    vector<int> unUserLayerID;

    if (m_pUILayerManager == NULL)
    {
        *pLayerID = NULL;
        nLayerCount = 0;

        return;
    }

    for (int i=ACTION_LAYER_TIME; i<MAX_ACTION_LAYER; i++)
    {
        ILayerUI* pLayer = m_pUILayerManager->FindLayer(i);
        if (pLayer == NULL)
        {
            unUserLayerID.push_back(i);
        }
    }

    if (unUserLayerID.size())
    {
        *pLayerID = new int[unUserLayerID.size()];
        nLayerCount = unUserLayerID.size();
    }
    else
    {
        *pLayerID = NULL;
        nLayerCount = 0;
    }

    int* p = *pLayerID;
    for (int i=0; i<nLayerCount; i++)
    {
        p[i] = unUserLayerID[i];
    }
}

QString CUIMainLayerLayout::LayerManager_GetLayerName(int nLayerID)
{
    if(m_pUILayerManager == NULL)
    {
        return "";
    }

    return m_pUILayerManager->GetLayerName(nLayerID);
}
void CUIMainLayerLayout::LayerManager_ReleaseMusic()
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->ReleaseMusic();
    }
}

//void CUIMainLayerLayout::LayerManager_AddMusic()
//{
//    if (m_pUILayerManager)
//    {
//        m_pUILayerManager->AddMusic();
//    }
//}

void CUIMainLayerLayout::LayerManager_AddLayer(int nLayerID)
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->AddAcionLayer(nLayerID);
    }
}

ILayerUI* CUIMainLayerLayout::LayerManager_GetLayer(int LayerID)
{
    if (m_pUILayerManager)
    {
        return m_pUILayerManager->FindLayer(LayerID);
    }

    return NULL;
}

void CUIMainLayerLayout::LayerManager_SetTimeScale(int timeScale)
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->SetTimeScaleValues(timeScale);
//        m_toolsWidget.ShowStatus(m_pUILayerManager->GetCurrentSelectCol(), timeScale);
    }
}

int CUIMainLayerLayout::LayerManager_GetTimeScaleValue()
{
    if (m_pUILayerManager)
    {
        return m_pUILayerManager->GetTimeScaleValues();
    }

    return -1;
}

//void CUIMainLayerLayout::LayerManager_LayerPlaying(int layerType, ILayerOptionHandler* optionHandle)
//{
//    if (m_pUILayerManager)
//    {
//        m_pUILayerManager->LayerPlaying(layerType, optionHandle);
//    }
//}
void CUIMainLayerLayout::LayerManager_SetMusicResource(QString strFilePath)
{
    if (m_pUILayerManager)
    {
//        ILayerUI* pMusicLayer = m_pUILayerManager->FindLayer(ACTION_LAYER_MUSIC);

//        ((CMusicLayer*)pMusicLayer)->AddMusic(strFilePath);
    }
}
void CUIMainLayerLayout::LayerManager_LayerStop()
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->LayerStop();
    }
}
void CUIMainLayerLayout::LayerManager_StopLayerDebug()
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->StopLayerDebug();
    }
}

//void CUIMainLayerLayout::LayerUIEvent_AddLayerControl(int nLayerType)
//{
////    CUBXMotionEditorLayout* pDlg = (CUBXMotionEditorLayout*)parentWidget();
////    pDlg->ILayerControlEvent_AddLayerControl(nLayerType);
//}

void CUIMainLayerLayout::LayerUIEvent_SetLayOptionHandler(ILayerOptionHandler* pHandler)
{
    if (m_pEvent)
    {
        m_pEvent->ILayerManager_SetLayOptionHandler(pHandler);
    }
}
void CUIMainLayerLayout::LayerUIEvent_SetMusicLayerSourceHandler(ILayerOptionHandler* pHandler , int nLayerType
                                                               , void *pData , int nDataLen , int nDataID)
{
    if (m_pEvent)
    {
        m_pEvent->ILayerManager_SetLayOptionHandler(pHandler);
//        m_pEvent->ILayerManager_SetMusicLayerSourceHandler(pHandler,nLayerType,pData , nDataLen ,nDataID);
    }
}
void CUIMainLayerLayout::LayerUIEvent_DeleteMusciFile(ILayerOptionHandler* pHandler)
{
    if (m_pEvent)
    {
        m_pEvent->ILayerManager_DeleteMusciFile(pHandler);
    }
}
void CUIMainLayerLayout::LayerUIEvent_OnDeletOptionHandler(ILayerOptionHandler* pHandler)
{
    if (m_pEvent)
    {
        m_pEvent->ILayerManager_OnDeletOptionHandler(pHandler);
    }
}
void CUIMainLayerLayout::LayerUIEvent_SetScaleValue(int nScale)
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->SetTimeScaleValues(nScale);
//        m_toolsWidget.ShowStatus(m_pUILayerManager->GetCurrentSelectCol(), nScale);
    }
}
//void CUIMainLayerLayout::ShowScale(int nScale)
//{
//    m_dlgTools.ShowStatus(m_pUILayerManager->GetCurrentSelectCol(), nScale);
//}
void CUIMainLayerLayout::LayerUIEvent_SetActionBlockDataManager(CActionBlockDataManager* pManager, int nCurrentGroupID,int nCurrentActionID, int nOffset)
{
    if (m_pEvent)
    {
        m_pEvent->ILayerManager_SetActionBlockDataManager(pManager, nCurrentGroupID, nCurrentActionID, nOffset);
    }
}
//void CUIMainLayerLayout::LayerUIEvent_SetScrollBarPos(int nPos)
//{
////    m_toolsWidget.SetHScrollPos(m_pUILayerManager->GetWidget(), m_pUILayerManager->GetCellCount(),nPos);
//}
//void CUIMainLayerLayout::LayerUIEvent_UpdateScrollBar()
//{
////    m_toolsWidget.ResetHScroll(m_pUILayerManager->GetWidget(), m_pUILayerManager->GetCellCount());
//}

//void CUIMainLayerLayout::LayerManager_DeleteLayer(int nLayerID)
//{
//    if (m_pUILayerManager)
//    {
//        m_pUILayerManager->DeleteActionLayer(nLayerID);
//    }
//}

void CUIMainLayerLayout::LayerManager_Set3DRobotViewInterface(IUBXRobot3DView* pRobotView)
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->Set3DRobotViewInterface(pRobotView);
    }
}

void CUIMainLayerLayout::LayerManager_SetTreeView(ILibTreeNavigation* pView)
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->SetTreeView(pView);
    }
}

//void CUIMainLayerLayout::LayerManager_TreeToView(char *pData, int nLen, QPoint* nPoint, QString FileName)
//{
//    if (m_pUILayerManager)
//    {
//        m_pUILayerManager->TreeToView(pData, nLen, nPoint, FileName);
//    }
//}

void CUIMainLayerLayout::LayerDebugStop(int layerType)
{
    m_StopDebugCount++;
    if (m_pEvent)
    {
// 		if (m_StopDebugCount == 2)
// 		{
//        m_toolsWidget.StopTotalDebug(layerType);
        m_StopDebugCount = 0;
        /*}*/
        m_pEvent->ILayerManager_LayerDebugStop(layerType);
    }
}

ILayerOptionHandler* CUIMainLayerLayout::LayerUIEvent_GetOptionHandler()
{
    return m_pEvent->ILayerManager_GetOptionHandler();
}

/**************************************************************************
* 函数名：LayerUIEvent_MusicHasLoaded
* 功能: 音乐加载完成事件通知
* 参数:
*    @[in ] strMp3Path: 音乐路径
*    @[in ] strTitle: 音乐标题
*    @[in ] nTime: 音乐总时长
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUIMainLayerLayout::LayerUIEvent_MusicHasLoaded(const QString& strMp3Path, const QString& strTitle, int nTime)
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->IUILayerManager_MusicHasLoaded(strMp3Path, strTitle, nTime);
    }
}

void CUIMainLayerLayout::LayerManager_SetSocketSession(CIUBXSocketSession* pSession)
{
//    m_pSocketSession = pSession;
    if (m_pUILayerManager)
    {
        m_pUILayerManager->IUILayerManager_SetSocketSession(pSession);
    }
}

//void CUIMainLayerLayout::LayerManager_UpdateScrollBar()
//{
//    if (m_pUILayerManager)
//    {
//        m_pUILayerManager->UpdateScrollBar();
//    }
//}

