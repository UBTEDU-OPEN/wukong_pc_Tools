#ifndef DIALOGCOMMANDBOX_H
#define DIALOGCOMMANDBOX_H

#include "motioneditor_global.h"
#include <QDialog>
#include <QString>
#include <QStyleOption>
#include <QPainter>


namespace Ui {
class DialogCommandBox;
}

class MOTIONEDITOR_EXPORT DialogCommandBox : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCommandBox(QWidget *parent = 0);
    ~DialogCommandBox();

private slots:
    void on_btnOK_clicked();

    void on_btnCancel_clicked();

    void on_pbClose_clicked();

protected:
    //If you subclass from QWidget, you need to provide a paintEvent for your custom QWidget as below:
    void paintEvent(QPaintEvent *)
    {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

private:
    Ui::DialogCommandBox *ui;
public:
    QString m_strCommandBoxName;
};

#endif // DIALOGCOMMANDBOX_H
