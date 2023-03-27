#ifndef UCONTROLDIALOG_H
#define UCONTROLDIALOG_H

#include <ccustomdialog.h>
#include "pubdef.h"
#include <QTranslator>
#include "updaterwindow.h"

namespace Ui {
class UControlDialog;
}

QT_BEGIN_NAMESPACE
class UsbComm;
class PlayerWindow;
class QProcess;
QT_END_NAMESPACE

class UControlDialog : public XDialog
{
    Q_OBJECT

public:
    explicit UControlDialog(QWidget *parent = 0);
    ~UControlDialog();

public:
    void    init ();
    void    initVar ();
    void    initWindow ();
    void    initPlayTable ();
    void    initPlayList ();
    void    initAction ();
    void    setConnectWinStyle (bool bConnect = true);              /**< 设置与发射器连接与否的界面样式 */
    void    setBtnEnable (bool bEnable = true);                     /**< 界面操作按钮使能 */
    void    showMaxWin ();                                          /**< 最大化显示 */
    void    showNormalWin ();                                       /**< 正常化显示(1360 * 768) */
    bool    registerUsbEvent();                                     /**< 注册USB事件 */
    void    autoCheckForUpdates();

protected:
    bool    nativeEvent(const QByteArray &eventType, void *message, long *result);
    bool    eventFilter(QObject* object, QEvent* event) Q_DECL_OVERRIDE;

public slots:
    void    slotChangeStyle(bool bIsMaxWindow);
    void    slotMinWin ();                                          /**< 最小化 */
    void    slotMaxWin ();                                          /**< 最大化 */
    void    slotClose ();                                           /**< 关闭*/
    void    slotMenu ();                                            /**< 菜单*/
    void    slotConnect();                                          /**< 连接*/

    void    slotActive ();                                          /**< 激活 */
    void    slotStand ();                                           /**< 站立 */
    void    slotSpuat ();                                           /**< 蹲下 */
    void    slotPower ();                                           /**< 电量 */
    void    slotQuit ();                                            /**< 退出集控 */
    void    slotTurnOff();                                          /**< 关闭机器人 */

    void    slotActiveDev ();                                       /**< 激活设备 */
    void    slotSongManage ();                                      /**< 歌单管理 */
    void    slotSendFile ();                                        /**< 传输文件 */
    void    slotDynamic();                                          /**< 动态播放 */
    void    slotSetting ();                                         /**< 设置 */
    void    slotUpdate();                                           /**< 检查更新 */
    void    slotAbout ();                                           /**< 关于 */

    void    slotPlay ();                                            /**< 播放 */
    void    slotStop ();                                            /**< 停止播放 */
    void    slotLoopPlay ();                                        /**< 循环播放 */

protected:
    bool    loadSerialCfg (QStringList& list);                      /**< 加载序列号配置 */
    void    startPlay ();                                           /**< 表演节目 */
    void    play (const QString& strMp4Path);                       /**< 播放视屏 */
    void    startAnimateTooltip (const QString& strToolTip);        /**< 动画显示文字 */
    bool    changeLanguage (int nLanguageIndex);                    /**< 切换语言 */

protected:
    QRect               m_orginRect;                                /**< 界面原始大小尺寸 */
    int                 m_nPlayLoop;                                /**< PlayLoop = 0 表示不循环播放单曲，默认不进行循环播放;
                                                                      *  PlayLoop = 1 表示循环播放单曲;
                                                                      *  PlayLoop = 2 表示循环播放所有歌曲 */
    QTimer*             m_pPlayTimer;
    QTypeMusicInfoMap   m_mapPlayList;
    UsbComm*            m_pUsbComm;
    PlayerWindow*       m_player;                                   /**< 内嵌视屏播放器 */
    QProcess*           m_pPlayProcess;                             /**< 调用第三方播放器进程 */
    int                 m_nLanguageType;
    QTranslator         m_pTranslator;
    UpdaterWindow       m_updater;
    bool                m_dontShowUpdateWindow;

private:
    Ui::UControlDialog *ui;
};

#endif // UCONTROLDIALOG_H
