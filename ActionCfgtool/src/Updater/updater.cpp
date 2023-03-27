#include "updater.h"
#include "updaterinterface.h"
#include <vector>
#include <algorithm>
#include <functional>
#include "configs.h"
#include <QProcess>
#include "fileutils.h"
#include "httpclient.h"
#include "systemutils.h"
#include <QMutex>
#include <QWaitCondition>
#include <QTimer>
#include <qmath.h>
//#ifdef _DEBUG
#include <qdebug.h>
#include <QThread>
//#endif

using std::vector;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using std::placeholders::_4;

QMutex          gMutex;
QWaitCondition  doneCondition;

Updater::Updater(QWidget *parent)
    : QDialog(parent)
    , m_pUpdater(new UpdaterInterface)
    , m_curDownloadLen (0.0f)
    , m_preLen (0.0f)
{
    ui.setupUi(this);
    connect(this, &Updater::sigProgressUpdate, this, &Updater::slotProgressUpdate);
    connect(this, &Updater::sigUpdateStatusMsg, this, &Updater::slotUpdateStatusMsg);
    qDebug() << "main :" << QThread::currentThreadId();
    //connect(&m_timer, &QTimer::timeout, this, &Updater::slotTest);
}

void Updater::slotTest () {
    CURDI curInfo;
    if (m_curl.isDownloadBegin())
    {
        m_curl.getCurrentDownloadInfo(&curInfo);
        m_curDownloadLen = curInfo.curDownloadLen;
        //printf("正在下载:%s,下载进度:%6.2lf%%,下载速度:%9.2lfKB/s\r", curInfo.fileName,
        //    ((double)curInfo.preLocalLen + curInfo.CurDownloadLen) / curInfo.totalFileLen * 100, (m_curDownloadLen - m_preLen) / (double)(GetTickCount() - tick));
        ui.progressBar->setValue(int((double)curInfo.preLocalLen + curInfo.curDownloadLen) / curInfo.totalFileLen * 100);
    }
    if (m_curl.isDownloadEnd()) {
        m_curDownloadLen = m_preLen;
    }
}

void Updater::start()
{
    // 结束主进程
    m_dones.clear();
    int killCode = SystemUtils::killProcess(m_parentId);
    VERSIONINFO remote, native;
    FileUtils::clearDir(CConfigs::getDownloadPath(), false);
    const QString qsRemoteXmlPath = CConfigs::getDownloadPath() + "/" + VERSION_FILE_NAME;
    const QString qsNativeXmlDir = CConfigs::getLocalUserConfigDir();
    if (!m_pUpdater->parseVersionXmlFile(qsRemoteXmlPath, &remote) || !m_pUpdater->upgradingVersionXmlFile(qsNativeXmlDir, &native)) {
        //读取版本信息失败， 写入日志
#ifndef TEST
        return;
#endif
    }
    if (m_pUpdater->calculatingPatch(&native, &remote, m_patchs) && m_patchs.size() > 0) {
        startDownloadTask(m_patchs);
    }
    else {
        //不需要更新，退出更新进程，重启主程序
        QProcess *mainProcess = new QProcess;
        QString cmd = m_parentProcessPath;
        mainProcess->start(cmd);
        mainProcess->deleteLater();
    }
}

/**************************************************************************
* 函数名:   startDownloadTask
* 功能:
* 参数:
*....@[in]  const vector<ProgramFileInfo> & patchs
*....@[out] void
* 返回值:
* 时间:     2017/11/08 10:52
* 作者:   Leo
*/
void Updater::startDownloadTask(const list<ProgramFileInfo> &patchs)
{
    const QString qsDownloadDir = CConfigs::getDownloadPath() + "/";
    //ui.progressBar->show();
    //m_taskCount = m_patchs.size();
    //startOneTask();
    // new test
    for (auto it = patchs.begin(); it != patchs.end(); ++it) {
        DLIO work;
        const ProgramFileInfo & info = *it;
        work.url = info._url.toStdString();
        const QString &qsFullpath = qsDownloadDir + info._name;
        work.filePath = qsFullpath.toStdString();
        m_curl.addDownloadWork(work, std::bind(&Updater::downloadCallBack, this, _1, _2, _3));
    }

//    for each (ProgramFileInfo info in patchs) {
//        DLIO work;
//        work.url = info._url.toStdString();
//        const QString &qsFullpath = qsDownloadDir + info._name;
//        work.filePath = qsFullpath.toStdString();
//        m_curl.addDownloadWork(work, std::bind(&Updater::downloadCallBack, this, _1, _2, _3));
//    }
    m_curl.startDownloadThread();
    //m_timer.start(500);
    //new test end
    //for each (ProgramFileInfo info in patchs) {
    //    HttpRequest request;
    //    const QString &qsFullPath = qsDownloadDir + info._name;
    //    request.set_url(info._url.toStdString());
    //    HttpClient::instance()->asyncDownloadRequest(request, qsFullPath.toStdString(),
    //        curlpp::options::ProgressFunction(std::bind(&Updater::progressCallback, this, _1, _2, _3, _4)));
    //}
}

/**************************************************************************
* 函数名:   startOneTask
* 功能:
* 参数:
*....@[out] void
* 返回值:
* 时间:     2017/11/09 11:08
* 作者:   Leo
*/
void Updater::startOneTask()
{
    ProgramFileInfo info = m_patchs.front();
    m_patchs.pop_front();
    HttpRequest request;
    const QString qsDownloadDir = CConfigs::getDownloadPath() + "/";
    const QString &qsFullPath = qsDownloadDir + info._name;
    request.set_url(info._url.toStdString());
    HttpClient::instance()->asyncDownloadRequest(request, qsFullPath.toStdString(),
            curlpp::options::ProgressFunction(std::bind(&Updater::progressCallback, this, _1, _2, _3, _4)));
}

/**************************************************************************
* 函数名:   downloadCallBack
* 功能:
* 参数:
*....@[in]  const std::string
*....@[in]  qint64
*....@[in]  qint64
*....@[out] void
* 返回值:
* 时间:     2017/11/10 16:21
* 作者:   Leo
*/
void Updater::downloadCallBack(const std::string name, qint64 done, qint64 total)
{
    qDebug()<<"name :" << QString::fromStdString(name) << "done :" << done << "total :" << total;
    emit sigProgressUpdate(QString::fromStdString(name), done, total);
}

/**************************************************************************
* 函数名:   progressCallback
* 功能:
* 参数:
*....@[in]  double dltotal
*....@[in]  double dlnow
*....@[in]  double ultotal
*....@[in]  double ulnow
*....@[out] int
* 返回值:
* 时间:     2017/11/08 15:21
* 作者:   Leo
*/
int Updater::progressCallback(double dltotal, double dlnow, double ultotal, double ulnow)
{
    qDebug() << "progressCallback :" << QThread::currentThreadId();
    if (dltotal > -0.1 && dltotal < 0.1) {
        return 0;
    }
    // int nPos = (int)((dlnow / dltotal) * 100);
    //通知进度条更新下载进度  
    std::cout << "dltotal: " << (long)dltotal << " ---- dlnow:" << (long)dlnow << std::endl;
    //写入日志
    //emit sigProgressUpdate((qint64)dlnow, (qint64)dltotal);
    if (dltotal - dlnow < 1e-6 && dltotal > 1e-6 && m_patchs.size() > 0) {
        startOneTask();
    }
    return 0;
}

/**************************************************************************
* 函数名:   setParentProcessPathAndId
* 功能:
* 参数:
*....@[in]  const QString & path
*....@[in]  const int & id
*....@[out] void
* 返回值:
* 时间:     2017/11/08 15:44
* 作者:   Leo
*/
void Updater::setParentProcessPathAndId(const QString& path, const int &id)
{
    if (!path.isEmpty()) {
        m_parentProcessPath = path;
    }

    if (id > 0) {
        m_parentId = id;
    }
}

/**************************************************************************
* 函数名:   merge
* 功能:
* 参数:
*....@[out] void
* 返回值:
* 时间:     2017/11/07 19:15
* 作者:   Leo
*/
bool Updater::merge()
{
    backup();
    QString qsDownloadDir = CConfigs::getDownloadPath();
    for (auto it = m_patchs.begin(); it != m_patchs.end(); ++it) {
        const ProgramFileInfo & fileInfo = *it;
        emit sigUpdateStatusMsg(tr("updating: ") + fileInfo._name + tr("!"));
        if (!FileUtils::copyFile(qsDownloadDir + "/" + fileInfo._name, fileInfo._path + "/" + fileInfo._name))
        {
            revert();
            return false;
        }
    }

//    for each(ProgramFileInfo fileInfo in m_patchs) {
//        emit sigUpdateStatusMsg(tr("updating: ") + fileInfo._name + tr("!"));
//        if (!FileUtils::copyFile(qsDownloadDir + "/" + fileInfo._name, fileInfo._path + "/" + fileInfo._name))
//        {
//            revert();
//            return false;
//        }
//    }
    return true;
}

/**************************************************************************
* 函数名:   backup
* 功能:
* 参数:
*....@[out] void
* 返回值:
* 时间:     2017/11/08 10:54
* 作者:   Leo
*/
bool Updater::backup()
{
    sigUpdateStatusMsg(tr("backup updagrading files..."));
    QString qsTmpDir = CConfigs::getLocalTempDir();
    for (auto it = m_patchs.begin(); it != m_patchs.end(); ++it) {
        const ProgramFileInfo & fileInfo = *it;
        if (!FileUtils::copyFile(fileInfo._path + "/" + fileInfo._name, qsTmpDir)) {
            //备份失败，不进行升级
            return false;
        }
    }
//    for each(ProgramFileInfo fileInfo in m_patchs) {
//        if (!FileUtils::copyFile(fileInfo._path + "/" + fileInfo._name, qsTmpDir)) {
//            //备份失败，不进行升级
//            return false;
//        }
//    }
    return true;
}

/**************************************************************************
* 函数名:   revert
* 功能:
* 参数:
*....@[out] void
* 返回值:
* 时间:     2017/11/08 13:58
* 作者:   Leo
*/
bool Updater::revert()
{
    sigUpdateStatusMsg(tr("upgrade failed, revert updagrading files..."));
    QString qsTmpDir = CConfigs::getLocalTempDir();
    for (auto it = m_patchs.begin(); it != m_patchs.end(); ++it) {
        const ProgramFileInfo & fileInfo = *it;
        if (FileUtils::copyFile(qsTmpDir + "/" + fileInfo._name, fileInfo._path)) {
            //还原出错，写入日志
            return false;
        }
    }
//    for each (ProgramFileInfo fileInfo in m_patchs) {
//        if (FileUtils::copyFile(qsTmpDir + "/" + fileInfo._name, fileInfo._path)) {
//            // 还原出错，写入日志
//            return false;
//        }
//    }
    return true;
}

/**************************************************************************
* 函数名:   slotProgressUpdate
* 功能:
* 参数:
*....@[in]  qint64
*....@[in]  qint64
*....@[out] void
* 返回值:
* 时间:     2017/11/08 15:22
* 作者:   Leo
*/
//void Updater::slotProgressUpdate(qint64 done, qint64 total)
//{
//    qDebug() << "slotProgressUpdate :" << QThread::currentThreadId();
//    //for (int i = 1; i < 100; i++) {
//    //    QThread::msleep(10);
//    //    ui.progressBar->setValue(i);
//    //}
//    if (done == total && done != 0 && m_patchs.size() == 0) {
//        // 开始合并
//
//    }
//    else if (done * total >= 0) {
//        ui.progressBar->setValue((int)(done / total * 100));
//    }
//}

/**************************************************************************
* 函数名:   slotProgressUpdate
* 功能:
* 参数:
*....@[in]  QString name
*....@[in]  qint64 done
*....@[in]  qint64 sum
*....@[out] void
* 返回值:
* 时间:     2017/11/13 16:09
* 作者:   Leo
*/
void Updater::slotProgressUpdate(QString name, qint64 done, qint64 sum)
{
    QString msg = tr("downloading: ");
    ui.downloadLabel->setText(msg + name);
    double dProgress = (double)done / (double)sum;
    int nProgress = qCeil(dProgress * 100.0f);
    ui.progressBar->setValue(nProgress > 100 ? 100 : nProgress);
    if (done == sum && done != 0 && !m_dones.contains(name)) {
        //note : 下载完成时会多次回调
        m_dones.insert(name);
        if (m_dones.size() == m_patchs.size()) {
            //下载完成，开始合并
            if (merge()) {
                SystemUtils::startProcess(m_parentProcessPath);
                qApp->exit(0);
            }
            else {
                emit sigUpdateStatusMsg(tr("Upgrading failed!"));
            }
        }
    }
}

/**************************************************************************
* 函数名:   slotUpdateStateMsg
* 功能:
* 参数:
*....@[in]  QString msg
*....@[out] void
* 返回值:
* 时间:     2017/11/13 16:34
* 作者:   Leo
*/
void Updater::slotUpdateStatusMsg(QString msg)
{
    ui.downloadLabel->setText(msg);
}
