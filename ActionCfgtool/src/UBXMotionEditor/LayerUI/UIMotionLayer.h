/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CUIMotionLayer.h
* 创建时间：2016/01/12
* 文件标识：
* 文件摘要：动作层类，负责动作层绘制，同步等
*
* 当前版本：1.0.0.0
* 作    者：hels
* 完成时间：2016/01/12
* 版本摘要：
*/


#ifndef ACTIONLAYER_H
#define ACTIONLAYER_H
//#include <QObject>
#include "motioneditor_global.h"
#include "ActionBlockDataManager.h"
#include "ILayerCommonWidget.h"
#include "UILayerImpl.h"
//class CUILayerCommonWidget;

class MOTIONEDITOR_EXPORT CUIMotionLayer : public CUILayerImpl/*, public QObject*/
{
    Q_OBJECT
public:
    CUIMotionLayer(ILayerCommonWidget *mp3Widget);

    //获取层类型
    virtual emActionLayerType GetType();
};


// 头部层
class CUIHeadMotionLayer : public CUILayerImpl/*, public QObject*/
{
    Q_OBJECT
public:
    CUIHeadMotionLayer(ILayerCommonWidget *mp3Widget);

    //获取层类型
    virtual emActionLayerType GetType();
};


// 手部层
class CUIHandMotionLayer : public CUILayerImpl/*, public QObject*/
{
    Q_OBJECT
public:
    CUIHandMotionLayer(ILayerCommonWidget *mp3Widget);

    //获取层类型
    virtual emActionLayerType GetType();
};


// 腿部层
class CUILegMotionLayer : public CUILayerImpl/*, public QObject*/
{
    Q_OBJECT
public:
    CUILegMotionLayer(ILayerCommonWidget *mp3Widget);

    //获取层类型
    virtual emActionLayerType GetType();
};

#endif // ACTIONLAYER_H
