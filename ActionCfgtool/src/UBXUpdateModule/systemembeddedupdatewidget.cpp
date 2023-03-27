/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：SystemEmbeddedUpdateWidget.cpp
* 创建时间：2016/07/23 16:17
* 文件标识：
* 文件摘要：更新嵌入式版本
*
* 当前版本：1.0.0.0
* 作    者：hwx
* 完成时间：2016/07/23 16:17
* 版本摘要：
*/

#include <QPainter>

#include "systemembeddedupdatewidget.h"
#include "ui_systemembeddedupdatewidget.h"
#include "ubxupdatemoduleconst.h"
#include "alpharobotlog.h"
#include "../zxLib/widget/ubxwidget.h"
#include "../zxLib/configs.h"
#include "ubxupdateadministrator.h"
#include "../zxLib/widget/messagebox.h"
#include "UBXPublic.h"
#include "configs.h"
#include "../zxLib/widget/progressdialog.h"
#include "cubxudiskmanager.h"


#include "MemoryReportFilter.h"
#ifdef  Q_OS_WIN
#include <windows.h>
#endif

SystemEmbeddedUpdateWidget::SystemEmbeddedUpdateWidget(QWidget *parent) :
    QDialog(parent),  m_eRobotType(eCheckUpdateTypeOldRobot1S),
    ui(new Ui::SystemEmbeddedUpdateWidget),
    m_httpOperSync(NULL), m_pProgressDialog(NULL),
    m_bOnMenuClickedTrigger(false),
    m_bIsChinese(true),
    m_bCanShowMBox(true),
    m_nUpdateProgressTimerID(0)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);

    resize(480, 360);
    ui->labelTitle->setText(tr("Firmware Version"));

    m_strLocalSysEmbedVersion = ""; // 当前嵌入式版本

    ui->pbUpdate->setEnabled(false);
    ui->versiontextEdit->setReadOnly(true);

    m_formatUdiskProcess.setProcessChannelMode(QProcess::MergedChannels);

    connect(&m_oThreadCopyFile, SIGNAL(finished()), this, SLOT(slotOnCopyFileFinished()));
    connect(ui->pbUpdate, &QPushButton::clicked, this, &SystemEmbeddedUpdateWidget::slotOnUpdate, Qt::QueuedConnection);
    connect(&m_oThreadWaitUdiskShowUp, &CThreadWaitForUdiskShowup::sigIsUdiskShowUp, this, &SystemEmbeddedUpdateWidget::slotOnRobotUDiskShowUp);
    connect(&m_formatUdiskProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(slotOnFormatUdiskFinished(int,QProcess::ExitStatus)));
    connect(&m_formatUdiskProcess, &QProcess::stateChanged, this, &SystemEmbeddedUpdateWidget::slotOnFormatUdiskStateChanged);

#ifdef Q_OS_WIN
    ui->pbCloseMac->hide();
    connect(ui->pbCloseWin, &QPushButton::clicked, this, &SystemEmbeddedUpdateWidget::slotOnClosed);
#else
    ui->pbCloseWin->hide();
    connect(ui->pbCloseMac, &QPushButton::clicked, this, &SystemEmbeddedUpdateWidget::slotOnClosed);
#endif


    if(g_pRobotController)
    {
        AddSubscribe(g_pRobotController); //添加通信消息订阅
    }

    UBXWidget::setDefaultStyle(this, UPDATE_MODULE_DEFAULT_QSS);

    m_dragPosition = QPoint(-1, -1);
}

SystemEmbeddedUpdateWidget::~SystemEmbeddedUpdateWidget()
{
    delete ui;

    SAFE_DELETE(m_httpOperSync);
    SAFE_DELETE(m_pProgressDialog);

#ifdef Q_OS_WIN
    setFilterDebugHook();
#endif
}

/**************************************************************************
* 函数名: doCheckUpdate
* 功能: 检查嵌入式版本更新
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
bool SystemEmbeddedUpdateWidget::doCheckUpdate(bool bCanShowMBox)
{
    //
    m_bCanShowMBox = bCanShowMBox;

    m_strLocalSysEmbedVersion = "";
    m_LatestVersionInfo.reset();

    // 检查、更新版本，需要根据硬件版本和嵌入式版本从服务器上获取
    // 最新版本信息
    // 1、调用接口获取当前机器人嵌入式版本，获取到嵌入式版本后再调用接口获取硬件版本
    // 2、调用接口获取服务器嵌入式嵌入式版本
    // 3、比较两版本，判断是否需要更新

    // 菜单点击方式才会触发，防止其他地方请求
    // 会触发版本信息的界面弹出
    setOnMenuClickedTrigger(true);

    // 依次获取当前嵌入式版本、机器人型号
    reqLocalSystemEmbedVersion();

    return true;
}

bool SystemEmbeddedUpdateWidget::isForceCopyUpateVersion()
{
    if (m_strLocalSysEmbedVersion.compare(UpdateModuleConstant::SYSTEM_EMBEDED_VERSION_082214) == 0)
    {
        return true;
    }

    return false;
}

/**************************************************************************
* 函数名: setOnMenuClickedTrigger
* 功能: 用户点击菜单栏的固件版本标识
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
void SystemEmbeddedUpdateWidget::setOnMenuClickedTrigger(bool bIsMenuClickedTrigger)
{
    m_bOnMenuClickedTrigger = bIsMenuClickedTrigger;
}

void SystemEmbeddedUpdateWidget::timerEvent(QTimerEvent *event)
{
    int nIDEvent = event->timerId();
    if (m_nUpdateProgressTimerID == nIDEvent)
    {
        m_nTimeCount++;
        QString strInfo = tr("Activating the firmware, it may takes several minutes, please keep your Alpha in connect with computer and wait...") + QString("(%1 S)").arg(m_nTimeCount);
        m_pProgressDialog->setLabelText(strInfo);
        return;
    }
}

void SystemEmbeddedUpdateWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

/*************
* 函数名:mouseMoveEvent
* 功能:鼠标移动事件
* 参数:
* 返回值:
* 时间: 2016/06/07
* 作者: hwx
*****************/
void SystemEmbeddedUpdateWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() &Qt::LeftButton)
    {
        if (m_dragPosition != QPoint(-1, -1))
            move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

/*************
* 函数名:mousePressEvent
* 功能:鼠标按下事件
* 参数:
* 返回值:
* 时间: 2016/06/07
* 作者: hwx
*****************/
void SystemEmbeddedUpdateWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

/*************
* 函数名:mouseReleaseEvent
* 功能:鼠标离开事件
* 参数:
* 返回值:
* 时间: 2016/06/07
* 作者: hwx
*****************/
void SystemEmbeddedUpdateWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragPosition = QPoint(-1, -1);
        event->accept();
    }
}

/**************************************************************************
* 函数名: slotOnUpdate
* 功能: 更新版本
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
void SystemEmbeddedUpdateWidget::slotOnUpdate()
{
    // 为了使用和机器人通信的文件传输方式
    // 跟新时先将更新文件下载到本地，再从本地
    // 传输文件到机器人

    // 这里不能调用slotClosed()，不应该设置
    // m_bOnMenuClickedTrigger，否则下载
    // 进度条会受影响
    this->close();

    if (!downloadUpdateToLocalComputer())
    {
        if(m_bCanShowMBox)
        {
            CMessageBox::critical(NULL, tr("Error"), tr("Download update file failed"));
        }

        return;
    }

    if (!downloadUpdateToRobot())
    {
        if(m_bCanShowMBox)
        {
            CMessageBox::critical(NULL, tr("Error"), tr("Download update file to robot failed"));
        }

        return;
    }

//    // 这里不能调用slotClosed()，不应该设置
//    // m_bOnMenuClickedTrigger，否则下载
//    // 进度条会受影响
//    this->close();
}

/**************************************************************************
* 函数名: slotOnCancelUpdate
* 功能: 取消更新版本
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
void SystemEmbeddedUpdateWidget::slotOnCancelUpdate()
{
    AlphaRobotLog::getInstance()->printDebug("Cancel update");

    // 由于点击下载后，下载界面自动关闭，此时不能设
    // m_bOnMenuClickedTrigger为false，因此需要在按下
    // 取消后复位该值
    setOnMenuClickedTrigger(false);
    g_pRobotController->Req_CancelTransFile();
}

/**************************************************************************
* 函数名: slotOnClosed
* 功能: 关闭界面的槽函数
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
void SystemEmbeddedUpdateWidget::slotOnClosed()
{
    // 恢复是否菜单触发的标识
    setOnMenuClickedTrigger(false);
    close();
}

#if 0
/**************************************************************************
* 函数名: updateFileDownloadNotify
* 功能: 下载进度通知
* 时间: 2016/07/23 8:51
* 作者: hwx
*/
void CALLBACK updateFileDownloadNotify(LPVOID pUser, const QString& sFileName, qint64 nFileTotalSize, qint64 nFileCurSize)
{
    SystemEmbeddedUpdateWidget* pWidget = (SystemEmbeddedUpdateWidget*)pUser;
    if(NULL == pWidget)
    {
        return;
    }

    QProgressDialog *progressDialog = pWidget->getProgressDialog();
    if (NULL == progressDialog)
    {
        return;
    }

    QString sName = sFileName.mid(sFileName.lastIndexOf(QDir::separator()) + 1);

    QString sTemp;
    sTemp.sprintf("%s %s", sName.toStdString().c_str(), "UpdateProgress");
    progressDialog->setWindowTitle(sTemp);

    double fprocess = nFileCurSize*1.0/nFileTotalSize * 100;
    int nCurCtrlPos =  progressDialog->value();
    if(fprocess - nCurCtrlPos >= 1)
    {
        progressDialog->setValue(fprocess);

        //sTemp.sprintf("%d%%", (int)fprocess);
        //pDlg->ui->label_processinfo->setText(sTemp);
    }
}
#endif

/**************************************************************************
* 函数名: downloadUpdateToLocalComputer
* 功能: 下载更新文件到电脑本地目录
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
bool SystemEmbeddedUpdateWidget::downloadUpdateToLocalComputer()
{
    if (NULL == m_httpOperSync)
    {
        m_httpOperSync = new QtHttpOperSync(this);
    }

    QString strUrl = m_LatestVersionInfo.m_sUrl;
    QString strSaveFilePath = getLocalDownloadFilePath();

    //////////////////////////////Log//////////////////////////////
    QString sLog = QString("http request: url->%1; save file path: %2").arg(strUrl, strSaveFilePath);
    AlphaRobotLog::getInstance()->printDebug(sLog);
    /////////////////////////////Log///////////////////////////////

    ///////////////////////////////////////////////////////////
    // 相对于从PC下载软件到机器人，从服务器
    // 下载软件到PC很快，暂时不添加进度条，
    // 从而不要让用户感知更新软件先下载到本地PC
//    m_pProgressDialog->reset();
//    m_pProgressDialog->setMinimum(0);
//    m_pProgressDialog->setMaximum(100);
    ///////////////////////////////////////////////////////////
   if (!m_httpOperSync->DownloadFile(strUrl, strSaveFilePath, NULL, this))
   // if (!m_httpOperSync->DownloadFile(strUrl, strSaveFilePath, updateFileDownloadNotify, this))
    {
        AlphaRobotLog::getInstance()->printError(QString("Failed to download file %1 from %2").arg(strSaveFilePath).arg(m_LatestVersionInfo.m_sUrl));
        return false;
    }

    AlphaRobotLog::getInstance()->printDebug(QString("Download file %1 to local successfully").arg(strSaveFilePath));

    return true;
}

/**************************************************************************
* 函数名: downloadUpdateToRobot
* 功能: 从电脑本地目录传输版本更新文件到机器人
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
bool SystemEmbeddedUpdateWidget::downloadUpdateToRobot()
{
    //TODO:周志平 该模块2代要重做，暂时屏蔽
//    char* pGuid = NULL;
//    char* pSrcFile = NULL;
//    char* pTransFile = NULL;
//    char* pDestFile = NULL;

//    QString sLocalFilePath = getLocalDownloadFilePath();
//    if (!QFile::exists(sLocalFilePath))
//    {
//        AlphaRobotLog::getInstance()->printError(QString("File %1 doesn't exist").arg(sLocalFilePath));
//        return false;
//    }

//    //sLocalFilePath = "E:/workspace/build-AlphaRobot-Desktop_Qt_5_5_1_MSVC2010_32bit-Debug/AlphaRobot/debug/actlib/1468570878055/1468570878055.hts";
//    //sLocalFilePath = "E:/workspace/build-AlphaRobot-Desktop_Qt_5_5_1_MSVC2010_32bit-Debug/AlphaRobot/debug/actlib/1468570878055/1468570878055.mp3";
////    sLocalFilePath = "E:/workspace/build-AlphaRobot-Desktop_Qt_5_5_1_MSVC2010_32bit-Debug/AlphaRobot/debug/actlib/1468570878055/alpha1_v2016070416.bin";

//    QFileInfo fileInfo(sLocalFilePath);
//    QString sDestFilePath = fileInfo.fileName();
//    QString sGuid = QUuid::createUuid().toString();

//    int nGuidLen = UnicodeToAnsi(sGuid, pGuid);
//    int nSrcFileLen = UnicodeToAnsi(sLocalFilePath, pSrcFile);
//    int nTransFileLen = UnicodeToAnsi(sLocalFilePath, pTransFile);
//    int nDestFileLen = UnicodeToAnsi(sDestFilePath, pDestFile);

//    memcpy(m_TransFileInfo.sGuid, pGuid, nGuidLen);
//    memcpy(m_TransFileInfo.sSrcFilePath, pSrcFile, nSrcFileLen);
//    memcpy(m_TransFileInfo.sTransFilePath, pTransFile, nTransFileLen);
//    memcpy(m_TransFileInfo.sDestFilePath, pDestFile, nDestFileLen);

//    SAFE_DELETE_ARRAY(pGuid);
//    SAFE_DELETE_ARRAY(pSrcFile);
//    SAFE_DELETE_ARRAY(pTransFile);
//    SAFE_DELETE_ARRAY(pDestFile);

//    ///////////////////////////////Log////////////////////////////
//    QString sLog = QString("download %1 to robot %2").arg(sLocalFilePath).arg(sDestFilePath);
//    AlphaRobotLog::getInstance()->printDebug(sLog);
//    //////////////////////////////Log//////////////////////////////

//    m_TransFileInfo.lFileSize = CUtil::GetFileSize(sLocalFilePath);
//    m_TransFileInfo.lTransTimeBegin = QDateTime::currentDateTime().toTime_t(); //记录传输开始时间

////    if (!m_pProgressDialog)
////    {
//        m_pProgressDialog = new CProgressDialog(tr("Update robot software"), tr("Abort"), 0, 100);
////        m_pProgressDialog->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
//         m_pProgressDialog->setMinimumDuration(0);

//        m_pProgressDialog->setModal(true);
//        m_pProgressDialog->setAutoClose(true);
//        m_pProgressDialog->setKeyPressEnable(false);
//        connect(m_pProgressDialog, &QProgressDialog::canceled, this, &SystemEmbeddedUpdateWidget::slotOnCancelUpdate);
////    }
////    m_pProgressDialog->reset();
////    m_pProgressDialog->setMinimum(0);
////    m_pProgressDialog->setMaximum(100);
////    m_pProgressDialog->setMaximum(m_TransFileInfo.lFileSize);
//    /////////////////////////////////////Log//////////////////////////////////////
//    sLog = QString("Start transfer file %1").arg(sLocalFilePath);
//    AlphaRobotLog::getInstance()->printDebug(sLog);
//    ////////////////////////////////////Log///////////////////////////////////////
//    BOOL bRet = g_pRobotController->Req_TransFile(&m_TransFileInfo, emTranFileUpdateSysEmbed);
//    if (!bRet)
//    {
//        AlphaRobotLog::getInstance()->printError("Failed to update software version");
//        return false;
//    }

    return true;
}

/**************************************************************************
* 函数名: getLocalDownloadFilePath
* 功能: 获取下载到本地的更新文件
*
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
QString SystemEmbeddedUpdateWidget::getLocalDownloadFilePath()
{
    QString strUrl = QString(m_LatestVersionInfo.m_sUrl);
    if (strUrl.isEmpty())
    {
        return "";
    }

    QFileInfo fileInfo(strUrl);
    return QDir::toNativeSeparators(CConfigs::getLocalTempDir() + "/" + fileInfo.fileName());
}

/**************************************************************************
* 函数名: getVersionInNumber
* 功能: 获取仅含有数字的版本号（如果含有V开头，则去掉V）
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
QString SystemEmbeddedUpdateWidget::getVersionInNumber(const QString &sVersion)
{
    if (sVersion.isEmpty())
    {
        return "";
    }

    QString strLatestVersion = sVersion;
    if(strLatestVersion.indexOf('V', 0, Qt::CaseInsensitive) == 0)
    {
        strLatestVersion = strLatestVersion.remove(0, 1);
    }

    return strLatestVersion;
}

void SystemEmbeddedUpdateWidget::slotOnChangedToUDiskResult(bool bOk)
{
    ///////////////////////////////////针对082214版本特殊处理///////////////////
    // 1 切换到U盘模式
    // 2 判断中英文，只有Action目录下存在中文名字，就是用中文默认动作
    // 3 格式化机器人内存卡
    // 4 拷贝默认动作和bin文件到机器人
    ///////////////////////////////////////////////////////////////////////////////////////////////

    if (!bOk)
    {
        AlphaRobotLog::getInstance()->printDebug("Failed changed Udisk");
        return;
    }

    AlphaRobotLog::getInstance()->printDebug("Success changed Udisk");
    m_oThreadWaitUdiskShowUp.start();
}

void SystemEmbeddedUpdateWidget::slotOnRobotUDiskShowUp(bool bShowup)
{
    if (!bShowup)
    {
        AlphaRobotLog::getInstance()->printError("Failed to detect udisk.");
        CMessageBox::critical(NULL, tr("Error"), tr("Failed to detect robot disk card"));
        m_oThreadWaitUdiskShowUp.quit();
        m_oThreadWaitUdiskShowUp.wait();
        m_pProgressDialog->close();
        return;
    }

    QString strBsdPath;
    QString strUdiskName = CUBXUdiskManager::getUdiskName(strBsdPath);
    QString strUdiskRootPath = CUBXUdiskManager::getUdiskRootPath();

    AlphaRobotLog::getInstance()->printDebug(QString("get UBT udisk disk name: %1").arg(strUdiskName));
    if (strUdiskName.isEmpty() || strUdiskRootPath.isEmpty())
    {
        CMessageBox::critical(NULL, tr("Error"), tr("Failed to detect robot disk card"));
        return;
    }

    m_oThreadCopyFile.setRobotRootPath(strUdiskRootPath);

    // 判断U盘动作文件中是否存在中文名称的动作
    QString strActionDir = CConfigs::getFilePath(strUdiskRootPath, UpdateModuleConstant::ROBOT_ACTION_FILE_PATH);
    m_bIsChinese  = isActionContainChineseChar(strActionDir);

    m_oThreadCopyFile.setLanguageFlag(m_bIsChinese);
    m_oThreadCopyFile.setRobotFlag(isRobotType1P());

    QString strBinPath = getBinFilePath(m_bIsChinese);
    if (strBinPath.isEmpty())
    {
        AlphaRobotLog::getInstance()->printError(QString("get null bin file path"));
    }
    m_oThreadCopyFile.setBinFilePath(QDir::toNativeSeparators(strBinPath));

    // 格式化机器人移动盘
    formatUDisk(UpdateModuleConstant::UDISK_FORMAT_FAT32, strUdiskName, strBsdPath);
}

/**************************************************************************
* 函数名: reqRobotVersion
* 功能: 获取当前机器人类型，Alpha1s
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
void SystemEmbeddedUpdateWidget::reqRobotHardwareVersion()
{
        //TODO:周志平 该模块2代要重做，暂时屏蔽
//    if (NULL == g_pRobotController)
//    {
//        AlphaRobotLog::getInstance()->printError("doUpdate(): null g_pRobotController");
//        return;
//    }

//    AlphaRobotLog::getInstance()->printDebug("request robot type");
//    g_pRobotController->Req_RobotHardwareVersion();
}

/**************************************************************************
* 函数名: reqLocalSystemEmbedVersion
* 功能: 获取嵌入式版本
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
void SystemEmbeddedUpdateWidget::reqLocalSystemEmbedVersion()
{
        //TODO:周志平 该模块2代要重做，暂时屏蔽
//    if (NULL == g_pRobotController)
//    {
//        AlphaRobotLog::getInstance()->printError("reqLocalSystemEmbedVersion(): null g_pRobotController");
//        return;
//    }

//   AlphaRobotLog::getInstance()->printDebug("reqLocalSystemEmbedVersion: Get robot system version");
//    g_pRobotController->Req_RobotSysEmbeddedVersion();
}

/**************************************************************************
* 函数名: reqLatestVersionOnServer
* 功能: 从服务器请求最新版本
* 时间: 2016/07/26 16:39
* 作者: hwx
*/
void SystemEmbeddedUpdateWidget::reqLatestSysEmbededVersionOnServer()
{
    QString sUpdateUrl = UBXUpdateAdministrator::getInstance()->getUpdateBasicUrl();
//#ifndef QT_NO_DEBUG
////    m_eRobotType = eCheckUpdateTypeNewRobot1SV2;  //更新版1S机器人端软件
//    sUpdateUrl =  "http://10.10.1.14:8080/ubx/version/verify/";
// #endif

    if (sUpdateUrl.isEmpty() || m_strLocalSysEmbedVersion.isEmpty() || m_eRobotType == eCheckUpateTypeUnknown)
    {
        return;
    }

    CheckUpdate(sUpdateUrl, m_LatestVersionInfo, m_strLocalSysEmbedVersion, m_eRobotType);
}

/**************************************************************************
* 函数名: OnNotify
* 功能: 处理通知消息
*     * 参数:
*    @[in ] pSubject: 主题
*    @[in ] nMsgID: 消息ID
*    @[in ] dwContext: 上下文
*    @[in ] wParam: 参数1
*    @[in ] lParam: 参数2
*
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
void SystemEmbeddedUpdateWidget::OnNotify(const zxObserverSubjectBase* pSubject, int nMsgID, DWORD dwContext, WPARAM wParam, LPARAM lParam)
{
    zxEvent* pNewEvent = new zxEvent(nMsgID);
    pNewEvent->eEventType = zxEventTypeWindow;
    pNewEvent->lReceiver = (long)this;
    pNewEvent->dwMsgId = nMsgID;
    pNewEvent->wParam = wParam;
    pNewEvent->lParam = lParam;
    QCoreApplication::postEvent(this, pNewEvent);
}

/**************************************************************************
* 函数名: processRobotSoftwareVersion
* 功能: 处理嵌入式版本信息
*     * 参数:
*    @[in ] wParam: 参数1，对应参数2的char长度
*    @[in ] lParam: 参数2
*
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
bool SystemEmbeddedUpdateWidget::processRobotSoftwareVersion(WPARAM wParam, LPARAM lParam)
{
    // wParam为字符串长度
    if (wParam <= 0)
    {
        if(m_bCanShowMBox)
        {
            CMessageBox::information(NULL, tr("Information"), tr("Failed to get robot system software version"));
        }
        AlphaRobotLog::getInstance()->printDebug("Failed to get system embeded version");
    }

    QString strCurrentVersion((char *)lParam);
    m_strLocalSysEmbedVersion = strCurrentVersion;

    AlphaRobotLog::getInstance()->printDebug(QString("Robot system embeded version: %1").arg(strCurrentVersion));

    // 请求机器人型号信息
     reqRobotHardwareVersion();

    return true;
}

/**************************************************************************
* 函数名: processRobotHardwareVersion
* 功能: 处理机器人版本信息
*     * 参数:
*    @[in ] wParam: 参数1，对应参数2的char长度
*    @[in ] lParam: 参数2
*
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
bool SystemEmbeddedUpdateWidget::processRobotHardwareVersion(WPARAM wParam, LPARAM lParam)
{
    QString strRobotType((char *) lParam);
    AlphaRobotLog::getInstance()->printDebug(QString("Robot hardware version: %1").arg(strRobotType));

    // wParam为bool类型
    if (!wParam)
    {
        m_eRobotType = eCheckUpateTypeUnknown;
        CMessageBox::critical(NULL, tr("Error"), tr("Failed to read robot version"));
    }
    else
    {
        if (strRobotType.indexOf(UpdateModuleConstant::STRING_VERSION_ALPHA1S_V, 0, Qt::CaseInsensitive) >= 0)
        {
            m_eRobotType = eCheckUpdateTypeNewRobot1S;  //更新版1S机器人端软件
        }
        else if ( strRobotType.indexOf(UpdateModuleConstant::STRING_VERSION_ALPHA1P_V,  0, Qt::CaseInsensitive) >= 0)
        {
            m_eRobotType = eCheckUpdateTypeRobot1P; //更新1P机器人端软件
        }
        else if (strRobotType.indexOf(UpdateModuleConstant::STRING_VERSION_ALPHA1SP_V, 0, Qt::CaseInsensitive) >= 0)
        {
            m_eRobotType = eCheckUpdateTypeRobot1SP; // 更新机器人1SP版本
        }
        else if (strRobotType.indexOf(UpdateModuleConstant::STRING_VERSION_ALPHA1P_V2, 0, Qt::CaseInsensitive) >= 0)
        {
            m_eRobotType = eCheckUpdateTypeRobot1PV2; // 更新机器人1P V2.05版本
        }
        else if (strRobotType.indexOf(UpdateModuleConstant::STRING_VERSION_ALPHA1P_PLUS_V2, 0, Qt::CaseInsensitive) >= 0)
        {
            m_eRobotType = eCheckUpdateTypeRobot1PPlus;  // 更新机器人1P Plus版本
        }
        // 注意，由于版本字符串之间存在包含关系，如V2.05包含V2.0，因此应该优先判断V2.05
        else if (strRobotType.indexOf(UpdateModuleConstant::STRING_VERSION_ALPHA1S_V2,  0, Qt::CaseInsensitive) >= 0)
        {
            m_eRobotType = eCheckUpdateTypeNewRobot1SV2; //更新版1SV2.0机器人端软件
        }
        else
        {
            m_eRobotType = eCheckUpdateTypeOldRobot1S; //更新旧版1S机器人端软件
        }
    }

    // 如果需要强制拷贝升级，则切换到U盘模式拷贝bin文件升级
    if (isForceCopyUpateVersion())
    {
        CMessageBox::information(NULL, tr("Info"), tr("Prepare to activate the robot firmwork. Please place Alpha horizontally and plug in the adapter to ensure sufficient power"));

        repaint();

        emit sigIsSystemEmbedVersionNeedCopyUpdate(true);

        m_pProgressDialog = new CProgressDialog(NULL);
        m_pProgressDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
        m_pProgressDialog->setAttribute(Qt::WA_AlwaysStackOnTop);
        m_pProgressDialog->setModal(true);
        m_pProgressDialog->setCancelButton(NULL);
        m_pProgressDialog->setRange(0, 0);
        m_pProgressDialog->setMinimumDuration(0);
//        m_pProgressDialog->setStyleSheet("background: #404040; color: 12pt;");
//        QLabel *pLabelText = new QLabel(m_pProgressDialog);
//        pLabelText->setStyleSheet("color: #d3d3d3, font: 12pt");
        m_pProgressDialog->setLabelText(tr("Activating the firmware, it may takes several minutes, please keep your Alpha in connect with computer and wait..."));
//        m_pProgressDialog->setLabel(pLabelText);
        Sleep(1000);
        m_pProgressDialog->show();
        update();
        m_nUpdateProgressTimerID = startTimer(UpdateModuleConstant::TIMEOUT_1000MS, Qt::PreciseTimer);
        m_nTimeCount = 0;
    }
    else
    {
        emit sigIsSystemEmbedVersionNeedCopyUpdate(false);

        // 从服务器获取当前嵌入式最新版本
        // 由于从服务器获取版本时，还需要传入本地版本，因此
        // 需要再获取到本地版本后进行（串行执行）
        reqLatestSysEmbededVersionOnServer();
        slotUpdateUI();
    }

    return true;
}

/**************************************************************************
* 函数名: updateUI
* 功能: 更新UI版本信息
*
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
void SystemEmbeddedUpdateWidget::slotUpdateUI()
{
//#ifndef QT_NO_DEBUG
//    m_strLocalSysEmbedVersion = "2016070415";
//#endif

    if (!m_strLocalSysEmbedVersion.isEmpty())
    {
        ui->labLocalVersionText->setText(m_strLocalSysEmbedVersion);
    }
    else
    {
        ui->labLocalVersionText->setText(tr("Unknown"));
    }

    // 获取的版本含有V，如V20170726
    QString strLatestVersion = getVersionInNumber(m_LatestVersionInfo.m_sVersion);
    if (!strLatestVersion.isEmpty())
    {
        ui->labLatestVerionText->setText(strLatestVersion);
    }
    else
    {
        ui->labLatestVerionText->setText(tr("Unknown"));
    }

    // 当前版本未知，服务器上能获取到版本

    // 当前版本已知，服务器上获取不到版本

    // 当前版本和服务器上版本均已知
    if ((!strLatestVersion.isEmpty()) && (!m_strLocalSysEmbedVersion.isEmpty()))
    {
        if (CompareVersion(m_strLocalSysEmbedVersion, strLatestVersion, m_eRobotType) <= 0)
        {
            ui->pbUpdate->setEnabled(false);
            emit sigSystemEmbedVersionUpdateStatus(CHECKRESULT_NOUPDATE);
        }
        else
        {
            ui->pbUpdate->setEnabled(true);            
            emit sigSystemEmbedVersionUpdateStatus(CHECKRESULT_NEEDUPDATE);
        }
    }

    ui->versiontextEdit->clear();
    if (!strLatestVersion.isEmpty())
    {
        ui->versiontextEdit->insertPlainText(strLatestVersion);
        ui->versiontextEdit->insertPlainText(tr(" Description"));
        ui->versiontextEdit->insertPlainText("\n");
    }

    for (int nUpdateDes = 0; nUpdateDes < m_LatestVersionInfo.m_slstDescriptions.size(); nUpdateDes++)
    {
        QString sDesItem = m_LatestVersionInfo.m_slstDescriptions[nUpdateDes] + "\n";
        ui->versiontextEdit->append(sDesItem);
    }
}

void SystemEmbeddedUpdateWidget::slotOnFormatUdiskFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
     AlphaRobotLog::getInstance()->printDebug("finish format Udisk");

    if (exitStatus != QProcess::NormalExit)
    {
        CMessageBox::critical(NULL, tr("Error"), tr("Format robot udisk failed"));
        return;
    }

    m_oThreadCopyFile.start();
}

void SystemEmbeddedUpdateWidget::slotOnFormatUdiskStateChanged(QProcess::ProcessState newState)
{
    AlphaRobotLog::getInstance()->printDebug(QString("Format udisk current state: %1").arg((int) newState));
    qDebug() << "Format udisk current state: " << newState << endl;
}

void SystemEmbeddedUpdateWidget::slotOnCopyFileFinished()
{
        //TODO:周志平 该模块2代要重做，暂时屏蔽
//    AlphaRobotLog::getInstance()->printDebug(QString("Finish copy file"));

//    // 设置为最新版本
//    QString strLatestVersion = getVersionInNumber(m_LatestVersionInfo.m_sVersion);
//    g_pRobotController->setRobotSoftVersion(strLatestVersion);
//    killTimer(m_nUpdateProgressTimerID);
//    if (m_pProgressDialog)
//    {
//        m_pProgressDialog->close();
//    }

//    CMessageBox::information(NULL, tr("Info"), tr("Please restart Alpha to complete the firmware activation. Please wait for the headlamp to blink until it is lit. It takes about 25 seconds. Please be patient."));
}

/**************************************************************************
* 函数名: onDownloadUpdateProgress
* 功能: 下载到机器人进度
*
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
void SystemEmbeddedUpdateWidget::onDownloadUpdateProgress(WPARAM wParam, LPARAM lParam)
{
    // 取消更新
    if (m_pProgressDialog->wasCanceled())
    {
        return;
    }

    SENDDATAOBJ* pCurSendFileInfo = (SENDDATAOBJ*)lParam;
    if(pCurSendFileInfo)
    {
        m_TransFileInfo.lSendedSize = pCurSendFileInfo->m_nCurSendSize; //当前文件已传输的长度

        //int lTotolSize = g_objTransFileList.GetTotalSize();
        //int lAllSendedSize = g_objTransFileList.GetSendedSize();
        int lTotolSize = pCurSendFileInfo->m_senddata.datalen;   //计划发送的当前文件长度
        int lAllSendedSize = pCurSendFileInfo->m_nCurSendSize; //当前文件已经发送的数据长度

        int nProgress = lAllSendedSize * 1.0 / lTotolSize * 100;

        AlphaRobotLog::getInstance()->printDebug(QString("send size: %1, total size: %2, percent: %3").arg(lAllSendedSize).arg(lTotolSize).arg(nProgress));

        int nLastProgress = m_pProgressDialog->value();

        if(nProgress - nLastProgress >= 1)
        {
            m_pProgressDialog->setValue(nProgress);
//              m_pProgressDialog->setValue(lAllSendedSize);
#if 0
            QString sShortName = CFileDirHandle::getFileName(pTransInfo->sSrcFilePath);
            QString sInfo = tr("Syncing... ") + sShortName + tr("，Time Remaining");

            qint64 lCurTime = QDateTime::currentDateTime().toTime_t();//单位秒
            qint64 lDifTime = (lCurTime - pTransInfo->lTransTimeBegin); //时间差
            if(lDifTime == 0)
                lDifTime = 1;
            int nTransRate = lAllSendedSize / lDifTime;
            if(nTransRate == 0)
                nTransRate = 1;
            int nRemainTime = (lTotolSize - lAllSendedSize) / nTransRate;

            QString sTimeStr;
            if(nRemainTime >= 3600)
            {
                sTimeStr.sprintf("%02d:%02d:%02d", nRemainTime / 3600, nRemainTime % 3600 / 60, nRemainTime % 3600 % 60);
            }
            else
            {
                sTimeStr.sprintf("%02d:%02d", nRemainTime / 60, nRemainTime % 60);
            }

            sInfo += " " + sTimeStr + " ...";
#endif
        }
    }

    return;
}

/**************************************************************************
* 函数名: onDownloadUpdateProgressEnd
* 功能: 下载到机器人完成
*
* 时间: 2016/07/23 16:39
* 作者: hwx
*/
void SystemEmbeddedUpdateWidget::onDownloadUpdateProgressEnd(WPARAM wParam, LPARAM lParam)
{
        //TODO:周志平 该模块2代要重做，暂时屏蔽
//    AlphaRobotLog::getInstance()->printDebug("Update download to robot finish");
//    QString strLatestVersion = getVersionInNumber(m_LatestVersionInfo.m_sVersion);
//    g_pRobotController->setRobotSoftVersion(strLatestVersion);

//    // 由于点击下载后，下载界面自动关闭，此时不能设
//    // m_bOnMenuClickedTrigger为false，因此需要再下
//    // 完成后复位该值
//    setOnMenuClickedTrigger(false);

//    // 版本更新完成后需要通知
//    //    SENDDATAOBJ* pCurSendFileInfo = (SENDDATAOBJ*)lParam;
}

bool SystemEmbeddedUpdateWidget::isActionContainChineseChar(const QString &strFilePath)
{
    if (strFilePath.isEmpty())
    {
        return false;
    }

    QDir dir(strFilePath);
    if (!dir.exists())
    {
        AlphaRobotLog::getInstance()->printError(QString("Dir %1 not exist!").arg(strFilePath));
        return false;
    }

    QStringList slstFilter;
    slstFilter << "*.hts";
    QFileInfoList slstActionList = dir.entryInfoList(slstFilter);
    for (int i = 0; i < slstActionList.size(); i++)
    {
        QFileInfo fileInfo = slstActionList[i];
        if (isContainsChineseChar(fileInfo.filePath()))
        {
            return true;
        }
    }

    return false;
}

bool SystemEmbeddedUpdateWidget::isContainsChineseChar(const QString &strText)
{
    return strText.contains(QRegExp("[\u4e00-\u9fa5]+"));
}

void SystemEmbeddedUpdateWidget::formatUDisk(const QString &strFormat, const QString &strDiskName, const QString strBSDName)
{
    AlphaRobotLog::getInstance()->printDebug(QString("start format Udisk: %1, %2, %3").arg(strFormat).arg(strDiskName).arg(strBSDName));
#ifdef Q_OS_WIN
    m_formatUdiskProcess.start("cmd", QStringList()<<"/c"<<QString("format %1:/fs:%2 /q /y").arg(strDiskName).arg(strFormat));
#else
    m_formatUdiskProcess.start("diskutil", QStringList()<<"eraseDisk"<< strFormat << strDiskName << "MBRFormat"<< strBSDName);
#endif
    m_formatUdiskProcess.waitForStarted(UpdateModuleConstant::WAIT_FOR_FOTMAT_UDISK_START_TIMEOUT);
}


bool SystemEmbeddedUpdateWidget::isRobotType1P()
{
    bool bIsRobot1P = false;
    switch (m_eRobotType) {
    case eCheckUpdateTypeRobot1P:
    case eCheckUpdateTypeRobot1PV2:
    case eCheckUpdateTypeRobot1PPlus:
        bIsRobot1P = true;
        break;
    default:
        break;
    }

    return bIsRobot1P;
}

QString SystemEmbeddedUpdateWidget::getBinFilePath(bool bIsChinese)
{
    QString strBinPath = CConfigs::getFilePath(CConfigs::getLocalResDir(), UpdateModuleConstant::CN_BIN_FILE_2016103001);
    if (!bIsChinese)
    {
        strBinPath = CConfigs::getFilePath(CConfigs::getLocalResDir(), UpdateModuleConstant::EN_BIN_FILE_2016103001);
    }

    return strBinPath;
}
void SystemEmbeddedUpdateWidget::customEvent(QEvent *event)
{
        //TODO:周志平 该模块2代要重做，暂时屏蔽
//    // 如果不是由用户点击菜单触发，则忽略不处理(在连接机器人时也会进行同样请求，此时会忽略)
////    if (!m_bOnMenuClickedTrigger)
////    {
////        AlphaRobotLog::getInstance()->printDebug("Not menu clicked update of system embed");
////        return;
////    }

//    zxEvent* pEvent = (zxEvent*)event;
//    DWORD nMsgID = pEvent->dwMsgId;
//    WPARAM wParam = pEvent->wParam;
//    LPARAM lParam = pEvent->lParam;

//    // 获取嵌入式软件版本信息
//    if (WM_MSG_ROBOT_SOFTWARE_VERSION_GET == nMsgID)
//    {
//        if (processRobotSoftwareVersion(wParam, lParam))
//        {
//            return;
//        }
//    }
//    // 获取机器人类型信息
//    else if (WM_MSG_ROBOT_HARDWARE_VERSION_GET == nMsgID)
//    {
//        if (processRobotHardwareVersion(wParam, lParam))
//        {
//            return;
//        }
//    }
//    else if (WM_MSG_UPDATE_DOWNLOAD_ING == nMsgID)
//    {
//        onDownloadUpdateProgress(wParam, lParam);
//    }
//    else if (WM_MSG_UPDATE_DOWNLOAD_END == nMsgID)
//    {
//        onDownloadUpdateProgressEnd(wParam, lParam);
//    }
}
