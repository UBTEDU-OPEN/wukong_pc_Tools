/*********************************************
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：ubxslider.cpp
* 创建时间：2016/08/22 18:20
* 文件标识：
* 文件摘要：拖动控件
*
* 当前版本：1.0.0.0
* 作    者：hwx
* 完成时间：2016/08/27 18:20
* 版本摘要：
*/

#include "ubxslider.h"

UBXSlider::UBXSlider(QWidget *parent) : QSlider(parent)
{
    setOrientation(Qt::Horizontal);
    setMouseTracking(true); //mouseMoveEvent without press.
}

UBXSlider::~UBXSlider()
{

}

void UBXSlider::enterEvent(QEvent *event)
{
    emit sigOnEnter();
    QSlider::enterEvent(event);
}

void UBXSlider::leaveEvent(QEvent *e)
{
    emit sigOnLeave();
    QSlider::leaveEvent(e);
}

void UBXSlider::mouseMoveEvent(QMouseEvent *event)
{
    const int pixMetric = style()->pixelMetric(QStyle::PM_SliderLength ) - 1;
    int nValue = QStyle::sliderValueFromPosition(minimum(), maximum(), event->pos().x()-pixMetric/2, width()-pixMetric, false);
    emit sigOnHover(event->x(), nValue);
    QSlider::mouseMoveEvent(event);
}

// Based on code from qslider.cpp
void UBXSlider::mousePressEvent(QMouseEvent *event)
{
    qDebug("pressed (%d, %d)", event->pos().x(), event->pos().y());
    if (event->button() == Qt::LeftButton) {
        QStyleOptionSlider opt;
        initStyleOption(&opt);
        const QRect sliderRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
        const QPoint center = sliderRect.center() - sliderRect.topLeft();
        // to take half of the slider off for the setSliderPosition call we use the center - topLeft

        if (!sliderRect.contains(event->pos())) {
            qDebug("accept");
            event->accept();

            int v = pixelPosToRangeValue(pick(event->pos() - center));
            setSliderPosition(v);
            triggerAction(SliderMove);
            setRepeatAction(SliderNoAction);
            emit sliderMoved(v);//TODO: ok?
            emit sliderPressed(); //TODO: ok?
        } else {
            QSlider::mousePressEvent(event);
        }
    } else {
        QSlider::mousePressEvent(event);
    }
}

inline int UBXSlider::pick(const QPoint &pt) const
{
    return orientation() == Qt::Horizontal ? pt.x() : pt.y();
}

// Function copied from qslider.cpp and modified to make it compile
void UBXSlider::initStyleOption_Qt430(QStyleOptionSlider *option) const
{
    if (!option)
        return;

    option->initFrom(this);
    option->subControls = QStyle::SC_None;
    option->activeSubControls = QStyle::SC_None;
    option->orientation = orientation();
    option->maximum = maximum();
    option->minimum = minimum();
    option->tickPosition = (QSlider::TickPosition) tickPosition();
    option->tickInterval = tickInterval();
    option->upsideDown = (orientation() == Qt::Horizontal) ?
                     (invertedAppearance() != (option->direction == Qt::RightToLeft))
                     : (!invertedAppearance());
    option->direction = Qt::LeftToRight; // we use the upsideDown option instead
    option->sliderPosition = sliderPosition();
    option->sliderValue = value();
    option->singleStep = singleStep();
    option->pageStep = pageStep();
    if (orientation() == Qt::Horizontal)
        option->state |= QStyle::State_Horizontal;
}

// Function copied from qslider.cpp and modified to make it compile
int UBXSlider::pixelPosToRangeValue(int pos) const
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect gr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    int sliderMin, sliderMax, sliderLength;
    if (orientation() == Qt::Horizontal) {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    } else {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }
    return QStyle::sliderValueFromPosition(minimum(), maximum(), pos - sliderMin,
                                           sliderMax - sliderMin, opt.upsideDown);
}



