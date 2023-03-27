#pragma once

#include "motioneditor_global.h"
#include "IGroupDataManagerPlay.h"
#include "IGroupDataPlay.h"
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class MOTIONEDITOR_EXPORT CUpdateProcess: public QObject
{
    Q_OBJECT
public:
    CUpdateProcess(IUILayerManager* pLayerManager);
    ~CUpdateProcess(void);

public:
    void CUpdateTimeLine(quint64 nPosition, int nType);

signals:
    void SigUpdateTimeLine(quint64 nPosition, int nType);
};

class CActionBlockGroupManager;
class CGroupDataManagerPlay :public QThread, public IGroupDataManagerPlay
{
public:
	CGroupDataManagerPlay(void);
	~CGroupDataManagerPlay(void);

public:
	// IGroupDataManagerPlay接口实现
    /**************************************************************************
    * 函数名：Connection
    * 功能: 建立联系，初始化必要的参数
    * 参数:
    *    @[in ] pDataManager: LayerUI管理者
    *    @[in ] pEvent: 仿真事件响应者
    *    @[in ] PlayHandle: 仿真执行句柄
    *    @[in ] bSingleLayer: 是否为单层仿真
    * 返回值: 无
    * 时间: 2017/2/26
    * 作者: wanxf
    */
    virtual bool Connection(ILayerUI* pDataManager, IGroupDataManagerPlay_Event* pEvent, void* PlayHandle, bool bSingleLayer);
	virtual void ReleaseConnection();
	virtual void StartPlay();
	// 正在播放的时候步进值
	virtual bool OnPlaying(int nOffsetTime);
	virtual void StopPlay();
    virtual void PausePlay(emActionLayerType nType);
    virtual void OnResumePlay(emActionLayerType nType);

//    virtual void IGroupDataManagerPlay_SetRobot3DView(IUBXRobot3DView* pSession);
    virtual void IGroupDataManagerPlay_SetLayerManager(IUILayerManager* pLayerManager);

    /**************************************************************************
    * 函数名：IGroupDataManagerPlay_SetSocketSession
    * 功能: 设置Socket对象，以便与机器人通信
    * 参数:
    *    @[in ] pSession: Socket对象指针
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual void IGroupDataManagerPlay_SetSocketSession(CIUBXSocketSession* pSession);

private:
    bool m_bStop;
	// 是否为只播放一层，用于仿真单层
	bool m_bSinglePlay;
    bool m_bPausePlay;

	ILayerUI* m_pLayer;
	IGroupDataManagerPlay_Event* m_pEvent;
    QWaitCondition m_oWait;

	// 当前播放的组数据
	CActionBlockGroupManager* m_pCurrentPlayGroupManager;
	// 当前播放的序号（在列表中的序号）
	int m_nCurrentPlayIndex;
	// 当前已经执行过的时间
	int m_nCurrentConsumeTime;
	// 块的结束时间
	int m_nGroupEndTime;
	// 组开始时间
	int m_nGroupStartTime;
	// 是否已经执行完
    bool m_bIsCompleted;

    int m_nCurrentCol;

	IGroupDataPlay*	m_pGroupDataPlay;

	void* m_PlayHandle;

//    IUBXRobot3DView* m_pRobot3dView;
    IUILayerManager* m_pLayerManager;
    CIUBXSocketSession*	 m_pSocketSession;

    int m_nLayerType;
    CUpdateProcess* m_pUpdateProcess;

    bool GetNextGroupPlay();
	void ReSetState();
    void run();
};

