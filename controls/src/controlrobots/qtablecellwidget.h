#ifndef QTABLECELLWIDGET_H
#define QTABLECELLWIDGET_H

#include <QWidget>
#include <QUrl>
#include <QDir>
#include "pubdef.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QNetworkReply;
class QNetworkAccessManager;
class UsbComm;
class CFileInfo;
QT_END_NAMESPACE

class QTableCellWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QTableCellWidget(int nRow, QWidget *parent = 0);
    ~QTableCellWidget();

signals:
    void    removeRow (QList<int> row);                     /**< 移除行 */
    void    sync ();                                        /**< 添加同步信号 */
    void    refreshMusicInfo (int nMusicType,
                              const QString& musicName,
                              tagMusicInfo* pMusic);        /**< 更新歌单是刷新界面 */

public:
    void    init ();
    void    initVar ();
    void    initWindows ();
    bool    isDownload ();                                  /**< 歌单是否下载 */
    void    setDownloadState (bool bDownload);              /**< 下载歌曲后，修改按钮图片 */
    void    setSyncWaiteState ();                           /**< 设置同步按钮等待时显示样式 */
    void    restoreSyncState ();                            /**< 恢复同步按钮初始样式 */
    void    cancelSyncFile();                               /**< 取消正在同步文件 */
    void    setDownloadBtnVisible(bool bVisible);           /**< 设置下载按钮是否可见 */
    void    removeDir (const QString& strPath);             /**< 删除文件夹 */
    void    startDeleteFolder ();                           /**< 开始删除 */
    bool    requestUrl ();                                  /**< 请求下载URL*/
    void    setSelectRowTextStyle (bool bSelect);           /**< 设置下载或同步时，选择行的进度文字样式 */

public slots:
    void    slotDownload ();                                /**< 下载 */
    void    slotSync ();                                    /**< 同步 */
    void    slotOpenFolder ();                              /**< 打开文件夹 */
    void    slotDelete ();                                  /**< 删除 */
    void    startDownload (const QUrl& url);                /**< 开始下载 */
    void    updateProgress (qint64 received, qint64 total); /**< 更新下载进度 */
    void    saveFile (qint64 received, qint64 total);       /**< 下载后保存文件 */
    void    syncProgress(qint64 received, qint64 total);    /**< 同步进度 */

public:
    void    setMusicTime (int nTime);
    int     getMusicTime ();

    void    setUrl (const QString& url);
    QString getUrl () const;

    void    setFileName (const QString& fileName);
    QString getFileName () const;

    void    setRow (int nRow);
    int     getRow ();

protected:
    QString                 m_strUrl;                   /**< 下载URL */
    QString                 m_strFileName;              /**< 下载文件名称 */
    QNetworkReply*          m_reply;                    /**< 下载回复 */
    QNetworkAccessManager*  m_manager;
    UsbComm*                m_pUsbComm;                 /**< 同步传输文件接口 */
    QDir                    m_downloadDir;              /**< 下载目录 */
    int                     m_nRow;

private:
    QLabel*                 m_pLabel;                   /**< 播放时间 */
    QPushButton*            m_pDownloadBtn;             /**< 下载 */
    QPushButton*            m_pSyncBtn;                 /**< 同步 */
    QPushButton*            m_pOpenFolderBtn;           /**< 打开文件夹 */
    QPushButton*            m_pDeleteBtn;               /**< 删除 */
};

#endif // QTABLECELLWIDGET_H
