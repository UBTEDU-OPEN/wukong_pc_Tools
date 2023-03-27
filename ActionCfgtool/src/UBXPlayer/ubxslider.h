/*********************************************
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：ubxslider.h
* 创建时间：2016/08/22 18:20
* 文件标识：
* 文件摘要：拖动控件
*
* 当前版本：1.0.0.0
* 作    者：hwx
* 完成时间：2016/08/27 18:20
* 版本摘要：
*/

#ifndef UBXSLIDER_H
#define UBXSLIDER_H

#include <QSlider>
#include <QPoint>
#include <QEvent>
#include <QMouseEvent>
#include <QStyleOptionSlider>

class UBXSlider : public QSlider
{
    Q_OBJECT
public:
    explicit UBXSlider(QWidget *parent = 0);
    ~UBXSlider();

signals:
    void sigOnEnter();
    void sigOnLeave();
    void sigOnHover(int pos, int value);

public slots:

protected:
    virtual void enterEvent(QEvent* event);
    virtual void leaveEvent(QEvent *eevent);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);

    inline int pick(const QPoint &pt) const;
    int pixelPosToRangeValue(int pos) const;
    void initStyleOption_Qt430(QStyleOptionSlider *option) const;

};

#endif // UBXSLIDER_H
