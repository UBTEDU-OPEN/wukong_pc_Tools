#ifndef CCUSTOMMAINWINDOW_H
#define CCUSTOMMAINWINDOW_H

#include <QMainWindow>
#include "libcontrol_global.h"
#include <QTableWidget>

namespace Ui {
class CCustomMainWindow;
}

class LIBCONTROLSHARED_EXPORT CCustomMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CCustomMainWindow(QWidget *parent = 0);
    ~CCustomMainWindow();

public:
    void drawShadow(QPainter &painter);
    void setStyle(QString strStylePath);

public:
    virtual void paintEvent(QPaintEvent *event);
};

#endif // CCUSTOMMAINWINDOW_H
