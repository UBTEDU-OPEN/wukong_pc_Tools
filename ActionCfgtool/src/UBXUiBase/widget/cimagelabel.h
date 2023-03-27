#ifndef CIMAGELABEL_H
#define CIMAGELABEL_H

#include "ubxuibase_global.h"
#include <QObject>
#include <QLabel>
#include <QPixmap>


class CImageLabel : public QLabel
{
    Q_OBJECT

public:
    explicit CImageLabel(QWidget *parent = 0);
    virtual ~CImageLabel();

public:
    void setImage(const QString &strImagePath, Qt::AspectRatioMode aspectMode = Qt::KeepAspectRatio, float factor = 1.0);
    void setPixmap(const QPixmap &pixmap);

    void setEnableElidedText(bool bEnableElided);


signals:
    void sigClick();

    
protected:
    void mousePressEvent(QMouseEvent *ev);    
    void paintEvent(QPaintEvent *);

private:
    QPixmap m_pixmap;

    bool m_bEnableElidedText;
};

#endif // CIMAGELABEL_H
