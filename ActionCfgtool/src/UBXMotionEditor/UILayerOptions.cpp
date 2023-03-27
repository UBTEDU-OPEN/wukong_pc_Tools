#include "UILayerOptions.h"
#include "ui_UILayerOptions.h"

CUILayerOptions::CUILayerOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CUILayerOptions),
    m_pGroupManager(NULL),
    m_pOptionHandler(NULL)
{
    ui->setupUi(this);
    m_pOptionHandler =NULL;
}

CUILayerOptions::~CUILayerOptions()
{
    delete ui;
}


// UILayerOptions.cpp : 实现文件
//

#include "UBXMotionEditor.h"
#include "UILayerOptions.h"
#include "LayerUI/ILayerOptionHandler.h"
#include "OptionsMotionWidget.h"
//#include "OptionsMusicDlg.h"

bool CreateLayerOption(ILayerOption** ppOption)
{
    CUILayerOptions* pProxy = new CUILayerOptions();
    if (pProxy)
    {
        *ppOption = pProxy;

        return true;
    }

    return false;
}

bool CUILayerOptions::Connect(QWidget* pParent)
{
//    if(pParent && pParent->layout())
//    {
//        pParent->layout()->addWidget(this);
//    }
//    else
//    {
//        setParent(pParent);
//    }
    m_pOptionHandler =NULL;
    return true;
}

void CUILayerOptions::ReleaseConnection()
{
    disconnect();
    delete this;
}

QWidget* CUILayerOptions::GetWidget()
{
    return this;
}

void CUILayerOptions::SetActionBlockDataManager(CActionBlockDataManager* pManager, int nCurrentGroupID,int nCurrentActionID, int nOffset)
{
    m_pGroupManager = pManager;
    if (m_pOptionHandler)
    {
        m_pOptionHandler->SetActionBlockDataManager(pManager, nCurrentGroupID,nCurrentActionID, nOffset);
    }
}

void CUILayerOptions::ResetState()
{
    m_pOptionHandler = NULL;
}

void CUILayerOptions::OnDeletOptionHandler(ILayerOptionHandler* pHandler)
{
    if (m_pOptionHandler == pHandler)
        m_pOptionHandler = NULL;
}
void CUILayerOptions::DeleteMusicResource(ILayerOptionHandler* pHandler)
{
    //    COptionsMusicDlg *pDlg = (COptionsMusicDlg*)(pHandler->GetCWnd());
    //    if (pDlg)
    //    {
    //        //pDlg->AddLayerBlock1S(pData , nDataLen , nDataID);
    //        pDlg->DeleteMusicFile();
    //    }
}
void CUILayerOptions::SetMusicOptionSources(ILayerOptionHandler* pHandler , int nLayerType , void *pData , int nDataLen , int nDataID)
{
    //    m_pOptionHandler = pHandler;
    //    COptionsMusicDlg *pDlg = (COptionsMusicDlg*)(pHandler->GetCWnd());
    //    if (pDlg)
    //    {
    //        pDlg->AddLayerBlock1S(pData , nDataLen , nDataID);
    //    }
}
void CUILayerOptions::SetOptionHandler(ILayerOptionHandler* pHandler)
{  
    if (m_pOptionHandler == pHandler)
    {
        return;
    }

    if (m_pOptionHandler)
    {
        if (m_pOptionHandler->GetWidget()->winId())
        {
            m_pOptionHandler->GetWidget()->hide();
        }
    }

    m_pOptionHandler = pHandler;
    if (m_pOptionHandler == NULL)
        return;

    m_pOptionHandler->GetWidget()->show();
    update();

    //PostMessage(WM_SIZE, 0, 0);
}

ILayerOptionHandler* CUILayerOptions::GetOptionHandler()
{
    return m_pOptionHandler;
}
