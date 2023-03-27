#include "stdafx.h"
#include "IEventHandler.h"

CIEventHandler::CIEventHandler()
{
	m_bIsConfigure			= false;
	m_pFlowControlData		= NULL;
	m_nFlowControlDataLen	= 0;
}

CIEventHandler::~CIEventHandler()
{
	if (m_pFlowControlData)
	{
		delete [] m_pFlowControlData;
		m_pFlowControlData = NULL;
		m_nFlowControlDataLen = 0;
	}
}

void CIEventHandler::SetFlowControlDate(char* pData, int nLen)
{
	if (m_pFlowControlData)
	{
		delete [] m_pFlowControlData;
		m_pFlowControlData = NULL;
	}

	m_nFlowControlDataLen = nLen;
	if (pData && nLen)
	{
		m_pFlowControlData = new char[nLen];
		if (m_pFlowControlData == NULL)
			return;

		memcpy(m_pFlowControlData, pData, nLen);
	}
}

int CIEventHandler::GetFlowContolData(char** ppData)
{
	if (ppData == NULL)
		return 0;

	if (m_nFlowControlDataLen)
	{
		*ppData = new char[m_nFlowControlDataLen];
		ASSERT(*ppData);
		memcpy(*ppData, m_pFlowControlData, m_nFlowControlDataLen);
	}

	return m_nFlowControlDataLen;
}