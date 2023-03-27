#ifndef SYSTEMPCSOFTVERSIONUI_H
#define SYSTEMPCSOFTVERSIONUI_H
#include "ubxupdatemodule_global.h"
#include "util.h"
#include "global.h"
#include "ubxwidget.h"


class CUpdateProgressDlg;


namespace Ui {
class SystemEmbeddedUpdateWidget;
}


class UBXUPDATEMODULESHARED_EXPORT systemPCSoftVersionUI : public UBXWidget
{
    Q_OBJECT

public:
    explicit systemPCSoftVersionUI(QWidget *parent = 0);
    ~systemPCSoftVersionUI();


public:
    void initUI();

    static bool getVersionParams(QString &sUpdateURL, QString &sLocalVer, emCheckUpdateType &eCheckType);

    int getUpdateCheckStatus();

protected:


private slots:
    /**************************************************************************
    * 函数名: slotOnUpdate
    * 功能: 更新版本
    * 时间: 2016/07/28
    * 作者: asu
    */
    void slotOnUpdate();


private:
    Ui::SystemEmbeddedUpdateWidget *ui;

    CUpdateProgressDlg* pUpdateProgressDlg;

    //
    emCheckUpdateType  m_eChkType;
    FILEINFO         m_LatestVersionInfo;//下载的文件信息
    QString          m_sUpdateURL;//更新地址
    QString          m_sLocalVer;//本地版本
};

#endif // SYSTEMPCSOFTVERSIONUI_H
