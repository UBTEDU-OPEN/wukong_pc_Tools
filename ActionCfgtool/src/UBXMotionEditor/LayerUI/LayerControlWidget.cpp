/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称:LayerControlWidget.cpp
* 创建时间: 2017/1/16
* 文件标识:
* 文件摘要:Layer层的控制，包括仿真、添加删除等
*
* 当前版本：1.0.0.0
* 作    者：Joker
* 完成时间：2017/1/16
* 版本摘要：
*/
#include "LayerControlWidget.h"
#include "ui_LayerControlWidget.h"

CLayerControlWidget::CLayerControlWidget(QWidget *parent, emActionLayerType nType, QString strName) :
    QWidget(parent),
    ui(new Ui::CLayerControlWidget),
    m_nType(nType),
    m_strLayerName(strName)
{
    ui->setupUi(this);
//    connect(ui->pbDeleteLayer, &QPushButton::clicked, this, &CLayerControlWidget::onDeleteLayerClicked);
    connect(ui->pbPauseLayer, &QPushButton::clicked, this, &CLayerControlWidget::onPauseLayerClicked);
    connect(ui->pbPlayLayer, &QPushButton::clicked, this, &CLayerControlWidget::onPlayLayerClicked);
    connect(ui->pbStopLayer, &QPushButton::clicked, this, &CLayerControlWidget::onStopLayerClicked);
    ui->labelLayer->installEventFilter(this);
    InitWidget();
}

CLayerControlWidget::~CLayerControlWidget()
{
    delete ui;
}

/**************************************************************************
* 函数名：GetLayerType
* 功能: 获取Layer类型
* 参数: 无
* 返回值: emActionLayerType
* 时间: 2017/1/16
* 作者: Joker
*/
emActionLayerType CLayerControlWidget::GetLayerType()
{
    return m_nType;
}

/**************************************************************************
* 函数名：InitWidget
* 功能: 初始化窗体
* 参数: 无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CLayerControlWidget::InitWidget()
{
//    if (m_nType == ACTION_LAYER_MOTION)
//    {
//        ui->pbDeleteLayer->hide();
//    }
    ui->pbPauseLayer->hide();
    ui->labelLayer->setText(m_strLayerName);
}

/**************************************************************************
* 函数名：onDeleteLayerClicked
* 功能:删除Layer按钮响应
* 参数: 无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CLayerControlWidget::onDeleteLayerClicked()
{
    emit SigDeleteLayerClicked(m_nType);
//    this->deleteLater();
}

/**************************************************************************
* 函数名：onPlayLayerClicked
* 功能:仿真按钮响应
* 参数: 无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CLayerControlWidget::onPlayLayerClicked()
{
    emit SigPlayLayerClicked(m_nType);
    ui->pbPauseLayer->show();
    ui->pbPlayLayer->hide();
}

/**************************************************************************
* 函数名：onPauseLayerClicked
* 功能:暂停仿真按钮响应
* 参数: 无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CLayerControlWidget::onPauseLayerClicked()
{
    ui->pbPlayLayer->show();
    ui->pbPauseLayer->hide();
    emit SigPauseLayerClicked(m_nType);
}

/**************************************************************************
* 函数名：onPlayLayerClicked
* 功能:停止仿真按钮响应
* 参数: 无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CLayerControlWidget::onStopLayerClicked()
{
    emit SigStopLayerClicked(m_nType);
}

void CLayerControlWidget::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(e);
}

bool CLayerControlWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (ui->labelLayer == obj)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            emit sigLaberNameClicked(m_nType);
        }
    }

    return QObject::eventFilter(obj , event);
}

void CLayerControlWidget::OnStop()
{
    ui->pbPauseLayer->hide();
    ui->pbPlayLayer->show();
}
