#ifndef EVENTEDITORMAINDIALOG
#define EVENTEDITORMAINDIALOG

#include "EventEditorToolsDialog.h"
#include "EventManager.h"

// CEventEditorMainDialog 对话框

class CEventEditorMainDialog : public CDialog, public IUBXEventEditor, public CEventManager
{
	DECLARE_DYNAMIC(CEventEditorMainDialog)

public:
	CEventEditorMainDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEventEditorMainDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_EVENTEDITOR_MAIN };

public:
	//IUBXEventEditor 的实现
	virtual bool Connect(CWnd* pwnd);
	virtual void ReleaseConnection();
	virtual CWnd* GetCWnd();
	virtual void GetData(char** ppData, int& nLen);
	virtual void SetData(char* pData, int nLen);
	virtual void Set3DRobotView(IUBXRobot3dView* pView);
	virtual void SetTreeView(ITreeNavigation* pView);
	virtual void ClearFlowChart();
	virtual IUBXFlowControl* GetEventFlowCtr();
	virtual void SetSocketSession(CIUBXSocketSession* pSession);

	// 重写MANAGER方法
	virtual void OnEventHandlerSelChange(CIEventHandler* pEvent);
	virtual void OnShowConfigureWindow(bool Show);
	virtual bool IsShowConfigure(){return m_bShowConfigureUI;};

private:
	void SaveAllDataToEvent();

private:
	CEventEditorToolsDialog	m_dlgTools;
	CIEventHandler*			m_pCurrentEventHandler;
	IUBXFlowControl*		m_pFlowControl;
	// 是否显示配置UI
	bool					m_bShowConfigureUI;	
	CWnd*					m_pWndLastConfigueShow;
	CIUBXSocketSession*		m_pSocketSession;
	IUBXRobot3dView*		m_pRobot3DView;
	ITreeNavigation*		m_pTreeNavigation;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
};

#endif
