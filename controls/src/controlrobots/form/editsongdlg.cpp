#include "editsongdlg.h"
#include "ui_editsongdlg.h"

#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QMenu>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

#include "titlewidget.h"
#include "addplaydlg.h"
#include "sharelog.h"
#include "JlCompress.h"
#include "usbcomm.h"
#include "cfileinfo.h"
#include "qtablecellwidget.h"

#define PLAY_LIST (QCoreApplication::applicationDirPath() + "/Config/playlist.txt")

CEditSongDlg::CEditSongDlg(const QString &playListPath, QWidget *parent) :
    XDialog(parent),
    m_playListPath(playListPath),
    ui(new Ui::CEditSongDlg)
{
    ui->setupUi(this);
    init ();
}

CEditSongDlg::~CEditSongDlg()
{
    QTypeMusicInfoMap::iterator it = m_mapSongInfo.begin();
    for ( ; it != m_mapSongInfo.end(); it ++ ){
        QMusicInfoMap* p = it.value();
        qDeleteAll(*p);
        p->clear();
    }
    qDeleteAll(m_mapSongInfo);
    m_mapSongInfo.clear();
    m_listUsbSync.clear();
    delete ui;
}

void CEditSongDlg::init ()
{
    initVar ();
    initWindows ();
    initAction ();
    layout ();
    setStyle(QString(":/qss/qss/udpsendfile.qss"));
}

void CEditSongDlg::initVar ()
{
    m_pTitleWidget = Q_NULLPTR;
    qDeleteAll(m_mapSongInfo);
    m_mapSongInfo.clear();
    CPubFun::loadPlayList(m_playListPath, m_mapSongInfo);
    m_listUsbSync.clear();
}

void CEditSongDlg::initSongTableWidget ()
{
    setTableStyle(ui->tableWidget);
    SCheckBoxHeaderView* pHeader = new SCheckBoxHeaderView(0, Qt::Horizontal, this);
    ui->tableWidget->setHorizontalHeader(pHeader);
    connect(pHeader, &SCheckBoxHeaderView::checkStausChange, [=](bool check){
        for ( int i = 0; i < ui->tableWidget->rowCount(); i ++ )
            selectRows (i, check);
    });
    /* 单击任务表格中的item, 去掉全选选项 */
    connect(ui->tableWidget, &QTableWidget::pressed, [pHeader](){
        pHeader->setChecked(false);
    });
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget->horizontalHeader()->setFixedHeight(32); //设置表头的高度
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(40);  //设置行高
    ui->tableWidget->setStyleSheet(QString("QTableWidget {"
                                           "border:1px solid #F0F0F0;"
                                           "selection-background-color: #12C2F4;"
                                           "font-size:14px;"
                                           "color:#6A6A6A;"
                                           "}"));
    ui->tableWidget->horizontalHeader()->setStyleSheet(QString("QHeaderView {"
                                                               "background-color:#f9f9f9;"
                                                               "border-bottom:1px solid #d7d7d7;"
                                                               "font-size:14px;"
                                                               "color:#6A6A6A;"
                                                               "}"
                                                               "QHeaderView::section {"
                                                               "background-color:#F5F6FA;"
                                                               "padding-left: 4px;"
                                                               "border: none;"
                                                               "border-right:1px solid #BEBEBE;"
                                                               "border-bottom:1px solid #BEBEBE;"
                                                               "padding: 1px;"
                                                               "}"));
    QStringList header;
    header << tr("序号")
           << tr("文件名称")
           << tr("舞蹈名称")
           << tr("舞蹈类型")
           << tr("舞蹈时长(单位: 秒)");
    int nCol = header.count();
    ui->tableWidget->setColumnCount(nCol);

    // 设置列宽
    ui->tableWidget->setColumnWidth(0, 80);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 200);
    ui->tableWidget->setColumnWidth(3, 150);
    ui->tableWidget->setHorizontalHeaderLabels(header);

    // 表头文字居中
    for ( int i = 0; i < nCol; i ++ ) {
        ui->tableWidget->horizontalHeaderItem(i)->setTextAlignment(Qt::AlignCenter);
    }
}

void CEditSongDlg::createTitleWidget ()
{
    m_pTitleWidget = new CTitleWidget(this);
    m_pTitleWidget->setMenuBtnVisible(false);
    m_pTitleWidget->setLogoVisible(false);
}

void CEditSongDlg::initWindows ()
{
    createTitleWidget ();
    initSongTableWidget ();
    refreshSongTableWidget ();
    updateNumberLabel ();
}

void CEditSongDlg::initAction ()
{
    connect(ui->updateBtn, SIGNAL(pressed()), this, SLOT(slotUpdate()));
    connect(ui->addBtn, SIGNAL(pressed()), this, SLOT(slotAdd()));
    connect(ui->saveBtn, SIGNAL(pressed()), this, SLOT(slotSave()));

    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(slotTableCustomContextMenuRequested(QPoint)) );
    connect(ui->tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
            this, SLOT(slotEdit()));
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, [=](){
        QList<int> rows;
        getSelRows(rows);
        for ( int i = 0; i < ui->tableWidget->rowCount(); i ++ ){
            QTableCellWidget* pCellWidget = qobject_cast<QTableCellWidget*>(ui->tableWidget->cellWidget(i, 4));
            if ( pCellWidget ){
                pCellWidget->setSelectRowTextStyle(rows.contains(i) ? true : false);
            }
        }
    });
}

void CEditSongDlg::layout()
{
    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(m_pTitleWidget);
    pVLayout->addWidget(ui->mainWidget);
    pVLayout->setSpacing(0);
    pVLayout->setContentsMargins(QMargins(0,0,0,0));
    this->setLayout(pVLayout);
}

void CEditSongDlg::refreshSongTableWidget ()
{
    clearContentsTable(ui->tableWidget);
    int nCount = 0;
    QTypeMusicInfoMap::iterator it = m_mapSongInfo.begin();
    for ( ; it != m_mapSongInfo.end(); it ++ ) {
        int nType = it.key();
        QMusicInfoMap* p = it.value();
        nCount += p->count();
        QMusicInfoMap::iterator itM = p->begin();
        for ( ; itM != p->end(); itM ++ ){
            QString strFileName = itM.key();
            tagMusicInfo* pT = itM.value();
            addRow(nType, strFileName, pT);
        }
    }
}

void CEditSongDlg::addRow (int nType, const QString& strFileName, tagMusicInfo* p)
{
    int nRowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(nRowCount);
    QString strMusicType = CPubFun::getMusicTypeName(nType);

    QString strNum = QString::number( nRowCount + 1 );
    QTableWidgetItem* qtableItem = new QTableWidgetItem(strNum);
    qtableItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(nRowCount, 0, qtableItem);

    qtableItem = new QTableWidgetItem(strFileName);
    qtableItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(nRowCount, 1, qtableItem);

    qtableItem = new QTableWidgetItem(p->m_strDisplayName);
    qtableItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(nRowCount, 2, qtableItem);

    qtableItem = new QTableWidgetItem(strMusicType);
    qtableItem->setTextAlignment(Qt::AlignCenter);
    qtableItem->setData(Qt::UserRole, nType);
    ui->tableWidget->setItem(nRowCount, 3, qtableItem);

    QTableCellWidget* pCellWidget = new QTableCellWidget(nRowCount, ui->tableWidget);
    pCellWidget->setMusicTime(p->m_nMusicTime);
    pCellWidget->setUrl(p->m_strDownLoadUrl);
    pCellWidget->setFileName(strFileName);
    if ( nType == EN_CUSTOM_MUSIC_TYPE ) {
        pCellWidget->setDownloadBtnVisible(false);
    }
    connect(pCellWidget, SIGNAL(removeRow (QList<int>)),
            this,        SLOT  (removeRows(QList<int>)));
    connect(pCellWidget, SIGNAL(sync ()),
            this,        SLOT  (slotSync()));
    connect(pCellWidget, SIGNAL(refreshMusicInfo     (int,QString,tagMusicInfo*)),
            this,        SLOT  (slotRefreshMusicInfo (int,QString,tagMusicInfo*)));
    ui->tableWidget->setCellWidget(nRowCount, 4, pCellWidget);
}

int CEditSongDlg::getSelRows (QList<int>& rows)
{
    for ( int i = 0; i < ui->tableWidget->selectedRanges().count(); i ++ ){
        QTableWidgetSelectionRange range = ui->tableWidget->selectedRanges().at(i);
        for ( int row = range.topRow(); row <= range.bottomRow(); row ++ ){
            rows.append(row);
        }
    }

    return rows.count();
}

void CEditSongDlg::refreshSyncState ()
{
    for ( int i = 1; i < m_listUsbSync.count(); i ++ ){
        int row = m_listUsbSync.at(i);
        QTableCellWidget* pCellWidget = qobject_cast<QTableCellWidget*>(ui->tableWidget->cellWidget(row, 4));
        if ( pCellWidget ){
            pCellWidget->setSyncWaiteState();
        }
    }
}

void CEditSongDlg::selectRows (int row, bool bSel)
{
    QItemSelectionModel::SelectionFlags flags;
    if ( bSel ) {
        flags = QItemSelectionModel::Select;
    } else {
        flags = QItemSelectionModel::Deselect | QItemSelectionModel::Clear;
    }

    QAbstractItemModel *pModel = ui->tableWidget->model();
    QModelIndex topLeft = pModel->index(row, 0);
    QModelIndex bottomRight = pModel->index(row, pModel->columnCount() - 1);
    QItemSelection selection;
    selection.merge(QItemSelection(topLeft, bottomRight), flags);
    ui->tableWidget->selectionModel()->select(selection, flags);
}

void CEditSongDlg::slotUpdate()
{
    ui->numLabel->setText(tr("正在更新舞蹈..."));
    if ( updateDance () )
        refreshSongTableWidget(); // 刷新界面
}

void CEditSongDlg::slotAdd()
{
    CAddPlayDlg* pDlg = new CAddPlayDlg(this);
    pDlg->setTitleText(tr("添加舞蹈"));
    pDlg->setSongs(m_mapSongInfo);
    pDlg->setDlgType(CAddPlayDlg::TYPE_ADD_DLG);
    if ( pDlg->exec() == QDialog::Accepted ){
        QString strFileName = pDlg->getFileName();
        QString strDisplayName = pDlg->getDisplayName();
        int nTime = pDlg->getMusicTime();
        // 修改內存中结构
        tagMusicInfo* p = new tagMusicInfo;
        p->m_strDisplayName = strDisplayName;
        p->m_nMusicTime = nTime;
        p->m_strDownLoadUrl = "";
        QMusicInfoMap* pM = m_mapSongInfo.value(EN_CUSTOM_MUSIC_TYPE);
        if ( pM == Q_NULLPTR ) {
            pM = new QMusicInfoMap;
            m_mapSongInfo.insert(EN_CUSTOM_MUSIC_TYPE, pM);
        }
        pM->insert(strFileName, p);
        // 刷新界面
        addRow(EN_CUSTOM_MUSIC_TYPE, strFileName, p);
    }
    pDlg->deleteLater();

    updateNumberLabel();
}

void CEditSongDlg::slotEdit()
{
    int row = ui->tableWidget->currentRow();
    if ( row == -1 ){
        return ;
    }

    QTableWidgetItem* pFileNameItem     = ui->tableWidget->item(row, 1);
    QTableWidgetItem* pDisplayNameItem  = ui->tableWidget->item(row, 2);
    QTableWidgetItem* pTypeItem         = ui->tableWidget->item(row, 3);
    QTableCellWidget* pCellWidget  = qobject_cast<QTableCellWidget*>(ui->tableWidget->cellWidget(row, 4));

    int nOldTime = pCellWidget->getMusicTime();
    int nType = pTypeItem->data(Qt::UserRole).toInt();
    QString strOldFileName = pFileNameItem->text();
    QString strOldDisplayName = pDisplayNameItem->text();

    CAddPlayDlg* pDlg = new CAddPlayDlg(this);
    pDlg->setDlgType(CAddPlayDlg::TYPE_EDIT_DLG);
    pDlg->setTitleText(tr("编辑舞蹈"));
    pDlg->setSongs(m_mapSongInfo);
    pDlg->setFileName(strOldFileName);
    pDlg->setDisplayName(strOldDisplayName);
    pDlg->setMusicTime(nOldTime);
    if ( pDlg->exec() == QDialog::Accepted ) {
        QString strFileName = pDlg->getFileName();
        QString strDisplayName = pDlg->getDisplayName();
        int nTime = pDlg->getMusicTime();
        if ( !(strOldDisplayName == strDisplayName && strOldFileName == strFileName && nOldTime == nTime) ) {
            pFileNameItem->setText(strFileName);
            pDisplayNameItem->setText(strDisplayName);
            pCellWidget->setMusicTime(nTime);

            QTypeMusicInfoMap::Iterator it = m_mapSongInfo.find(nType);
            if ( it != m_mapSongInfo.end() ){
                QMusicInfoMap* pM = it.value();
                QMusicInfoMap::iterator itM = pM->find(strOldFileName);
                if ( itM != pM->end() ){
                    tagMusicInfo* p = itM.value();
                    p->m_strDisplayName = strDisplayName;
                    p->m_nMusicTime = nTime;
                    p->m_strDownLoadUrl = pCellWidget->getUrl();
                }
            }
        }
    }
    pDlg->deleteLater();
}

void CEditSongDlg::slotSave ()
{
    qDeleteAll(m_mapSongInfo);
    m_mapSongInfo.clear();

    int nCount = ui->tableWidget->rowCount();
    for ( int i = 0; i < nCount; i ++ ){
        QString strFileName = ui->tableWidget->item(i, 1)->text();
        int nType = ui->tableWidget->item(i, 3)->data(Qt::UserRole).toInt();
        tagMusicInfo* p = new tagMusicInfo;
        p->m_strDisplayName = ui->tableWidget->item(i, 2)->text();
        QTableCellWidget* pCellWidget = qobject_cast<QTableCellWidget*>(ui->tableWidget->cellWidget(i, 4));
        if ( pCellWidget ){
            p->m_nMusicTime = pCellWidget->getMusicTime();
        }
        QMusicInfoMap* pM = m_mapSongInfo.value(nType);
        if ( pM == Q_NULLPTR ) {
            pM = new QMusicInfoMap;
            m_mapSongInfo.insert(nType, pM);
        }
        pM->insert(strFileName, p);
    }

    if ( CPubFun::savePlayList(m_playListPath, m_mapSongInfo) ) {
        QMessageBox::information(this, tr("集控软件"), tr("保存成功"));
    } else {
        QMessageBox::warning(this, tr("集控软件"), tr("保存失败"));
    }
}

void CEditSongDlg::slotBatchDownload ()
{
    QList<int> rows;
    int nCount = getSelRows(rows);  /* 优先保存选择的行号  */
    for ( int i = 0; i < nCount; i ++ ){
        QTableCellWidget* pCellWidget = qobject_cast<QTableCellWidget*>(ui->tableWidget->cellWidget(rows.at(i), 4));
        if ( pCellWidget ){
            pCellWidget->slotDownload();
        }
    }
}

void CEditSongDlg::slotBatchSync ()
{
    /* 根据m_listUsbSync数量判定是否正在进行文件传输，如果存在，则仅需添加m_listUsbSync列表 */
    bool bNeedStart = m_listUsbSync.count() > 0 ? false : true;
    QList<int> rows;
    getSelRows(rows);  /* 优先保存选择的行号  */
    m_listUsbSync.append(rows);
    refreshSyncState ();
    if ( bNeedStart ){
        startSync();
    }
}

void CEditSongDlg::slotBatchDelete ()
{
    QList<int> rows;
    for ( int i = 0; i < ui->tableWidget->selectedRanges().count(); i ++ ){
        QTableWidgetSelectionRange range = ui->tableWidget->selectedRanges().at(i);
        for ( int row = range.topRow(); row <= range.bottomRow(); row ++ ){
            rows.append(row);
        }
    }

    int cnt = rows.count();
    if ( cnt > 0 ){
        int nBtnNum = QMessageBox::warning(this,
                                           tr("集控软件"),
                                           tr("确定删除%1个舞蹈吗").arg(cnt),
                                           tr("取消"),
                                           tr("确定"),
                                           tr(""),
                                           0,
                                           0);
        if ( nBtnNum == 0 ) {
            return ;
        }

        removeRows(rows);
    }
}

void CEditSongDlg::slotSync ()
{
    /* 根据m_listUsbSync数量判定是否正在进行文件传输，如果存在，则仅需添加m_listUsbSync列表 */
    bool bNeedStart = m_listUsbSync.count() > 0 ? false : true;
    QTableCellWidget* p = qobject_cast<QTableCellWidget*>(sender());
    if ( p ){
        m_listUsbSync.append(p->getRow());
    }
    refreshSyncState ();
    if ( bNeedStart ){
        startSync();
    }
}

void CEditSongDlg::startSync ()
{
    while ( m_listUsbSync.count() > 0 ){
        int row = m_listUsbSync.at(0);
        QTableCellWidget* pCellWidget = qobject_cast<QTableCellWidget*>(ui->tableWidget->cellWidget(row, 4));
        if ( pCellWidget ){
            pCellWidget->slotSync();
        }
        m_listUsbSync.removeAt(0);
    }
}

void CEditSongDlg::slotBatchOpenDir ()
{
    QList<int> rows;
    int nCount = getSelRows(rows);
    for ( int i = 0; i < nCount; i ++ ){
        QTableCellWidget* pCellWidget = qobject_cast<QTableCellWidget*>(ui->tableWidget->cellWidget(rows.at(i), 4));
        if ( pCellWidget ){
            pCellWidget->slotOpenFolder();
        }
    }
}

void CEditSongDlg::slotTableCustomContextMenuRequested(QPoint pos)
{
    Q_UNUSED(pos);
    QMenu menu;
    menu.setStyleSheet("QMenu{"
                       "background: rgba(245,246,250,0.8);"
                       "border: 1px solid gray;"
                       "padding: 1px;"
                       "font-size: 16px;"
                       "border-radius: 4px;"
                       "color: #6A6A6A;"
                       "}"
                       "QMenu::item { "
                       "background-color: transparent;"
                       "padding:0px 10px 0px 12px;"
                       "border: 1px solid transparent;"
                       "height: 25px;"
                       "}"
                       "QMenu::item:selected {"
                       "background:transparent;"
                       "background-color: rgb(146, 206, 237);"
                       "}");
    int nCount = 0;
    for( int i = 0; i < ui->tableWidget->selectedRanges().count(); i ++ )
        nCount += ui->tableWidget->selectedRanges().at(i).rowCount();

    if ( nCount > 1 ){
        menu.addAction(tr("下载"), this, SLOT(slotBatchDownload()));
        menu.addAction(tr("同步到机器人"), this, SLOT(slotBatchSync()));
        menu.addAction(tr("打开所在的文件夹"), this, SLOT(slotBatchOpenDir()));
        menu.addSeparator();
        menu.addAction(tr("删除"), this, SLOT(slotBatchDelete()));
    } else if ( nCount == 1 ) {
        menu.addAction(tr("下载"), this, SLOT(slotBatchDownload()));
        menu.addAction(tr("同步到机器人"), this, SLOT(slotBatchSync()));
        menu.addAction(tr("打开所在的文件夹"), this, SLOT(slotBatchOpenDir()));
        menu.addSeparator();
        menu.addAction(tr("编辑"), this, SLOT(slotEdit()));
        menu.addAction(tr("删除"), this, SLOT(slotBatchDelete()));
    } else {
        menu.addAction(tr("添加"), this, SLOT(slotAdd()));
    }

    menu.exec(QCursor::pos());
}

void CEditSongDlg::slotRefreshMusicInfo (int nMusicType, const QString& musicName, tagMusicInfo* pMusic)
{
    QTableCellWidget* p = qobject_cast<QTableCellWidget*>(sender());
    if ( p ){
        int nRow = p->getRow();
        int nType = ui->tableWidget->item(nRow, 3)->data(Qt::UserRole).toInt();
        if ( nType != nMusicType ) {
            ui->tableWidget->item(nRow, 3)->setText(CPubFun::getMusicTypeName(nMusicType));
            ui->tableWidget->item(nRow, 3)->setData(Qt::UserRole, nMusicType);

            /* 清除旧舞蹈类型中的数据 */
            QMusicInfoMap* pMusicInfo = m_mapSongInfo.value(nType);
            if ( pMusicInfo ){
                delete pMusicInfo->value(musicName);
                pMusicInfo->remove(musicName);
            }
        }

        /* 若没有添加过，则创建，若已经存在，则删除，最后添加更新后的舞蹈数据 */
        QMusicInfoMap* pMusicInfo = m_mapSongInfo.value(nMusicType);
        if ( pMusicInfo == Q_NULLPTR ){
            pMusicInfo = new QMusicInfoMap;
            m_mapSongInfo.insert(nMusicType, pMusicInfo);
        } else {
            delete pMusicInfo->value(musicName);
        }
        pMusicInfo->insert(musicName, pMusic);

        ui->tableWidget->item(nRow, 2)->setText(pMusic->m_strDisplayName);
        p->setMusicTime(pMusic->m_nMusicTime);
    }
}

void CEditSongDlg::removeRows(QList<int> rows)
{
    for ( int i = 0; i < rows.count(); i ++ ) {
        int nRow = rows.at(i);
        int nMusicType = ui->tableWidget->item(nRow, 3)->data(Qt::UserRole).toInt();
        QString strMusicName = ui->tableWidget->item(nRow, 1)->text();
        /* 2、删除文件夹 */
        QTableCellWidget* pCellWidget = qobject_cast<QTableCellWidget*>(ui->tableWidget->cellWidget(nRow, 4));
        if ( pCellWidget )
            pCellWidget->startDeleteFolder();
        /* 3、删除内存数据 */
        QMusicInfoMap* pM = m_mapSongInfo.value(nMusicType);
        if ( pM ){
            QMusicInfoMap::iterator itM = pM->find(strMusicName);
            if ( itM != pM->end() ){
                delete itM.value();
                pM->erase(itM);
            }
        }
    }

    refreshSongTableWidget();
    updateNumberLabel ();
}

void CEditSongDlg::updateNumberLabel ()
{
    ui->numLabel->setText(QString("共%1首").arg(ui->tableWidget->rowCount()));
}

bool CEditSongDlg::updateDance ()
{
    bool bRet = false;
    int nAddCnt = 0; /* 新增舞蹈数量 */
    QNetworkRequest request;
    CPubFun::addSignRequest(&request, HTTP_GET_ACTIONLIST_URL, APPID, "", qApp->applicationName());
    QNetworkAccessManager pManager(this);
    QNetworkReply *reply = pManager.get(request);
    int nState = CPubFun::waitForReply (reply);
    int nHttpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if ( nState == EN_REQUEST_WAIT_TIME_OUT ) {
        LOG(ERROR) << HTTP_GET_ACTIONLIST_URL << ", request time out.";
        QMessageBox::critical (this,
                               tr ("错误"),
                               tr ("网络不给力，无法更新。"),
                               QMessageBox::Close);
    } else if ( nState == EN_REQUEST_WAIT_NETERROR ) {
        LOG(ERROR) << HTTP_GET_ACTIONLIST_URL << ", HttpStatusCode:" << nHttpStatusCode;
        QMessageBox::critical (this,
                               tr ("错误"),
                               tr ("网络不给力，无法更新。"),
                               QMessageBox::Close);
    } else {
        do{
            if ( nHttpStatusCode != 200 ){
                LOG(ERROR) << HTTP_GET_ACTIONLIST_URL << ", HttpStatusCode:" << nHttpStatusCode;
                break;
            }

            QByteArray data = reply->readAll();
            QJsonDocument document = QJsonDocument::fromJson (data);
            if (document.isNull()) {
                LOG(ERROR) << "JSON is invalid";
                break;
            }

            /* 更新歌单放置非系统目录 */
            QMusicInfoMap* pMapSysMusic = m_mapSongInfo.value(EN_SYSTEM_MUSIC_TYPE);
            QMusicInfoMap* pMapCustomMusic = m_mapSongInfo.value(EN_CUSTOM_MUSIC_TYPE);
            QMusicInfoMap* pMapUpdateMusic = m_mapSongInfo.value(EN_UPDATE_MUSIC_TYPE);
            if ( pMapUpdateMusic == Q_NULLPTR ) {
                pMapUpdateMusic = new QMusicInfoMap;
                m_mapSongInfo.insert(EN_UPDATE_MUSIC_TYPE, pMapUpdateMusic);
            }
            int nCount = document.array().count();
            for ( int i = 0; i < nCount; i ++ ){
                QJsonObject action    = document.array().at(i).toObject();
                QString strActionName = action.value("actionName").toString();
                int nActionTime       = action.value("actionTime").toString().toInt();
                QString strActionUrl  = action.value("actionUrl").toString();
                QString strFileName   = action.value("fileName").toString();

                /* 去掉系统歌单中重复名称 */
                if ( pMapSysMusic ) {
                    tagMusicInfo* pSysMusic = pMapSysMusic->value(strFileName);
                    if ( pSysMusic ) {
                        SAFE_DELETE(pSysMusic);
                        pMapSysMusic->remove(strFileName);
                    }
                }

                /* 去掉自定义歌单中重复名称 */
                if ( pMapCustomMusic ) {
                    tagMusicInfo* pCustomMusic = pMapCustomMusic->value(strFileName);
                    if ( pCustomMusic ) {
                        SAFE_DELETE(pCustomMusic);
                        pMapCustomMusic->remove(strFileName);
                    }
                }


                tagMusicInfo* pUpdateMusic = pMapUpdateMusic->value(strFileName);
                if ( pUpdateMusic == Q_NULLPTR  ) {
                    pUpdateMusic = new tagMusicInfo;
                    nAddCnt ++;
                }
                pUpdateMusic->m_strDisplayName = strActionName;
                pUpdateMusic->m_nMusicTime = nActionTime;
                pUpdateMusic->m_strDownLoadUrl = strActionUrl;
                pMapUpdateMusic->insert(strFileName, pUpdateMusic);
            }
            bRet = true;
        }while(0);
    }
    reply->abort();
    reply->deleteLater();

    if ( bRet ) {
        ui->numLabel->setText(tr("更新完成，新增%1首").arg(nAddCnt));
    } else {
        ui->numLabel->setText(tr("更新出错"));
    }
    QTimer::singleShot(3000, this, SLOT(updateNumberLabel()));  /* 三秒之后，显示总共的歌曲数量 */

    return bRet;
}

void CEditSongDlg::closeEvent(QCloseEvent *event)
{
    /* 取消同步数据 */
    while ( m_listUsbSync.count() > 0 ){
        QTableCellWidget* pCellWidget = qobject_cast<QTableCellWidget*>(ui->tableWidget->cellWidget(m_listUsbSync.at(0), 4));
        if ( pCellWidget )
            pCellWidget->cancelSyncFile();
        m_listUsbSync.removeAt(0);
    }
    event->accept();
}
