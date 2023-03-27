// EventHandlerSonarUIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UBXEventEditor.h"
#include "EventHandlerSonarUIDlg.h"
#include "afxdialogex.h"


// CEventHandlerSonarUIDlg 对话框

IMPLEMENT_DYNAMIC(CEventHandlerSonarUIDlg, CDialog)

	CEventHandlerSonarUIDlg::CEventHandlerSonarUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEventHandlerSonarUIDlg::IDD, pParent)
{

}

CEventHandlerSonarUIDlg::~CEventHandlerSonarUIDlg()
{
}

void CEventHandlerSonarUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_NUM, m_edDistance);
}


BEGIN_MESSAGE_MAP(CEventHandlerSonarUIDlg, CDialog)
	ON_EN_KILLFOCUS(IDC_ED_NUM, OnKillfocusNum)
END_MESSAGE_MAP()


// CEventHandlerInfrareUIDlg 消息处理程序
void CEventHandlerSonarUIDlg::GetData(char** ppData, int& nLen)
{
	vector<char> DataSave;

	int nDistance;

	int nLens = 0;
	// 总长度, 后面再修改成真实的长度
	AddVectorData(&DataSave, (char*)&nLens, sizeof(nLens));

	CString strDistance;
	m_edDistance.GetWindowText(strDistance);
	nDistance = _wtoi(strDistance);

	AddVectorData(&DataSave, (char*)&nDistance, sizeof(nDistance));


	nLen = DataSave.size();
	ModifyVertorData(DataSave, 0, (char*)&nLen, sizeof(nLen));
	*ppData = new char[nLen];
	if (*ppData)
	{
		CopyVertorToPointer(DataSave, *ppData);
	}
}

void CEventHandlerSonarUIDlg::SetData(char* pData, int nLen)
{
	int nLens = 0;
	UBX_MEMCPY_INC(nLens, pData);

	if (nLens != nLen)
		return;

	int nDistance = 0;
	UBX_MEMCPY_INC(nDistance, pData);

	CString strDistance;
	strDistance.Format(L"%d",nDistance);
	m_edDistance.SetWindowTextW(strDistance);
}

BOOL CEventHandlerSonarUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edDistance.SetWindowTextW(L"1000");
	GetDlgItem(ID_TEXT_DESTANCE_LESS_THAN_SONAR)->SetWindowText(language_loadstring(IDS_STRING_DESTANCE_LESS_THAN));
	GetDlgItem(ID_TXET_TRIGGER_SONAR)->SetWindowText(language_loadstring(ID_TXET_TRIGGER_SONAR));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CEventHandlerSonarUIDlg::OnKillfocusNum()
{
	CString strValue;

	m_edDistance.GetWindowTextW(strValue);
	if (strValue.IsEmpty())
		m_edDistance.SetWindowTextW(L"1000");
}


void CEventHandlerSonarUIDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}


void CEventHandlerSonarUIDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnCancel();
}
