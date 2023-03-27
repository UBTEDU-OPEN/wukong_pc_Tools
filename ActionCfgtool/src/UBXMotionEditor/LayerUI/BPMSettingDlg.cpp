//// BPMSettingDlg.cpp : 实现文件
////

//#include "stdafx.h"
//#include "BPMSettingDlg.h"
//#include "afxdialogex.h"


//// CBPMSettingDlg 对话框

//IMPLEMENT_DYNAMIC(CBPMSettingDlg, CDialogEx)

//CBPMSettingDlg::CBPMSettingDlg(CWnd* pParent /*=NULL*/)
//	: CDialogEx(CBPMSettingDlg::IDD, pParent)
//{
//	m_nBpmValue = (int)(SecsPerMinute / 50);
//}

//CBPMSettingDlg::~CBPMSettingDlg()
//{
//}

//void CBPMSettingDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CDialogEx::DoDataExchange(pDX);
//	DDX_Text(pDX, IDC_EDIT_BPMVALUE, m_nBpmValue);
//	DWORD   dwStyle = GetDlgItem(IDC_EDIT_BPMVALUE)->GetStyle();
//	dwStyle |= ES_NUMBER;
//	SetWindowLong(GetDlgItem(IDC_EDIT_BPMVALUE)->m_hWnd, GWL_STYLE, dwStyle);
//}


//BEGIN_MESSAGE_MAP(CBPMSettingDlg, CDialogEx)
//	ON_BN_CLICKED(IDOK, &CBPMSettingDlg::OnBnClickedOk)
//END_MESSAGE_MAP()


//// CBPMSettingDlg 消息处理程序


//void CBPMSettingDlg::OnBnClickedOk()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CString str;
//	GetDlgItemText(IDC_EDIT_BPMVALUE,str);
//	m_nBpmValue = _wtoi(str);
//	CDialogEx::OnOK();
//}
