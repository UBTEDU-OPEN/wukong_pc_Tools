#include "qmessagedlg.h"
#include "ui_qmessagedlg.h"
#include "titlewidget.h"
#include "cfgini.h"
#include "pubdef.h"

QMessageDlg::QMessageDlg(QWidget *parent) :
    XDialog(parent),
    ui(new Ui::QMessageDlg)
{
    ui->setupUi(this);
    init ();
}

QMessageDlg::~QMessageDlg()
{
    delete ui;
}

void QMessageDlg::init ()
{
    initAction ();
    layout ();
    setStyle(QString(":/qss/qss/udpsendfile.qss"));
}

void QMessageDlg::initAction ()
{
    connect(ui->confirmBtn, SIGNAL(pressed()), this, SLOT(slotConfirm()));
    connect(ui->cancelBtn, SIGNAL(pressed()), this, SLOT(slotCancel()));
}

void QMessageDlg::layout ()
{
    m_pTitleWidget = new CTitleWidget(this);
    m_pTitleWidget->setMenuBtnVisible(false);
    m_pTitleWidget->setMaxBtnVisible(false);
    m_pTitleWidget->setMinBtnVisible(false);
    m_pTitleWidget->setLogoVisible(false);

    setMoveWindowEnable(false);
    ui->checkBox->setChecked(false);

    ui->messageLabel->setStyleSheet("QLabel{"
                                    "background: transparent;"
                                    "font-size: 14px;"
                                    "text-align: center;"
                                    "}");

    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(m_pTitleWidget);
    pVLayout->addWidget(ui->mainWidget);
    pVLayout->setContentsMargins(QMargins(0,0,0,0));
    this->setLayout(pVLayout);
}

void QMessageDlg::setShowTips (const QString& strTips)
{
    ui->messageLabel->setText(strTips);
}

void QMessageDlg::slotConfirm ()
{
    saveCfgIni ();
    accept();
    close ();
}

void QMessageDlg::slotCancel ()
{
    saveCfgIni ();
    reject();
    close ();
}

bool QMessageDlg::saveCfgIni ()
{
    bool bCheck = !ui->checkBox->isChecked();
    return CCfgIni::getInstance().writeItem(KEY_SYSSETTING_CANSHOWSQUATTIPS, QVariant(bCheck));
}
