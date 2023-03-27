#ifndef BATCHMODIFYDIALOG_H
#define BATCHMODIFYDIALOG_H

#include "motioneditor_global.h"
#include <QDialog>
#include <QString>
#include <QStyleOption>
#include <QPainter>

namespace Ui {
class BatchModifyDialog;
}

class MOTIONEDITOR_EXPORT BatchModifyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BatchModifyDialog(QWidget *parent = 0);
    ~BatchModifyDialog();

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
    Ui::BatchModifyDialog *ui;

public:
    QString m_strModifyValue;
};

#endif // BATCHMODIFYDIALOG_H
