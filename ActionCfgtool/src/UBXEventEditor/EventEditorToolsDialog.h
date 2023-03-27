#pragma once

#include "../../UBXLanguage/UBXLChs/resource.h"
#include "EventManager.h"
#include "EventHandlerListDlg.h"
//#include "resource.h"

// CEventEditorToolsDialog 对话框

class CEventEditorToolsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CEventEditorToolsDialog)

public:
	CEventEditorToolsDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEventEditorToolsDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_EVENTEDITOR_TOOLS };

	CEventManager*	m_pEventManager;

	// 根据用户配置的事件处理LIST，更新相应的模块
	void UpdateEventHandlerList();
	void ShowConfigureWindow(bool bShow);

private:
	CComboBox m_cmbEventHandler;
	//CEventHandlerListDlg	m_dlgHandleList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAddEventHandler();
	afx_msg void OnCbnSelchangeComboEvent();
	afx_msg void OnBnClickedShowConfigureWindow();
	virtual void OnOK();
	virtual void OnCancel();
};
