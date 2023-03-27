﻿#pragma once
#include "ieventhandler.h"
#include "EventHandlerSonarUIDlg.h"

class CEventHandlerSonar :
	public CIEventHandler
{
public:
	CEventHandlerSonar(void);
	~CEventHandlerSonar(void);

public:
	//IEventHandler 接口实现
	virtual bool Connect(CWnd* pWnd);
	virtual void ReleaseConnection();
	virtual LPCWSTR EventHandler_GetDescription();
	virtual int EventHandler_GetType();
	virtual CWnd* EventHandler_GetCWnd();
	virtual void GetData(char** ppData, int& nLen);
	virtual void SetData(char* pData, int nLen);
	virtual void SetConfigureData(char* pData, int nLen);
	virtual int GetConfigureData(char** ppData);

private:
	CEventHandlerSonarUIDlg*	m_pdlgUIConfigure;
	TCHAR m_strDescription[MAX_PATH];
};

