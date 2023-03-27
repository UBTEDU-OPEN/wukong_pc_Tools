#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include "ccustomdialog.h"

QT_BEGIN_NAMESPACE
class CTitleWidget;
QT_END_NAMESPACE

namespace Ui {
class QSettingDialog;
}

class QSettingDialog : public XDialog
{
    Q_OBJECT

public:
    explicit QSettingDialog(QWidget *parent = 0);
    ~QSettingDialog();

public slots:
    void    slotConfirm ();
    void    slotCancel ();

public:
    void    setLanguageType (int nLanguageType);
    int     getLanguageType (){ return m_nLanguageType; }

protected:
    CTitleWidget*         m_pTitleWidget;
    int                   m_nLanguageType;

private:
    Ui::QSettingDialog *ui;
};

#endif // SETTINGDIALOG_H
