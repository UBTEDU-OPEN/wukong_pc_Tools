#include "downloadmanager.h"
#include <QCoreApplication>
#include <QFileInfo>
#include "JlCompress.h"
#include <QDirIterator>
#include <QMessageBox>
#include <urldef.h>

DownloadManager::DownloadManager(QObject *parent) : QObject(parent)
{
    m_strFolder = QCoreApplication::applicationDirPath();
    m_param = new UpdateParam;
    // 初始化进度条
    m_progress = new QProgressBar;
    m_progress->setWindowModality(Qt::NonModal);
    m_progress->setWindowTitle(tr("检查更新"));
    m_progress->setWindowFlags(Qt::CustomizeWindowHint |
                               Qt::WindowTitleHint);
    m_progress->setRange(0, 100);
    m_progress->setValue(0);
    m_progress->setStyleSheet(
                QString("QProgressBar {"
                        "border: 2px solid grey;"
                        "border-radius: 5px;"
                        "text-align: center;"
                        "}"
                        "QProgressBar::chunk {"
                        "background-color: #CD96CD;"
                        "width: 10px;"
                        "margin: 0.5px;"
                        "}")
                );
    m_progress->hide();
}

DownloadManager::~DownloadManager ()
{
    m_file.close();
    m_progress->deleteLater();

    delete m_param;
    m_param = NULL;
}

void DownloadManager::startDownLoad ()
{
    QUrl url (m_param->getPackageUrl());
    if ( m_param->getIncremental() ){
        url.setUrl(m_param->getIncrementUrl());
    }
    QString strFileName = urlToLocalPath (url);
    if ( QFile::exists(strFileName) ){
        QFile::remove(strFileName);
    }
    m_file.setFileName(strFileName);
    if ( !m_file.open(QIODevice::WriteOnly) ){
        return ;
    }

    QNetworkRequest request(url);
    m_pReply = m_pManager.get(request);
    m_pManager.setNetworkAccessible(QNetworkAccessManager::Accessible);
    connect(m_pReply, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(slotDownloadProgress(qint64,qint64)));
    connect(m_pReply, SIGNAL(finished()),
            SLOT(slotDownloadFinished()));
    connect(m_pReply, SIGNAL(readyRead()),
            SLOT(slotDownloadReadyRead()));
    connect(&m_pManager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
            SLOT(slotNetWorkError(QNetworkAccessManager::NetworkAccessibility)));

    // 显示进度条
    m_progress->show();
    m_downloadTime.start();
}

void DownloadManager::slotDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << " recv : " << bytesReceived << ", total : " << bytesTotal;
    // calculate the download speed
    double speed = bytesReceived * 1000.0 / m_downloadTime.elapsed();
    QString unit;
    if (speed < 1024) {
        unit = "bytes/sec";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "kB/s";
    } else {
        speed /= 1024*1024;
        unit = "MB/s";
    }

    QString strSize = QString::number(bytesTotal/(1024*1024.0), 'f', 2);

    m_progress->setMaximum(bytesTotal);
    m_progress->setValue(bytesReceived);
    m_progress->setFormat(
                tr("size：%1M speed: %2%3 %p%")
                .arg(strSize)
                .arg(QString::number(speed, 'f', 2))
                .arg(unit)
                );
}

void DownloadManager::slotDownloadFinished()
{
    // 1、关闭文件
    m_file.close();

    // 2、下载失败，删除已下载的文件数据。
    //    下载成功，校验文件完整性。
    QString strTip = "";
    QString strFileName = m_file.fileName();
    if (m_pReply->error() != QNetworkReply::NoError) {
        m_file.remove();
        qDebug() << "download failed.";
    } else {
        QString strMd5Hex = clacMd5(strFileName).toHex();
        QString strServerMd5 = m_param->getPackageMd5();
        if ( m_param->getIncremental() ){
            strServerMd5 = m_param->getIncrementMd5();
        }
        if ( strMd5Hex == strServerMd5 ){
            // 更新包完整, 解压，拷贝、覆盖。
            installPackge(strFileName);
            strTip = tr("已更新至最新版本，请重新启动");
        }else{
            // 更新包不完整，删除
            m_file.remove();
            strTip = tr("更新出错");
        }
    }

    // 3、关闭请求
    m_pReply->deleteLater();

    // 4、隐藏进度条
    m_progress->hide();

    // 5、更新完毕后提醒
    QMessageBox::information(NULL, tr("检查更新"), strTip);

    emit finished();
}

void DownloadManager::slotDownloadReadyRead()
{
    m_file.write(m_pReply->readAll());
}

void DownloadManager::slotNetWorkError (QNetworkAccessManager::NetworkAccessibility error)
{
    if ( error != QNetworkAccessManager::Accessible ){
        QMessageBox::warning(NULL, tr("检查更新"), tr("网络异常，请检查网络情况。"));
        m_pReply->abort();
        m_pReply->deleteLater();
        emit finished();
    }
}

QByteArray DownloadManager::clacMd5 (QString strFilePath)
{
    QFile file(strFilePath);
    if ( !file.open(QIODevice::ReadOnly) ){
        return "";
    }
    QByteArray contentArray = file.readAll();
    file.close();
    QByteArray strMd5 = QCryptographicHash::hash(contentArray, QCryptographicHash::Md5);
    return strMd5;
}

QString DownloadManager::urlToLocalPath (QUrl url)
{
    QString strPath = url.path();
    QString strBaseName = QFileInfo(strPath).fileName();
    QString strFilePath = m_strFolder + "/" + strBaseName;
    return strFilePath;
}

void DownloadManager::installPackge (QString strFileName)
{
    // 解压压缩文件
    QStringList list = JlCompress::extractDir(strFileName, m_strFolder);
    if ( !list.isEmpty() ){
        QString strFilePath = list.at(0);
        // 覆盖文件
        copyDir(strFilePath, m_strFolder, true);
        // 删除压缩包
        QFile::remove(strFileName);
        // 删除压缩后的文件
        delDir (strFilePath);
    }else{
        qDebug() << "JlCompress::extractDir failed.";
    }
}

void DownloadManager::copyDir (QString strSrcDir, QString strDestDir, bool isOverride /*= false*/)
{
    if ( strSrcDir == strDestDir ){
        return ;
    }

    QDir srcDir(strSrcDir);
    QFileInfoList fileInfoList = srcDir.entryInfoList(QDir::Files |
                                                      QDir::Hidden |
                                                      QDir::NoDotAndDotDot |
                                                      QDir::NoSymLinks |
                                                      QDir::AllDirs);
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if (fileInfo.isDir())
        {
            QString strSubSrcPath = fileInfo.filePath ();
            if ( strSrcDir == strSubSrcPath ){
                continue;
            }
            QString strDestDirName = fileInfo.fileName();
            QString strSubDestDir = strDestDir + "/" + strDestDirName;
            QDir dir(strSubDestDir);
            if ( !dir.exists() ){
                dir.mkdir(strSubDestDir);
            }

            copyDir (strSubSrcPath, strSubDestDir, isOverride);
        }
        else
        {
            QString strFileName = fileInfo.fileName();
            QString strDestFilePath = strDestDir + "/" + strFileName;
            QString strSrcFilePath  = fileInfo.filePath();
            if ( isOverride ){
                if( QFile::exists(strDestFilePath) ){
                    QFile::remove(strDestFilePath);
                }
            }

            QFile::copy(strSrcFilePath, strDestFilePath);
        }
    }
}

void DownloadManager::delDir(QString strPath)
{
    QDir dir(strPath);
    if( !dir.exists() ){
        return;
    }

    // 删除文件
    QDirIterator it(strPath,
                    QStringList("*"),
                    QDir::Files,
                    QDirIterator::Subdirectories);
    while(it.hasNext()){
        dir.remove(it.next());
    }

    // 删除文件夹
    QDirIterator itDir(strPath,
                       QStringList("*"),
                       QDir::AllDirs,
                       QDirIterator::Subdirectories);
    while(itDir.hasNext()){
        dir.rmdir(itDir.next());
    }

    // 删除文件夹本身
    dir.rmdir(strPath);
}
