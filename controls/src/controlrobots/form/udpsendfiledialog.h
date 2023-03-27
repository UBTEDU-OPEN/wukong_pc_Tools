#ifndef UDPSENDFILEDIALOG_H
#define UDPSENDFILEDIALOG_H

#include "ccustomdialog.h"
#include "cfileinfo.h"

namespace Ui {
class CUdpSendFileDialog;
}

QT_BEGIN_NAMESPACE
class CTitleWidget;
class QComboBox;
class QMenu;
class UsbComm;
QT_END_NAMESPACE

class CUdpSendFileDialog : public XDialog
{
    Q_OBJECT

public:
    explicit CUdpSendFileDialog(QWidget *parent = 0);
    ~CUdpSendFileDialog();

signals:
    void sigSendFileService (QStringList list, int nState);
    void sigSendFileInfo (CFileInfo* fileInfo);

public:
    void init ();
    void initVar ();
    void createTitleWidget ();
    void initWindows ();
    void initAction ();
    void layout();
    void initFileInfoTableWidget ();

protected:
    void addFileInfoRow (CFileInfo *pFileInfo);
    void updateFileInfoTableWidget (int nRow);

    /**< 刷新界面到内部数据结构 */
    void reloadFileInfoData ();
    void reloadFileInfoRowData (int iRow);

    void addRobotTypeCombox (QComboBox* pCombox);
    void setFileInfoSelAll (bool bEnable);
    void enableSend ();
    QString  translate (int nRobotFileType);

    void enabledFileInfoTableWidget (bool bEnable);
    void enabledFileInfoTableWidget(int nRow, bool bEnable);

protected:
    virtual void closeEvent(QCloseEvent *event);

public slots:
    void slotSendFile ();
    /**< 文件信息表格右键鼠标事件, 文件信息表格右键菜单相应槽函数， 依次为全选、全不选、添加、删除、清空、刷新、全部刷新 */
    void slotTableCustomContextMenuRequested(QPoint pos);
    void slotSelAll();
    void slotSelNone ();
    void slotAddFile();
    void slotAddDir();
    void slotDeleteFile();
    void slotDeleteAllFile();
    void slotReloadFile();
    void slotReloadAllFile();
    void slotMenuSendFile ();
    /**< 发送数据 */
    void slotSendBytes(QString strPath, int nSize);
    void slotSendOver(QString strPath, bool bState);

protected:
    enum EN_FILE_INFO_COL_NAME{
        EN_FILE_INFO_COL_INDEX      = 0,        /**< 序号 */
        EN_FILE_INFO_COL_NAME       = 1,        /**< 文件名称 */
        EN_FILE_INFO_COL_SIZE       = 2,        /**< 文件大小（KB） */
        EN_FILE_INFO_COL_MD5        = 3,        /**< MD5 */
        EN_FILE_INFO_COL_TYPE       = 4,        /**< 机器人路径类型 */
        EN_FILE_INFO_COL_PATH       = 5,        /**< 本地路径（不包含名称） */
        EN_FILE_INFO_COL_PROGRESS   = 6,        /**< 发送进度 */
        EN_FILE_INFO_COL_STATE      = 7         /**< 发送结果 */
    };

protected:
    CTitleWidget*     m_pTitleWidget;
    CFileInfos        m_sFileInfo;
    UsbComm*          m_pUsbComm;

protected:
    QStringList       m_listSendingFilePath;    /**< 待发送的文件路径 */
    QThread*          m_pThread;
    QMenu*            m_pMenu;

private:
    Ui::CUdpSendFileDialog *ui;
};

#endif // UDPSENDFILEDIALOG_H
