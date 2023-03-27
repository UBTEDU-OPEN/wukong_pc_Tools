#include "assignactdev.h"
#include "ui_assignactdev.h"
#include "titlewidget.h"
#include "cfgini.h"
#include "pubdef.h"
#include <QMessageBox>

CAssignActDev::CAssignActDev(QWidget *parent) :
    XDialog(parent),
    ui(new Ui::CAssignActDev)
{
    ui->setupUi(this);
    init ();
}

CAssignActDev::~CAssignActDev()
{
    delete ui;
}

void CAssignActDev::init ()
{
    initVar ();
    initWindows ();
    initAction ();
    layout ();
    setStyle(QString(":/qss/qss/udpsendfile.qss"));
}

void CAssignActDev::initVar ()
{
    m_pTitleWidget = NULL;
    m_listSerialInfo.clear();
    loadSerialInfo ();
}

void CAssignActDev::createTitleWidget ()
{
    m_pTitleWidget = new CTitleWidget(this);
    m_pTitleWidget->setMenuBtnVisible(false);
    m_pTitleWidget->setLogoVisible(false);
}

void CAssignActDev::initWindows ()
{
    createTitleWidget ();
    initSerialNumTableWidget ();
    refreshSerialNumTableWidget ();
}

void CAssignActDev::initAction ()
{
    connect(ui->saveBtn, SIGNAL(pressed()),
            this, SLOT(slotSave()));
    connect(ui->closewinBtn, SIGNAL(pressed()),
            this, SLOT(slotClose()));
}

void CAssignActDev::initSerialNumTableWidget ()
{
    setTableStyle (ui->tableWidget, 35, 40, true);
    clearContentsTable (ui->tableWidget);

    QStringList header;
    header << tr("序号")
           << tr("序列号");
    int nCol = header.count();
    ui->tableWidget->setColumnCount(nCol);

    // 设置列宽
    ui->tableWidget->setColumnWidth(0, 100);
    ui->tableWidget->setColumnWidth(1, 200);
    ui->tableWidget->setHorizontalHeaderLabels(header);

    // 表头文字居中
    for ( int i = 0; i < nCol; i ++ )
    {
        ui->tableWidget->horizontalHeaderItem(i)->setTextAlignment(Qt::AlignCenter);
    }
}

void CAssignActDev::refreshSerialNumTableWidget ()
{
    QTableWidgetItem* qtableItem = NULL;
    for ( int i = 0; i < ASSGIN_ACTIVE_DEVICE_NUM_MAX; i ++ )
    {
        int nRowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(nRowCount);

        QString strNum = QString::number( i + 1);
        qtableItem = new QTableWidgetItem(strNum);
        qtableItem->setFlags(qtableItem->flags() & ~Qt::ItemIsEditable);
        qtableItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 0, qtableItem);

        QString strSerialNum = "";
        if ( i < m_listSerialInfo.count() )
        {
            strSerialNum = m_listSerialInfo.at(i);
        }

        qtableItem = new QTableWidgetItem(strSerialNum);
        qtableItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 1, qtableItem);
    }
}

void CAssignActDev::layout()
{
    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(m_pTitleWidget);
    pVLayout->addWidget(ui->mainWidget);
    pVLayout->setContentsMargins(QMargins(0,0,0,0));
    this->setLayout(pVLayout);
}

void CAssignActDev::loadSerialInfo ()
{
    QVariant var;
    CCfgIni::getInstance().readItem(KEY_SERIALSETTING_SERIAL, var);
    QString strSerialInfo = var.toString();
    if ( !strSerialInfo.isEmpty() ) {
        m_listSerialInfo = strSerialInfo.split('#', QString::SkipEmptyParts);
    }
}

void CAssignActDev::slotSave ()
{
    m_listSerialInfo.clear();

    int nCount = ui->tableWidget->rowCount();
    for ( int i = 0; i < nCount; i ++ ) {
        QTableWidgetItem* pItem = ui->tableWidget->item(i, 1);
        QString strText = pItem->text();
        if ( strText.isEmpty() ){
            continue;
        }
        // 暂不考虑增加SN码的校验机制
        m_listSerialInfo.append(strText);
    }

    QString strSerialInfo = "";
    for ( int i = 0; i < m_listSerialInfo.count(); i ++ ) {
        if ( i == m_listSerialInfo.count() - 1 ) {
            strSerialInfo += m_listSerialInfo.at(i);
        } else {
            strSerialInfo += m_listSerialInfo.at(i) + "#";
        }
    }

    bool bRet = CCfgIni::getInstance().writeItem(KEY_SERIALSETTING_SERIAL, QVariant(strSerialInfo));
    QString strTip = bRet ? tr("保存成功") : tr("保存失败");
    QMessageBox::warning(this, tr("集控软件"), strTip);
}

void CAssignActDev::slotClose ()
{
    close();
}
