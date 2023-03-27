/*
* Copyright (c) 2017, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CUILayerCommonContainer.h
* 创建时间：2016/01/12
* 文件标识：
* 文件摘要：公共层窗体界面的容器类，负责各个层UI的创建
*
* 当前版本：1.0.0.0
* 作    者：Joker
* 完成时间：2017/01/17
* 版本摘要：
*/


#ifndef MP3CONTAINER_H
#define MP3CONTAINER_H

#include "motioneditor_global.h"
#include <QWidget>
#include <QScrollBar>
#include <QStyleOption>
#include "UILayerCommonWidget.h"

namespace Ui {
class widgetCommonContainer;
}

class MOTIONEDITOR_EXPORT CUILayerCommonContainer : public QWidget
{
    Q_OBJECT

public:
    CUILayerCommonWidget* GetCUILayerCommonWidget()
    {
        return m_pCommonWidget;
    }

    explicit CUILayerCommonContainer(QWidget *parent = 0);
    ~CUILayerCommonContainer();

public slots:
    //hels 设置滚动条的最大最小范围，以及步进大小
    void OnRangeChanged(int min, int max, int nStep);
    //hels 音乐播放时，同步滚动条的位置
    void OnCellIndexChanged(int index);

    /**************************************************************************
    * 函数名：UpdateScaleAndRealTime
    * 功能:更新时间粒度和实际时间到UI
    * 参数:
    *    @[in ] nScaleValue: 时间粒度
    *    @[in ] nCol: 当前时间格数
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void onUpdateScaleAndRealTime(int nScaleValue, int nCol);

protected:
    //If you subclass from QWidget, you need to provide a paintEvent for your custom QWidget as below:
    void paintEvent(QPaintEvent *)
    {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
    virtual void wheelEvent(QWheelEvent *event);

private:
    Ui::widgetCommonContainer *ui;

    CUILayerCommonWidget* m_pCommonWidget;//公共层窗体指针
    QScrollBar* m_pScrollbar;//滚动条
};

#endif // MP3CONTAINER_H
