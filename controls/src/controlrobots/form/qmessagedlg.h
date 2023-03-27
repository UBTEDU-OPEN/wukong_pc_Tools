#ifndef QMESSAGEDLG_H
#define QMESSAGEDLG_H

#include "ccustomdialog.h"

namespace Ui {
class QMessageDlg;
}

QT_BEGIN_NAMESPACE
class CTitleWidget;
QT_END_NAMESPACE

class QMessageDlg : public XDialog
{
    Q_OBJECT

public:
    explicit QMessageDlg(QWidget *parent = 0);
    ~QMessageDlg();

public:
    void    init ();
    void    initAction ();
    void    layout ();
    void    setShowTips (const QString& strTips);

public slots:
    void    slotConfirm ();
    void    slotCancel ();

protected:
    bool    saveCfgIni ();

private:
    CTitleWidget*    m_pTitleWidget;
    Ui::QMessageDlg *ui;
};

#endif // QMESSAGEDLG_H
