#pragma  once

class CIEventHandler;
class CEventManager
{
public:
	~CEventManager();
	// 添加事件处理到列表里
	bool AddEventHandler(CIEventHandler* pHandler);
	// 查找事件处理接口
	CIEventHandler* FindEventHandler(int nType);
	// 清除列表
	void ClearEventHandlerList();
	// 获取总的个数
	int GetEventListCount(){return m_lstEventHandler.GetCount();};
	// 按序号查找事件处理列表
	CIEventHandler* GetAt(int nIndex){return m_lstEventHandler.GetAt(m_lstEventHandler.FindIndex(nIndex));};
	// 按描述查找事件处理列表
	CIEventHandler* GetEventByDescription(LPCWSTR strDescription);
	

	// 设置EVENT为选中状态
	void SetEventHandlerConfigure(int nType, bool bSet);
	// 获取已经配置过的个数
	int GetHasConfigureEventCount();


	// 显示列表改变处理
	virtual void OnEventHandlerSelChange(CIEventHandler* pEvent);
	// 显示/隐藏配置对话框
	virtual void OnShowConfigureWindow(bool Show){};
	// 获取显示配置对话框状态
	virtual bool IsShowConfigure(){return false;};

protected:
	// 添加的列表
	CList<CIEventHandler*, CIEventHandler*>	m_lstEventHandler;
};