/**
 * @file file titlewidget.cpp
 * @brief 实现标题控件类
 *
 * Details.
 *
 * @version 1.1
 * @author yangjie
 * @date 2015-10-29
 *
 * 增加doxygen注释
 */

#include "titlewidget.h"
#include <QMouseEvent>
#include <QPushButton>
#include <QLayout>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QMenu>
#include <QLabel>
#include <QDesktopWidget>
#include <QApplication>
/**
 * @brief 标题控件构造函数
 *
 * @param parent 父窗口
 */
CTitleWidget::CTitleWidget(QWidget *parent) :
    QWidget(parent), m_pParentWidget(parent)
{
    this->setObjectName("CTitleWidget");
    initVars();
    createWidget();
    initWidget();
    initAction();
    translateLanguage();
}

/**
 * @brief 初始化窗体
 *
 */
void CTitleWidget::initVars()
{
    m_bIsMaximized = false;
    m_bPressed = false;
    m_pointDragPosition = QPoint(0,0);
    m_orginRect = m_pParentWidget->geometry();
    m_logoPixmap = QPixmap(":/png/logo_ubt.png");
}

void CTitleWidget::createWidget()
{
    plblLogo = new QLabel(this);//logo
    plblVersionTitle = new QLabel(this); //标题
    pbtnChangeSkin = new QPushButton(this); //换肤
    pbtnMainMenu = new QPushButton(this); //主菜单
    pbtnMin = new QPushButton(this); //最小化
    pbtnMax = new QPushButton(this); //最大化
    pbtnClose = new QPushButton(this); //关闭
    QHBoxLayout *pTitleHLayout = new QHBoxLayout(this);
    pTitleHLayout->addSpacing(5);
    pTitleHLayout->addWidget(plblLogo,0,Qt::AlignVCenter);

    QSpacerItem * pSpacerItem = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
    pTitleHLayout->addItem(pSpacerItem);
    pTitleHLayout->addWidget(plblVersionTitle,0,Qt::AlignCenter);

    pSpacerItem = new QSpacerItem(100, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
    pTitleHLayout->addItem(pSpacerItem);
    pTitleHLayout->addWidget(pbtnChangeSkin,0,Qt::AlignTop);
    pTitleHLayout->addWidget(pbtnMainMenu,0,Qt::AlignTop);
    pTitleHLayout->addWidget(pbtnMin,0,Qt::AlignTop);
    pTitleHLayout->addWidget(pbtnMax,0,Qt::AlignTop);
    pTitleHLayout->addWidget(pbtnClose,0,Qt::AlignTop);
    pTitleHLayout->setSpacing(0);
    pTitleHLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pTitleHLayout);
    setFixedHeight(35);
}

void CTitleWidget::initWidget()
{
    m_logoPixmap = QPixmap(":/png/logo_ubt.png");
    this->setLogo(m_logoPixmap);
    this->setStyleSheet(QString("QWidget#CTitleWidget{"
                                "border-bottom:1px solid #F5F5F5;"
                                "background-color:#F5F5F5;"
                                "}"));

    pbtnMainMenu->setFixedSize (25, 23);
    pbtnMainMenu->setStyleSheet (QString::fromUtf8 ("QPushButton {"
                                                    "background:transparent;"
                                                    "border-radius:0px;"
                                                    "background-image: url(:/png/BtnMenu_Normal.png);"
                                                    "}"
                                                    "QPushButton:hover{"
                                                    "background:transparent;"
                                                    "border-radius:0px;"
                                                    "background-image: url(:/png/BtnMenu_Hover.png);"
                                                    "}"
                                                    "QPushButton:pressed{"
                                                    "background:transparent;"
                                                    "border-radius:0px;"
                                                    "background-image: url(:/png/BtnMenu_Pressed.png);"
                                                    "}"));
    pbtnMax->setFixedSize(25, 23);
    pbtnMax->setStyleSheet (QString::fromUtf8 ("QPushButton{"
                                               "background:transparent;"
                                               "border-radius:0px;"
                                               "background-image: url(:/png/btn_Maximize_Normal.png);"
                                               "}"
                                               "QPushButton:hover{"
                                               "background:transparent;"
                                               "border-radius:0px;"
                                               "background-image: url(:/png/btn_Maximize_Hover.png);"
                                               "}"
                                               "QPushButton:pressed{"
                                               "background:transparent;"
                                               "border-radius:0px;"
                                               "background-image: url(:/png/btn_Maximize_Pressed.png);"
                                               "}"));
    pbtnMin->setFixedSize (25, 23);
    pbtnMin->setStyleSheet (QString::fromUtf8 ("QPushButton{"
                                               "background:transparent;"
                                               "border-radius:0px;"
                                               "background-image: url(:/png/btn_Minimize_Normal.png);"
                                               "}"
                                               "QPushButton:hover{"
                                               "background:transparent;"
                                               "border-radius:0px;"
                                               "background-image: url(:/png/btn_Minimize_Hover.png);"
                                               "}"
                                               "QPushButton:pressed{"
                                               "background:transparent;"
                                               "border-radius:0px;"
                                               "background-image: url(:/png/btn_Minimize_Pressed.png);"
                                               "}"));
    pbtnClose->setFixedSize (25, 23);
    pbtnClose->setStyleSheet (QString::fromUtf8 ("QPushButton{"
                                                 "background:transparent;"
                                                 "border-radius:0px;"
                                                 "background-image: url(:/png/btn_Close_Normal.png);"
                                                 "}"
                                                 "QPushButton:hover{"
                                                 "background:transparent;"
                                                 "border-radius:0px;"
                                                 "background-image: url(:/png/btn_Close_Hover.png);"
                                                 "}"
                                                 "QPushButton:pressed{"
                                                 "background:transparent;"
                                                 "border-radius:0px;"
                                                 "background-image: url(:/png/btn_Close_Pressed.png);"
                                                 "}"));

    pbtnChangeSkin->setVisible(false);
}
void CTitleWidget::initAction()
{
    connect(pbtnChangeSkin, SIGNAL(clicked()), this, SIGNAL(showSkin()));
    connect(pbtnMainMenu, SIGNAL(clicked()), this, SIGNAL(showMainMenu()));
    connect(pbtnMin, SIGNAL(clicked()), this, SLOT(slotShowMin()));
    connect(pbtnMax, SIGNAL(clicked()), this, SLOT(slotShowMax()));
    connect(pbtnClose, SIGNAL(clicked()), this, SLOT(slotClose()));
}

/**
 * @brief 析构
 *
 */
CTitleWidget::~CTitleWidget()
{
    delete plblLogo;
    delete plblVersionTitle;
    delete pbtnChangeSkin;
    delete pbtnMainMenu;
    delete pbtnMin;
    delete pbtnMax;
    delete pbtnClose;
}

/**
 * @brief 设置logo图标
 *
 * @param pixmap 传入图标QPixmap
 */
void CTitleWidget::setLogo(QPixmap &pixmap)
{
    plblLogo->setFixedSize(pixmap.size());
    plblLogo->setPixmap(pixmap);
    plblLogo->setStyleSheet(QString("background:transparent;"));
}

/**
 * @brief 设置标题文本
 *
 * @param strTitle 传入的标题字符串
 */
void CTitleWidget::setVerion(QString strTitle)
{
    plblVersionTitle->setStyleSheet("QLabel{font-size: 14px;font-weight: bold;}");
    plblVersionTitle->setText(strTitle);
}
/**
 * @brief 设置标题图标
 *
 * @param bgPix 标题图标
 */
void CTitleWidget::setTitle(QPixmap bgPix)
{
    plblVersionTitle->setPixmap(bgPix);
}

/**
 * @brief 控件显示名称
 *
 */
void CTitleWidget::translateLanguage()
{
    pbtnChangeSkin->setToolTip(QString("change skin"));
    pbtnMainMenu->setToolTip(QString("main menu"));
    pbtnMin->setToolTip(QString("minimize"));
    pbtnClose->setToolTip(QString("close"));

}
void CTitleWidget::slotClose()
{
    m_pParentWidget->close();
}
void CTitleWidget::slotShowMin()
{
    m_pParentWidget->showMinimized();
}
/**
 * @brief 最大化槽
 *
 */
void CTitleWidget::slotShowMax()
{
    if(!m_bIsMaximized)
    {
        pbtnMax->setStyleSheet (QString::fromUtf8 ("QPushButton{"
                                                   "background:transparent;"
                                                   "border-radius:0px;"
                                                   "background-image: url(:/png/BtnRevert_Normal.png);"
                                                   "}"
                                                   "QPushButton:hover{"
                                                   "background:transparent;"
                                                   "border-radius:0px;"
                                                   "background-image: url(:/png/BtnRevert_Hover.png);"
                                                   "}"
                                                   "QPushButton:pressed{"
                                                   "background:transparent;"
                                                   "border-radius:0px;"
                                                   "background-image: url(:/png/BtnRevert_Pressed.png);"
                                                   "}"));
        showMaximized();

    }
    else
    {
        pbtnMax->setStyleSheet (QString::fromUtf8 ("QPushButton{"
                                                   "background:transparent;"
                                                   "border-radius:0px;"
                                                   "background-image: url(:/png/btn_Maximize_Normal.png);"
                                                   "}"
                                                   "QPushButton:hover{"
                                                   "background:transparent;"
                                                   "border-radius:0px;"
                                                   "background-image: url(:/png/btn_Maximize_Hover.png);"
                                                   "}"
                                                   "QPushButton:pressed{"
                                                   "background:transparent;"
                                                   "border-radius:0px;"
                                                   "background-image: url(:/png/btn_Maximize_Pressed.png);"
                                                   "}"));
        showNormal ();
    }
    m_bIsMaximized = !m_bIsMaximized;
}

void CTitleWidget::showMaximized ()
{
    if ( m_pParentWidget ){
        QRect deskRect = QApplication::desktop()->availableGeometry();
        QRect rect (deskRect.x() - SHADOW_WIDTH,
                    deskRect.y() - SHADOW_WIDTH,
                    deskRect.width() + SHADOW_WIDTH * 2,
                    deskRect.height() + SHADOW_WIDTH * 2);
        m_pParentWidget->setGeometry(rect);
    }
}

void CTitleWidget::showNormal ()
{
    if ( m_pParentWidget ){
        m_pParentWidget->setGeometry(m_orginRect);
    }
}

/**
 * @brief 鼠标点击事件
 *
 * @param event 鼠标事件
 */
void CTitleWidget::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton) //点击左边鼠标
    {
        m_bPressed = true;
        m_pointDragPosition = event->globalPos() - m_pParentWidget->frameGeometry().topLeft();
        //globalPos()获取根窗口的相对路径，frameGeometry().topLeft()获取主窗口左上角的位置
        event->accept();   //鼠标事件被系统接收
    }
}
/**
 * @brief 鼠标移动事件
 *
 * @param event 鼠标事件
 */
void CTitleWidget::mouseMoveEvent(QMouseEvent * event)
{
    if ( m_bPressed )
    {
        if ( !m_bIsMaximized )
        {
            m_pParentWidget->move(event->globalPos() - m_pointDragPosition);//移动窗口
            event->accept();
        }
    }
}
/**
 * @brief 鼠标释放事件
 *
 * @param
 */
void CTitleWidget::mouseReleaseEvent(QMouseEvent * /*event*/)
{
    m_bPressed = false;
    m_pointDragPosition = QPoint(0,0);
}

/**
 * @brief 鼠标双击事件
 *
 * @param
 */
void CTitleWidget::mouseDoubleClickEvent(QMouseEvent * )
{
    slotShowMax();
}

/**
 * @brief 重写绘图事件 解决背景设置setStyleSheet只在标题栏的label中起作用而不在 当前widget起作用
 *
 * @param
 */
void CTitleWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}

void CTitleWidget::setMaxBtnVisible (bool bVisible)
{
    pbtnMax->setVisible (bVisible);
}

void CTitleWidget::setMinBtnVisible (bool bVisible)
{
    pbtnMin->setVisible (bVisible);
}

void CTitleWidget::setCloseBtnVisible (bool bVisible)
{
    pbtnClose->setVisible (bVisible);
}

void CTitleWidget::setMenuBtnVisible (bool bVisible)
{
    pbtnMainMenu->setVisible (bVisible);
}

void CTitleWidget::setLogoVisible (bool bVisible)
{
    plblLogo->setVisible(bVisible);
}

void CTitleWidget::setTitleLabelVisible (bool bVisible)
{
    plblVersionTitle->setVisible (bVisible);
}


