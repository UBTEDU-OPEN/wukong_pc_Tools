#include "cimagelabel.h"

#include <QPainter>
#include <QFile>
#include <QApplication>



CImageLabel::CImageLabel(QWidget *parent) :
    QLabel(parent),
    m_bEnableElidedText(true)
{
    this->setAlignment(Qt::AlignCenter);
}

CImageLabel::~CImageLabel()
{

}

void CImageLabel::setImage(const QString &strImagePath, Qt::AspectRatioMode aspectMode, float factor)
{    
    if(m_pixmap.load(strImagePath))
    {
        const int width = this->width();
        const int height = this->height();
        m_pixmap = m_pixmap.scaled(width*factor,
                                   height*factor,
                                   aspectMode,
                                   Qt::SmoothTransformation);
    }
}

void CImageLabel::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
}

void CImageLabel::setEnableElidedText(bool bEnableElided)
{
    m_bEnableElidedText = bEnableElided;
}

void CImageLabel::mousePressEvent(QMouseEvent *ev)
{
    QLabel::mousePressEvent(ev);

    emit sigClick();
}

void CImageLabel::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    QRect rect = this->rect();
    painter.drawPixmap(rect, m_pixmap);

    //绘制text
    QFont font = this->font();
    painter.setFont(font);
    QFontMetrics metrics(font);
    QString elidedText = this->text();
    if(m_bEnableElidedText)
    {
        elidedText = metrics.elidedText(elidedText, Qt::ElideRight, rect.width());
    }

    painter.setBrush(this->palette().brush(QPalette::WindowText));

    painter.drawText(rect.left(),
                      rect.top(),
                      rect.width(),
                      rect.height(),
                      this->alignment(),
                      elidedText,
                      &rect);
}
