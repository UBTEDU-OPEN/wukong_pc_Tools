/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：UBXEventEditorWidget.h
* 创建时间：2016/11/07
* 文件标识：
* 文件摘要：事件编辑界面
*
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2016/11/07
* 版本摘要：
*/
#ifndef UBXEVENTEDITORWIDGET_H
#define UBXEVENTEDITORWIDGET_H

#include <QWidget>
#include "ubxeventeditor.h"
#include <vector>
using namespace std;

namespace Ui {
class UBXEventEditorWidget;
}

//监听事件数据
typedef struct tagLISTENEVENTDATA
{
    emListenEventType eListenEventType;//监听类型
    int  nListenDistance;  //距离
    QByteArray bytesFlowData; //流程图数据

    tagLISTENEVENTDATA(emListenEventType eListenType, int nDistance)
    {
        eListenEventType = eListenType;
        nListenDistance = nDistance;
    }

}LISTENEVENTDATA;

typedef vector<LISTENEVENTDATA*>  VecListenEventType; //监听事件列表类型

class UBXEventEditorWidget : public QWidget, public IUBXEventEditor
{
    Q_OBJECT

public:
    explicit UBXEventEditorWidget(QWidget *parent = 0);
    ~UBXEventEditorWidget();

    /**************************************************************************
    * 函数名: Connect
    * 功能: 连接通知地址和事件窗口
    * 参数:
    *    @[in ] pWnd: 窗口地址
    *    @[in ] pNotify: 通知地址
    * 返回值: 成功返回true，失败返回false
    * 时间: 2016/11/07
    * 作者: ZDJ
    */
    virtual bool Connect(QWidget* pWnd, IUBXEventEditorNotify* pNotify);

    /**************************************************************************
    * 函数名: ReleaseConnection
    * 功能: 释放通知地址和事件窗口
    * 参数: void
    * 返回值: void
    * 时间: 2016/11/07
    * 作者: ZDJ
    */
    virtual void ReleaseConnection();

    /**************************************************************************
    * 函数名: GetWidget
    * 功能: 获取窗口
    * 参数: void
    * 返回值: 窗口地址
    * 时间: 2016/11/07
    * 作者: ZDJ
    */
    virtual QWidget* GetWidget();

    /**************************************************************************
    * 函数名: GetData
    * 功能: 获取数据
    * 参数:
    *    @[out] ppData: 数据指针地址
    *    @[out] nLen: 数据长度
    * 返回值: void
    * 时间: 2016/11/05
    * 作者: ZDJ
    */
    virtual void GetData(char** ppData, int& nLen);

    /**************************************************************************
    * 函数名: SetData
    * 功能: 设置数据
    * 参数:
    *    @[in ] pData: 数据地址
    *    @[in ] nLen: 数据长度
    * 返回值: void
    * 时间: 2016/11/05
    * 作者: ZDJ
    */
    virtual void SetData(char* pData, int nLen);

//    //设置流程图
//    virtual void setFlowControl(IUBXFlowControl* pFlowControl);

protected:
    virtual void paintEvent(QPaintEvent* pEvent);

private slots:
    /**************************************************************************
    * 函数名: onAddEventBtnClicked
    * 功能: 添加事件按钮点击事件槽
    * 参数: void
    * 返回值: void
    * 时间: 2016/11/05
    * 作者: ZDJ
    */
    void onAddEventBtnClicked();

    /**************************************************************************
    * 函数名: onTabBarClicked
    * 功能: 标签页切换事件槽
    * 参数:
    *    @[in ] nIndex: tab页索引
    * 返回值: void
    * 时间: 2016/11/05
    * 作者: ZDJ
    */
    void onTabBarClicked(int nIndex);

#if 0
    /**************************************************************************
    * 函数名: onEventClicked
    * 功能: 事件点击槽
    * 参数: void
    * 返回值: void
    * 时间: 2016/11/10
    * 作者: ZDJ
    */
    void onEventClicked();
#endif

private:
    /**************************************************************************
    * 函数名: initCtrls
    * 功能: 初始化控件
    * 参数: void
    * 返回值: void
    * 时间: 2016/11/05
    * 作者: ZDJ
    */
    void initCtrls();

    /**************************************************************************
    * 函数名: clearListenEvents
    * 功能: 清空监听事件列表
    * 参数: void
    * 返回值: void
    * 时间: 2016/11/05
    * 作者: ZDJ
    */
    void clearListenEvents();

    /**************************************************************************
    * 函数名: existEvent
    * 功能: 是否存在指定类型的事件
    * 参数:
    *    @[in ] eEventType: 事件类型
    * 返回值: 成功返回true，失败返回false
    * 时间: 2016/11/05
    * 作者: ZDJ
    */
    bool existEvent(emListenEventType eEventType);

    /**************************************************************************
    * 函数名: findEventData
    * 功能: 查找指定类型的事件
    * 参数:
    *    @[in ] eEventType: 事件类型
    * 返回值: 成功返回事件地址，失败返回NULL
    * 时间: 2016/11/05
    * 作者: ZDJ
    */
    LISTENEVENTDATA* findEventData(emListenEventType eEventType);

    /**************************************************************************
    * 函数名: setDefaultStyle
    * 功能: 设置默认界面
    * 参数: void
    * 返回值: void
    * 时间: 2016/11/08
    * 作者: ZDJ
    */
    void setDefaultStyle();

private:
    Ui::UBXEventEditorWidget *ui;
    IUBXEventEditorNotify*    m_pNotify;        //通知地址
    VecListenEventType        m_vecListenEvent; //监听事件列表
    //IUBXFlowControl*         m_pFlowControl;  //流程图控件
};

#endif // UBXEVENTEDITORWIDGET_H
