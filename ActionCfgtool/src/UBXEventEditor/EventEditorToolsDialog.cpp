// EventEditorToolsDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "UBXEventEditor.h"
#include "EventEditorToolsDialog.h"
#include "afxdialogex.h"
#include "IEventHandler.h"


// CEventEditorToolsDialog 对话框

IMPLEMENT_DYNAMIC(CEventEditorToolsDialog, CDialogEx)

CEventEditorToolsDialog::CEventEditorToolsDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEventEditorToolsDialog::IDD, pParent)
	, m_pEventManager(NULL)
{

}

CEventEditorToolsDialog::~CEventEditorToolsDialog()
{
}

void CEventEditorToolsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_EVENTSELECT, m_cmbEventHandler);
}


BEGIN_MESSAGE_MAP(CEventEditorToolsDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADDEVENT, &CEventEditorToolsDialog::OnBnClickedAddEventHandler)
	ON_BN_CLICKED(IDC_BTN_SHOW_CONFIGURE, &CEventEditorToolsDialog::OnBnClickedShowConfigureWindow)
	ON_CBN_SELCHANGE(IDC_CMB_EVENTSELECT, &CEventEditorToolsDialog::OnCbnSelchangeComboEvent)
END_MESSAGE_MAP()


// CEventEditorToolsDialog 消息处理程序


BOOL CEventEditorToolsDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_BTN_ADDEVENT)->SetWindowTextW(language_loadstring(IDC_BTN_ADDEVENT));
	GetDlgItem(IDC_BTN_SHOW_CONFIGURE)->SetWindowTextW(language_loadstring(IDS_STRING_HIDE));

	//m_dlgHandleList.m_pEventManager = m_pEventManager;
	UpdateEventHandlerList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CEventEditorToolsDialog::OnBnClickedAddEventHandler()
{
	CEventHandlerListDlg dlg;

	dlg.m_pEventManager = m_pEventManager;
	if (dlg.DoModal() == IDOK)
	{
		UpdateEventHandlerList();
	}
}

void CEventEditorToolsDialog::UpdateEventHandlerList()
{
	if (m_pEventManager == NULL)
		return;

	m_cmbEventHandler.ResetContent();

	for (int i=0; i<m_pEventManager->GetEventListCount(); i++)
	{
		CIEventHandler* pEvent = m_pEventManager->GetAt(i);
		if (pEvent == NULL)
			continue;

		if (pEvent->EventHandler_IsConfigure() == false)
			continue;

		CString strDes = pEvent->EventHandler_GetDescription();
		int nItem = m_cmbEventHandler.AddString(strDes);
		m_cmbEventHandler.SetItemData(nItem, pEvent->EventHandler_GetType());
	}

	m_cmbEventHandler.SetCurSel(0);
	OnCbnSelchangeComboEvent();
}

void CEventEditorToolsDialog::OnCbnSelchangeComboEvent()
{
	if (m_pEventManager)
	{
		CIEventHandler* pEvent = m_pEventManager->FindEventHandler(m_cmbEventHandler.GetItemData(m_cmbEventHandler.GetCurSel()));
		m_pEventManager->OnEventHandlerSelChange(pEvent);
	}
}

void CEventEditorToolsDialog::ShowConfigureWindow(bool bShow)
{
	if (bShow)
	{
		GetDlgItem(IDC_BTN_SHOW_CONFIGURE)->SetWindowTextW(language_loadstring(IDS_STRING_HIDE));
	}
	else
	{
		GetDlgItem(IDC_BTN_SHOW_CONFIGURE)->SetWindowTextW(language_loadstring(IDS_STRING_SHOW));
	}
	if (m_pEventManager)
	{
		m_pEventManager->OnShowConfigureWindow(bShow);
	}
}

void CEventEditorToolsDialog::OnBnClickedShowConfigureWindow()
{
	if (m_pEventManager)
	{
		ShowConfigureWindow(!m_pEventManager->IsShowConfigure());
	}
}


void CEventEditorToolsDialog::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


void CEventEditorToolsDialog::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnCancel();
}
