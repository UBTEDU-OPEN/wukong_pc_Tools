#include "ucontroldialog.h"
#include "ui_ucontroldialog.h"
#include <QMenu>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QTimer>
#include <QDateTime>
#include <QProcess>
#include <QDir>
#include <QPropertyAnimation>

#include "sharelog.h"
#include "cpubfun.h"
#include "mystyle.h"
#include "usbcomm.h"
#include "protocoldefine.h"
#include "assignactdev.h"
#include "editsongdlg.h"
#include "udpsendfiledialog.h"
#include "dynamicplaydlg.h"
#include "cfgini.h"
#include "playerwindow.h"
#include "qmessagedlg.h"
#include "qsettingdlg.h"

#ifdef Q_OS_WIN
#include <dbt.h>
#include <WinUser.h>
#endif

#define ApplicationPath QCoreApplication::applicationDirPath()

UControlDialog::UControlDialog(QWidget *parent) :
    XDialog(parent),
    ui(new Ui::UControlDialog)
{
    ui->setupUi(this);
    init ();
}

UControlDialog::~UControlDialog()
{
    qDeleteAll (m_mapPlayList);
    m_mapPlayList.clear();
    m_pPlayTimer->deleteLater();
    if( m_pPlayProcess ){
        m_pPlayProcess->deleteLater();
    }
    if ( m_player ){
        m_player->deleteLater();
    }
    delete m_pUsbComm;
    m_pUsbComm = NULL;

    delete ui;
}

void UControlDialog::init ()
{
    initVar ();
    initAction ();
    initWindow ();
    setStyle(QString(":/qss/qss/mainwindow.qss"));
    setConnectWinStyle (false);
    registerUsbEvent ();
    autoCheckForUpdates ();

    ui->dynamicToolBtn->setVisible(false);
    /**< 程序初始化后等待1s连接usb */
    QTimer::singleShot(1000, this, SLOT(slotConnect()));
}

void UControlDialog::initVar ()
{
    m_mapPlayList.clear();
    m_nPlayLoop = EN_PLAY_SINGLE_TYPE;
    m_pPlayTimer = new QTimer(this);
    m_pPlayTimer->setSingleShot(true);
    m_orginRect = this->geometry();
    m_pUsbComm = new UsbComm;
    m_pPlayProcess = NULL;
    m_player = NULL;
    m_dontShowUpdateWindow = false;

    QVariant var;
    CCfgIni::getInstance().readItem(KEY_LANGUAGESETTING_CURRENTLANG, var);
    m_nLanguageType = var.isNull() ? EN_LANGUAGE_ZH_CN : var.toInt();
    changeLanguage (m_nLanguageType);
}

void UControlDialog::initWindow ()
{
    initPlayTable ();
    initPlayList();
    setMoveWindowEnable (true);
}

void UControlDialog::initPlayTable ()
{
    setTableStyle (ui->playTableWidget, 40, 50);
    clearContentsTable (ui->playTableWidget);

    QStringList header;
    header << tr("舞蹈列表");
    int nCol = header.count();
    ui->playTableWidget->setColumnCount(nCol);

    // 设置列宽
    ui->playTableWidget->setHorizontalHeaderLabels(header);

    // 表头文字居中
    for ( int i = 0; i < nCol; i ++ ){
        ui->playTableWidget->horizontalHeaderItem(i)->setTextAlignment(Qt::AlignLeft);
    }
}

void UControlDialog::initPlayList ()
{
    clearContentsTable (ui->playTableWidget);
    auto strFilePath = ApplicationPath + "/Config/playlist.txt";
    QVariant var;
    CCfgIni::getInstance().readItem(KEY_LANGUAGESETTING_CURRENTLANG, var);
    m_nLanguageType = var.isNull() ? EN_LANGUAGE_ZH_CN : var.toInt();
    if (m_nLanguageType == EN_LANGUAGE_EN_US) {
        strFilePath = ApplicationPath + "/Config/playlist_en.txt";
    }
    if ( !CPubFun::loadPlayList(strFilePath, m_mapPlayList) ){
        LOG(ERROR) << "load playlist.txt failed.";
        return ;
    }

    QTypeMusicInfoMap::iterator it = m_mapPlayList.begin();
    for ( ; it != m_mapPlayList.end(); it ++ ) {
        int nMusicType = it.key();
        QMusicInfoMap* pMusicInfoMap = it.value();
        QMusicInfoMap::iterator itSub = pMusicInfoMap->begin();
        for ( ; itSub != pMusicInfoMap->end(); itSub ++ ) {
            QString strFileName = itSub.key();
            tagMusicInfo* pTagMusicInfo = itSub.value();
            QString strDisplayName = pTagMusicInfo->m_strDisplayName;

            // 若没有配置显示名称，则用文件名称
            if ( strDisplayName.isEmpty() ){
                strDisplayName = strFileName;
            }

            int nRow = ui->playTableWidget->rowCount();
            ui->playTableWidget->insertRow(nRow);
            QTableWidgetItem* pItem = new QTableWidgetItem(strDisplayName);
            pItem->setData(Qt::UserRole, strFileName);
            pItem->setData(Qt::UserRole + 1, nMusicType);
            ui->playTableWidget->setItem(nRow, 0, pItem);
            QString strMp4Name = strFileName + ".mp4";
            QString strMp4Path = ApplicationPath + "/Config/" + strMp4Name;
            if ( QFile::exists(strMp4Path) ){
                pItem->setData(Qt::UserRole + 2, strMp4Path);
            }
        }
    }

    if ( ui->playTableWidget->rowCount() > 0 ) {
        ui->playTableWidget->selectRow(0);
    }
}

void UControlDialog::initAction ()
{
    connect(ui->minBtn, SIGNAL(pressed()), this, SLOT(slotMinWin()));
    connect(ui->maxBtn, SIGNAL(pressed()), this, SLOT(slotMaxWin()));
    connect(ui->closeBtn, SIGNAL(pressed()), this, SLOT(slotClose()));
    connect(ui->menuBtn, SIGNAL(pressed()), this, SLOT(slotMenu()));
    connect(ui->connectBtn, SIGNAL(pressed()), this, SLOT(slotConnect()));

    connect(ui->activeToolBtn, SIGNAL(pressed()), this, SLOT(slotActive ()));
    connect(ui->standToolBtn, SIGNAL(pressed()), this, SLOT(slotStand()));
    connect(ui->spuatToolBtn, SIGNAL(pressed()), this, SLOT(slotSpuat()));
    connect(ui->powerToolBtn, SIGNAL(pressed()), this, SLOT(slotPower()));
    connect(ui->dynamicToolBtn, SIGNAL(pressed()), this, SLOT(slotDynamic()));
    connect(ui->quitToolBtn, SIGNAL(pressed()), this, SLOT(slotQuit()));
    connect(ui->turnoffToolBtn, SIGNAL(pressed()), this, SLOT(slotTurnOff()));

    connect(ui->playBtn, SIGNAL(pressed()), this, SLOT(slotPlay()));
    connect(ui->stopBtn, SIGNAL(pressed()), this, SLOT(slotStop()));
    connect(ui->loopPlayBtn, SIGNAL(pressed()), this, SLOT(slotLoopPlay()));

    connect(m_pPlayTimer, SIGNAL(timeout()), this, SLOT(slotPlay()));
    connect(this, SIGNAL(signal_show_max_min_state(bool)), this, SLOT(slotChangeStyle(bool)));

    connect(&m_updater, &UpdaterWindow::dontShowUpdateWindowChanged, [=](bool state){m_dontShowUpdateWindow = state;});
}

void UControlDialog::setConnectWinStyle (bool bConnect/* = true*/)
{
    if ( bConnect ) {
        ui->playTableWidget->setVisible(true);
        ui->unconnectWidget->setVisible(false);

        ui->playBtn->setVisible(true);
        ui->stopBtn->setVisible(true);
        ui->loopPlayBtn->setVisible(true);

        ui->connectBtn->setText(tr(" 已连接"));
        ui->connectBtn->setIcon(QIcon(":/png/png/ic_connect.png"));
        ui->connectBtn->setStyleSheet(QString("QPushButton#connectBtn {"
                                              "color: #02BBF1;"
                                              "}"));

        ui->titleWidget->setStyleSheet(QString("QWidget#titleWidget {"
                                               "background: transparent;"
                                               "border-image: url(:/png/png/bg_title_connect.png);"
                                               "}"));
        setBtnEnable (true);
    } else {
        ui->playTableWidget->setVisible(false);
        ui->unconnectWidget->setVisible(true);

        ui->playBtn->setVisible(false);
        ui->stopBtn->setVisible(false);
        ui->loopPlayBtn->setVisible(false);

        ui->connectBtn->setText(tr(" 未连接"));
        ui->connectBtn->setIcon(QIcon(":/png/png/ic_unconnect.png"));
        ui->connectBtn->setStyleSheet(QString("QPushButton#connectBtn {"
                                              "color: #7685B6;"
                                              "}"));

        ui->titleWidget->setStyleSheet(QString("QWidget#titleWidget {"
                                               "background: transparent;"
                                               "border-image: url(:/png/png/bg_title_unconnect.png);"
                                               "}"));
        setBtnEnable (false);
    }
}

void UControlDialog::setBtnEnable (bool bEnable/* = true*/)
{
    ui->activeToolBtn->setEnabled(bEnable);
    ui->standToolBtn->setEnabled(bEnable);
    ui->spuatToolBtn->setEnabled(bEnable);
    ui->powerToolBtn->setEnabled(bEnable);
    ui->dynamicToolBtn->setEnabled(bEnable);
    ui->quitToolBtn->setEnabled(bEnable);
    ui->turnoffToolBtn->setEnabled(bEnable);

    ui->playBtn->setEnabled(bEnable);
    ui->stopBtn->setEnabled(bEnable);
    ui->loopPlayBtn->setEnabled(bEnable);
}

void UControlDialog::slotChangeStyle(bool bIsMaxWindow)
{
    if (bIsMaxWindow){
        ui->maxBtn->setStyleSheet (QString::fromUtf8 ("QPushButton{"
                                                      "background:transparent;"
                                                      "border-image: url(:/png/png/BtnRevert_Normal.png);"
                                                      "}"
                                                      "QPushButton:hover{"
                                                      "background:transparent;"
                                                      "border-image: url(:/png/png/BtnRevert_Hover.png);"
                                                      "}"
                                                      "QPushButton:pressed{"
                                                      "background:transparent;"
                                                      "border-image: url(:/png/png/BtnRevert_Pressed.png);"
                                                      "}"));
        this->showMaxWin();
    }else{
        ui->maxBtn->setStyleSheet (QString::fromUtf8 ("QPushButton{"
                                                      "background:transparent;"
                                                      "border-image: url(:/png/png/btn_Maximize_Normal.png);"
                                                      "}"
                                                      "QPushButton:hover{"
                                                      "background:transparent;"
                                                      "border-image: url(:/png/png/btn_Maximize_Hover.png);"
                                                      "}"
                                                      "QPushButton:pressed{"
                                                      "background:transparent;"
                                                      "border-image: url(:/png/png/btn_Maximize_Pressed.png);"
                                                      "}"));
        this->showNormalWin();
    }
}

void UControlDialog::showMaxWin ()
{
    QRect deskRect = QApplication::desktop()->availableGeometry();
    QRect rect (deskRect.x() - SHADOW_WIDTH,
                deskRect.y() - SHADOW_WIDTH,
                deskRect.width() + SHADOW_WIDTH * 2,
                deskRect.height() + SHADOW_WIDTH * 2);
    this->setGeometry(rect);

    // 同比放大缩小
    double dScaleW = (rect.width() * 1.0) / m_orginRect.width();
    double dScaleH = (rect.height() * 1.0) / m_orginRect.height();
    int nSpacerW = dScaleW * ui->spacerLabel->width();
    ui->spacerLabel->setFixedWidth(nSpacerW);
    int nTitleHeight = ui->titleWidget->height() * dScaleH;
    ui->titleWidget->setFixedHeight(nTitleHeight);
}

void UControlDialog::showNormalWin ()
{
    this->setGeometry(m_orginRect);
    QDesktopWidget *pDesktop = QApplication::desktop();
    move(pDesktop->width() /2 -width()/2, pDesktop->height() /2 -height()/2);

    ui->spacerLabel->setFixedWidth(144);
    ui->titleWidget->setFixedHeight(100);
}

void UControlDialog::slotMinWin ()
{
    showMinimized();
}

void UControlDialog::slotMaxWin ()
{
    m_isMaxWindow = !m_isMaxWindow;
    slotChangeStyle(m_isMaxWindow);
}

void UControlDialog::slotClose ()
{
    close();
}

void UControlDialog::slotMenu ()
{
    QMenu menu;
    QVariant var;
    CCfgIni::getInstance().readItem(KEY_LANGUAGESETTING_CURRENTLANG, var);
    m_nLanguageType = var.isNull() ? EN_LANGUAGE_ZH_CN : var.toInt();
    if (m_nLanguageType == EN_LANGUAGE_ZH_CN) {
        menu.setStyleSheet(QString("QMenu { "
                                   "border-image: url(:/png/png/bg_menu_dropdown.png);"
                                   "font-size: 18px;"
                                   "color: #FFFFFF;"
                                   "padding:15 0 1 20;"
                                   "}"
                                   "QMenu::item { "
                                   "height:18px;"
                                   "width:108px;"
                                   "padding:7 10 7 36;"
                                   "}"
                                   "QMenu::item:selected {"
                                   "background:transparent;"
                                   "border-image: url(:/png/png/img_menu_press/hover.png);"
                                   "}"));
    } else {  // EN_LANGUAGE_EN_US
        menu.setStyleSheet(QString("QMenu { "
                                   "border-image: url(:/png/png/bg_menu_dropdown.png);"
                                   "font-size: 18px;"
                                   "color: #FFFFFF;"
                                   "padding:15 0 1 20;"
                                   "}"
                                   "QMenu::item { "
                                   "height:18px;"
                                   "width:250px;"
                                   "padding:7 10 7 36;"
                                   "}"
                                   "QMenu::item:selected {"
                                   "background:transparent;"
                                   "border-image: url(:/png/png/img_menu_press/hover.png);"
                                   "}"));
    }


    menu.addAction(QIcon(":/png/png/ic_menu_jihuo.png"),
                   tr("激活设备"),
                   this,
                   SLOT(slotActiveDev()));
    menu.addAction(QIcon(":/png/png/ic_menu_musiclist.png"),
                   tr("歌单管理"),
                   this,
                   SLOT(slotSongManage()));
    menu.addAction(QIcon(":/png/png/ic_menu_file.png"),
                   tr("传输文件"),
                   this,
                   SLOT(slotSendFile()));

    CCfgIni::getInstance().readItem(KEY_DYNAMICSETTING_SHOWDYNAMICFUN, var);
    bool bShow = (bool)var.toInt();
    if ( bShow ) {
        menu.addAction(QIcon(":/png/png/ic_drampaly.png"),
                       tr("动态播放"),
                       this,
                       SLOT(slotDynamic()));
    }

    /* 由于集控发射器无法出口，暂不考虑语言切换 */
    menu.addAction(QIcon(""),
                   tr("区域设置"),
                   this,
                   SLOT(slotSetting()));

    menu.addAction(QIcon(""),
                   tr("检查更新"),
                   this,
                   SLOT(slotUpdate()));

    menu.addAction(QIcon(":/png/png/ic_settings.png"),
                   tr("关于"),
                   this,
                   SLOT(slotAbout()));

    menu.setStyle(new myStyle);

    //设置主菜单出现的位置;
    QPoint p = ui->menuBtn->frameGeometry().bottomRight();
    p.setX(p.x() + 88);
    p.setY(p.y() + 2);
    menu.exec(mapToParent(p));
}

void UControlDialog::slotConnect()
{
    bool bConnected = false;
    if ( m_pUsbComm->isOpen() ){
        m_pUsbComm->closeHidDev();
    }else{
        if ( m_pUsbComm->openHidDev() ) {
            bConnected = true;
        }else{
            QMessageBox::warning(NULL, tr("集控软件"), tr("连接失败,请确认是否已连接集控发射器"));
        }
    }
    setConnectWinStyle(bConnected);
}

void UControlDialog::slotActive ()
{
    setBtnEnable(false);
    QStringList strSerialList;
    loadSerialCfg(strSerialList);
    m_pUsbComm->sendOpenFileServiceCmd(strSerialList, 1);
    m_pUsbComm->sendActiveCmd(1);
    setBtnEnable(true);

    QMessageBox::information(this, tr("集控软件"), tr("激活集控命令发送完成"));
}

void UControlDialog::slotStand ()
{
    setBtnEnable(false);
    m_pUsbComm->sendCmd (UBXPROTOCOL_CMDID_WAKEUP);
    setBtnEnable(true);

    QMessageBox::information(this, tr("集控软件"), tr("站立待命命令发送完成"));
}

void UControlDialog::slotSpuat ()
{
    QVariant var;
    CCfgIni::getInstance().readItem(KEY_SYSSETTING_CANSHOWSQUATTIPS, var);
    bool bShowWarninigInfo = var.isNull() ? true : var.toBool();
    if ( bShowWarninigInfo ){
        QMessageDlg dlg;
        dlg.setShowTips(tr("确定蹲下吗?请确保机器人没有插电源线，否则容易摔倒"));
        if ( dlg.exec() == QDialog::Rejected ){
            return ;
        }
    }

    setBtnEnable(false);
    m_pUsbComm->sendCmd (UBXPROTOCOL_CMDID_REST);
    setBtnEnable(true);

    QMessageBox::information(this, tr("集控软件"), tr("蹲下待命命令发送完成"));
}

void UControlDialog::slotPower ()
{
    setBtnEnable(false);
    m_pUsbComm->sendCmd (UBXPROTOCOL_CMDID_SHOW_BATTERY);
    setBtnEnable(true);

    QMessageBox::information(this, tr("集控软件"), tr("电量显示命令发送完成"));
}

void UControlDialog::slotDynamic()
{
    if ( !m_pUsbComm->isOpen() ){
        QMessageBox::warning(this, tr("集控软件"), tr("请连接集控发射器"));
        return ;
    }
    CDynamicPlayDlg* dlg = new CDynamicPlayDlg(m_pUsbComm, this);
    dlg->setModal(true);
    dlg->exec();
    dlg->deleteLater();
}

void UControlDialog::slotSetting()
{
    QSettingDialog* dlg = new QSettingDialog(this);
    dlg->setLanguageType(m_nLanguageType);
    if ( dlg->exec() == QDialog::Accepted ){
        if ( dlg->getLanguageType() != m_nLanguageType ){
            m_nLanguageType = dlg->getLanguageType();
            CCfgIni::getInstance().writeItem(KEY_LANGUAGESETTING_CURRENTLANG, QVariant(m_nLanguageType));
            this->changeLanguage(m_nLanguageType);
        }
    }

    dlg->deleteLater();
}

void UControlDialog::slotUpdate()
{
    m_updater.checkForUpdates(true);
}

void UControlDialog::slotQuit ()
{
    setBtnEnable(false);
    QStringList strSerialList;
    loadSerialCfg(strSerialList);
    m_pUsbComm->sendActiveCmd(0);
    m_pUsbComm->sendOpenFileServiceCmd(strSerialList, 0);
    setBtnEnable(true);

    QMessageBox::information(this, tr("集控软件"), tr("退出集控命令发送完成"));
}

void UControlDialog::slotTurnOff()
{
    setBtnEnable(false);
    m_pUsbComm->sendCmd (UBXPROTOCOL_CMDID_POWEROFF);
    setBtnEnable(true);

    QMessageBox::information(this, tr("集控软件"), tr("关机命令发送完成"));
}

void UControlDialog::slotActiveDev ()
{
    CAssignActDev* dlg = new CAssignActDev(this);
    dlg->setModal(true);
    dlg->exec();
    dlg->deleteLater();
}

void UControlDialog::slotSongManage ()
{
    if ( !m_pUsbComm->isOpen() ){
        QMessageBox::warning(this, tr("集控软件"), tr("请连接集控发射器"));
        return ;
    }
    auto playListPath = ApplicationPath + "/Config/playlist.txt";
    QVariant var;
    CCfgIni::getInstance().readItem(KEY_LANGUAGESETTING_CURRENTLANG, var);
    m_nLanguageType = var.isNull() ? EN_LANGUAGE_ZH_CN : var.toInt();
    if (m_nLanguageType == EN_LANGUAGE_EN_US) {
        playListPath = ApplicationPath + "/Config/playlist_en.txt";
    }
    CEditSongDlg* dlg = new CEditSongDlg(playListPath, this);
    dlg->setModal(true);
    dlg->exec();
    dlg->deleteLater();
    // 刷新actionList
    initPlayList();
}

void UControlDialog::slotSendFile ()
{
    if ( !m_pUsbComm->isOpen() ){
        QMessageBox::warning(this, tr("集控软件"), tr("请连接集控发射器"));
        return ;
    }

    CUdpSendFileDialog* dlg = new CUdpSendFileDialog(this);
    dlg->setModal(true);
    dlg->exec();
    dlg->deleteLater();
}

void UControlDialog::slotAbout ()
{
    // about
    static const QDate buildDate = QLocale(QLocale::English).toDate(QString(__DATE__).replace("  ", " 0" ), "MMM dd yyyy");
    static const QTime buildTime = QTime::fromString(__TIME__, "hh:mm:ss" );
    QDateTime dt(buildDate, buildTime);
    QString sTitle = tr("集控软件");
    QString sAbout = tr(" <b>集控软件(%1)</b><br>"
                        " Bulid: %2 <br>"
                        " Copyright 2012-2019 UBTECH Robotics, Inc. All rights reserved <br>")
            .arg(qApp->applicationVersion())
            .arg(dt.toString("yyyy-MM-dd hh:mm:ss"));
    QMessageBox::about(NULL, sTitle, sAbout);
}

void UControlDialog::slotPlay ()
{
    if ( m_nPlayLoop == EN_PLAY_SINGLE_TYPE ) {
        startPlay ();
    } else if ( m_nPlayLoop == EN_PLAY_SINGLE_LOOP_TYPE ) {
        //获取当前的播放时长， 若播放时长未配置或配置为0，则跳过当前歌曲。
        int nMusicTime = 0;
        QTableWidgetItem *pItem = ui->playTableWidget->currentItem();
        if ( pItem ) {
            int nMusicType = pItem->data(Qt::UserRole + 1).toInt();
            QTypeMusicInfoMap::iterator it = m_mapPlayList.find(nMusicType);
            if ( it != m_mapPlayList.end() ){
                QMusicInfoMap* p = it.value();
                QString strData = pItem->data(Qt::UserRole).toString();
                QMusicInfoMap::iterator itSub = p->find(strData);
                if ( itSub != p->end() ){
                    tagMusicInfo* p = itSub.value();
                    nMusicTime = p->m_nMusicTime;
                }
            }
        }

        if ( nMusicTime > 0 ) {
            startPlay (); // 播放当前歌曲
            nMusicTime += 3; // 调完舞蹈后，机器人复位时间。
            m_pPlayTimer->start(nMusicTime * 1000);
        } else {
            QMessageBox::information(this,
                                     tr("集控软件"),
                                     tr("歌曲未配置循环"));
        }
    } else if ( m_nPlayLoop == EN_PLAY_LIST_LOOP_TYPE ) {
        bool  bFirstPlay = false;
        QObject* pSender = sender();
        if ( pSender == ui->playBtn ){
            bFirstPlay = true;
        }

        // 列表循环，第二播放时，选择歌曲。
        if ( !bFirstPlay ) {
            int nCurRow = ui->playTableWidget->currentRow();
            int nRowCount = ui->playTableWidget->rowCount();
            if ( nRowCount > 0 )
            {
                if ( nCurRow >= nRowCount - 1 ){
                    nCurRow = 0;
                } else {
                    nCurRow ++;
                }
            }

            ui->playTableWidget->selectRow(nCurRow);
        }

        //获取当前的播放时长， 若播放时长未配置或配置为0，则跳过当前歌曲。
        int nMusicTime = 0;
        QTableWidgetItem *pItem = ui->playTableWidget->currentItem();
        if ( pItem ) {
            int nMusicType = pItem->data(Qt::UserRole + 1).toInt();
            QTypeMusicInfoMap::iterator it = m_mapPlayList.find(nMusicType);
            if ( it != m_mapPlayList.end() ){
                QMusicInfoMap* p = it.value();
                QString strData = pItem->data(Qt::UserRole).toString();
                QMusicInfoMap::iterator itSub = p->find(strData);
                if ( itSub != p->end() ){
                    tagMusicInfo* p = itSub.value();
                    nMusicTime = p->m_nMusicTime;
                }
            }
        }

        // 若播放歌曲配置播放时长，则播放，否则跳过，播放下一首
        if ( nMusicTime > 0 ) {
            startPlay ();
        }

        nMusicTime += 3; // 调完舞蹈后，机器人复位时间。
        m_pPlayTimer->start(nMusicTime * 1000); // 下一次播放
    }
}

void UControlDialog::slotStop ()
{
    if ( m_nPlayLoop == EN_PLAY_SINGLE_LOOP_TYPE || m_nPlayLoop == EN_PLAY_LIST_LOOP_TYPE ){
        if ( m_pPlayTimer && m_pPlayTimer->isActive() ){
            m_pPlayTimer->stop();
        }
    }
    setBtnEnable(false);
    m_pUsbComm->sendCmd (UBXPROTOCOL_CMDID_STOP);
    setBtnEnable(true);
}

void UControlDialog::slotLoopPlay ()
{
    m_nPlayLoop = (m_nPlayLoop + 1) % EN_PLAY_LOOP_TYPE_COUNT;
    switch(m_nPlayLoop) {
    case EN_PLAY_SINGLE_TYPE:
        ui->loopPlayBtn->setStyleSheet(QString("QPushButton#loopPlayBtn{"
                                       "background: transparent;"
                                       "border-image: url(:/png/png/ic_single_play.png);"
                                       "}"
                                       "QPushButton#loopPlayBtn:hover{"
                                       "background: transparent;"
                                       "border-image: url(:/png/png/ic_single_play_hover.png);"
                                       "}"
                                       "QPushButton#loopPlayBtn:pressed{"
                                       "background: transparent;"
                                       "border-image: url(:/png/png/ic_single_play_hover.png);"
                                       "}"));
        startAnimateTooltip(tr("进入单曲播放模式"));
        break;
    case EN_PLAY_SINGLE_LOOP_TYPE:
        ui->loopPlayBtn->setStyleSheet(QString("QPushButton#loopPlayBtn{"
                                       "background: transparent;"
                                       "border-image: url(:/png/png/ic_single_loop_play.png);"
                                       "}"
                                       "QPushButton#loopPlayBtn:hover{"
                                       "background: transparent;"
                                       "border-image: url(:/png/png/ic_single_loop_play_hover.png);"
                                       "}"
                                       "QPushButton#loopPlayBtn:pressed{"
                                       "background: transparent;"
                                       "border-image: url(:/png/png/ic_single_loop_play_hover.png);"
                                       "}"));
        startAnimateTooltip(tr("进入单曲循环模式"));
        break;
    case EN_PLAY_LIST_LOOP_TYPE:
        ui->loopPlayBtn->setStyleSheet(QString("QPushButton#loopPlayBtn{"
                                       "background: transparent;"
                                       "border-image: url(:/png/png/ic_list_loop_play.png);"
                                       "}"
                                       "QPushButton#loopPlayBtn:hover{"
                                       "background: transparent;"
                                       "border-image: url(:/png/png/ic_list_loop_play_hover.png);"
                                       "}"
                                       "QPushButton#loopPlayBtn:pressed{"
                                       "background: transparent;"
                                       "border-image: url(:/png/png/ic_list_loop_play_hover.png);"
                                       "}"));
        startAnimateTooltip(tr("进入列表循环模式"));
        break;
    default:
        break;
    }
}

void UControlDialog::startAnimateTooltip (const QString& strToolTip)
{
    QDialog* pWidget = new QDialog();
    pWidget->setStyleSheet("background: transparent;background-color:#F0FFFF;border-radius:20px;font-size:20px;");
    QLabel* pLabel = new QLabel(pWidget);
    pLabel->setStyleSheet("border-radius:20px;color:#FF1493;");
    pLabel->setText(strToolTip);
    QHBoxLayout* pHlayout = new QHBoxLayout(pWidget);
    pHlayout->addWidget(pLabel);
    pHlayout->setContentsMargins(QMargins(0,0,0,0));
    pWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    QPropertyAnimation *animation = new QPropertyAnimation(pWidget, "windowOpacity");
    animation->setDuration(2000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, SIGNAL(finished()), pWidget, SLOT(close()));

    pWidget->show();
    pWidget->move((this->width() - pWidget->width())/2 + geometry().left(),(this->height() - pWidget->height())/2 + geometry().top());
}

bool UControlDialog::loadSerialCfg (QStringList& list)
{
    QVariant var;
    if ( !CCfgIni::getInstance().readItem(KEY_SERIALSETTING_SERIAL, var) ){
        return false;
    }
    QString strSerialInfo = var.toString();
    list = strSerialInfo.split('#', QString::SkipEmptyParts);
    return true;
}

void UControlDialog::startPlay ()
{
    setBtnEnable(false);
    QTableWidgetItem *pItem = ui->playTableWidget->currentItem();
    if ( pItem ) {
        QString strFileName = pItem->data(Qt::UserRole).toString();
        if ( !strFileName.endsWith(".xml") ){
            strFileName += ".xml";
        }

        int nMusicType = pItem->data(Qt::UserRole + 1).toInt();
        int nRobotMusicPath = 0;
        if( nMusicType == EN_SYSTEM_MUSIC_TYPE ){
            nRobotMusicPath = 1;
        }

        // 播放视屏
        QString strMp4Path = pItem->data(Qt::UserRole + 2).toString();
        if ( !strMp4Path.isEmpty() ) {
            play (strMp4Path);
        }

        // 文件名称发送悟空机器人
        m_pUsbComm->sendPlayCmd (strFileName, nRobotMusicPath);
    }
    setBtnEnable(true);
}

void UControlDialog::play (const QString& strMp4Path)
{
    QVariant var;
    CCfgIni::getInstance().readItem(KEY_VEDIOSETTING_VEDIOTYPE, var);
    int nVideoType = var.isNull() ? 0 : var.toInt();  // nVideoType = 0 表示采用第三方播放器，= 1 表示采用内嵌播放器
    if ( nVideoType == 1 ) {
        if ( m_player == NULL ) {
            m_player = new PlayerWindow;
        }
        m_player->play(strMp4Path);
    } else {
        QStringList args;
        args.append(strMp4Path);
        args.append("/fullscreen");
        CCfgIni::getInstance().readItem(KEY_VEDIOSETTING_VEDIOPLAYERPATH, var);
        QString strPlayerPath = var.isNull() ? "" : var.toString();
        strPlayerPath = QDir::toNativeSeparators(strPlayerPath);
        strPlayerPath = "\"" + strPlayerPath + "\"";
        if ( !m_pPlayProcess ) {
            m_pPlayProcess = new QProcess;
            m_pPlayProcess->setProgram(strPlayerPath);
        } else {
            m_pPlayProcess->close();
        }

        m_pPlayProcess->setArguments(args);
        m_pPlayProcess->start();
        m_pPlayProcess->waitForStarted();
    }
}

bool UControlDialog::registerUsbEvent()
{
#ifdef Q_OS_WIN
    const GUID GUID_DEVINTERFACE_LIST[] =
    {
        // GUID_DEVINTERFACE_USB_DEVICE
        { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },

        // GUID_DEVINTERFACE_DISK
        { 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },

        // GUID_DEVINTERFACE_HID,
        { 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } },

        // GUID_NDIS_LAN_CLASS
        { 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } }
    };

    HDEVNOTIFY hDevNotify;
    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
    ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
    NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    for ( int i = 0; i < sizeof(GUID_DEVINTERFACE_LIST)/sizeof(GUID); i++ ) {
        NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];
        hDevNotify = RegisterDeviceNotification((HANDLE)this->winId(),
                                                &NotificationFilter,
                                                DEVICE_NOTIFY_WINDOW_HANDLE);
        if(!hDevNotify) {
            int nErr = GetLastError();
            LOG(ERROR) << trs(QString("regist failed, error: %1").arg(nErr));
            return false;
        }
    }
#endif

    return true;
}

/**
 * Checks for updates, if an update is found, then the updater dialog will show
 * up, otherwise, no notification shall be showed
 */
void UControlDialog::autoCheckForUpdates()
{
    m_updater.installEventFilter(this);
    m_updater.setShowWindowDisable(m_dontShowUpdateWindow);
    m_updater.checkForUpdates(false);
}

bool UControlDialog::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
#ifdef Q_OS_WIN
    Q_UNUSED(eventType);
    Q_UNUSED(result);
    MSG* msg = reinterpret_cast<MSG*>(message);
    int msgType = msg->message;
    if ( msgType == (int)WM_DEVICECHANGE ) {
        if (msg->wParam == (int)DBT_DEVICEARRIVAL || msg->wParam == (int)DBT_DEVICEREMOVECOMPLETE) {
            PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
            if ( lpdb->dbch_devicetype == (int)DBT_DEVTYP_DEVICEINTERFACE ) {
                //dbcc_name = \\?\USB#VID_0483&PID_5750#00000000001A#{a5dcbf10-6530-11d2-901f-00c04fb951ed}
                PDEV_BROADCAST_DEVICEINTERFACE pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
                QString dbcc_name = QString::fromStdWString(pDevInf->dbcc_name);
                QString strStandDbccName = QString("USB#VID_%1&PID_%2")
                        .arg(VENDORID, 4, 16, QLatin1Char('0'))
                        .arg(PRODUCTID, 4, 16, QLatin1Char('0'));
                /**< 仅检测集控发射器的USB插拔事件 */
                if ( dbcc_name.contains(strStandDbccName) ) {
                    /**<  DBT_DEVICEARRIVAL 插入设备
                     *    DBT_DEVICEREMOVECOMPLETE 移除设备
                     */
                    if ( msg->wParam == (int)DBT_DEVICEARRIVAL ){
                        /**< 等待usb设备启动完成后(1s)连接, 必须采用延迟方式 */
                        QTimer::singleShot(1000, Qt::PreciseTimer, this, SLOT(slotConnect()));
                    }else if ( msg->wParam == (int)DBT_DEVICEREMOVECOMPLETE ){
                        /**< 等待200ms断开usb设备 */
                        QTimer::singleShot(200, Qt::PreciseTimer, this, SLOT(slotConnect()));
                    }
                }
            }
        }
    }
#endif
    return false;
}

/**
* @brief
* Mostly take care on the event happened on widget whose filter installed to tht mainwindow
*/
bool UControlDialog::eventFilter (QObject *object, QEvent *event)
{
    switch (event->type()){
    case QEvent::Show:
        if(object == &m_updater){

            QRect rect = m_updater.geometry();
            QRect appRect = geometry();
            int titleBarHeight = 28 ;

            int x = appRect.x() + (appRect.width() - rect.width())/2.0;
            int y = appRect.y() + titleBarHeight  + (appRect.height() - rect.height())/2.0;

            m_updater.setGeometry(QRect(x, y, rect.width(), rect.height()));
        }
        break;
    default:
        break;
    }

    return QObject::eventFilter(object, event);
}

bool UControlDialog::changeLanguage (int nLanguageIndex)
{
    qApp->removeTranslator(&m_pTranslator);
    bool bRet = true;
    switch (nLanguageIndex) {
    case EN_LANGUAGE_EN_US:
        bRet = m_pTranslator.load(":/ts/ts/jk_en_US.qm");
        bRet = qApp->installTranslator(&m_pTranslator);
        break;
    case EN_LANGUAGE_ZH_CN:
//        bRet = m_pTranslator.load(":/ts/ts/jk_zh_CN.qm");
//        bRet = qApp->installTranslator(&m_pTranslator);
        break;
    default:
        break;
    }
    ui->retranslateUi(this);
    ui->playTableWidget->setHorizontalHeaderLabels(QStringList() << tr("舞蹈列表"));
    initPlayList();
    m_updater.onLanguageChanged();

    return bRet;
}

