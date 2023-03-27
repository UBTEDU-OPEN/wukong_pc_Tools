#include "qtablecellwidget.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDirIterator>
#include <QJsonObject>
#include <QJsonDocument>

#include "JlCompress.h"
#include "cfileinfo.h"
#include "usbcomm.h"
#include "sharelog.h"
#include "cpubfun.h"

static const QString PARTIAL_DOWN (".part");

QTableCellWidget::QTableCellWidget(int nRow, QWidget *parent) :
    QWidget(parent), m_nRow(nRow)
{
    init ();
}

QTableCellWidget::~QTableCellWidget()
{
    m_pLabel->deleteLater();
    m_pDownloadBtn->deleteLater();
    m_pSyncBtn->deleteLater();
    m_pOpenFolderBtn->deleteLater();
    m_pDeleteBtn->deleteLater();

    if ( m_reply ){
        m_reply->abort();
        delete m_reply;
    }
    delete m_manager;
    delete m_pUsbComm;
}

void QTableCellWidget::init ()
{
    initVar ();
    initWindows ();
}

void QTableCellWidget::initVar ()
{
    m_pLabel         = Q_NULLPTR;
    m_pDownloadBtn   = Q_NULLPTR;
    m_pSyncBtn       = Q_NULLPTR;
    m_pOpenFolderBtn = Q_NULLPTR;
    m_pDeleteBtn     = Q_NULLPTR;
    m_reply          = Q_NULLPTR;
    m_strUrl         = "";
    m_strFileName    = "";
    m_pUsbComm       = new UsbComm;
    m_manager        = new QNetworkAccessManager();
    m_downloadDir    = QCoreApplication::applicationDirPath() + "/data";
}

void QTableCellWidget::initWindows ()
{
    m_pLabel = new QLabel(this);
    m_pLabel->setStyleSheet("font-size: 14px;"
                            "color: #6A6A6A;");
    m_pLabel->setText("");

    m_pDownloadBtn = new QPushButton(this);
    m_pDownloadBtn->setFixedSize(25, 25);
    m_pDownloadBtn->setStyleSheet("QPushButton{"
                                  "background:transparent;"
                                  "outline: none;"
                                  "font-size:12px;"
                                  "color: #12C2F4;"
                                  "}");
    setDownloadState(false);
    m_pDownloadBtn->setToolTip(tr("下载"));
    connect(m_pDownloadBtn, SIGNAL(pressed()), this, SLOT(slotDownload()));

    m_pSyncBtn = new QPushButton(this);
    m_pSyncBtn->setFixedSize(25, 25);
    m_pSyncBtn->setStyleSheet("QPushButton{"
                              "background:transparent;"
                              "outline: none;"
                              "font-size:12px;"
                              "color: #12C2F4;"
                              "}");
    m_pSyncBtn->setIcon(QIcon(":/png/png/ic_tongbu_gray.png"));
    m_pSyncBtn->setToolTip(tr("同步到机器人"));
    connect(m_pSyncBtn, SIGNAL(pressed()), this, SIGNAL(sync()));

    m_pOpenFolderBtn = new QPushButton(this);
    m_pOpenFolderBtn->setFixedSize(25, 25);
    m_pOpenFolderBtn->setStyleSheet("QPushButton{"
                                    "background:transparent;"
                                    "outline: none;"
                                    "}");
    m_pOpenFolderBtn->setIcon(QIcon(":/png/png/ic_folder_gray.png"));
    m_pOpenFolderBtn->setToolTip(tr("打开所在文件夹"));
    connect(m_pOpenFolderBtn, SIGNAL(pressed()), this, SLOT(slotOpenFolder()));

    m_pDeleteBtn = new QPushButton(this);
    m_pDeleteBtn->setFixedSize(25, 25);
    m_pDeleteBtn->setStyleSheet("QPushButton{"
                                "background:transparent;"
                                "outline: none;"
                                "}");
    m_pDeleteBtn->setIcon(QIcon(":/png/png/ic_garbage_gray.png"));
    m_pDeleteBtn->setToolTip(tr("删除"));
    connect(m_pDeleteBtn, SIGNAL(pressed()), this, SLOT(slotDelete()));

    QHBoxLayout* pHlayout = new QHBoxLayout(this);
    pHlayout->addWidget(m_pLabel);
    pHlayout->addWidget(m_pDownloadBtn);
    pHlayout->addWidget(m_pSyncBtn);
    pHlayout->addWidget(m_pOpenFolderBtn);
    pHlayout->addWidget(m_pDeleteBtn);
    pHlayout->setSpacing(10);
    pHlayout->setContentsMargins(QMargins(10,0,20,0));

    setLayout(pHlayout);
}

bool QTableCellWidget::isDownload ()
{
    return QFileInfo(m_downloadDir.filePath(m_strFileName)).exists();
}

void QTableCellWidget::setDownloadState (bool bDownload)
{
    QString res = bDownload ? ":/png/png/ic_finished.png" : ":/png/png/ic_download_gray.png";
    m_pDownloadBtn->setText("");
    m_pDownloadBtn->setIcon(QIcon(res));
}

void QTableCellWidget::setSyncWaiteState ()
{
    m_pSyncBtn->setText("");
    m_pSyncBtn->setIcon(QIcon(":/png/png/ic_loading.png"));
}

void QTableCellWidget::restoreSyncState ()
{
    m_pSyncBtn->setIcon(QIcon(":/png/png/ic_tongbu_gray.png"));
    m_pSyncBtn->setText("");
}

void QTableCellWidget::cancelSyncFile ()
{
    m_pUsbComm->setSendCancel(true);
}

void QTableCellWidget::setDownloadBtnVisible(bool bVisible)
{
    m_pDownloadBtn->setVisible(bVisible);
}

void QTableCellWidget::removeDir (const QString& strPath)
{
    QDir dir(strPath);
    if( !dir.exists() )
        return;

    // 删除文件
    QDirIterator it(strPath, QStringList("*"), QDir::Files, QDirIterator::Subdirectories);
    while(it.hasNext())
        dir.remove(it.next());

    // 删除文件夹
    QDirIterator itDir(strPath, QStringList("*"), QDir::AllDirs, QDirIterator::Subdirectories);
    while(itDir.hasNext())
        dir.rmdir(itDir.next());

    // 删除文件夹本身
    dir.rmdir(strPath);
}

void QTableCellWidget::startDeleteFolder ()
{
    removeDir(m_downloadDir.filePath (m_strFileName));
    QFile::remove (m_downloadDir.filePath (m_strUrl.split("/").last()));
    QFile::remove (m_downloadDir.filePath (m_strUrl.split("/").last() + PARTIAL_DOWN));
}

bool QTableCellWidget::requestUrl ()
{
    // 请求获取更新数据
    bool bRet = false;
    tagMusicInfo* pMusic = Q_NULLPTR;
    QNetworkRequest request;
    CPubFun::addSignRequest(&request, HTTP_POST_GETACTIONLIST_URL, APPID, "", qApp->applicationName());
    QJsonObject jValue;
    jValue.insert("fileName", m_strFileName);
    QJsonDocument document;
    document.setObject(jValue);
    QByteArray dataArray = document.toJson(QJsonDocument::Compact);
    QNetworkReply *reply = m_manager->post(request, dataArray);
    int nState = CPubFun::waitForReply (reply);
    int nHttpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if ( nState == EN_REQUEST_WAIT_TIME_OUT ) {
        LOG(ERROR) << HTTP_GET_ACTIONLIST_URL << ", request time out.";
        QMessageBox::critical (this,
                               tr ("错误"),
                               tr ("网络不给力，无法更新。"),
                               QMessageBox::Close);
    } else if ( nState == EN_REQUEST_WAIT_NETERROR ) {
        QString strError = tr ("网络错误，返回码: %1").arg(nHttpStatusCode);
        LOG(ERROR) << HTTP_GET_ACTIONLIST_URL << trs(strError);
        QMessageBox::critical (this,
                               tr ("错误"),
                               tr ("网络不给力，无法更新。"),
                               QMessageBox::Close);
    } else  {
        do{
            if ( nHttpStatusCode != 200 ){
                QString strError = tr ("错误，返回码: %1").arg(nHttpStatusCode);
                LOG(ERROR) << HTTP_GET_ACTIONLIST_URL << trs(strError);
                break;
            }

            QByteArray data = reply->readAll();
            QJsonDocument document = QJsonDocument::fromJson (data);
            if (document.isNull()) {
                LOG(ERROR) << "JSON is invalid";
                break;
            }

            pMusic = new tagMusicInfo;
            pMusic->m_strDisplayName = document.object().value("actionName").toString();
            pMusic->m_nMusicTime = document.object().value("actionTime").toString().toInt();
            pMusic->m_strDownLoadUrl = document.object().value("actionUrl").toString();
            setUrl(pMusic->m_strDownLoadUrl);
            bRet = true;
        }while(0);

        if ( !bRet ) {
            QMessageBox::critical (this,
                                   tr ("错误"),
                                   tr ("没有找到舞蹈(%1)，无法下载!").arg(m_strFileName),
                                   QMessageBox::Close);
        }
    }
    reply->abort();
    reply->deleteLater();

    if ( bRet ){
        // 刷新界面
        emit refreshMusicInfo(EN_UPDATE_MUSIC_TYPE, m_strFileName, pMusic);
        // 执行下载
        startDownload(m_strUrl);
    }

    return bRet;
}

void QTableCellWidget::setSelectRowTextStyle (bool bSelect)
{
    QString strStyle = "";
    if ( bSelect ) {
        strStyle = "QPushButton{"
                               "background:transparent;"
                               "outline: none;"
                               "font-size:12px;"
                               "color: #FFFFFF;"
                               "}";
    } else {
        strStyle = "QPushButton{"
                               "background:transparent;"
                               "outline: none;"
                               "font-size:12px;"
                               "color: #12C2F4;"
                               "}";
    }

    m_pDownloadBtn->setStyleSheet(strStyle);
    m_pSyncBtn->setStyleSheet(strStyle);
}

void QTableCellWidget::slotDownload ()
{
    if ( !m_strUrl.isEmpty() ){
        startDownload(m_strUrl);
    }else{
        requestUrl ();
    }
}

void QTableCellWidget::slotSync ()
{
    m_pSyncBtn->setEnabled(false);
    restoreSyncState ();
    QList<CFileInfo*> listFileInfo;
    QString strBasePath = m_downloadDir.filePath(m_strFileName);
    QString strActions = strBasePath + "/actions/" + m_strFileName + ".ubx";
    if ( QFile::exists(strActions) ){
        CFileInfo* pFileInfo = new CFileInfo(strActions);
        pFileInfo->m_nRobotPathType = EN_ROBOT_ACTIONS_TYPE;
        listFileInfo.append(pFileInfo);
    }

    QString strBehaviors = strBasePath + "/behaviors/" + m_strFileName + ".xml";
    if ( QFile::exists(strBehaviors) ){
        CFileInfo* pFileInfo = new CFileInfo(strBehaviors);
        pFileInfo->m_nRobotPathType = EN_ROBOT_BEHAVIORS_TYPE;
        listFileInfo.append(pFileInfo);
    }

    if (listFileInfo.isEmpty()){
        int nBtnNum = QMessageBox::warning(this,
                                           tr("集控软件"),
                                           tr("资源未下载，无法同步到机器人"),
                                           tr("取消"),
                                           tr("下载资源"),
                                           tr(""),
                                           1,
                                           1);
        if ( nBtnNum == 1 ) {
            slotDownload();
        }
    }else{
        connect(m_pUsbComm, SIGNAL(downloadProgress(qint64,qint64)),
                this, SLOT(syncProgress(qint64, qint64)));
        m_pUsbComm->slotSendFileInfo(listFileInfo);
        qDeleteAll(listFileInfo);
        listFileInfo.clear();
    }
    m_pSyncBtn->setEnabled(true);
}

void QTableCellWidget::slotOpenFolder ()
{
    QString strFolder = m_downloadDir.filePath (m_strFileName);
    bool bRet = QDesktopServices::openUrl(QUrl::fromLocalFile (strFolder));
    if ( !bRet ){
        QMessageBox::critical (this,
                               tr ("错误"),
                               tr ("打不开文件夹"),
                               QMessageBox::Close);
    }
}

void QTableCellWidget::slotDelete ()
{
    int nBtnNum = QMessageBox::warning(this,
                                       tr("集控软件"),
                                       tr("确定删除舞蹈吗"),
                                       tr("取消"),
                                       tr("确定"),
                                       tr(""),
                                       0,
                                       0);
    if ( nBtnNum == 0 ) {
        return ;
    }

    QList<int> list;
    list << m_nRow;
    emit removeRow(list);
}

void QTableCellWidget::startDownload (const QUrl& url)
{
    m_reply = m_manager->get(QNetworkRequest(url));
    /* Ensure that downloads directory exists */
    if ( !m_downloadDir.exists() )
        m_downloadDir.mkpath (".");

    /* Remove old downloads */
    removeDir (m_downloadDir.filePath (m_strFileName));
    QFile::remove (m_downloadDir.filePath (url.toString().split("/").last()));
    QFile::remove (m_downloadDir.filePath (url.toString().split("/").last() + PARTIAL_DOWN));

    /* Update UI when download progress changes or download finishes */
    connect (m_reply, SIGNAL (downloadProgress (qint64, qint64)),
             this,      SLOT (updateProgress   (qint64, qint64)));
    connect (m_reply, SIGNAL (redirected       (QUrl)),
             this,      SLOT (startDownload    (QUrl)));
}

void QTableCellWidget::updateProgress (qint64 received, qint64 total)
{
    if ( total > 0 ) {
        int nPercent = (received * 100) / total;
        m_pDownloadBtn->setIcon(QIcon(""));
        m_pDownloadBtn->setText(QString::number(nPercent) + "%");
        saveFile (received, total);
    }
}

/**
 * Writes the downloaded data to the disk
 */
void QTableCellWidget::saveFile (qint64 received, qint64 total) {
    /* Check if we need to redirect */
    QUrl url = m_reply->attribute (
                QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (!url.isEmpty()) {
        startDownload (url);
        return;
    }

    /* Save downloaded data to disk */
    QString strDownloadName = m_strUrl.split("/").last();
    QFile file (m_downloadDir.filePath (strDownloadName + PARTIAL_DOWN));
    if (file.open (QIODevice::WriteOnly | QIODevice::Append)) {
        file.write (m_reply->readAll());
        file.close();
    }

    /* Open downloaded update */
    if (received >= total && total > 0) {
        /* Rename file */
        QFile::rename (m_downloadDir.filePath (strDownloadName + PARTIAL_DOWN),
                       m_downloadDir.filePath (strDownloadName));

        /* Install the update */
        m_reply->close();

        JlCompress::extractDir(m_downloadDir.filePath (strDownloadName), m_downloadDir.path());
        setDownloadState(true);
    }
}

void QTableCellWidget::syncProgress(qint64 received, qint64 total)
{
    if ( total > 0 ) {
        int nPercent = (received * 100) / total;
        m_pSyncBtn->setIcon(QIcon(""));
        m_pSyncBtn->setText(QString::number(nPercent) + "%");

        if (received >= total) {
            restoreSyncState ();
        }
    }
}

void QTableCellWidget::setMusicTime (int nTime)
{
    m_pLabel->setText(QString::number(nTime));
}

int QTableCellWidget::getMusicTime ()
{
    return m_pLabel->text().toInt();
}

void QTableCellWidget::setUrl (const QString& url)
{
    m_strUrl = url;
}

QString QTableCellWidget::getUrl () const
{
    return m_strUrl;
}

void QTableCellWidget::setFileName (const QString& fileName)
{
    m_strFileName = (fileName.isEmpty() ? "test.bin" : fileName);
    setDownloadState(isDownload());
}

QString QTableCellWidget::getFileName () const
{
    return m_strFileName;
}

void QTableCellWidget::setRow (int nRow)
{
    m_nRow = nRow;
}

int QTableCellWidget::getRow ()
{
    return m_nRow;
}
