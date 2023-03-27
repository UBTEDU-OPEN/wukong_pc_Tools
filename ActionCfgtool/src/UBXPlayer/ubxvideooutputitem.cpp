/*********************************************
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：ubxvideooutputwidget.cpp
* 创建时间：2016/08/22 18:20
* 文件标识：
* 文件摘要：播放器输出控件
*
* 当前版本：1.0.0.0
* 作    者：hwx
* 完成时间：2016/08/27 18:20
* 版本摘要：
*/

#include <QPainter>
#include <QDebug>

#include "ubxplayerconst.h"
#include "configs.h"
#include "ubxvideooutputitem.h"

#include <QPainter>
#include <QTransform>
#include <QVideoSurfaceFormat>
#include <QGraphicsScene>

UBXVideoOutputItem::UBXVideoOutputItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , imageFormat(QImage::Format_Invalid)
    , framePainted(false)
{
}

UBXVideoOutputItem::~UBXVideoOutputItem()
{
}

QRectF UBXVideoOutputItem::boundingRect() const
{
//    return this->scene()->sceneRect();
    return QRectF(QPointF(0,0), QSizeF(this->scene()->sceneRect().width(), this->scene()->sceneRect().height()));
//    return QRectF(QPointF(0,0), surfaceFormat().sizeHint());
}

void UBXVideoOutputItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (currentFrame.map(QAbstractVideoBuffer::ReadOnly)) {

        const QTransform oldTransform = painter->transform();
        QRectF rectBounding = boundingRect();
        if (surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop) {
           painter->scale(1, -1);
           painter->translate(0, -rectBounding.height());
        }

        QImage imageVideo(currentFrame.bits(), currentFrame.width(), currentFrame.height(), imageFormat);
        QImage imageScaled = imageVideo.scaled(rectBounding.width(), rectBounding.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        painter->drawImage(rectBounding, imageScaled);

        painter->setTransform(oldTransform);

        framePainted = true;
        currentFrame.unmap();
    }
}

QList<QVideoFrame::PixelFormat> UBXVideoOutputItem::supportedPixelFormats(
        QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB32
                << QVideoFrame::Format_ARGB32
                << QVideoFrame::Format_ARGB32_Premultiplied
                << QVideoFrame::Format_RGB565
                << QVideoFrame::Format_RGB555;
    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}

bool UBXVideoOutputItem::start(const QVideoSurfaceFormat &format)
{
    if (isFormatSupported(format)) {
        imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
        imageSize = format.frameSize();
        framePainted = true;

        QAbstractVideoSurface::start(format);

        prepareGeometryChange();

        return true;
    } else {
        return false;
    }
}

void UBXVideoOutputItem::stop()
{
    currentFrame = QVideoFrame();
    framePainted = false;

    QAbstractVideoSurface::stop();
}

bool UBXVideoOutputItem::present(const QVideoFrame &frame)
{
    if (!framePainted) {
        if (!QAbstractVideoSurface::isActive())
            setError(StoppedError);

        return false;
    } else {
        currentFrame = frame;
        framePainted = false;

        update();

        return true;
    }
}

void UBXVideoOutputItem::keyPressEvent(QKeyEvent *event)
{
    QGraphicsItem::keyPressEvent(event);
}

void UBXVideoOutputItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
//    setFullScreen(!isFullScreen());
//    event->accept();
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void UBXVideoOutputItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    emit sigOnMousePressed();
    QGraphicsItem::mousePressEvent(event);
}
