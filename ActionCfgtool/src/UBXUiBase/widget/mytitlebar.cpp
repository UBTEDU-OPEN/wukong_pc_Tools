#include "mytitlebar.h"
#include "ui_mytitlebar.h"


#include <QShowEvent>

#include "ubxwidget.h"




MyTitleBar::MyTitleBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myTitleBar)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    setTitleStyle(eTitleStyleWin);
#else
    setTitleStyle(eTitleStyleMac);
#endif

    QObject::connect(ui->pbCloseMac, &QPushButton::pressed, this, &MyTitleBar::sigClose);
    QObject::connect(ui->pbCloseWin, &QPushButton::pressed, this, &MyTitleBar::sigClose);

    this->setFixedHeight(TITLE_BAR_HEIGHT);

    UBXWidget::setDefaultStyle(this, DEFAULT_ZXLIB_QSS_PATH);

    this->setObjectName(MY_TITLE_BAR_OBJECT_NAME);
}

MyTitleBar::~MyTitleBar()
{
    delete ui;
}

QLabel *MyTitleBar::getTitleLabel() const
{
    return ui->label_title;
}

void MyTitleBar::setTitleLabel(const QString &title)
{
    ui->label_title->setText(title);
}

void MyTitleBar::setTitleStyle(MyTitleBar::emTitleSystemStyle style)
{
    switch (style)
    {
    case eTitleStyleMac:
    {
        ui->pbCloseWin->hide();
        ui->pbCloseMac->show();
    }
        break;

    case eTitleStyleWin:
    {
        ui->pbCloseWin->show();
        ui->pbCloseMac->hide();
    }

    default:
        break;
    }
}

void MyTitleBar::setLayoutMargins(int left, int top, int right, int bottom)
{
    ui->horizontalLayout->setContentsMargins(left, top, right, bottom);
}

void MyTitleBar::showEvent(QShowEvent *event)
{
    if(event->type() == QEvent::Show)
    {
        resizeToParent();
    }

    QWidget::showEvent(event);
}

void MyTitleBar::paintEvent(QPaintEvent *)
{
    UBXWidget::paintEvent(this);
}

void MyTitleBar::resizeEvent(QResizeEvent *event)
{
    resizeToParent();
    QWidget::resizeEvent(event);
}

void MyTitleBar::resizeToParent()
{
    QWidget* pParent = dynamic_cast<QWidget*>(this->parent());
    if(pParent)
    {
        const int nParentWidth = pParent->width();
        this->setFixedWidth(nParentWidth);

        int nLeft;
        int nTop;
        int nRight;
        int nBottom;
        ui->horizontalLayout->getContentsMargins(&nLeft, &nTop, &nRight, &nBottom);

        const int nTitleWidth  = nParentWidth - (ui->pbCloseMac->width() + ui->pbCloseWin->width())
                - nLeft - nRight;

        ui->label_title->setMinimumWidth(nTitleWidth);
    }
}
