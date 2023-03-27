// EventEditorMainDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "UBXEventEditor.h"
#include "EventEditorMainDialog.h"
#include "afxdialogex.h"
#include "EventHandlerInfrared.h"
#include "EventHandlerSonar.h"

bool CreateEventEditor(IUBXEventEditor** ppEventEditor)
{
	CEventEditorMainDialog* pProxy = new CEventEditorMainDialog();
	if (pProxy)
	{
		*ppEventEditor = pProxy;

		return true;
	}

	return false;
}

// CEventEditorMainDialog 对话框

IMPLEMENT_DYNAMIC(CEventEditorMainDialog, CDialog)

CEventEditorMainDialog::CEventEditorMainDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEventEditorMainDialog::IDD, pParent)
	, m_pCurrentEventHandler(NULL)
	, m_pFlowControl(NULL)
	, m_bShowConfigureUI(true)
	, m_pWndLastConfigueShow(NULL)
	, m_pSocketSession(NULL)
	, m_pRobot3DView(NULL)
	, m_pTreeNavigation(NULL)
{

}

CEventEditorMainDialog::~CEventEditorMainDialog()
{
	if (m_pFlowControl)
	{
		m_pFlowControl->Disconnect();
		m_pFlowControl = NULL;
	}
}

void CEventEditorMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEventEditorMainDialog, CDialog)
	ON_WM_SIZE()
//	ON_WM_PAINT()
END_MESSAGE_MAP()


bool CEventEditorMainDialog::Connect(CWnd* pwnd)
{
	if (GetSafeHwnd())
	{
		return false;
	}

	if (Create(CEventEditorMainDialog::IDD, pwnd))
	{
		return true;
	}

	return false;
}

void CEventEditorMainDialog::ReleaseConnection()
{
	DestroyWindow();

	delete this;
}

CWnd* CEventEditorMainDialog::GetCWnd()
{
	return this;
}
// CEventEditorMainDialog 消息处理程序


BOOL CEventEditorMainDialog::OnInitDialog()
{
	__super::OnInitDialog();

	if (CreateFlowControl(&m_pFlowControl))
	{
		m_pFlowControl->Connect(this, NULL);
	}

	CIEventHandler *pInfraredEvent = new CEventHandlerInfrared();
	pInfraredEvent->Connect(this);
	AddEventHandler(pInfraredEvent);

	pInfraredEvent = new CEventHandlerSonar();
	pInfraredEvent->Connect(this);
	AddEventHandler(pInfraredEvent);

	m_dlgTools.m_pEventManager = this;
	m_dlgTools.Create(CEventEditorToolsDialog::IDD, this);
	m_dlgTools.ShowWindow(SW_SHOW);

	//m_dlgTools.ShowConfigureWindow(true);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CEventEditorMainDialog::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if (m_dlgTools.GetSafeHwnd() == NULL)
		return;

	int nHightOffset = 0;

	CRect rcTools;
	m_dlgTools.GetWindowRect(rcTools);
	ScreenToClient(rcTools);
	//m_dlgTools.MoveWindow(0, 0, cx, rcTools.Height());
	m_dlgTools.SetWindowPos(NULL, 0, nHightOffset, cx, rcTools.Height(), SWP_SHOWWINDOW);
	nHightOffset += (rcTools.Height()+2);

	// 如果当前没有选中，则不显示
	if (m_pCurrentEventHandler == NULL)
	{
		if (m_pWndLastConfigueShow)
		{
			m_pWndLastConfigueShow->ShowWindow(SW_HIDE);
			m_pWndLastConfigueShow = NULL;
		}

		if (m_pFlowControl)
		{
			m_pFlowControl->GetCWnd()->ShowWindow(SW_HIDE);
		}

		return;
	}

	if (m_bShowConfigureUI)
	{
		if (m_pCurrentEventHandler && m_pCurrentEventHandler->EventHandler_GetCWnd())
		{
			CWnd* pWnd = m_pCurrentEventHandler->EventHandler_GetCWnd();
			CRect rcWnd;
			pWnd->GetWindowRect(rcWnd);
			ScreenToClient(rcWnd);

			pWnd->SetWindowPos(NULL, 0, nHightOffset, cx, rcWnd.Height(), SWP_SHOWWINDOW);
			nHightOffset += (rcWnd.Height()+2);
		}
	}
	else
	{
		if (m_pCurrentEventHandler && m_pCurrentEventHandler->EventHandler_GetCWnd())
		{
			m_pCurrentEventHandler->EventHandler_GetCWnd()->ShowWindow(SW_HIDE);
		}
	}

	// FlowConrol ui
	if (m_pFlowControl)
	{
		m_pFlowControl->GetCWnd()->SetWindowPos(NULL, 0, nHightOffset, cx, cy - nHightOffset-2, SWP_SHOWWINDOW);
	}
}

void CEventEditorMainDialog::SaveAllDataToEvent()
{
	char* pData = NULL;
	int		nLen = 0;

	if (m_pFlowControl && m_pCurrentEventHandler)
	{
		m_pFlowControl->UBXFlowControl_GetData(&pData, nLen);
		m_pCurrentEventHandler->SetFlowControlDate(pData, nLen);
		if (pData && nLen)
		{
			delete [] pData;
			pData = NULL;
		}
	}
}

void CEventEditorMainDialog::OnEventHandlerSelChange(CIEventHandler* pEvent)
{
	if (pEvent == m_pCurrentEventHandler)
		return;

	char* pData = NULL;
	int		nLen = 0;
	// 保存原来EVENTHANDLER的数据
	if (m_pFlowControl && m_pCurrentEventHandler)
	{
		m_pFlowControl->UBXFlowControl_GetData(&pData, nLen);
		m_pCurrentEventHandler->SetFlowControlDate(pData, nLen);
		if (pData && nLen)
		{
			delete [] pData;
			pData = NULL;
		}
	}

	// 恢复当前的数据
	m_pCurrentEventHandler = pEvent;
	if (pEvent)
	{
		if (m_pFlowControl)
		{
			m_pFlowControl->Disconnect();
			m_pFlowControl = NULL;
		}

		if (CreateFlowControl(&m_pFlowControl))
		{
			m_pFlowControl->Connect(this, NULL);
			m_pFlowControl->Set3DRobotView(m_pRobot3DView);
			m_pFlowControl->SetTreeView(m_pTreeNavigation);
			m_pFlowControl->SetSocketSession(m_pSocketSession);
		}

		nLen = pEvent->GetFlowContolData(&pData);
		if (m_pFlowControl && pData && nLen)
		{
			m_pFlowControl->UBXFlowControl_SetData(pData, nLen);
		}

		if (pData && nLen)
		{
			delete[] pData;
		}
	}

	if (m_pWndLastConfigueShow)
	{
		m_pWndLastConfigueShow->ShowWindow(SW_HIDE);
	}

	if (m_pCurrentEventHandler)
		m_pWndLastConfigueShow = m_pCurrentEventHandler->EventHandler_GetCWnd();

	CRect rcClient;
	GetWindowRect(rcClient);
	ScreenToClient(rcClient);
	OnSize(0, rcClient.Width(), rcClient.Height());
}

void CEventEditorMainDialog::GetData(char** ppData, int& nLen)
{
	SaveAllDataToEvent();

	vector<char> DataSave;

	// 保存长度，后面再修改成实际的长度
	AddVectorData(&DataSave, (char*)&nLen, sizeof(nLen));
	// 保存已经配置过的事件
	int nEventCount = GetHasConfigureEventCount();
	AddVectorData(&DataSave, (char*)&nEventCount, sizeof(nEventCount));

	// 保存每一项的数据
	for (int i=0; i<m_lstEventHandler.GetCount(); i++)
	{
		CIEventHandler* pEvent = GetAt(i);
		if (pEvent == NULL)
			continue;

		if (pEvent->EventHandler_IsConfigure())
		{
			char* pEventData = NULL;
			int nEventDataLen = 0;

			// 保存类型
			int nType = pEvent->EventHandler_GetType();
			AddVectorData(&DataSave, (char*)&nType, sizeof(nType));

			pEvent->GetData(&pEventData, nEventDataLen);
			// 保存长度
			AddVectorData(&DataSave, (char*)&nEventDataLen, sizeof(nEventDataLen));
			// 保存数据
			if (pEventData && nEventDataLen)
			{
				AddVectorData(&DataSave, pEventData, nEventDataLen);

				delete [] pEventData;
			}
		}
	}

	nLen = DataSave.size();
	// 修改成实际的长度
	ModifyVertorData(DataSave, 0, (char*)&nLen, sizeof(nLen));
	if (nLen)
	{
		*ppData = new char[nLen];
		CopyVertorToPointer(DataSave, *ppData);
	}

	DataSave.clear();
}

void CEventEditorMainDialog::SetData(char* pData, int nLen)
{
	int nTotalLen = 0;
	UBX_MEMCPY_INC(nTotalLen, pData);
	if (nTotalLen != nLen)
		return;

	int nEventCount = 0;
	UBX_MEMCPY_INC(nEventCount, pData);

	for (int i=0; i<nEventCount; i++)
	{
		// 类型
		int nType = 0;
		UBX_MEMCPY_INC(nType, pData);

		char* pEventData = NULL;
		int nEventDataLen = 0;
		// 获取长度
		UBX_MEMCPY_INC(nEventDataLen, pData);
		if (nEventDataLen)
		{
			// 数据
			pEventData = new char[nEventDataLen];
			memcpy(pEventData, pData, nEventDataLen);
			pData += nEventDataLen;

			CIEventHandler* pEvent = FindEventHandler(nType);
			if (pEvent)
			{
				pEvent->SetData(pEventData, nEventDataLen);
				pEvent->EventHandler_SetConfigure(true);
			}

			delete [] pEventData;
		}
	}

	m_dlgTools.UpdateEventHandlerList();
}

void CEventEditorMainDialog::OnShowConfigureWindow(bool Show)
{
	m_bShowConfigureUI = Show;
	
	CRect rcClient;
	GetWindowRect(rcClient);
	ScreenToClient(rcClient);
	OnSize(0, rcClient.Width(), rcClient.Height());
}

void CEventEditorMainDialog::Set3DRobotView(IUBXRobot3dView* pView)
{
	m_pRobot3DView = pView;
	if (m_pFlowControl)
	{
		m_pFlowControl->Set3DRobotView(pView);
	}
}

void CEventEditorMainDialog::SetTreeView(ITreeNavigation* pView)
{
	m_pTreeNavigation = pView;
	if (m_pFlowControl)
	{
		m_pFlowControl->SetTreeView(pView);
	}
}

void CEventEditorMainDialog::ClearFlowChart()
{
	if (m_pFlowControl)
	{
		m_pFlowControl->ClearFlowChart();
	}
}

IUBXFlowControl* CEventEditorMainDialog::GetEventFlowCtr()
{
	return m_pFlowControl;
}


void CEventEditorMainDialog::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//__super::OnOK();
}


void CEventEditorMainDialog::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//__super::OnCancel();
}

void CEventEditorMainDialog::SetSocketSession(CIUBXSocketSession* pSession)
{
	m_pSocketSession = pSession;
	if (m_pFlowControl)
	{
		m_pFlowControl->SetSocketSession(pSession);
	}
}

void CEventEditorMainDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect   rc;
	GetClientRect( &rc );// 获取客户区
	dc.FillSolidRect(&rc, RGB(250, 250, 220));

	CDialog::OnPaint();
}
