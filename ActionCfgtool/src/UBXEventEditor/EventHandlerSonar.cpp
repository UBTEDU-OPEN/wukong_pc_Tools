#include "StdAfx.h"
#include "EventHandlerSonar.h"


CEventHandlerSonar::CEventHandlerSonar(void)
{
	ZeroMemory(m_strDescription, sizeof(m_strDescription));
	CString strDes = language_loadstring(IDS_STRING_EVENTHANDLER_SONAR);
	wcscpy(m_strDescription, strDes.GetBuffer());
}


CEventHandlerSonar::~CEventHandlerSonar(void)
{
}

LPCWSTR CEventHandlerSonar::EventHandler_GetDescription()
{
	//TCHAR strDescription[MAX_PATH] = L"";
	//ZeroMemory(m_strDescription, sizeof(m_strDescription));
	//CString strDes = language_loadstring(IDS_STRING_EVENTHANDLE_INFRARENAME);

	//wcscpy(m_strDescription, strDes.GetBuffer());

	return m_strDescription;
}
int CEventHandlerSonar::EventHandler_GetType()
{
	return EVENTHANDLE_TYPE_SONAR;
}

bool CEventHandlerSonar::Connect(CWnd* pWnd)
{
	m_pdlgUIConfigure = new CEventHandlerSonarUIDlg();
	if (m_pdlgUIConfigure->Create(CEventHandlerSonarUIDlg::IDD, pWnd))
		return true;

	return false;
}

void CEventHandlerSonar::ReleaseConnection()
{
	if (m_pdlgUIConfigure)
	{
		m_pdlgUIConfigure->DestroyWindow();
		delete m_pdlgUIConfigure;
	}
}

CWnd* CEventHandlerSonar::EventHandler_GetCWnd()
{
	return m_pdlgUIConfigure;
}

void CEventHandlerSonar::GetData(char** ppData, int& nLen)
{
	vector<char> DataSave;

	char* pConfigureData = NULL;
	int nConfigureLen = 0;
	if (m_pdlgUIConfigure)
	{
		m_pdlgUIConfigure->GetData(&pConfigureData, nConfigureLen);
	}

	char* pFlowChartData = NULL;
	int nFlowCharDataLen = 0;
	nFlowCharDataLen = GetFlowContolData(&pFlowChartData);

	// 先保存长度,后面再修改实际的长度
	AddVectorData(&DataSave, (char*)&nLen, sizeof(nLen));

	// 保存配置数据
	AddVectorData(&DataSave, (char*)&nConfigureLen, sizeof(nConfigureLen));
	if (nConfigureLen && pConfigureData)
	{
		AddVectorData(&DataSave, pConfigureData, nConfigureLen);
		delete [] pConfigureData;
	}

	// 保存FLOWCHAR数据
	AddVectorData(&DataSave, (char*)&nFlowCharDataLen, sizeof(nFlowCharDataLen));
	if (nFlowCharDataLen && pFlowChartData)
	{
		AddVectorData(&DataSave, pFlowChartData, nFlowCharDataLen);
		delete [] pFlowChartData;
	}

	nLen = DataSave.size();
	ModifyVertorData(DataSave, 0, (char*)&nLen, sizeof(nLen));
	if (nLen)
	{
		*ppData = new char[nLen];
		CopyVertorToPointer(DataSave, *ppData);
	}
}

void CEventHandlerSonar::SetData(char* pData, int nLen)
{
	int nTotalLen = 0;
	UBX_MEMCPY_INC(nTotalLen, pData);
	if (nTotalLen != nLen)
		return;

	// 配置数据
	int nConfigureLen = 0;
	char* pConfigureData = NULL;
	UBX_MEMCPY_INC(nConfigureLen, pData);
	if(nConfigureLen)
	{
		pConfigureData = new char[nConfigureLen];
		memcpy(pConfigureData, pData, nConfigureLen);
		pData += nConfigureLen;

		if (m_pdlgUIConfigure)
		{
			m_pdlgUIConfigure->SetData(pConfigureData, nConfigureLen);
		}

		delete [] pConfigureData;
	}

	// 流程图数据
	int nFlowChartDataLen = 0;
	char* pFlowChartData = NULL;
	UBX_MEMCPY_INC(nFlowChartDataLen, pData);
	if (nFlowChartDataLen)
	{
		pFlowChartData = new char[nFlowChartDataLen];
		memcpy(pFlowChartData, pData, nFlowChartDataLen);
		pData += nFlowChartDataLen;

		SetFlowControlDate(pFlowChartData, nFlowChartDataLen);
		delete [] pFlowChartData;
	}
}

void CEventHandlerSonar::SetConfigureData(char* pData, int nLen)
{
	if (m_pdlgUIConfigure)
	{
		m_pdlgUIConfigure->SetData(pData, nLen);
	}
}

int CEventHandlerSonar::GetConfigureData(char** ppData)
{
	int nLen = 0;

	if (m_pdlgUIConfigure)
	{
		m_pdlgUIConfigure->GetData(ppData, nLen);
	}

	return nLen;
}
