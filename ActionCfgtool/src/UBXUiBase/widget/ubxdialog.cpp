/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CUBXDialog
* 创建时间：20160816
* 文件标识：
* 文件摘要：the base dialog
*
* 当前版本：1.0.0.0
* 作    者：asu
*/


#include "ubxdialog.h"

#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QApplication>



#include "ubxwidget.h"



CUBXDialog::CUBXDialog(QWidget *parent):
    QDialog(parent),
    m_bLeftDown(false),
    m_bEnableMove(false),
    m_nTitleBarHeight(TITLE_BAR_HEIGHT),
    m_ptPre(QPoint(-1, -1))
{

}

CUBXDialog::~CUBXDialog()
{

}

void CUBXDialog::paintEvent(QPaintEvent *)
{
    UBXWidget::paintEvent(this);
}


void CUBXDialog::setEnableMove(bool bEnableMove)
{
    m_bEnableMove = bEnableMove;
}

void CUBXDialog::setTitleBarHeight(const int &titleBarHeight)
{
    if(0 <= titleBarHeight)
    {
        m_nTitleBarHeight = titleBarHeight;
    }
}


void CUBXDialog::mousePressEvent(QMouseEvent *event)
{
    //for move
    if(Qt::LeftButton == event->button())
    {
        m_ptPre = event->globalPos();
        m_bLeftDown = true;
    }
    else
    {
        m_bLeftDown = false;
    }
}

void CUBXDialog::mouseMoveEvent(QMouseEvent *event)
{
    //for move
    if(m_bEnableMove)
    {
        QRect rectTitleBar(0, 0, this->width(), m_nTitleBarHeight);
        if(m_bLeftDown && rectTitleBar.contains(mapFromGlobal(m_ptPre)))
        {
            QPoint nowPt = event->globalPos();
            QPoint moveTo = mapToParent(nowPt - m_ptPre);
            move(moveTo);
            m_ptPre = nowPt;
        }
    }
}

void CUBXDialog::mouseReleaseEvent(QMouseEvent * /*event*/)
{
    //for move
    m_bLeftDown = false;
}
