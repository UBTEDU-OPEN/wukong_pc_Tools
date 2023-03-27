/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：UpdateProgressDlg.cpp
* 创建时间：2015/06/02 16:17
* 文件标识：
* 文件摘要：更新进度窗体类
*
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/06/02 16:17
* 版本摘要：
*/
#include "stdafx.h"
#include "UpdateProgressDlg.h"
#include "Ui_UpdateProgressDlg.h"
#include <QDesktopWidget>
#include <QApplication>


#include "../zxLib/widget/ubxwidget.h"
#include "../common/filedirhandle.h"
#include "../zxLib/widget/messagebox.h"


const UINT MSG_DOWNLOADRESULT = QEvent::registerEventType();  //下载结果通知消息
const UINT MSG_DOWNLOAD_LOADING = QEvent::registerEventType();  //下载过程通知消息

static CUpdateProgressDlg*   g_pUpdateDlg = NULL;
static QString               g_sFileSavePath;//文件保存路径
static QThread              g_threadDownload;//下载线程
static FILEINFO            g_downloadfileinfo;//下载的文件信息

/**************************************************************************
* 函数名: OnFileDownloadNotify
* 功能: 下载进度通知
* 时间: 2015/06/03 8:51
* 作者: ZDJ
*/
void CALLBACK OnFileDownloadNotify(LPVOID pUser, const QString& sFileName, qint64 nFileTotalSize, qint64 nFileCurSize)
{
    CUpdateProgressDlg* pDlg = (CUpdateProgressDlg*)pUser;
    if(pDlg)
    {
        double fprocess = nFileCurSize*1.0/nFileTotalSize * 100;
        int nCurCtrlPos =  pDlg->ui->progressBar->value();
        if((fprocess - nCurCtrlPos >= 1) || (nFileTotalSize == nFileCurSize))
        {
            zxEvent* pNewEvent = new zxEvent(MSG_DOWNLOAD_LOADING);
            pNewEvent->eEventType = zxEventTypeWindow;
            pNewEvent->lReceiver = (long)pUser;
            pNewEvent->dwMsgId = MSG_DOWNLOAD_LOADING;
            pNewEvent->wParam = fprocess;
            pNewEvent->strParam = sFileName;
            QCoreApplication::postEvent((QWidget*)pUser, pNewEvent);
        }
    }
}

MyDownloadProc::MyDownloadProc(QWidget* pParent)
{
    m_pParent = pParent;
}

MyDownloadProc::~MyDownloadProc()
{

}

void MyDownloadProc::start()
{
    DownProc(m_pParent);
}


/**************************************************************************
* 函数名: DownProc
* 功能: 下载执行函数
* 时间: 2015/06/02 16:39
* 作者: ZDJ
*/
void MyDownloadProc::DownProc(LPVOID lParam)
{
    CUpdateProgressDlg* pDlg = (CUpdateProgressDlg*)lParam;
    if(pDlg)
    {
//        QString sShortName = QString::fromStdString(pDlg->m_downloadfileinfo.name);
//        QString sFileAddr = QString::fromStdString(pDlg->m_downloadfileinfo.url);
        QString sShortName = pDlg->m_LatestVersionInfo.m_sName;
        QString sFileAddr =pDlg->m_LatestVersionInfo.m_sUrl;

        g_sFileSavePath = QDir::toNativeSeparators(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + QDir::separator() + sShortName);

        BOOL bRet = DownloadFile(sFileAddr, g_sFileSavePath, OnFileDownloadNotify, pDlg);

        zxEvent* pNewEvent = new zxEvent(MSG_DOWNLOADRESULT);
        pNewEvent->eEventType = zxEventTypeWindow;
        pNewEvent->lReceiver = (long)m_pParent;
        pNewEvent->dwMsgId = MSG_DOWNLOADRESULT;
        pNewEvent->wParam = bRet;
        QCoreApplication::postEvent(m_pParent, pNewEvent);
    }
}

CUpdateProgressDlg::CUpdateProgressDlg(QWidget *pParent /*=NULL*/)
    : QDialog(pParent)
    , ui(new Ui::UpdateProgressDlg)
    , m_bShowTipOnFailed(TRUE)
    , m_eChkType(eCheckUpdateTypeWindows)
    , m_pDownloadTask(NULL)
{
    ui->setupUi(this);

    this->setVisible(false);

    UBXWidget::CenterWidget(NULL, this);

    connect(ui->pushButton_Cancel, SIGNAL(clicked(bool)), this, SLOT(OnCancel()));
}

CUpdateProgressDlg::~CUpdateProgressDlg()
{
    if(!g_threadDownload.isFinished())
    {
        g_threadDownload.exit();
    }

    delete ui;
}

/**************************************************************************
* 函数名: DoCheck
* 功能: 检查更新
* 时间: 2015/06/02 16:30
* 作者: ZDJ
*/
void CUpdateProgressDlg::DoCheck()
{
    int nRet = CheckUpdate(m_sUpdateURL, m_LatestVersionInfo, m_sLocalVer, m_eChkType);

    if(nRet == CHECKRESULT_NEEDUPDATE)
    {
        if(CMessageBox::question(this,
                                 tr("prompt"),
                                 tr("find new version and update"),
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes)
                == QMessageBox::Yes)
        {
            ui->progressBar->setRange(0, 100);
            ui->progressBar->setValue(0);

            //ui->label_processinfo->setText("0%");

            SAFE_DELETE(m_pDownloadTask);
            m_pDownloadTask = new MyDownloadProc(this);
            connect(&g_threadDownload, SIGNAL(started()), m_pDownloadTask, SLOT(start()));
            m_pDownloadTask->moveToThread(&g_threadDownload);
            g_threadDownload.start();
        }
        else
        {
            this->close();
        }
    }
    else if(nRet == CHECKRESULT_NOUPDATE)
    {
        if(m_bShowTipOnFailed)
        {
            CMessageBox::information(this, tr("info"), tr("no update"));
        }

        this->close();
    }
    else
    {
        if(m_bShowTipOnFailed)
        {
            CMessageBox::warning(this, tr("info"), tr("check update error"));
        }

        this->close();
    }
}

/**************************************************************************
* 函数名: OnDownloadResult
* 功能: 下载完成通知
* 参数:
*    @[in ] bRet: 下载结果，成功true，失败false
*    @[in ] pData: 通知数据
*    @[in ] nDataLen: 通知数据长度
* 返回值: void
* 时间: 2016/03/09 16:30
* 作者: ZDJ
*/
void CUpdateProgressDlg::OnDownloadResult(bool bRet, char* pData, int nDataLen)
{
    SAFE_DELETE(m_pDownloadTask);

    if(bRet)
    {
        if(m_eChkType == eCheckUpdateTypeWindows)
        {
            if(CMessageBox::question(this,
                                     tr("prompt"),
                                     tr("download successful and update"),
                                     QMessageBox::Yes | QMessageBox::No,
                                     QMessageBox::Yes)
                    == QMessageBox::Yes)
            {
#ifdef Q_OS_WIN
                //启动安装程序
                WinExec(g_sFileSavePath.toStdString().c_str(), SW_SHOW);
                TerminateProcess(GetCurrentProcess(), 0);
#endif

            }
        }
        else if(m_eChkType == eCheckUpdateTypeMac)
        {
            CMessageBox::information(this, tr("info"), tr("download successful"));
        }
    }
    else
    {
        CMessageBox::warning(this, tr("info"), tr("download failed"));
    }

    this->close();
}

void CUpdateProgressDlg::customEvent(QEvent* event)
{
    zxEvent* pEvent = (zxEvent*)event;
    const int nMsgId = pEvent->dwMsgId;

    if(nMsgId == MSG_DOWNLOADRESULT)
    {
        bool bResult = pEvent->wParam;
        OnDownloadResult(bResult, NULL, 0);
    }

    else if(nMsgId == MSG_DOWNLOAD_LOADING)
    {
        if(!this->isVisible())
        {
            this->setVisible(true);
        }

        QString sName = CFileDirHandle::getFileName(pEvent->strParam);
        QString sTemp;
        sTemp.sprintf("%s %s", sName.toStdString().c_str(), "UpdateProgress");
        this->setWindowTitle(sTemp);

        this->ui->progressBar->setValue(pEvent->wParam);

        if(100 <= pEvent->wParam)
        {
            this->hide();
        }
    }
}

void CUpdateProgressDlg::OnCancel()
{
    CancelDownload();
}
