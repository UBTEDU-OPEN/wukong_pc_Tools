#pragma once
#include "afxwin.h"

//#include "resource.h"
#include "../../UBXLanguage/UBXLChs/resource.h"


// CEventHandlerInfrareUIDlg 对话框

class CEventHandlerSonarUIDlg : public CDialog
{
	DECLARE_DYNAMIC(CEventHandlerSonarUIDlg)

public:
	CEventHandlerSonarUIDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEventHandlerSonarUIDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_EVENTEDITOR_SONARCONFIGURE };

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


