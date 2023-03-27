#include "systempcsoftversionui.h"

#include "ui_systemembeddedupdatewidget.h"
#include "alpharobotlog.h"

#include <QPainter>

#include "../zxLib/widget/ubxwidget.h"
#include "updateprogressdlg.h"
#include "systemembeddedupdatewidget.h"
#include "ubxupdateadministrator.h"

systemPCSoftVersionUI::systemPCSoftVersionUI(QWidget *parent):
    UBXWidget(parent),
    ui(new Ui::SystemEmbeddedUpdateWidget),
    pUpdateProgressDlg(NULL)
{
    ui->setupUi(this);

    UBXWidget::applyUBXWinFlags(this);
    this->setWindowModality(Qt::ApplicationModal);


    resize(480, 360);
    ui->labelTitle->setText(tr("Soft Version"));

    ui->pbUpdate->setEnabled(false);
    ui->versiontextEdit->setReadOnly(true);

    connect(ui->pbUpdate, &QPushButton::clicked, this, &systemPCSoftVersionUI::slotOnUpdate);

#ifdef Q_OS_WIN
    ui->pbCloseMac->hide();
    connect(ui->pbCloseWin, &QPushButton::clicked, this, &systemPCSoftVersionUI::close);
#else
    ui->pbCloseWin->hide();
    connect(ui->pbCloseMac, &QPushButton::clicked, this, &systemPCSoftVersionUI::close);
#endif


    pUpdateProgressDlg = new CUpdateProgressDlg(NULL);

    //获取版本号
    getVersionParams(m_sUpdateURL, m_sLocalVer, m_eChkType);

    UBXWidget::setDefaultStyle(this, UPDATE_MODULE_DEFAULT_QSS);

    setEnableMove(true);
}

systemPCSoftVersionUI::~systemPCSoftVersionUI()
{
    SAFE_DELETE(pUpdateProgressDlg);
}


void systemPCSoftVersionUI::initUI()
{
    int nVersionRet = getUpdateCheckStatus();

    ui->labLocalVersionText->setText(m_sLocalVer);

    // 获取的版本含有V，如V20170726
    QString strLatestVersion = SystemEmbeddedUpdateWidget::getVersionInNumber(m_LatestVersionInfo.m_sVersion);
    if (!strLatestVersion.isEmpty())
    {
        ui->labLatestVerionText->setText(strLatestVersion);
    }
    else
    {
        ui->labLatestVerionText->setText(tr("--"));
    }

    ui->versiontextEdit->clear();
    if(!strLatestVersion.isEmpty() &&
            (m_LatestVersionInfo.m_slstDescriptions.size() > 0))
    {
        ui->versiontextEdit->insertPlainText(strLatestVersion);
        ui->versiontextEdit->insertPlainText(" ");
        ui->versiontextEdit->insertPlainText(tr("Description"));
        ui->versiontextEdit->insertPlainText("\n");

        for (int nUpdateDes = 0; nUpdateDes < m_LatestVersionInfo.m_slstDescriptions.size(); nUpdateDes++)
        {
            QString sDesItem = m_LatestVersionInfo.m_slstDescriptions[nUpdateDes] + "\n";
            ui->versiontextEdit->append(sDesItem);
        }
    }


    // 当前版本已知，服务器上获取不到版本
    // 当前版本和服务器上版本均已知
    switch (nVersionRet)
    {
    case CHECKRESULT_NEEDUPDATE:
        ui->pbUpdate->setEnabled(true);
        ui->pbUpdate->show();
        break;

    default:
        ui->pbUpdate->hide();
        break;
    }

    UBXWidget::CenterWidget(NULL, this);
}

bool systemPCSoftVersionUI::getVersionParams(QString &sUpdateURL, QString &sLocalVer, emCheckUpdateType &eCheckType)
{
    sUpdateURL.clear();
    sLocalVer.clear();

    //获取版本号
    FILEVERINFO fileinfo;
    bool bGet = CUtil::GetFileVersion(QApplication::applicationFilePath(), fileinfo);
    if(!bGet)
    {
        return bGet;
    }

    sUpdateURL = UBXUpdateAdministrator::getInstance()->getUpdateBasicUrl();
    #ifdef Q_OS_WIN
    eCheckType = eCheckUpdateTypeWindows;
    #elif defined(Q_OS_MAC)
    eCheckType = eCheckUpdateTypeMac;
    #endif

    #ifndef QT_NO_DEBUG
    sLocalVer = "1.0.0.1";
    #else
    sLocalVer = fileinfo.sFileVer.trimmed();
    #endif
}

int systemPCSoftVersionUI::getUpdateCheckStatus()
{
    return CheckUpdate(m_sUpdateURL, m_LatestVersionInfo, m_sLocalVer, m_eChkType);
}

void systemPCSoftVersionUI::slotOnUpdate()
{
    this->hide();

    pUpdateProgressDlg->m_sUpdateURL = m_sUpdateURL;
    pUpdateProgressDlg->m_sLocalVer = m_sLocalVer;
    pUpdateProgressDlg->m_eChkType = m_eChkType;

    pUpdateProgressDlg->DoCheck();
}





