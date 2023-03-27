#include "qsettingdlg.h"
#include "ui_qsettingdlg.h"
#include "titlewidget.h"
#include <QComboBox>
#include "pubdef.h"

QSettingDialog::QSettingDialog(QWidget *parent) :
    XDialog(parent),
    ui(new Ui::QSettingDialog)
{
    ui->setupUi(this);

    m_nLanguageType = EN_LANGUAGE_EN_US;
    ui->languagecomb->addItem(tr("海外版"), QVariant(EN_LANGUAGE_EN_US));
    ui->languagecomb->addItem(tr("国内版"), QVariant(EN_LANGUAGE_ZH_CN));
    ui->languagecomb->setCurrentIndex(0);

    connect(ui->confirmBtn, &QPushButton::released, this, &QSettingDialog::slotConfirm);
    connect(ui->cancelBtn, &QPushButton::released, this, &QSettingDialog::slotCancel);

    ui->confirmBtn->setStyleSheet("QPushButton {"
        "background: transparent;"
        "font-size: 18px;"
        "color: #02BBF1;"
        "text-align: center;"
        "padding: 5px;"
        "border:1px solid #FFC0CB;"
        "border-radius:5px;"
        "background-image: url(:/new/res/newres/png/bg_button_normal.png);"
        "}"
        "QPushButton::hover {"
        "background-image: url(:/new/res/newres/png/bg_button_press/hover.png);"
        "}"
        "QPushButton::pressed {"
        "background-image: url(:/new/res/newres/png/bg_button_press/hover.png);"
        "}");

    ui->cancelBtn->setStyleSheet("QPushButton {"
        "background: transparent;"
        "font-size: 18px;"
        "color: #02BBF1;"
        "text-align: center;"
        "padding: 5px;"
        "border:1px solid #FFC0CB;"
        "border-radius:5px;"
        "background-image: url(:/new/res/newres/png/bg_button_normal.png);"
        "}"
        "QPushButton::hover {"
        "background-image: url(:/new/res/newres/png/bg_button_press/hover.png);"
        "}"
        "QPushButton::pressed {"
        "background-image: url(:/new/res/newres/png/bg_button_press/hover.png);"
        "}");

    m_pTitleWidget = new CTitleWidget(this);
    m_pTitleWidget->setMenuBtnVisible(false);
    m_pTitleWidget->setMinBtnVisible(false);
    m_pTitleWidget->setMaxBtnVisible(false);
    m_pTitleWidget->setLogoVisible(false);

    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(m_pTitleWidget);
    pVLayout->addWidget(ui->mainWidget);
    pVLayout->setContentsMargins(QMargins(0,0,0,0));
    this->setLayout(pVLayout);
    this->resize(359, 200);

}

QSettingDialog::~QSettingDialog()
{
    delete ui;
}

void QSettingDialog::slotConfirm ()
{
    m_nLanguageType = ui->languagecomb->currentData(Qt::UserRole).toInt();
    accept();
}

void QSettingDialog::slotCancel ()
{
    reject();
}

void QSettingDialog::setLanguageType (int nLanguageType)
{
    m_nLanguageType = nLanguageType;
    ui->languagecomb->setCurrentIndex(nLanguageType);
}
