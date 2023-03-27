#include "addplaydlg.h"
#include "ui_addplaydlg.h"
#include "titlewidget.h"
#include <QMessageBox>

CAddPlayDlg::CAddPlayDlg(QWidget *parent) :
    XDialog(parent),
    ui(new Ui::CAddPlayDlg)
{
    ui->setupUi(this);
    init ();
}

CAddPlayDlg::~CAddPlayDlg()
{
    m_mapSong.clear();
    delete ui;
}

void CAddPlayDlg::init ()
{
    m_mapSong.clear();
    m_strFileName = "";
    m_strDisplayName = "";
    m_nTime = -1;
    m_nDlgType = TYPE_ADD_DLG;

    createTitleWidget ();
    initAction ();
    layout ();
}

void CAddPlayDlg::createTitleWidget ()
{
    m_pTitleWidget = new CTitleWidget(this);
    m_pTitleWidget->setMenuBtnVisible(false);
    m_pTitleWidget->setVerion(tr("舞蹈管理"));
    m_pTitleWidget->setMinBtnVisible(false);
    m_pTitleWidget->setMaxBtnVisible(false);
}

void CAddPlayDlg::setTitleText (const QString& strTitle)
{
    m_pTitleWidget->setVerion(strTitle);
}

void CAddPlayDlg::initAction ()
{
    connect(ui->confirmBtn, SIGNAL(pressed()),
            this, SLOT(slotConfirm()));
    connect(ui->quitBtn, SIGNAL(pressed()),
            this, SLOT(slotClose()));
}

void CAddPlayDlg::layout()
{
    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(m_pTitleWidget);
    pVLayout->addWidget(ui->mainWidget);
    pVLayout->setContentsMargins(QMargins(0,0,0,0));
    this->setLayout(pVLayout);
}

bool CAddPlayDlg::isUniqueFileName (const QString& strFileName)
{
    QTypeMusicInfoMap::iterator it = m_mapSong.begin();
    for ( ; it != m_mapSong.end(); it ++ ) {
        QMusicInfoMap* p = it.value();
        QMusicInfoMap::iterator itM = p->find(strFileName);
        if ( itM != p->end() ) {
            return false;
        }
    }
    return true;
}

void CAddPlayDlg::slotConfirm()
{
    QString strFileName = ui->fileNameEdit->text();
    QString strDisplayName = ui->displayNameEdit->text();
    QString strTime = ui->playTimeEdit->text();
    int     nTime = 0;
    if ( !strTime.isEmpty() ){
        nTime = strTime.toInt();
    }

    if ( strFileName.isEmpty() ) {
        QMessageBox::warning(this, tr("集控软件"), tr("文件名不能为空。"));
        return ;
    }

    if ( m_nDlgType == TYPE_ADD_DLG ) {
        // 添加
        if ( !isUniqueFileName(strFileName) ) {
            QMessageBox::warning(this, tr("集控软件"), tr("文件名已存在。"));
            ui->fileNameEdit->setText("");
        }
    } else {
         // 编辑
        if ( strFileName != m_strFileName ) {
            if ( !isUniqueFileName(strFileName) ) {
                QMessageBox::warning(this, tr("集控软件"), tr("文件名已存在。"));
                ui->fileNameEdit->setText("");
            }
        }
    }

    m_strFileName = strFileName;
    m_strDisplayName = strDisplayName;
    m_nTime = nTime;

    accept();
    close ();
}

void CAddPlayDlg::slotClose ()
{
    reject();
    close ();
}

void CAddPlayDlg::setFileName (const QString& strFileName)
{
    m_strFileName = strFileName;
    ui->fileNameEdit->setText(m_strFileName);
}

void CAddPlayDlg::setDisplayName ( const QString& strDisplayName)
{
    m_strDisplayName = strDisplayName;
    ui->displayNameEdit->setText(m_strDisplayName);
}

void CAddPlayDlg::setMusicTime ( int nTime )
{
    m_nTime = nTime;
    ui->playTimeEdit->setText(QString::number(m_nTime));
}

void CAddPlayDlg::setMusicTimeVisiable(bool bVisiable)
{
    ui->playTimeLabel->setVisible(bVisiable);
    ui->playTimeEdit->setVisible(bVisiable);
}
