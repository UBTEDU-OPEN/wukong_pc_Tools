#include "ubxwidget.h"


#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QApplication>


UBXWidget::UBXWidget(QWidget *parent):
    QWidget(parent),
    m_bLeftDown(false),
    m_bEnableMove(false),
    m_nTitleBarHeight(TITLE_BAR_HEIGHT),
    m_ptPre(QPoint(-1, -1))
{

}

UBXWidget::~UBXWidget()
{

}

/************************************
* 名称: setDefaultStyle
* 功能: 设置窗体默认样式
* 返回:   void
* 时间:   2016/07/15
* 作者:   asu
************************************/
bool UBXWidget::setDefaultStyle(QWidget *pWidget, const QString& qssPath)
{
    bool bSetOk = false;
    QFile styleSheet(qssPath);
    if(pWidget && styleSheet.open(QIODevice::ReadOnly))
    {
        pWidget->setStyleSheet(styleSheet.readAll());

        bSetOk = true;
    }

    return bSetOk;
}


void UBXWidget::paintEvent(QPaintEvent *)
{
    paintEvent(this);
}

void UBXWidget::paintEvent(QWidget *pWidget)
{
    if(pWidget)
    {
        QStyleOption opt;
        opt.init(pWidget);
        QPainter p(pWidget);
        pWidget->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, pWidget);
    }
}



/**************************************************************************
* 函数名: CenterWidget
* 功能: 窗口居中
* 参数:
*    @[in ] pParent: 父窗口，为NULL时默认为桌面
*    @[in ] pChild: 子窗口
* 返回值: void
* 时间: 2016/03/09 13:51
* 作者: ZDJ
*/
void UBXWidget::CenterWidget(const QWidget * const pParent, QWidget * const pChild)
{
    if(!pChild)
    {
        return;
    }

    QPoint posCenter;
    if(pParent)
    {
        posCenter.setX(pParent->width()/2);
        posCenter.setY(pParent->height()/2);
    }
    else
    {
        QDesktopWidget* pDeskTop = QApplication::desktop();
        posCenter.setX(pDeskTop->width()/2);
        posCenter.setY(pDeskTop->height()/2);
    }

    const int nChildWidth = pChild->width();
    const int nChildHeight = pChild->height();

    QPoint posMove;
    posMove.setX(posCenter.x() - nChildWidth/2);
    posMove.setY(posCenter.y() - nChildHeight/2);

    pChild->move(posMove);
}

void UBXWidget::setEnableMove(bool bEnableMove)
{
    m_bEnableMove = bEnableMove;
}

void UBXWidget::setTitleBarHeight(const int &titleBarHeight)
{
    if(0 <= titleBarHeight)
    {
        m_nTitleBarHeight = titleBarHeight;
    }
}

void UBXWidget::applyUBXWinFlags(QWidget *pWidget)
{
    if(pWidget)
    {
        pWidget->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        pWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    }
}

void UBXWidget::execByAppModal(QWidget *pWidget)
{
    if(pWidget)
    {
        applyUBXWinFlags(pWidget);
        pWidget->setWindowModality(Qt::ApplicationModal);
        pWidget->show();
    }
}

void UBXWidget::showAlwayOnTop(QWidget *pWidget)
{
    if(pWidget)
    {
        UBXWidget::applyUBXWinFlags(pWidget);

        pWidget->setWindowFlags(pWidget->windowFlags()| Qt::WindowStaysOnTopHint);
        pWidget->show();
    }
}

void UBXWidget::mousePressEvent(QMouseEvent *event)
{
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

void UBXWidget::mouseMoveEvent(QMouseEvent *event)
{
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

void UBXWidget::mouseReleaseEvent(QMouseEvent * /*event*/)
{
    m_bLeftDown = false;
}
