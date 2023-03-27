#pragma once
#include "ubxsocketsession_global.h"
#include "type.h"

//通知有广播包数据进入
#define ALPAHA_BROADCASTPACK_ISCOMING    1
#define ALPAHA_BROADCASTPACK_REFRESH    2

class IUBXBroadcastEvent
{
public:
	virtual void OnBroadcastEvent_DevInfo(QString strIpAddress) = 0;
};

class CIUBXBroadcastSocket
{
public:
	virtual bool Initial(HWND m_hWnd,IUBXBroadcastEvent* pEvent) = 0;
	virtual bool Release(void) = 0;
};

 UBXSOCKETSESSIONSHARED_EXPORT bool CreateUBXBroadcastSocket(CIUBXBroadcastSocket **ppSession);
