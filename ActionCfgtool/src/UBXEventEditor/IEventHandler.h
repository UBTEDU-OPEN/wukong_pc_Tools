#pragma once

#define EVENTHANDLE_TYPE_INFRARE	0			// 红外距离
#define EVENTHANDLE_TYPE_SONAR		1			// 声纳

class CIEventHandler
{
public:
	CIEventHandler();
	~CIEventHandler();

	virtual bool Connect(CWnd* pWnd) = 0;
	virtual void ReleaseConnection() = 0;
	virtual void GetData(char** ppData, int& nLen) = 0;
	virtual void SetData(char* pData, int nLen) = 0;
	virtual void SetConfigureData(char* pData, int nLen) = 0;
	virtual int GetConfigureData(char** ppData) = 0;
	// 获取事件描述信息
	virtual LPCWSTR EventHandler_GetDescription() = 0;
	// 获取配置窗口
	virtual CWnd* EventHandler_GetCWnd() = 0;
	// 获取类型信息
	virtual int EventHandler_GetType(){return m_nType;};
	virtual bool EventHandler_IsConfigure(){return m_bIsConfigure;};
	virtual void EventHandler_SetConfigure(bool bSet){m_bIsConfigure = bSet;};
	virtual void SetFlowControlDate(char* pData, int nLen);
	virtual int GetFlowContolData(char** ppData);

protected:
	// 是否已经配置
	bool	m_bIsConfigure;
	// 参照宏定义
	int		m_nType;		
	// 事件触发的处理数据
	char*	m_pFlowControlData;
	// 事件触发处理数据的长度
	int		m_nFlowControlDataLen;
};
