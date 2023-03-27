#include "ccustommainwindow.h"
#include <QPainter>
#include <QFile>

CCustomMainWindow::CCustomMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    setAttribute(Qt::WA_TranslucentBackground);

    setContentsMargins(QMargins(SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH));
}

CCustomMainWindow::~CCustomMainWindow()
{

}

void CCustomMainWindow::drawShadow(QPainter &painter)
{
    //绘制左上角、左下角、右上角、右下角、上、下、左、右边框
    QList<QPixmap> pixmaps;
    pixmaps.append(QPixmap(":/png/shadow/Shadow_Left.png"));
    pixmaps.append(QPixmap(":/png/shadow/Shadow_Right.png"));
    pixmaps.append(QPixmap(":/png/shadow/Shadow_Top.png"));
    pixmaps.append(QPixmap(":/png/shadow/Shadow_Bottom.png"));
    pixmaps.append(QPixmap(":/png/shadow/Shadow_Left_Top.png"));
    pixmaps.append(QPixmap(":/png/shadow/Shadow_Right_Top.png"));
    pixmaps.append(QPixmap(":/png/shadow/Shadow_Left_Bottom.png"));
    pixmaps.append(QPixmap(":/png/shadow/Shadow_Right_Bottom.png"));

    painter.drawPixmap(0, 0, SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[4]);
    painter.drawPixmap(this->width()-SHADOW_WIDTH, 0, SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[5]);
    painter.drawPixmap(0,this->height()-SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[6]);
    painter.drawPixmap(this->width()-SHADOW_WIDTH, this->height()-SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[7]);
    painter.drawPixmap(0, SHADOW_WIDTH, SHADOW_WIDTH, this->height()-2*SHADOW_WIDTH, pixmaps[0].scaled(SHADOW_WIDTH, this->height()-2*SHADOW_WIDTH));
    painter.drawPixmap(this->width()-SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, this->height()-2*SHADOW_WIDTH, pixmaps[1].scaled(SHADOW_WIDTH, this->height()- 2*SHADOW_WIDTH));
    painter.drawPixmap(SHADOW_WIDTH, 0, this->width()-2*SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[2].scaled(this->width()-2*SHADOW_WIDTH, SHADOW_WIDTH));
    painter.drawPixmap(SHADOW_WIDTH, this->height()-SHADOW_WIDTH, this->width()-2*SHADOW_WIDTH, SHADOW_WIDTH, pixmaps[3].scaled(this->width()-2*SHADOW_WIDTH, SHADOW_WIDTH));
}

void CCustomMainWindow::setStyle(QString strStylePath)
{
    QFile file(strStylePath);
    if ( file.open(QFile::ReadOnly) )
    {
        QString styleSheet = QLatin1String(file.readAll());
        setStyleSheet(styleSheet);
        file.close();
    }
}

void CCustomMainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    this->drawShadow(painter);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(QRect(SHADOW_WIDTH, SHADOW_WIDTH, this->width()-2*SHADOW_WIDTH, this->height()-2*SHADOW_WIDTH));
}

