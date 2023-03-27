/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称:LayerControlWidget.h
* 创建时间: 2017/1/16
* 文件标识:
* 文件摘要:Layer层的控制，包括仿真、添加删除等
*
* 当前版本：1.0.0.0
* 作    者：Joker
* 完成时间：2017/1/16
* 版本摘要：
*/
#ifndef LAYERCONTROLWIDGET_H
#define LAYERCONTROLWIDGET_H

#include "motioneditor_global.h"
#include <QWidget>
#include "stable.h"

namespace Ui {
class CLayerControlWidget;
}

class MOTIONEDITOR_EXPORT CLayerControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CLayerControlWidget(QWidget *parent, emActionLayerType nType, QString strName);
    virtual ~CLayerControlWidget();

    /**************************************************************************
    * 函数名：GetLayerType
    * 功能: 获取Layer类型
    * 参数: 无
    * 返回值: emActionLayerType
    * 时间: 2017/1/16
    * 作者: Joker
    */
    emActionLayerType GetLayerType();

    /**************************************************************************
    * 函数名：InitWidget
    * 功能: 初始化窗体
    * 参数: 无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void InitWidget();

public slots:
    void OnStop();

signals:
    //删除layer
    void SigDeleteLayerClicked(emActionLayerType nType);
    //仿真
    void SigPlayLayerClicked(emActionLayerType nType);
    //暂停
    void SigPauseLayerClicked(emActionLayerType nType);
    //停止仿真
    void SigStopLayerClicked(emActionLayerType nType);
    //刷新仿真UI
    void sigLaberNameClicked(emActionLayerType nType);

protected:
    void paintEvent(QPaintEvent *e);

    bool eventFilter(QObject *obj, QEvent *event);
private slots:
    /**************************************************************************
    * 函数名：onDeleteLayerClicked
    * 功能:删除Layer按钮响应
    * 参数: 无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void onDeleteLayerClicked();
    /**************************************************************************
    * 函数名：onPlayLayerClicked
    * 功能:仿真按钮响应
    * 参数: 无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void onPlayLayerClicked();
    /**************************************************************************
    * 函数名：onPauseLayerClicked
    * 功能:暂停仿真按钮响应
    * 参数: 无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void onPauseLayerClicked();
    /**************************************************************************
    * 函数名：onStopLayerClicked
    * 功能:停止仿真按钮响应
    * 参数: 无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void onStopLayerClicked();

private:
    Ui::CLayerControlWidget *ui;
    emActionLayerType m_nType;
    QString m_strLayerName;
};

#endif // LAYERCONTROLWIDGET_H
