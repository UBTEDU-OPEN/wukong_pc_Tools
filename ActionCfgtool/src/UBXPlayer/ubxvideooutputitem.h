/*********************************************
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：ubxvideooutputwidget.h
* 创建时间：2016/08/22 18:20
* 文件标识：
* 文件摘要：播放器输出控件
*
* 当前版本：1.0.0.0
* 作    者：hwx
* 完成时间：2016/08/27 18:20
* 版本摘要：
*/

#ifndef UBXVIDEOOUTPUTWIDGET_H
#define UBXVIDEOOUTPUTWIDGET_H

#include <QVideoWidget>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>

#include <QAbstractVideoSurface>
#include <QGraphicsItem>

class UBXVideoOutputItem : public QAbstractVideoSurface, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit UBXVideoOutputItem(QGraphicsItem *parentItem = 0);
    ~UBXVideoOutputItem();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

    //video surface
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const override;

    bool start(const QVideoSurfaceFormat &format) override;
    void stop() override;
    bool present(const QVideoFrame &frame) override;

signals:
    /************************************
    * 名称: sigOnMousePressed
    * 功能: 点击视频播放界面的信号
    * 返回:   void
    * 时间:   2016/10/08
    * 作者:   hwx
    ************************************/
    void sigOnMousePressed();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

private:
    QImage::Format imageFormat;
    QSize imageSize;

    QVideoFrame currentFrame;
    bool framePainted;

};

#endif // UBXVIDEOOUTPUTWIDGET_H
