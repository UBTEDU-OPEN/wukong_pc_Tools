#include "dynamicplaydlg.h"
#include "ui_dynamicplaydlg.h"

#include "titlewidget.h"
#include "usbcomm.h"
#include "cpubfun.h"
#include "addplaydlg.h"
#include "playerwindow.h"
#include "cfgini.h"

#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QMenu>

#define ApplicationPath  QCoreApplication::applicationDirPath()

CDynamicPlayDlg::CDynamicPlayDlg(UsbComm *pUsbComm, QWidget *parent) :
    m_pUsbComm (pUsbComm),
    XDialog(parent),
    ui(new Ui::CDynamicPlayDlg)
{
    ui->setupUi(this);
    init ();
}

CDynamicPlayDlg::~CDynamicPlayDlg()
{
    if ( m_player ){
        m_player->deleteLater();
    }
    clearDynamicMusicMap ();
    if ( m_pPlayProcess ) {
        if ( m_pPlayProcess->waitForFinished() ) {
            delete m_pPlayProcess;
            m_pPlayProcess = NULL;
        }
    }

    delete ui;
}

void CDynamicPlayDlg::init ()
{
    initVar ();
    initWindows ();
    initAction ();
    layout ();
    setStyle(QString(":/qss/qss/udpsendfile.qss"));
    ui->dyStartBtn->setStyleSheet(QString(
                                      "QPushButton {"
                                      "background: transparent;"
                                      "border-image: url(:/png/png/ic_play_normal.png);"
                                      "}"
                                      "QPushButton:hover{"
                                      "background: transparent;"
                                      "border-image: url(:/png/png/ic_play_press_hover.png);"
                                      "}"
                                      "QPushButton:pressed{"
                                      "background: transparent;"
                                      "border-image: url(:/png/png/ic_play_press_hover.png);"
                                      "}"
                                      )
                                  );
    ui->dyStopBtn->setStyleSheet(QString(
                                     "QPushButton{"
                                     "background: transparent;"
                                     "border-image: url(:/png/png/ic_stop_normal.png);"
                                     "}"
                                     "QPushButton:hover{"
                                     "background: transparent;"
                                     "border-image: url(:/png/png/ic_stop_press_hover.png);"
                                     "}"
                                     "QPushButton:pressed{"
                                     "background: transparent;"
                                     "border-image: url(:/png/png/ic_stop_press_hover.png);"
                                     "}")
                                 );
}

void CDynamicPlayDlg::initVar ()
{
    m_pPlayProcess = NULL;
    m_player = NULL;
    m_pTitleWidget = NULL;
    clearDynamicMusicMap ();
    m_strConfigPath = ApplicationPath + "/Config/dynamicplaylist.txt";
    CPubFun::loadPlayList(m_strConfigPath, m_mapDynamicMusic);
}

void CDynamicPlayDlg::initPlayListTableWidget ()
{
    setTableStyle(ui->dyPlayListTableWidget);

    QStringList header;
    header << tr("序号")
           << tr("文件名称")
           << tr("显示名称")
           << tr("舞蹈类型")
           << tr("舞蹈时长(单位: 秒)");
    int nCol = header.count();
    ui->dyPlayListTableWidget->setColumnCount(nCol);

    // 设置列宽
    ui->dyPlayListTableWidget->setColumnWidth(0, 100);
    ui->dyPlayListTableWidget->setColumnWidth(1, 300);
    ui->dyPlayListTableWidget->setColumnWidth(2, 300);
    ui->dyPlayListTableWidget->setColumnWidth(3, 300);
    ui->dyPlayListTableWidget->setColumnWidth(4, 300);
    ui->dyPlayListTableWidget->setHorizontalHeaderLabels(header);

    // 表头文字居中
    for ( int i = 0; i < nCol; i ++ )
    {
        ui->dyPlayListTableWidget->horizontalHeaderItem(i)->setTextAlignment(Qt::AlignCenter);
    }

    ui->dyPlayListTableWidget->setColumnHidden(3, true);
    ui->dyPlayListTableWidget->setColumnHidden(4, true);
}

void CDynamicPlayDlg::createTitleWidget ()
{
    m_pTitleWidget = new CTitleWidget(this);
    m_pTitleWidget->setMenuBtnVisible(false);
    m_pTitleWidget->setLogoVisible(false);
}

void CDynamicPlayDlg::initWindows ()
{
    createTitleWidget ();
    initPlayListTableWidget ();
    refreshPlayListTableWidget ();
}

void CDynamicPlayDlg::initAction ()
{
    connect(ui->dyStartBtn, SIGNAL(pressed()),
            this, SLOT(slotStart()));
    connect(ui->dyStopBtn, SIGNAL(pressed()),
            this, SLOT(slotStop()));

    connect(ui->dyPlayListTableWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(slotTableCustomContextMenuRequested(QPoint)) );
    connect(ui->dyPlayListTableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
            this, SLOT(slotEdit()));
}

void CDynamicPlayDlg::layout()
{
    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(m_pTitleWidget);
    pVLayout->addWidget(ui->mainWidget);
    pVLayout->setContentsMargins(QMargins(0,0,0,0));
    this->setLayout(pVLayout);
}

void CDynamicPlayDlg::clearDynamicMusicMap()
{
    QTypeMusicInfoMap::iterator it = m_mapDynamicMusic.begin();
    for ( ; it != m_mapDynamicMusic.end(); it ++ ){
        QMusicInfoMap* p = it.value();
        qDeleteAll(*p);
        p->clear();
    }

    qDeleteAll(m_mapDynamicMusic);
    m_mapDynamicMusic.clear();
}

void CDynamicPlayDlg::enableButton (bool bEnable)
{
    ui->dyStartBtn->setEnabled(bEnable);
    ui->dyStopBtn->setEnabled(bEnable);

    ui->dyPlayListTableWidget->setEnabled(bEnable);
}

void CDynamicPlayDlg::refreshPlayListTableWidget ()
{
    clearContentsTable(ui->dyPlayListTableWidget);
    QTypeMusicInfoMap::iterator it = m_mapDynamicMusic.begin();
    for ( ; it != m_mapDynamicMusic.end(); it ++ ) {
        int nType = it.key();
        QMusicInfoMap* p = it.value();
        QMusicInfoMap::iterator itM = p->begin();
        for ( ; itM != p->end(); itM ++ ){
            QString strFileName = itM.key();
            tagMusicInfo* pT = itM.value();
            addRow(nType, strFileName, pT);
        }
    }
}

void CDynamicPlayDlg::addRow (int nType, const QString& strFileName, tagMusicInfo* p)
{
    int nRowCount = ui->dyPlayListTableWidget->rowCount();
    ui->dyPlayListTableWidget->insertRow(nRowCount);

    QString strNum = QString::number( nRowCount + 1 );
    QTableWidgetItem* qtableItem = new QTableWidgetItem(strNum);
    qtableItem->setTextAlignment(Qt::AlignCenter);
    ui->dyPlayListTableWidget->setItem(nRowCount, 0, qtableItem);

    qtableItem = new QTableWidgetItem(strFileName);
    qtableItem->setTextAlignment(Qt::AlignCenter);
    ui->dyPlayListTableWidget->setItem(nRowCount, 1, qtableItem);

    qtableItem = new QTableWidgetItem(p->m_strDisplayName);
    qtableItem->setTextAlignment(Qt::AlignCenter);
    ui->dyPlayListTableWidget->setItem(nRowCount, 2, qtableItem);

    QString strMusicType = CPubFun::getMusicTypeName(nType);

    qtableItem = new QTableWidgetItem(strMusicType);
    qtableItem->setTextAlignment(Qt::AlignCenter);
    qtableItem->setData(Qt::UserRole, nType);
    ui->dyPlayListTableWidget->setItem(nRowCount, 3, qtableItem);

    qtableItem = new QTableWidgetItem(QString::number(p->m_nMusicTime));
    qtableItem->setTextAlignment(Qt::AlignCenter);
    ui->dyPlayListTableWidget->setItem(nRowCount, 4, qtableItem);
}

void CDynamicPlayDlg::slotStart ()
{
    int nCurRow = ui->dyPlayListTableWidget->currentRow();
    if ( nCurRow < 0 || nCurRow >= ui->dyPlayListTableWidget->rowCount() ){
        return ;
    }

    enableButton(false);
    QString strFileName = ui->dyPlayListTableWidget->item(nCurRow, 1)->text();
    // 动态播放不需要.xml
//    if ( !strFileName.endsWith(".xml") ){
//        strFileName += ".xml";
//    }

    QString strMp4Path = ApplicationPath + "/Config/actionlist/" + strFileName + ".mp4";
    QFile file(strMp4Path);
    if ( file.exists() )
    {
        play(strMp4Path);
    }

    // 文件名称发送悟空机器人
    m_pUsbComm->sendPlayCmd(strFileName, EN_CUSTOM_MUSIC_TYPE);
    m_pUsbComm->sendCmd (UBXPROTOCOL_CMDID_DYNAMIC_PLAYER, strFileName);
    enableButton(true);

    QMessageBox::information(this,
                             tr("集控软件"),
                             tr("动态播放命令发送完成"));
}

void CDynamicPlayDlg::slotStop ()
{
    enableButton(false);
    m_pUsbComm->sendCmd (UBXPROTOCOL_CMDID_STOP);
    enableButton(true);
    QMessageBox::information(this,
                             tr("集控软件"),
                             tr("停止播放命令发送完成"));
}

void CDynamicPlayDlg::play (const QString& strMp4Path)
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

void CDynamicPlayDlg::slotTableCustomContextMenuRequested(QPoint pos)
{
    QMenu menu(this);
    QTableWidgetItem* p = ui->dyPlayListTableWidget->itemAt(pos);
    if ( p == NULL ) {
        menu.addAction(tr("添加"), this, SLOT(slotAdd()));
    } else {
        menu.addAction(tr("编辑"), this, SLOT(slotEdit()));
        menu.addAction(tr("删除"), this, SLOT(slotDelete()));
    }
    menu.exec(QCursor::pos());
}

void CDynamicPlayDlg::slotAdd()
{
    CAddPlayDlg* pDlg = new CAddPlayDlg(this);
    pDlg->setTitleText(tr("添加动态播放列表"));
    pDlg->setSongs(m_mapDynamicMusic);
    pDlg->setDlgType(CAddPlayDlg::TYPE_ADD_DLG);
    pDlg->setMusicTimeVisiable(false);
    if ( pDlg->exec() == QDialog::Accepted ){
        QString strFileName = pDlg->getFileName();
        QString strDisplayName = pDlg->getDisplayName();
        int nTime = pDlg->getMusicTime();

        // 修改內存中结构
        tagMusicInfo* p = new tagMusicInfo;
        p->m_strDisplayName = strDisplayName;
        p->m_nMusicTime = nTime;

        QMusicInfoMap* pM = NULL;
        QTypeMusicInfoMap::iterator it = m_mapDynamicMusic.find(EN_CUSTOM_MUSIC_TYPE);
        if ( it != m_mapDynamicMusic.end() ){
            pM = it.value();
        }else{
            pM = new QMusicInfoMap;
            m_mapDynamicMusic.insert(EN_CUSTOM_MUSIC_TYPE, pM);
        }

        pM->insert(strFileName, p);

        // 刷新界面
        addRow(EN_CUSTOM_MUSIC_TYPE, strFileName, p);
    }
    pDlg->deleteLater();
    CPubFun::savePlayList(m_strConfigPath, m_mapDynamicMusic);
}

void CDynamicPlayDlg::slotEdit()
{
    int nRow = ui->dyPlayListTableWidget->currentRow();
    if ( nRow == -1 ){
        return ;
    }

    QTableWidgetItem* pFileNameItem = ui->dyPlayListTableWidget->item(nRow, 1);
    QTableWidgetItem* pDisplayNameItem = ui->dyPlayListTableWidget->item(nRow, 2);
    QTableWidgetItem* pTypeItem = ui->dyPlayListTableWidget->item(nRow, 3);
    QTableWidgetItem* pPlayTimeItem = ui->dyPlayListTableWidget->item(nRow, 4);
    if ( !pFileNameItem || !pDisplayNameItem || !pPlayTimeItem || !pTypeItem ){
        return ;
    }

    int nType = pTypeItem->data(Qt::UserRole).toInt();
    QString strOldFileName = pFileNameItem->text();
    QString strOldDisplayName = pDisplayNameItem->text();
    int nOldTime = pPlayTimeItem->text().toInt();

    CAddPlayDlg* pDlg = new CAddPlayDlg(this);
    pDlg->setDlgType(CAddPlayDlg::TYPE_EDIT_DLG);
    pDlg->setTitleText(tr("编辑动态播放列表"));
    pDlg->setSongs(m_mapDynamicMusic);
    pDlg->setFileName(strOldFileName);
    pDlg->setDisplayName(strOldDisplayName);
    pDlg->setMusicTime(nOldTime);
    pDlg->setMusicTimeVisiable(false);
    if ( pDlg->exec() == QDialog::Accepted ) {
        QString strFileName = pDlg->getFileName();
        QString strDisplayName = pDlg->getDisplayName();
        int nTime = pDlg->getMusicTime();

        if ( !(strOldDisplayName == strDisplayName &&
               strOldFileName == strFileName &&
               nOldTime == nTime) ) {
            // 刷新界面
            pFileNameItem->setText(strFileName);
            pDisplayNameItem->setText(strDisplayName);
            pPlayTimeItem->setText(QString::number(nTime));

            // 修改内存结构
            tagMusicInfo* p = new tagMusicInfo;
            p->m_strDisplayName = strDisplayName;
            p->m_nMusicTime = nTime;

            QTypeMusicInfoMap::Iterator it = m_mapDynamicMusic.find(nType);
            if ( it != m_mapDynamicMusic.end() ){
                QMusicInfoMap* pM = it.value();

                QMusicInfoMap::iterator itM = pM->find(strOldFileName);
                if ( itM != pM->end() ){
                    tagMusicInfo* pT = itM.value();
                    delete pT;
                    pT = NULL;
                    pM->erase(itM);
                }

                pM->insert(strFileName, p);
            }
        }
    }
    pDlg->deleteLater();
    CPubFun::savePlayList(m_strConfigPath, m_mapDynamicMusic);
}

void CDynamicPlayDlg::slotDelete ()
{
    QList<QTableWidgetSelectionRange> list = ui->dyPlayListTableWidget->selectedRanges();
    int nRangeCount = list.count();
    if ( nRangeCount > 0 ) {
        int nButtons = QMessageBox::question(this,
                                             tr("集控软件"),
                                             tr("是否删除已选中的动态播放列表？"),
                                             tr("是"),
                                             tr("否"),
                                             tr(""),
                                             1,
                                             1);
        if ( nButtons == 1 ) {
            return ;
        }
    }
    for( int i = 0; i < nRangeCount; i ++ ) {
        QTableWidgetSelectionRange range = list.at(i);
        for ( int iRow = range.topRow(); iRow <= range.bottomRow(); iRow ++ ) {
            int nType = ui->dyPlayListTableWidget->item(iRow, 3)->data(Qt::UserRole).toInt();
            QString strText = ui->dyPlayListTableWidget->item(iRow, 1)->text();
            if ( !strText.isEmpty() ) {
                QTypeMusicInfoMap::Iterator it = m_mapDynamicMusic.find(nType);
                if ( it != m_mapDynamicMusic.end() ){
                    QMusicInfoMap* pM = it.value();
                    QMusicInfoMap::iterator itM = pM->find(strText);
                    if ( itM != pM->end() ){
                        tagMusicInfo* p = itM.value();
                        delete p;
                        p = NULL;
                        pM->erase(itM);
                    }
                }
            }

            ui->dyPlayListTableWidget->removeRow(iRow);
        }
    }

    refreshPlayListTableWidget();
    CPubFun::savePlayList(m_strConfigPath, m_mapDynamicMusic);
}
