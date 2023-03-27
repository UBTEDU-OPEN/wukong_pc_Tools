#pragma once
#include "afxcmn.h"
//#include "resource.h"
#include "../../UBXLanguage/UBXLChs/resource.h"
#include "EventManager.h"

// CEventHandlerListDlg 对话框

class CEventHandlerListDlg : public CDialog
{
	DECLARE_DYNAMIC(CEventHandlerListDlg)

public:
	CEventHandlerListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEventHandlerListDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_EVENTEDITOR_EVENTLIST };

	CEventManager*	m_pEventManager;

	vector<int> m_vecSelecItem;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listEvent;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
