#ifndef UBXEVENTEDITOR_H
#define UBXEVENTEDITOR_H

#include "ubxeventeditor_global.h"
#include <QWidget>
#include "libflowcontrol.h"

//事件编辑器的通知接口
class UBXEVENTEDITORSHARED_EXPORT IUBXEventEditorNotify
{
public:
    /**************************************************************************
    * 函数名: getEventFlowData
    * 功能: 获取事件流程图数据
    * 参数:
    *    @[in ] nEventType: 事件类型
    *    @[out] ppData: 数据地址
    *    @[out] nLen: 数据长度
    * 返回值: void
    * 时间: 2016/11/05
    * 作者: ZDJ
    */
    virtual void getEventFlowData(int nEventType, char** ppData, int& nLen) = 0;

    /**************************************************************************
    * 函数名: setEventFlowData
    * 功能: 设置事件流程图数据
    * 参数:
    *    @[in ] nEventType: 事件类型
    *    @[in ] pData: 数据地址
    *    @[in ] nLen: 数据长度
    * 返回值: void
    * 时间: 2016/11/05
    * 作者: ZDJ
    */
    virtual void setEventFlowData(int nEventType, char* pData, int nLen) = 0;

    /**************************************************************************
    * 函数名: showEventPage
    * 功能: 显示事件流程图页面
    * 参数:
    *    @[in ] nIndex: 页面索引
    * 返回值: void
    * 时间: 2016/11/05
    * 作者: ZDJ
    */
    virtual void showEventPage(int nIndex) = 0;

    /**************************************************************************
    * 函数名: enableFlowChart
    * 功能: 让流程图变为可用姿态
    * 参数:
    *    @[in ] nEventType: 事件类型
    * 返回值: void
    * 时间: 2017/3/22
    * 作者: wanxf
    */
    virtual void enableFlowChart(int nEventType) = 0;
};

class UBXEVENTEDITORSHARED_EXPORT IUBXEventEditor
{
public:
    virtual bool Connect(QWidget* pWnd, IUBXEventEditorNotify* pNotify) = 0;
    virtual void ReleaseConnection() = 0;
    virtual QWidget* GetWidget() = 0;
    virtual void GetData(char** ppData, int& nLen) = 0;
    virtual void SetData(char* pData, int nLen) = 0;
//	virtual void Set3DRobotView(IUBXRobot3dView* pView) = 0;
//	virtual void SetTreeView(ITreeNavigation* pView) = 0;
//	virtual void ClearFlowChart() = 0;
//	virtual IUBXFlowControl* GetEventFlowCtr() = 0;
//	virtual void SetSocketSession(CIUBXSocketSession* pSession) = 0;
//    virtual void setFlowControl(IUBXFlowControl* pFlowControl) = 0;
};

bool UBXEVENTEDITORSHARED_EXPORT CreateEventEditor(IUBXEventEditor** ppEventEditor);

#endif
