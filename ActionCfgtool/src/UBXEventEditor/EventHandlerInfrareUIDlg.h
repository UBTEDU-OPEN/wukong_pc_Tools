#pragma once
#include "afxwin.h"

//#include "resource.h"
#include "../../UBXLanguage/UBXLChs/resource.h"


// CEventHandlerInfrareUIDlg 对话框

class CEventHandlerInfrareUIDlg : public CDialog
{
	DECLARE_DYNAMIC(CEventHandlerInfrareUIDlg)

public:
	CEventHandlerInfrareUIDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEventHandlerInfrareUIDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_EVENTEDITOR_INFRAREDCONFIGURE };

public:
	void GetData(char** ppData, int& nLen);
	void SetData(char* pData, int nLen);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edDistance;
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusNum();
	virtual void OnOK();
	virtual void OnCancel();
};
