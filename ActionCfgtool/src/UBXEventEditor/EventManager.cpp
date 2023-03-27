#include "stdafx.h"
#include "EventManager.h"
#include "IEventHandler.h"

bool CEventManager::AddEventHandler(CIEventHandler* pHandler)
{
	if (pHandler == NULL)
		return false;

	if (FindEventHandler(pHandler->EventHandler_GetType()))
		return false;

	m_lstEventHandler.AddTail(pHandler);

	return true;
}

CEventManager::~CEventManager()
{
	ClearEventHandlerList();
}

CIEventHandler* CEventManager::FindEventHandler(int nType)
{
	CIEventHandler* pFind = NULL;

	for (int i=0; i<m_lstEventHandler.GetCount(); i++)
	{
		pFind = m_lstEventHandler.GetAt(m_lstEventHandler.FindIndex(i));
		if (pFind == NULL)
			continue;

		if (pFind->EventHandler_GetType() == nType)
			return pFind;
	}

	return NULL;
}

CIEventHandler* CEventManager::GetEventByDescription(LPCWSTR strDescription)
{
	CIEventHandler* pFound = NULL;

	for (int i=0; i<m_lstEventHandler.GetCount(); i++)
	{
		pFound = m_lstEventHandler.GetAt(m_lstEventHandler.FindIndex(i));
		if (pFound == NULL)
			continue;

		CString strDes = pFound->EventHandler_GetDescription();
		if (strDes.Compare(strDescription) == 0)
			return pFound;
	}

	return NULL;
}

void CEventManager::ClearEventHandlerList()
{
	for (int i=0; i<m_lstEventHandler.GetCount(); i++)
	{
		CIEventHandler* pFound = m_lstEventHandler.GetAt(m_lstEventHandler.FindIndex(i));
		if (pFound)
		{
			pFound->ReleaseConnection();
			delete pFound;
		}
	}

	m_lstEventHandler.RemoveAll();
}

void CEventManager::SetEventHandlerConfigure(int nType, bool bSet)
{
	CIEventHandler* pEvent = FindEventHandler(nType);
	if (pEvent)
	{
		pEvent->EventHandler_SetConfigure(bSet);
	}
}

void CEventManager::OnEventHandlerSelChange(CIEventHandler* pEvent)
{

}

int CEventManager::GetHasConfigureEventCount()
{
	int nCount = 0;

	for (int i=0; i<m_lstEventHandler.GetCount(); i++)
	{
		CIEventHandler* pEvent = GetAt(i);
		if (pEvent == NULL)
			continue;

		if (pEvent->EventHandler_IsConfigure())
			nCount++;
	}

	return nCount;
}