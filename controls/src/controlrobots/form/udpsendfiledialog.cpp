#include "udpsendfiledialog.h"
#include "ui_udpsendfiledialog.h"
#include "titlewidget.h"
#include "pubdef.h"
#include "usbcomm.h"

#include <QFileDialog>
#include <QMenu>
#include <QProgressBar>
#include <QThread>
#include <QMessageBox>
#include <qprogressbar.h>
#include <QComboBox>

CUdpSendFileDialog::CUdpSendFileDialog(QWidget *parent) :
    XDialog(parent),
    ui(new Ui::CUdpSendFileDialog)
{
    ui->setupUi(this);
    init ();
}

CUdpSendFileDialog::~CUdpSendFileDialog()
{
    if ( m_pThread && m_pThread->isRunning() ){
        m_pThread->quit();
        m_pThread->wait();
        delete m_pThread;
        m_pThread = NULL;
    }
    delete m_pUsbComm;
    m_pUsbComm = NULL;
    delete ui;
}

void CUdpSendFileDialog::init ()
{
    initVar ();
    initWindows ();
    initAction ();
    layout ();
    setStyle(QString(":/qss/qss/udpsendfile.qss"));

    ui->sendFileBtn->setEnabled(false);
}

void CUdpSendFileDialog::initVar ()
{
    m_pThread = NULL;
    m_pTitleWidget = NULL;
    m_pMenu = NULL;
    m_listSendingFilePath.clear();
    m_sFileInfo.clear();

    m_pUsbComm = new UsbComm;
    connect(m_pUsbComm, SIGNAL(sigSendBytes(QString,int)),
            this, SLOT(slotSendBytes(QString, int)));
    connect(m_pUsbComm, SIGNAL(sigSendOver(QString, bool)),
            this, SLOT(slotSendOver(QString, bool)));
    connect(this, SIGNAL(sigSendFileInfo(CFileInfo*)),
            m_pUsbComm, SLOT(slotSendFileInfo(CFileInfo*)));
}

void CUdpSendFileDialog::initWindows ()
{
    createTitleWidget ();
    initFileInfoTableWidget();
}

void CUdpSendFileDialog::createTitleWidget ()
{
    m_pTitleWidget = new CTitleWidget(this);
    m_pTitleWidget->setMenuBtnVisible(false);
    m_pTitleWidget->setLogoVisible(false);
}

void CUdpSendFileDialog::initAction ()
{
    connect(ui->sendFileBtn, SIGNAL(pressed()),
            this, SLOT(slotSendFile()));

    connect(ui->fileInfoTableWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(slotTableCustomContextMenuRequested(QPoint)) );
}

void CUdpSendFileDialog::layout()
{
    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(m_pTitleWidget);
    pVLayout->addWidget(ui->centralWidget1);
    pVLayout->setContentsMargins(QMargins(0,0,0,0));
    this->setLayout(pVLayout);
}

void CUdpSendFileDialog::initFileInfoTableWidget ()
{
    setTableStyle(ui->fileInfoTableWidget);
    clearContentsTable(ui->fileInfoTableWidget);

    QStringList header;
    header << tr("序号")
           << tr("文件名称")
           << tr("文件大小（KB）")
           << tr("MD5")
           << tr("机器人路径类型")
           << tr("本地路径")
           << tr("发送进度")
           << tr("发送结果");
    int nCol = header.count();
    ui->fileInfoTableWidget->setColumnCount(nCol);

    // 设置列宽
    ui->fileInfoTableWidget->setColumnWidth(EN_FILE_INFO_COL_INDEX, 100);
    ui->fileInfoTableWidget->setColumnWidth(EN_FILE_INFO_COL_NAME, 200);
    ui->fileInfoTableWidget->setColumnWidth(EN_FILE_INFO_COL_SIZE, 200);
    ui->fileInfoTableWidget->setColumnWidth(EN_FILE_INFO_COL_MD5, 200);
    ui->fileInfoTableWidget->setColumnWidth(EN_FILE_INFO_COL_TYPE, 200);
    ui->fileInfoTableWidget->setColumnWidth(EN_FILE_INFO_COL_PATH, 200);
    ui->fileInfoTableWidget->setColumnWidth(EN_FILE_INFO_COL_PROGRESS, 200);
    ui->fileInfoTableWidget->setColumnWidth(EN_FILE_INFO_COL_STATE, 200);
    ui->fileInfoTableWidget->setHorizontalHeaderLabels(header);

    // 表头文字居中
    for ( int i = 0; i < nCol; i ++ ){
        ui->fileInfoTableWidget->horizontalHeaderItem(i)->setTextAlignment(Qt::AlignCenter);
    }
}

void CUdpSendFileDialog::slotSendFile ()
{
    if ( !m_pUsbComm->isOpen() ){
        QMessageBox::warning(this, tr("集控软件"), tr("发送失败, 请确认是否已连接集控发射器"));
        return ;
    }
    m_listSendingFilePath.clear();
    QString strError = "";

    reloadFileInfoData();
    int nCount = m_sFileInfo.count();
    for ( int i = 0; i < nCount; i ++ ){
        CFileInfo* p = m_sFileInfo.at(i);
        /**< 文件中若未指定远程路径类型或者没有勾选，则不发送 */
        if ( p->m_nRobotPathType == EN_ROBOT_NONE_TYPE ){
            strError += QString("%1 %2\r\n").arg(p->m_strFileName).arg(tr("未选择"));
        }else if ( !p->getSendEnable() ){
            strError += QString("%1 %2\r\n").arg(p->m_strFileName).arg(tr("未选择机器人路径类型"));
        }else{
            m_listSendingFilePath.append(p->getFilePath());
            emit sigSendFileInfo(p);
        }
    }

    if ( m_listSendingFilePath.count() > 0 ) {
        ui->sendFileBtn->setEnabled(false);
        enabledFileInfoTableWidget(false);
    }else{
        ui->sendFileBtn->setEnabled(true);
    }

    if ( !strError.isEmpty() ) {
        QMessageBox::warning(this, tr("集控软件"), tr("发送不成功,原因:%1").arg(strError));
    }
}

void CUdpSendFileDialog::slotMenuSendFile ()
{
    if ( !m_pUsbComm->isOpen() ){
        QMessageBox::warning(this, tr("集控软件"), tr("发送失败, 请确认是否已连接集控发射器"));
        return ;
    }
    m_listSendingFilePath.clear();
    QString strError = "";

    QList<QTableWidgetSelectionRange> list = ui->fileInfoTableWidget->selectedRanges();
    int nRangeCount = list.count();
    for ( int i = 0; i < nRangeCount; i ++ ) {
        QTableWidgetSelectionRange range = list.at(i);
        for ( int iRow = range.topRow(); iRow <= range.bottomRow(); iRow ++ ) {
            reloadFileInfoRowData(iRow);
            QString strFilePath = ui->fileInfoTableWidget->item(iRow, EN_FILE_INFO_COL_INDEX)->data(Qt::UserRole).toString();
            CFileInfo* p = m_sFileInfo.find(strFilePath);
            if ( p == NULL ){
                continue ;
            }
            /**< 文件中若未指定远程路径类型，则不发送 */
            if ( p->m_nRobotPathType == EN_ROBOT_NONE_TYPE ) {
                strError += QString("%1 %2\r\n").arg(p->m_strFileName).arg(tr("未选择机器人路径类型"));
            } else {
                m_listSendingFilePath.append(p->getFilePath());
                emit sigSendFileInfo(p);
            }
        }
    }

    if ( m_listSendingFilePath.count() > 0 ) {
        ui->sendFileBtn->setEnabled(false);
        enabledFileInfoTableWidget(false);
    } else{
        ui->sendFileBtn->setEnabled(true);
    }

    if ( !strError.isEmpty() ) {
        QMessageBox::warning(this, tr("集控软件"), tr("发送不成功,原因:\r\n%1").arg(strError));
    }
}

void CUdpSendFileDialog::slotTableCustomContextMenuRequested(QPoint pos)
{
    if ( m_pMenu == NULL ){
        m_pMenu = new QMenu(this);
    }

    m_pMenu->clear();
    QTableWidgetItem* p = ui->fileInfoTableWidget->itemAt(pos);
    if ( p == NULL ) {
        if ( ui->fileInfoTableWidget->rowCount() > 0 ) {
            m_pMenu->addAction(tr("添加文件"), this, SLOT(slotAddFile()));
            m_pMenu->addAction(tr("添加文件夹"), this, SLOT(slotAddDir()));
            m_pMenu->addSeparator();
            m_pMenu->addAction(tr("全选"), this, SLOT(slotSelAll()));
            m_pMenu->addAction(tr("全不选"), this, SLOT(slotSelNone()));
            m_pMenu->addAction(tr("重新加载"), this, SLOT(slotReloadAllFile()));
            m_pMenu->addAction(tr("清空"), this, SLOT(slotDeleteAllFile()));
        } else {
            m_pMenu->addAction(tr("添加文件"), this, SLOT(slotAddFile()));
            m_pMenu->addAction(tr("添加文件夹"), this, SLOT(slotAddDir()));
        }
    } else {
        m_pMenu->addAction(tr("发送"), this, SLOT(slotMenuSendFile()));
        m_pMenu->addSeparator();
        m_pMenu->addAction(tr("刷新"), this, SLOT(slotReloadFile()));
        m_pMenu->addAction(tr("删除"), this, SLOT(slotDeleteFile()));
    }
    m_pMenu->exec(QCursor::pos());
}

void CUdpSendFileDialog::slotSelAll()
{
    setFileInfoSelAll (true);
}

void CUdpSendFileDialog::slotSelNone ()
{
    setFileInfoSelAll (false);
}

void CUdpSendFileDialog::setFileInfoSelAll (bool bEnable)
{
    Qt::CheckState state = bEnable ? Qt::Checked : Qt::Unchecked;
    QList<QTableWidgetSelectionRange> list = ui->fileInfoTableWidget->selectedRanges();
    int nRangeCount = list.count();
    if ( nRangeCount == 0 ) {
        int nRowCount = ui->fileInfoTableWidget->rowCount();
        for ( int iRow = 0; iRow < nRowCount; iRow ++ ) {
            QTableWidgetItem* p = ui->fileInfoTableWidget->item(iRow, EN_FILE_INFO_COL_INDEX);
            p->setCheckState(state);
        }
    } else {
        for ( int i = 0; i < nRangeCount; i ++ ) {
            QTableWidgetSelectionRange range = list.at(i);
            for ( int iRow = range.topRow(); iRow <= range.bottomRow(); iRow ++ ) {
                QTableWidgetItem* p = ui->fileInfoTableWidget->item(iRow, EN_FILE_INFO_COL_INDEX);
                p->setCheckState(state);
            }
        }
    }
}

void CUdpSendFileDialog::slotAddFile()
{
    QStringList strSelFiles = QFileDialog::getOpenFileNames(this, tr("选择文件"), "",
                                                            QString("resource files (*.ubx *.lua *.zip *.gif *.png *.json *.mp3 *.xml *.apk)"));
    int nCount = strSelFiles.count();
    if ( nCount > 0 ) {
        for( int i = 0; i < nCount; i ++ ) {
            QString strFilePath = strSelFiles.at(i);
            CFileInfo* pFileInfo = new CFileInfo(strFilePath);
            if ( m_sFileInfo.findName(pFileInfo->m_strFileName) != NULL ){
                QMessageBox::warning(this, tr("集控软件"), tr("待传输列表已存在%1").arg(pFileInfo->m_strFileName));
                delete pFileInfo;
                pFileInfo = NULL;
                continue;
            }
            m_sFileInfo.add(pFileInfo);
            addFileInfoRow(pFileInfo);
        }
        enableSend ();
    }
}

void CUdpSendFileDialog::slotAddDir()
{
    QString strSelDirPath = QFileDialog::getExistingDirectory(this, tr("选择文件夹"), "");
    if ( strSelDirPath.isEmpty() ){
        return ;
    }

    CFileInfo* pFileInfo = new CFileInfo(strSelDirPath);
    if ( m_sFileInfo.findName(pFileInfo->m_strFileName) ){
        QMessageBox::warning(this, tr("集控软件"), tr("待传输列表已存在%1").arg(pFileInfo->m_strFileName));
        delete pFileInfo;
        pFileInfo = NULL;
        return;
    }
    m_sFileInfo.add(pFileInfo);
    addFileInfoRow(pFileInfo);
    enableSend ();
}

void CUdpSendFileDialog::slotDeleteFile()
{
    QList<QTableWidgetSelectionRange> list = ui->fileInfoTableWidget->selectedRanges();
    int nRangeCount = list.count();
    for( int i = 0; i < nRangeCount; i ++ ) {
        QTableWidgetSelectionRange range = list.at(i);
        for ( int iRow = range.topRow(); iRow <= range.bottomRow(); iRow ++ ) {
            QTableWidgetItem* p = ui->fileInfoTableWidget->item(iRow, EN_FILE_INFO_COL_INDEX);
            QString strFilePath = p->data(Qt::UserRole).toString();
            m_sFileInfo.remove(strFilePath);
            ui->fileInfoTableWidget->removeRow(iRow);
        }
    }

    enableSend ();
}

void CUdpSendFileDialog::slotDeleteAllFile()
{
    while( ui->fileInfoTableWidget->rowCount() > 0 )
        ui->fileInfoTableWidget->removeRow(0);

    m_sFileInfo.clear();
    enableSend ();
}

void CUdpSendFileDialog::slotReloadFile()
{
    QList<QTableWidgetSelectionRange> list = ui->fileInfoTableWidget->selectedRanges();
    int nRangeCount = list.count();
    for( int i = 0; i < nRangeCount; i ++ ) {
        QTableWidgetSelectionRange range = list.at(i);
        for ( int iRow = range.topRow(); iRow <= range.bottomRow(); iRow ++ ) {
            QTableWidgetItem* p = ui->fileInfoTableWidget->item(iRow, EN_FILE_INFO_COL_INDEX);
            QString strFilePath = p->data(Qt::UserRole).toString();
            m_sFileInfo.update(strFilePath);
            updateFileInfoTableWidget (iRow);
        }
    }
}

void CUdpSendFileDialog::slotReloadAllFile()
{
    int nRowCount = ui->fileInfoTableWidget->rowCount();
    for ( int i = 0; i < nRowCount; i ++ )  {
        QString strFilePath = ui->fileInfoTableWidget->item(i, EN_FILE_INFO_COL_INDEX)->data(Qt::UserRole).toString();
        m_sFileInfo.update(strFilePath);
        updateFileInfoTableWidget (i);
    }
}

void CUdpSendFileDialog::slotSendBytes(QString strPath, int nSize)
{
    int nRowCount = ui->fileInfoTableWidget->rowCount();
    for ( int i = 0; i < nRowCount; i ++ ) {
        QString strFilePath = ui->fileInfoTableWidget->item(i, EN_FILE_INFO_COL_INDEX)->data(Qt::UserRole).toString();
        if ( strFilePath != strPath ){
            continue;
        }

        QProgressBar* pBar = qobject_cast<QProgressBar*>(ui->fileInfoTableWidget->cellWidget(i, 6));
        if ( !pBar ) {
            continue;
        }

        pBar->setValue(pBar->value() + nSize);
    }
}

void CUdpSendFileDialog::slotSendOver(QString strPath, bool bState)
{
    int nRowCount = ui->fileInfoTableWidget->rowCount();
    for ( int i = 0; i < nRowCount; i ++ ) {
        QString strFilePath = ui->fileInfoTableWidget->item(i, EN_FILE_INFO_COL_INDEX)->data(Qt::UserRole).toString();
        if ( strFilePath != strPath ){
            continue;
        }

        m_listSendingFilePath.removeOne(strFilePath);
        QString strState = bState ? tr("完成") : tr("未完成");
        ui->fileInfoTableWidget->item(i, EN_FILE_INFO_COL_STATE)->setText(strState);
    }

    if ( m_listSendingFilePath.count() == 0 ){
        ui->sendFileBtn->setEnabled(true);
        enabledFileInfoTableWidget(true);
    } else {
        ui->sendFileBtn->setEnabled(false);
    }
}

void CUdpSendFileDialog::addFileInfoRow (CFileInfo* pFileInfo)
{
    if ( pFileInfo == NULL ) {
        return ;
    }

    int nRowCount = ui->fileInfoTableWidget->rowCount();
    ui->fileInfoTableWidget->insertRow(nRowCount);

    QString strNum = QString::number( nRowCount + 1);
    QTableWidgetItem* qtableItem = new QTableWidgetItem(strNum);
    qtableItem->setTextAlignment(Qt::AlignCenter);
    qtableItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    Qt::CheckState checkState = pFileInfo->getSendEnable() ? Qt::Checked : Qt::Unchecked;
    qtableItem->setCheckState(checkState);
    qtableItem->setData(Qt::UserRole, (QVariant)pFileInfo->getFilePath());
    ui->fileInfoTableWidget->setItem(nRowCount, EN_FILE_INFO_COL_INDEX, qtableItem);

    qtableItem = new QTableWidgetItem(pFileInfo->m_strFileName);
    qtableItem->setTextAlignment(Qt::AlignCenter);
    qtableItem->setFlags(qtableItem->flags() & ~Qt::ItemIsEditable);
    ui->fileInfoTableWidget->setItem(nRowCount, EN_FILE_INFO_COL_NAME, qtableItem);

    QString strSize = QString::number(pFileInfo->m_nFileSize / 1024.0, 'f', 2);
    qtableItem = new QTableWidgetItem(strSize);
    qtableItem->setTextAlignment(Qt::AlignCenter);
    qtableItem->setFlags(qtableItem->flags() & ~Qt::ItemIsEditable);
    ui->fileInfoTableWidget->setItem(nRowCount, EN_FILE_INFO_COL_SIZE, qtableItem);

    qtableItem = new QTableWidgetItem(pFileInfo->md5ToHex());
    qtableItem->setTextAlignment(Qt::AlignCenter);
    qtableItem->setFlags(qtableItem->flags() & ~Qt::ItemIsEditable);
    ui->fileInfoTableWidget->setItem(nRowCount, EN_FILE_INFO_COL_MD5, qtableItem);

    QComboBox* pCombox = new QComboBox(this);
    addRobotTypeCombox (pCombox);
    pCombox->setCurrentText(translate(pFileInfo->m_nRobotPathType));
    ui->fileInfoTableWidget->setCellWidget(nRowCount, EN_FILE_INFO_COL_TYPE, pCombox);

    qtableItem = new QTableWidgetItem(pFileInfo->m_strLocalPath);
    qtableItem->setTextAlignment(Qt::AlignCenter);
    qtableItem->setFlags(qtableItem->flags() & ~Qt::ItemIsEditable);
    ui->fileInfoTableWidget->setItem(nRowCount, EN_FILE_INFO_COL_PATH, qtableItem);

    QProgressBar* pBar = new QProgressBar(this);
    pBar->setRange(0, pFileInfo->m_nFileSize);
    pBar->setValue(0);
    pBar->setVisible(false);
    ui->fileInfoTableWidget->setCellWidget(nRowCount, EN_FILE_INFO_COL_PROGRESS, pBar);

    qtableItem = new QTableWidgetItem("");
    qtableItem->setTextAlignment(Qt::AlignCenter);
    qtableItem->setFlags(qtableItem->flags() & ~Qt::ItemIsEditable);
    ui->fileInfoTableWidget->setItem(nRowCount, EN_FILE_INFO_COL_STATE, qtableItem);
}

void CUdpSendFileDialog::updateFileInfoTableWidget (int nRow)
{
    // 紧刷新文件大小和MD5码
    QString strFilePath = ui->fileInfoTableWidget->item(nRow, EN_FILE_INFO_COL_INDEX)->data(Qt::UserRole).toString();
    CFileInfo* p = m_sFileInfo.find(strFilePath);
    if ( !p ) return ;

    QString strSize = QString::number(p->m_nFileSize / 1024.0, 'f', 2);
    ui->fileInfoTableWidget->item(nRow, EN_FILE_INFO_COL_SIZE)->setText(strSize);
    ui->fileInfoTableWidget->item(nRow, EN_FILE_INFO_COL_MD5)->setText(p->md5ToHex());
}

void CUdpSendFileDialog::reloadFileInfoData ()
{
    int nRow = ui->fileInfoTableWidget->rowCount();
    for ( int i = 0; i < nRow; i ++ ) {
        reloadFileInfoRowData (i);
    }
}

void CUdpSendFileDialog::reloadFileInfoRowData (int iRow)
{
    QComboBox* pCombox = qobject_cast<QComboBox*>(ui->fileInfoTableWidget->cellWidget(iRow, EN_FILE_INFO_COL_TYPE));
    if ( !pCombox ) {
        return;
    }
    QString strFilePath = ui->fileInfoTableWidget->item(iRow, EN_FILE_INFO_COL_INDEX)->data(Qt::UserRole).toString();
    CFileInfo* p = m_sFileInfo.find(strFilePath);
    if ( p == NULL ){
        return ;
    }

    QProgressBar* pBar = qobject_cast<QProgressBar*>(ui->fileInfoTableWidget->cellWidget(iRow, EN_FILE_INFO_COL_PROGRESS));
    if ( !pBar ) {
        return;
    }

    QTableWidgetItem* pItem = ui->fileInfoTableWidget->item(iRow, EN_FILE_INFO_COL_INDEX);
    Qt::CheckState check = pItem->checkState();
    bool bEnable = check == Qt::Checked ? true : false;
    p->setSendEnable(bEnable);
    p->m_nRobotPathType = pCombox->currentData(Qt::UserRole).toInt();
    pBar->setValue(0);
    ui->fileInfoTableWidget->item(iRow, EN_FILE_INFO_COL_STATE)->setText("");
}

QString  CUdpSendFileDialog::translate (int nRobotFileType)
{
    QString strTypeName = "";
    switch (nRobotFileType) {
    case EN_ROBOT_NONE_TYPE:
        strTypeName = QString("Unkown");
        break;
    case EN_ROBOT_ACTIONS_TYPE:
        strTypeName = QString("actions");
        break;
    case EN_ROBOT_EXPRESS_TYPE:
        strTypeName = QString("express");
        break;
    case EN_ROBOT_BEHAVIORS_TYPE:
        strTypeName = QString("behaviors");
        break;
    case EN_ROBOT_SCENES_TYPE:
        strTypeName = QString("scenes");
        break;
    case EN_ROBOT_SCRIPTS_TYPE:
        strTypeName = QString("scripts");
        break;
    case EN_ROBOT_LOCALTTS_TYPE:
        strTypeName = QString("localTTs");
        break;
    case EN_ROBOT_MUSIC_TYPE:
        strTypeName = QString("music");
        break;
    case EN_ROBOT_APK_TYPE:
        strTypeName = QString("apk");
        break;
    case EN_ROBOT_LOACATION_TYPE:
        strTypeName = QString("Location");
        break;
    default:
        break;
    }

    return strTypeName;
}

void CUdpSendFileDialog::addRobotTypeCombox (QComboBox* pCombox)
{
    if ( !pCombox ) return;
    pCombox->addItem(translate(EN_ROBOT_NONE_TYPE), EN_ROBOT_NONE_TYPE);
    pCombox->addItem(translate(EN_ROBOT_ACTIONS_TYPE), EN_ROBOT_ACTIONS_TYPE);
    pCombox->addItem(translate(EN_ROBOT_EXPRESS_TYPE), EN_ROBOT_EXPRESS_TYPE);
    pCombox->addItem(translate(EN_ROBOT_BEHAVIORS_TYPE), EN_ROBOT_BEHAVIORS_TYPE);
    pCombox->addItem(translate(EN_ROBOT_SCENES_TYPE), EN_ROBOT_SCENES_TYPE);
    pCombox->addItem(translate(EN_ROBOT_SCRIPTS_TYPE), EN_ROBOT_SCRIPTS_TYPE);
    pCombox->addItem(translate(EN_ROBOT_LOCALTTS_TYPE), EN_ROBOT_LOCALTTS_TYPE);
    pCombox->addItem(translate(EN_ROBOT_MUSIC_TYPE), EN_ROBOT_MUSIC_TYPE);
    pCombox->addItem(translate(EN_ROBOT_APK_TYPE), EN_ROBOT_APK_TYPE);
    pCombox->addItem(translate(EN_ROBOT_LOACATION_TYPE), EN_ROBOT_LOACATION_TYPE);
}

void CUdpSendFileDialog::enableSend ()
{
    if ( m_sFileInfo.count() > 0 ){
        ui->sendFileBtn->setEnabled(true);
    } else {
        ui->sendFileBtn->setEnabled(false);
    }
}

void CUdpSendFileDialog::closeEvent(QCloseEvent *event)
{
    if ( m_listSendingFilePath.count() > 0 ){
        int nBtnNum = QMessageBox::warning(this,
                                           tr("集控软件"),
                                           tr("是否取消正在发送的文件?"),
                                           tr("是"),
                                           tr("否"),
                                           tr(""),
                                           1,
                                           1);
        if ( nBtnNum == 1 ) {
            event->ignore();
            return ;
        }
    }

    m_pUsbComm->setSendCancel(true);
    event->accept();
}

void CUdpSendFileDialog::enabledFileInfoTableWidget (bool bEnable)
{
    int nRowCount = ui->fileInfoTableWidget->rowCount();
    for ( int i = 0; i < nRowCount; i ++ ) {
        enabledFileInfoTableWidget (i, bEnable);
    }

    if ( m_pMenu ){
        m_pMenu->setEnabled(bEnable);
    }
}

void CUdpSendFileDialog::enabledFileInfoTableWidget(int nRow, bool bEnable)
{
    int nRowCount = ui->fileInfoTableWidget->rowCount();
    if ( nRow < 0 || nRow > nRowCount - 1 ){
        return ;
    }

    QComboBox* pCombox = qobject_cast<QComboBox*>(ui->fileInfoTableWidget->cellWidget(nRow, EN_FILE_INFO_COL_TYPE));
    if ( !pCombox ) {
        return;
    }

    pCombox->setEnabled(bEnable);
}
