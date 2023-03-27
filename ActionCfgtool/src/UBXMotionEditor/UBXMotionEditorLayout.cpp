#include "stable.h"
#include "UBXMotionEditorLayout.h"
#include "ui_UBXMotionEditorLayout.h"
#include "LayerUI/ILayerUI.h"
#include "LayerUI/IUILayerManager.h"
#include <QFile>
#include <QPainter>
#include <QDir>
#include <QResource>
#include "configs.h"

bool CreateIActionLayerManager(IActionLayerManager** ppManager)
{
    CUBXMotionEditorLayout* pProxy = new CUBXMotionEditorLayout();
    if(pProxy)
    {
        *ppManager = pProxy;
        return true;
    }
    return false;
}


CUBXMotionEditorLayout::CUBXMotionEditorLayout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CUBXMotionEditorLayout),
    m_pMainLayer(NULL)
{
    ui->setupUi(this);

    QString rccPath = CConfigs::getAppSubDirPath(SUBDIR_RES) + QDir::separator() + "UBXMotionEditor.rcc";
    QResource::registerResource(rccPath);
    setDefaultStyle();
}

CUBXMotionEditorLayout::~CUBXMotionEditorLayout()
{
    delete ui;
}

bool CUBXMotionEditorLayout::Connect(QWidget* pParent)
{
    if(CreateLayerOption(&m_pLayerOption))
    {
        m_pLayerOption->Connect(this);
    }
    m_pLayerOption->Connect(this);
    if(CreateLayerManager(&m_pMainLayer))
    {

        m_pMainLayer->Connect(this,this);
    }
    return true;
}
void CUBXMotionEditorLayout::ILayerManager_EnablePlay(int nLayerType ,bool bState)
{
    //    if(m_pLayerControl)
    //    {
    //        m_pLayerControl->LayerControlToolsEvent_EnablePlay(nLayerType , bState);
    //    }
}

void CUBXMotionEditorLayout::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CUBXMotionEditorLayout::setDefaultStyle()
{
    QFile styleSheet(":/res/qss/motiondefault.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        setStyleSheet(styleSheet.readAll());
    }
}
// void CUBXMotionEditorLayout::EnableControlPlay(int nLayerType , bool bState)
// {
// 	if (m_pLayerControl)
// 	{
// 	}
// }
void CUBXMotionEditorLayout::ReleaseConnection()
{


    if (m_pMainLayer)
    {
        m_pMainLayer->ReleaseConnection();
        m_pMainLayer = NULL;
    }

    if (m_pLayerOption)
    {
        m_pLayerOption->ReleaseConnection();
        m_pLayerOption = NULL;
    }

    disconnect();
    delete this;
}

bool CUBXMotionEditorLayout::GetData(char** ppData, int &nLen)
{
    if (m_pMainLayer)
    {
        return m_pMainLayer->GetData(ppData, nLen);
    }

    return false;
}
//void CUBXMotionEditorLayout::AdjustData()
//{
//    if (m_pMainLayer)
//    {
//        //return m_pMainLayer->SetData(pData, nLen);
//        m_pMainLayer->AdjustData();
//    }

//}
bool CUBXMotionEditorLayout::SetData(char* pData, int nLen)
{
    //    if (m_pLayerControl)
    //    {
    //        m_pLayerControl->LayerControl_DeleteAllControl();
    //    }

    if (m_pLayerOption)
    {
        m_pLayerOption->ResetState();
    }

    if (m_pMainLayer)
    {
        return m_pMainLayer->SetData(pData, nLen);
    }

    return true;
}

//void CUBXMotionEditorLayout::ILayerControlEvent_GetUnUseLayers(int **pLayerID, int& nLayerCount)
//{
//    if (m_pMainLayer)
//    {
//        return m_pMainLayer->LayerManager_GetUnUseLayers(pLayerID, nLayerCount);
//    }
//}

//QString CUBXMotionEditorLayout::ILayerControlEvent_GetLayerName(int nLayerID)
//{
//    if (m_pMainLayer)
//    {
//        return m_pMainLayer->LayerManager_GetLayerName(nLayerID);
//    }

//    return "";
//}
//void CUBXMotionEditorLayout::LayerControlEvent_AddMusic()
//{
//    if (m_pMainLayer)
//    {
//        m_pMainLayer->LayerManager_AddMusic();
//    }
//}
//void CUBXMotionEditorLayout::ILayerControlEvent_AddLayer(int nLayerID)
//{
//    if (m_pMainLayer)
//    {
//        m_pMainLayer->LayerManager_AddLayer(nLayerID);
//    }
//}

//void CUBXMotionEditorLayout::ILayerControlEvent_AddLayerControl(int nLayerType)
//{
////        if (m_pLayerControl)
////        {
////            m_pLayerControl->LayerControlToolsEvent_AddLayerControl(nLayerType);
////        }
//}

//void CUBXMotionEditorLayout::LayerControlEvent_DeleteLayer(int nLayerID)
//{
//    if (m_pMainLayer)
//    {
//        m_pMainLayer->LayerManager_DeleteLayer(nLayerID);
//    }
//}

//void CUBXMotionEditorLayout::LayerControlEvent_SetTimeScaleValue(int timeScale)
//{
//    if (m_pMainLayer)
//    {
//        m_pMainLayer->LayerManager_SetTimeScale(timeScale);
//    }
//}

//int CUBXMotionEditorLayout::LayerControlEvent_GetTimeScaleValue()
//{
//    if (m_pMainLayer)
//    {
//        return m_pMainLayer->LayerManager_GetTimeScaleValue();
//    }

//    return -1;
//}
void CUBXMotionEditorLayout::ILayerManager_SetMusicLayerSourceHandler(ILayerOptionHandler* pHandler , int nLayerType , void *pData , int nDataLen , int nDataID)
{
    if (m_pLayerOption)
    {
        m_pLayerOption->SetOptionHandler(pHandler);
    }
}
void CUBXMotionEditorLayout::ILayerManager_DeleteMusciFile(ILayerOptionHandler* pHandler)
{
    //    if (m_pLayerOption)
    //    {
    //        m_pLayerOption->DeleteMusicResource(pHandler);
    //    }
}
void CUBXMotionEditorLayout::ActionLayerManager_SetMusicResource(QString strFilePath)
{
    // 	if(m_pLayerOption->GetOptionHandler())
    // 	{
    // 		QString str;
    // 		str.Format(_T("%s"),szFilePath);
    // 		ILayerOptionHandler *pOptionHandler= m_pLayerOption->GetOptionHandler();
    // 		//pOptionHandler->SetMusicResource(str);
    // 	}
    if (m_pMainLayer)
    {
        //        QString str;
        //        str.sprintf("%s",szFilePath);
        //        str.Format(_T("%s"),szFilePath);
        // 		ILayerOptionHandler *pOptionHandler= m_pLayerOption->GetOptionHandler();
        // 		pOptionHandler->SetMusicResource(str);
        m_pMainLayer->LayerManager_SetMusicResource(strFilePath);
    }
}
void CUBXMotionEditorLayout::ILayerManager_SetLayOptionHandler(ILayerOptionHandler* pHandler)
{
    if (m_pLayerOption)
    {
        m_pLayerOption->SetOptionHandler(pHandler);
    }
}

void CUBXMotionEditorLayout::ILayerManager_OnDeletOptionHandler(ILayerOptionHandler* pHandler)
{
    if (m_pLayerOption)
    {
        m_pLayerOption->OnDeletOptionHandler(pHandler);
    }
}

void CUBXMotionEditorLayout::ILayerManager_SetActionBlockDataManager(CActionBlockDataManager* pManager, int nCurrentGroupID,int nCurrentActionID, int nOffset)
{
    if (m_pLayerOption)
    {
        m_pLayerOption->SetActionBlockDataManager(pManager, nCurrentGroupID,nCurrentActionID, nOffset);
    }
}

void CUBXMotionEditorLayout::ActionLayerManager_Set3DRobotViewInterface(IUBXRobot3DView* pRobotView)
{
    if (m_pMainLayer)
    {
        m_pMainLayer->LayerManager_Set3DRobotViewInterface(pRobotView);
    }
}

void CUBXMotionEditorLayout::ActionLayerManager_SetTreeView(ILibTreeNavigation* pView)
{
    if (m_pMainLayer)
    {
        m_pMainLayer->LayerManager_SetTreeView(pView);
    }
}

//void CUBXMotionEditorLayout::ActionLayerManager_TreeToView(char *pData, int nLen, QPoint* nPoint, QString FileName)
//{
//    if (m_pMainLayer)
//    {
//        m_pMainLayer->LayerManager_TreeToView(pData, nLen, nPoint, FileName);
//    }
//}
void CUBXMotionEditorLayout::ActionLayerManager_SetFileModified(bool bState)
{
    ILayerUI* pLayer = m_pMainLayer->LayerManager_GetLayer(ACTION_LAYER_MOTION);
    if (pLayer == NULL)
        return ;
        IUILayerManager* pLayerManager = pLayer->GetLayerManager();
        if(pLayerManager)
        {
            //pLayerManager->IUILayerManager_SetModifiedState(true);
            //bMofied = pLayerManager->IUILayerManager_GetModifiedState();
            pLayerManager->IUILayerManager_SetModifiedState(bState);
        }
}

//bool CUBXMotionEditorLayout::ActionLayerManager_IsMusicModified()
//{
//    ILayerUI* pLayer = m_pMainLayer->LayerManager_GetLayer(ACTION_LAYER_MUSIC);
//    if (pLayer == NULL)
//        return false;
//        IUILayerManager* pLayerManager = pLayer->GetLayerManager();
//    bool bMofied =false;
//        if(pLayerManager)
//        {
//            bMofied = pLayerManager->IUILayerManager_GetMusicModifiedState();
//        }
//    return bMofied;
//}

//void CUBXMotionEditorLayout::ActionLayerManager_SetMusicModified(bool bState)
//{
//    ILayerUI* pLayer = m_pMainLayer->LayerManager_GetLayer(ACTION_LAYER_MUSIC);
//    if (pLayer == NULL)
//        return ;
//        IUILayerManager* pLayerManager = pLayer->GetLayerManager();
//        if(pLayerManager)
//        {
//            pLayerManager->IUILayerManager_SetMusicModifiedState(bState);
//        }
//}
bool CUBXMotionEditorLayout::ActionLayerManager_IsRobotDebuging()
{
    ILayerUI* pLayer = m_pMainLayer->LayerManager_GetLayer(ACTION_LAYER_MOTION);
    //    if (pLayer == NULL)
    //        return FALSE;
    //    IUILayerManager* pLayerManager = pLayer->GetLayerManager();
    bool bMofied =false;
    //    if(pLayerManager)
    //    {
    //        //pLayerManager->IUILayerManager_SetModifiedState(true);
    //        bMofied = pLayerManager->IUILayerManager_IsRobotDebuging();
    //    }
    return bMofied;
}
bool CUBXMotionEditorLayout::ActionLayerManager_IsFileModified()
{
    ILayerUI* pLayer = m_pMainLayer->LayerManager_GetLayer(ACTION_LAYER_MOTION);
    if (pLayer == NULL)
        return false;
        IUILayerManager* pLayerManager = pLayer->GetLayerManager();
    bool bMofied =false;
        if(pLayerManager)
        {
            //pLayerManager->IUILayerManager_SetModifiedState(true);
            bMofied = pLayerManager->IUILayerManager_GetModifiedState();
        }
    return bMofied;
}

//void CUBXMotionEditorLayout::LayerControlEvent_LayerPlaying(int layerType)
//{
//    if (m_pMainLayer)
//    {
//        //        ILayerOptionHandler* optionHandle = m_pLayerOption->GetOptionHandler();
//        //        m_pMainLayer->LayerManager_LayerPlaying(layerType, optionHandle);
//    }
//}
//void CUBXMotionEditorLayout::LayerControlEvent_LayerStop()
//{
//    if (m_pMainLayer)
//    {
//        m_pMainLayer->LayerManager_LayerStop();
//    }
//}
//void CUBXMotionEditorLayout::LayerControlEvent_StopLayerDebug()
//{
//    if (m_pMainLayer)
//    {
//        m_pMainLayer->LayerManager_StopLayerDebug();
//    }
//}

void CUBXMotionEditorLayout::ILayerManager_LayerDebugStop(int layerType)
{
    //    if (m_pLayerControl && m_pLayerControl->GetWidget())
    //    {
    //        m_pLayerControl->LayerControl_LayerDebugStop(layerType);
    //    }
}

ILayerOptionHandler* CUBXMotionEditorLayout::ILayerManager_GetOptionHandler()
{
    //    return m_pLayerOption->GetOptionHandler();
    return NULL; //临时处理
}


void CUBXMotionEditorLayout::ActionLayerManager_SetSocketSession(CIUBXSocketSession* pSession)
{
    m_pSocketSession = pSession;
    if (m_pMainLayer)
    {
        m_pMainLayer->LayerManager_SetSocketSession(pSession);
    }
}

//void CUBXMotionEditorLayout::LayerControlEvent_SetMusicSource(int nLayerType , void *pData , int nDataLen , int nDataID)
//{
//    if (m_pMainLayer == NULL)
//    {
//        return;
//    }
//    ILayerUI* pLayer = m_pMainLayer->LayerManager_GetLayer(nLayerType);
//    if (pLayer == NULL)
//        return;

//    //    IUILayerManager* pLayerManager = pLayer->GetLayerManager();
//    //    if (pLayerManager == NULL)
//    //        return;
//    //    pLayerManager->IUILayerManager_SetMusicFile(nLayerType,pData,nDataLen,nDataID);
//}
//void CUBXMotionEditorLayout::LayerControlEvent_SetCurrentOption(int layerType)
//{
//    if (m_pMainLayer == NULL)
//        return;

//    ILayerUI* pLayer = m_pMainLayer->LayerManager_GetLayer(layerType);
//    if (pLayer == NULL)
//        return;

//    //    IUILayerManager* pLayerManager = pLayer->GetLayerManager();
//    //    if (pLayerManager == NULL)
//    //        return;

//    //    pLayerManager->IUILayerManager_SwitchLayer(layerType);
//}

//void CUBXMotionEditorLayout::ActionLayerManager_UpdateScrollBar()
//{
////    if (m_pMainLayer)
////    {
////        m_pMainLayer->LayerManager_UpdateScrollBar();
////    }
//}
void CUBXMotionEditorLayout::ActionLayerManager_ReleaseMusicSource()
{
    if (m_pMainLayer)
    {
        //m_pMainLayer->LayerManager_StopLayerDebug();
        //m_pMainLayer->la
        m_pMainLayer->LayerManager_ReleaseMusic();
    }
}
void CUBXMotionEditorLayout::ActionLayerManager_StopDebug()
{
    if (m_pMainLayer)
    {
        m_pMainLayer->LayerManager_StopLayerDebug();
    }
}

void CUBXMotionEditorLayout::ActionLayerManager_SwitchActionLayer(emActionLayerType eActionType)
{
    if (m_pMainLayer)
    {
        m_pMainLayer->LayerManager_SwitchActionLayer(eActionType);
    }
}