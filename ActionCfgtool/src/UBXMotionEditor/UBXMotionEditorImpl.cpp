#include "stable.h"
#include "UBXMotionEditorImpl.h"

bool CreateUBXMotionEditor(IUBXMotionEditor** ppEditor)
{
    CUBXMotionEditorImpl* pProxy = new CUBXMotionEditorImpl();
    if (pProxy) {
        *ppEditor = pProxy;
        return true;
    }
    return false;
}

CUBXMotionEditorImpl::CUBXMotionEditorImpl(void)
    : m_pUILayerManager(NULL)
    //, m_pActionLayerManager(NULL)
{
}

CUBXMotionEditorImpl::~CUBXMotionEditorImpl()
{
}

bool CUBXMotionEditorImpl::Connect(QWidget* pParent)
{
//    CAutolockRes res;
    if (CreateIActionLayerManager(&m_pUILayerManager))
    {
        m_pUILayerManager->Connect(pParent);
        return true;
    }

    return false;
}

void CUBXMotionEditorImpl::Disconnect()
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->ReleaseConnection();
        m_pUILayerManager = NULL;
    }

    delete this;
}

QWidget* CUBXMotionEditorImpl::GetWidget()
{
    if (m_pUILayerManager)
    {
        return m_pUILayerManager->GetWidget();
    }
    return NULL;
}

bool CUBXMotionEditorImpl::GetData(char** ppData, int &nLen)
{
    if (m_pUILayerManager)
    {
        return m_pUILayerManager->GetData(ppData, nLen);
    }
    return false;
}
//void CUBXMotionEditorImpl::AdjustData()
//{
//    if (m_pUILayerManager)
//    {
//        m_pUILayerManager->AdjustData();
//    }
//}
bool CUBXMotionEditorImpl::SetData(char* pData, int nLen)
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->ActionLayerManager_StopDebug();
    }

    if (m_pUILayerManager)
    {
        m_pUILayerManager->SetData(pData, nLen);
    }
    return false;
}

void CUBXMotionEditorImpl::UBXMotionEditor_Set3DRobotViewInterface(IUBXRobot3DView* pRobotView)
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->ActionLayerManager_Set3DRobotViewInterface(pRobotView);
    }
}

void CUBXMotionEditorImpl::UBXMotionEditor_SetTreeView(ILibTreeNavigation* pView)
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->ActionLayerManager_SetTreeView(pView);
    }
}

//void CUBXMotionEditorImpl::UBXMotionEditor_TreeToView(char *pData, int nLen, QPoint* nPoint, QString FileName)
//{
//    if (m_pUILayerManager)
//    {
//        m_pUILayerManager->ActionLayerManager_TreeToView(pData, nLen, nPoint, FileName);
//    }
//}

void CUBXMotionEditorImpl::UBXMotionEditor_SetSocketSession(CIUBXSocketSession* pSession)
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->ActionLayerManager_SetSocketSession(pSession);
    }
}

//void CUBXMotionEditorImpl::UBXMotionEditor_UpdateScrollBar()
//{
//    if (m_pUILayerManager)
//    {
//        m_pUILayerManager->ActionLayerManager_UpdateScrollBar();
//    }
//}
void CUBXMotionEditorImpl::UBXMotionEditor_SetMusicResource(QString strFilePath)
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->ActionLayerManager_SetMusicResource(strFilePath);
    }
}
void CUBXMotionEditorImpl::UBXMotionEditor_ReleaseMusicSource()
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->ActionLayerManager_ReleaseMusicSource();
    }
}
void CUBXMotionEditorImpl::UBXMotionEditor_StopDebug()
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->ActionLayerManager_StopDebug();
    }
}
void CUBXMotionEditorImpl::UBXMotionEditor_SetProjectModity(bool bState)
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->ActionLayerManager_SetFileModified(bState);
    }
}

//bool CUBXMotionEditorImpl::UBXMotionEditor_IsMusicModity()
//{
//    if (m_pUILayerManager)
//    {
//        return m_pUILayerManager->ActionLayerManager_IsMusicModified();
//    }
//    else
//        return false;
//}

//void CUBXMotionEditorImpl::UBXMotionEditor_SetMusicModity(bool bState)
//{
//    if (m_pUILayerManager)
//    {
//        m_pUILayerManager->ActionLayerManager_SetMusicModified(bState);
//    }
//}
bool CUBXMotionEditorImpl::UBXMotionEditor_IsRobotDebuging()
{
    if (m_pUILayerManager)
    {
        return m_pUILayerManager->ActionLayerManager_IsRobotDebuging();
    }
    else
        return false;
}
bool CUBXMotionEditorImpl::UBXMotionEditor_IsProjectModity()
{
// 	if (m_pUILayerManager)
// 	{
// 		char* pData = NULL;
// 		int nLens = 0;
// 		m_pUILayerManager->GetData(&pData, nLens);
// 		delete [] pData;
//
// 		if (nLens > 200)
// 			return true;
// 	}
//
// 	return false;
    if (m_pUILayerManager)
    {
        return m_pUILayerManager->ActionLayerManager_IsFileModified();
    }
    else
        return false;
}


void CUBXMotionEditorImpl::switchActionLayer(emActionLayerType eActionType)
{
    if (m_pUILayerManager)
    {
        m_pUILayerManager->ActionLayerManager_SwitchActionLayer(eActionType);
    }
}
