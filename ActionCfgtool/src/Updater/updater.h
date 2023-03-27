#pragma once

#include <QtWidgets/QDialog>
#include "ui_updater.h"
#include "updaterinterface.h"
#include <list>
#include <QSet>
#include <QTimer>
#include "httpdownload.h"

using std::list;

using namespace Ubxdownload;
class UpdaterInterface;

class Updater : public QDialog
{
    Q_OBJECT

public:
    Updater(QWidget *parent = Q_NULLPTR);
    void start();
    void startDownloadTask(const list<ProgramFileInfo> &patchs);
    int progressCallback(double dltotal, double dlnow, double ultotal, double ulnow);
    void setParentProcessPathAndId(const QString& path, const int &id);
    void startOneTask();
    void downloadCallBack(const std::string, qint64, qint64);
signals:
    void sigProgressUpdate(QString name, qint64 done, qint64 sum);
    void sigUpdateStatusMsg(QString);
public slots:
    void slotTest();
    bool merge();
    bool backup();
    bool revert();
    void slotProgressUpdate(QString name, qint64 done, qint64 sum);
    void slotUpdateStatusMsg(QString msg);
    //void slotProgressUpdate(qint64 done, qint64 total);
private:
    Ui::Updater ui;
    UpdaterInterface *m_pUpdater;
    list<ProgramFileInfo>  m_patchs;
    QString                  m_parentProcessPath;
    int                      m_parentId;
    int                      m_taskCount;
    int                      m_doneTask;
    //new test
    QTimer                   m_timer;
    HttpDownload             m_curl;
    double                   m_curDownloadLen;
    double                   m_preLen;
    //int                      m_nComplete;
    QSet<QString>            m_dones;//已下载好的文件名
};
