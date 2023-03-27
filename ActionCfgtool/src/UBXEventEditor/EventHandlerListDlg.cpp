// EventHandlerListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UBXEventEditor.h"
#include "EventHandlerListDlg.h"
#include "afxdialogex.h"
#include "IEventHandler.h"


// CEventHandlerListDlg 对话框

IMPLEMENT_DYNAMIC(CEventHandlerListDlg, CDialog)

CEventHandlerListDlg::CEventHandlerListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEventHandlerListDlg::IDD, pParent)
	, m_pEventManager(NULL)
{

}

CEventHandlerListDlg::~CEventHandlerListDlg()
{
}

void CEventHandlerListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EVENTHANDLE, m_listEvent);
}


BEGIN_MESSAGE_MAP(CEventHandlerListDlg, CDialog)
END_MESSAGE_MAP()


// CEventHandlerListDlg 消息处理程序


BOOL CEventHandlerListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwStyle = m_listEvent.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl） 
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl） 
	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件 
	m_listEvent.SetExtendedStyle(dwStyle); //设置扩展风格 

	m_listEvent.InsertColumn( 0, L"", LVCFMT_LEFT, 400 );//插入列 

	if (m_pEventManager)
	{
		for (int i=0; i<m_pEventManager->GetEventListCount(); i++)
		{
			CIEventHandler* pEvent = m_pEventManager->GetAt(i);
			if (pEvent == NULL)
				continue;

			//if (!pEvent->EventHandler_IsConfigure())
			{
				CString strDes = pEvent->EventHandler_GetDescription();
				int nItem = m_listEvent.InsertItem(m_listEvent.GetItemCount(), strDes);
				m_listEvent.SetItemData(nItem, pEvent->EventHandler_GetType());
				m_listEvent.SetCheck(nItem, pEvent->EventHandler_IsConfigure());
			}
		}
	}
	SetWindowText(language_loadstring(IDD_DIALOG_EVENTEDITOR_EVENTLIST));
	GetDlgItem(IDOK)->SetWindowText(language_loadstring(IDOK));
	GetDlgItem(IDCANCEL)->SetWindowText(language_loadstring(IDCANCEL));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CEventHandlerListDlg::OnOK()
{
	if (m_pEventManager == NULL)
		return;

	for (int i=0; i<m_listEvent.GetItemCount(); i++)
	{
		m_pEventManager->SetEventHandlerConfigure(m_listEvent.GetItemData(i), m_listEvent.GetCheck(i)? true : false);
	}

	CDialog::OnOK();
}
