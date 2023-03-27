#ifndef DYNAMICPLAYDLG_H
#define DYNAMICPLAYDLG_H

#include "pubdef.h"
#include "ccustomdialog.h"

QT_BEGIN_NAMESPACE
class CTitleWidget;
class PlayerWindow;
class QProcess;
class UsbComm;
QT_END_NAMESPACE

namespace Ui {
class CDynamicPlayDlg;
}

class CDynamicPlayDlg : public XDialog
{
    Q_OBJECT

public:
    explicit CDynamicPlayDlg(UsbComm* pUsbComm, QWidget *parent = 0);
    ~CDynamicPlayDlg();

public:
    void init ();
    void initVar ();
    void initPlayListTableWidget ();
    void createTitleWidget ();
    void initWindows ();
    void initAction ();
    void layout();

protected:
    void refreshPlayListTableWidget ();
    void addRow (int nType, const QString& strFileName, tagMusicInfo* p);
    void clearDynamicMusicMap ();
    void enableButton (bool bEnable);
    void play (const QString& strMp4Path);

public slots:
    void slotStart();
    void slotStop ();
    void slotTableCustomContextMenuRequested(QPoint pos);
    void slotAdd();
    void slotEdit();
    void slotDelete ();

protected:
    CTitleWidget*         m_pTitleWidget;
    QTypeMusicInfoMap     m_mapDynamicMusic;
    QString               m_strConfigPath;
    UsbComm*              m_pUsbComm;             /**< 数据发送处理 */
    PlayerWindow*         m_player;               /**< 内嵌视屏播放器 */
    QProcess*             m_pPlayProcess;         /**< 调用第三方播放器进程 */

private:
    Ui::CDynamicPlayDlg *ui;
};

#endif // DYNAMICPLAYDLG_H
