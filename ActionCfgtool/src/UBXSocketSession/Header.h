#pragma once

//#include "libpackclass_global.h"

class CHeader
{
public:
	CHeader(void);
	~CHeader(void);

	bool SetMsg(const char* pData, int nLen);
	int GetHeaderData(char** ppData);
	int SetHeaderData(char * pData ,int nLen , int &nOffset);

public:
	short	m_nFlag;
	int		m_nLength;
	short	m_nVersion;
	short	m_nExtInfo;
	char*	m_pMsg;
	int		m_nMsgLen;
	
	bool PrintInHex(char* pData, int nLen);
};

