#include "OptionsEarLedWidget.h"
#include "ui_OptionsEarLedWidget.h"
#include "LayerUI/ActionBlockDataManager.h"
#include "dialogcommandbox.h"
#include <QMessageBox>
#include <QMenu>
#include <QTimer>
#include <QMouseEvent>
#include <QClipboard>
#include <QMimeData>
#include "baseutil.h"

bool CreateEarsLayOptionHandler(ILayerOptionHandler** ppHandler)
{
    COptionsEarLedWidget * pProxy = new COptionsEarLedWidget();
    if(pProxy)
    {
        *ppHandler = pProxy;
        return true;
    }
    return false;
}

COptionsEarLedWidget::COptionsEarLedWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::COptionsEarLedWidget),
    m_pGroupListWidget(NULL),
    m_pBlockListWidget(NULL)
{
    ui->setupUi(this);
    m_pGroupListWidget = ui->m_ListActionGroup;
    m_pBlockListWidget = ui->m_ListActionBlock;

    m_Left = 0;		// 左耳灯
    m_Right = 0;    // 右耳灯
    m_Bright = 9;
}

COptionsEarLedWidget::~COptionsEarLedWidget()
{
    delete ui;
}

bool COptionsEarLedWidget::Connect(QWidget *pParent, ILayerOptionHandler_Event *pEvent)
{
    setParent(pParent);
    QLayout* layout = pParent->layout();
    layout->addWidget(this);
    initWidget();
    m_pEvent = pEvent;
    return true;
}

void COptionsEarLedWidget::ReleaseConnection()
{
    disconnect();
    m_ShowId.clear();
    m_nCurrentGroupListSelectItem = -1;

    m_nCurrentGroupID = -1;
    m_ShowId.clear();
    deleteLater();
}

void COptionsEarLedWidget::UpdateEarLedUI()
{
    // 左眼LED
    ui->widgetLedsLeft->setLedsSettings(m_Left);

    // 右眼LED
    ui->widgetLedsRight->setLedsSettings(m_Right);

    // 亮度
    ui->combo_bright->setCurrentIndex(m_Bright);

    //更新灯亮时间灯灭时间
    ui->edit_led_on->setText(QString::number(m_LedLight));
    ui->edit_led_off->setText(QString::number(m_LedPutOut));
}

void COptionsEarLedWidget::initWidget()
{
//    QRegExp regx("[1-9]{1}[0-9]{1,5}");
    QRegExp regx("[0-9]{1,5}");
    QValidator *validatorRunTime = new QRegExpValidator(regx, ui->edit_exec_time);
    ui->edit_exec_time->setValidator(validatorRunTime);

    QValidator *validatorStopTime = new QRegExpValidator(regx, ui->edit_stop_time);
    ui->edit_stop_time->setValidator(validatorStopTime);

    ui->edit_exec_time->setText("10");
    ui->edit_stop_time->setText("10");

    connect(ui->widgetLedsLeft, &CLedsWidget::sigButtonToggled, this, &COptionsEarLedWidget::onLeftLedsBtnToggled);
    connect(ui->widgetLedsRight, &CLedsWidget::sigButtonToggled, this, &COptionsEarLedWidget::onRightLedsBtnToggled);
    connect(ui->pbAdd, &QPushButton::clicked, this, &COptionsEarLedWidget::onPbAddclicked);
    connect(ui->pbInsert, &QPushButton::clicked, this, &COptionsEarLedWidget::onPbInsertclicked);
    connect(ui->pbModify, &QPushButton::clicked, this, &COptionsEarLedWidget::onPbModifyclicked);
    for (int i = 0; i < LED_LIGTH_MAX_BRIGHT_LEVEL + 1; i++)
    {
        QString strBright = QString::number(i);
        ui->combo_bright->addItem(strBright);
    }
    QString strCurBright = QString::number(LED_LIGTH_MAX_BRIGHT_LEVEL);
    ui->combo_bright->setCurrentText(strCurBright);

    QString strDescription = "DefaultName0";

    ui->lEditEarDescription->setText(strDescription);
    ui->edit_led_on->setText("0");
    ui->edit_led_off->setText("0");

    this->hide();
}

/**************************************************************************
* 函数名：UpdateBlockOptionUI
* 功能:更新描述具体的Block信息的UI
* 参数:
*    @[in ] pBlockData: Block 信息
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void COptionsEarLedWidget::UpdateBlockOptionUI(CActionBlockData *pBlockData)
{
    if (pBlockData == NULL)
    {
        return;
    }
    QString strRunTime = QString::number(pBlockData->GetRunTime());
    ui->edit_exec_time->setText(strRunTime);

    QString strStopTime  = QString::number(pBlockData->GetStopTime());
    ui->edit_stop_time->setText(strStopTime);

    QString strDestription = QString::fromStdWString(pBlockData->GetDescription());
    //QString strDestription = QString::fromUtf16((WCHAR *)pBlockData->GetDescription());
    ui->lEditEarDescription->setText(strDestription);

    char* pData = NULL;
    int nLen = 0;
    pBlockData->GetExtBlockData(&pData, nLen);

    SetRawData(pData, nLen);
    //根据数据显示灯的内容
    UpdateEarLedUI();
    SAFE_DELETE_ARRAY(pData);
}

void COptionsEarLedWidget::SetCurrentSelectGroupItem(QListWidgetItem * item, int nIndex)
{
    if(m_pGroupListWidget == NULL)
    {
        return;
    }
    if(m_nCurrentGroupListSelectItem !=-1)
    {
        m_pGroupListWidget->setItemSelected(item, true);//此处Windows版加了图标
    }
    if(nIndex == -1)
    {
        if(m_pBlockListWidget->winId())
        {
            m_pBlockListWidget->clear();
        }
        return;
    }

    m_pGroupListWidget->setItemSelected(item,true);
    m_nCurrentGroupListSelectItem = nIndex;
    if(m_pGroupManager)
    {
        CActionBlockGroupManager* pGroup = m_pGroupManager->GetAt(nIndex);
        if(pGroup)
        {
            m_nCurrentGroupID = pGroup->GetID();
            UpdataBlockListByGroupManager(pGroup);
        }
    }
}

void COptionsEarLedWidget::UpdataBlockListByGroupManager(CActionBlockGroupManager *pGroup)
{
    if(m_pBlockListWidget == NULL)
        return;
    m_pBlockListWidget->clear();

    for(int i = 0; i<pGroup->GetActionCount(); i++)
    {
        CActionBlockData* pBlockData = pGroup->GetAt(i);
        if(pBlockData)
        {
            QString strDestription = QString::fromStdWString(pBlockData->GetDescription());
            //QString strDestription = QString::fromUtf16((WCHAR *)pBlockData->GetDescription());
            QListWidgetItem * pItem = new QListWidgetItem(strDestription);
            m_pBlockListWidget->insertItem(i,pItem);

            pItem->setData(ActionBlockDataID, pBlockData->GetID());
            pItem->setSelected(true);
            m_pBlockListWidget->show();
        }
    }

    // Modify by jianjie 2017/8/22 组切换更新块数据
    m_pBlockListWidget->setCurrentRow(0);
    QListWidgetItem* pItem = m_pBlockListWidget->item(0);
    if (NULL == pItem)
    {
        return;
    }

    int nSelectBlockID = pItem->data(ActionBlockDataID).toInt();

    CActionBlockData* pActionBlockData = pGroup->FindBlockData(nSelectBlockID);
    if (pActionBlockData == NULL)
        return;
    UpdateBlockOptionUI(pActionBlockData);
}

QWidget *COptionsEarLedWidget::GetWidget()
{
    return this;
}

void COptionsEarLedWidget::SetActionBlockDataManager(CActionBlockDataManager* pManager, int nCurrentGroupID,int nCurrentActionID, int nOffset)
{
    if (nCurrentGroupID == 0)
    {
        m_nCurrentGroupID = nCurrentGroupID;
        m_pGroupManager = pManager;
    }

    m_nCurrentGroupID = nCurrentGroupID;
    m_pGroupManager = pManager;

    if (nCurrentGroupID == -1)
    {
        m_nCurrentGroupListSelectItem = -1;
        m_ShowId.clear();
    }

    if (g_IsModifyGroup || g_IsUpdateUIByData)
    {
        UpdateUIByData();
    }

    if(m_pGroupListWidget == NULL)
    {
        return;
    }
    else
    {
        QListWidgetItem * pItem = NULL;
        for (int i=0; i<m_pGroupListWidget->count(); i++)
        {
            pItem= m_pGroupListWidget->item(i);
            if(pItem == NULL)
            {
                qDebug()<<"can't find pItem point"<<endl;
                break;
            }
            QVariant value = pItem->data(GroupID);
            int nValue = value.toInt();
            if (nValue == nCurrentGroupID)
            {
                m_nCurrentGroupListSelectItem = i;
                break;
            }
        }
    }

    if (g_IsModifyGroup || g_IsUpdateUIByData)
    {

        QListWidgetItem * item =m_pGroupListWidget->item(m_nCurrentGroupListSelectItem);
        SetCurrentSelectGroupItem(item,m_nCurrentGroupListSelectItem);
        g_IsUpdateUIByData = false;
        g_IsModifyGroup  = false;
    }

    if (nCurrentActionID != -1 && m_nCurrentGroupListSelectItem != -1)
    {
        int nActionID = 0;
        if (pManager)
        {
            CActionBlockData *pData = pManager->GetAt(m_nCurrentGroupListSelectItem)->GetAt(nCurrentActionID);
            if (pData)
            {
                nActionID = pData->GetID();
                SetShowID(&nActionID,1);
                ShowSelectRow(nCurrentActionID);
                UpdateBlockOptionUI(pData);
            }
        }
    }
}

//void COptionsEarLedWidget::Set3DRobotViewInterface(IUBXRobot3DView *pRobotView)
//{
//    m_pRobotView = pRobotView;
//}

void COptionsEarLedWidget::SetControlState(bool controlState, int nLayerType)
{
    emit SigSetControlState(controlState, nLayerType);
}

void COptionsEarLedWidget::InsertGroupByMenu(int nCol)
{
}

int COptionsEarLedWidget::GetCurrenGroupManagerID()
{
    if (m_nCurrentGroupListSelectItem == -1)
        return -1;

    if (m_pGroupManager == NULL)
        return -1;

    CActionBlockGroupManager* pGroupManager = m_pGroupManager->GetAt(m_nCurrentGroupListSelectItem);
    if (pGroupManager)
    {
        return pGroupManager->GetID();
    }

    return -1;
}

/**************************************************************************
* 函数名: SetShowID
* 功能:   保存需要显示的ID
* 参数:
* 返回值:
* 时间:   2015/12/25 16:36
* 作者: 周志平
**************************************************************************/
void COptionsEarLedWidget::SetShowID(int *pID, int nCount)
{
    m_ShowId.clear();

    for(int i=0; i<nCount; i++)
    {
        m_ShowId.push_back(pID[i]);
    }
}

void COptionsEarLedWidget::ShowSelectRow(int row)
{
    if(m_pBlockListWidget)
    {
        m_pBlockListWidget->setCurrentRow(row);
    }
}

void COptionsEarLedWidget::UpdateUIByData()
{
    if(m_pGroupManager ==NULL)
    {
        return;
    }
    if(m_pGroupListWidget ==NULL)
    {
        return;
    }
    if((m_pGroupListWidget->count() == m_pGroupManager->GetCount()) && !g_IsModifyGroup)
    {
        m_pGroupListWidget->setItemSelected(m_pGroupListWidget->item(m_nCurrentGroupListSelectItem),true);
        return;
    }
    m_pGroupListWidget->clear();
    for(int i = 0;i<m_pGroupManager->GetCount();i++)
    {
        CActionBlockGroupManager* pGroup = m_pGroupManager->GetAt(i);
        QString strDescription = QString::fromStdWString(pGroup->GetDescription());
        //QString strDestcription = QString::fromUtf16((WCHAR *)pGroup->GetDescription());
        QListWidgetItem * pItem = new QListWidgetItem(strDescription);
        m_pGroupListWidget->insertItem(i,pItem);
        int nID = pGroup->GetID();
        pItem->setData(GroupID,nID);
    }
    m_pGroupListWidget->show();

    m_pEvent->ILayerOptionHandler_HasMotionData(m_pGroupListWidget->count() > 0, ACTION_LAYER_EAR);

    //    if(m_pGroupListWidget->count() > 0)
    //    {
    //        ui->btnUnitedSimulation->setEnabled(true);
    //        ui->btnStop->setEnabled(true);
    //        ui->pbInsert->setEnabled(true);
    //        ui->pbModify->setEnabled(true);
    //    }
    //    else
    //    {
    //        ui->btnUnitedSimulation->setEnabled(false);
    //        ui->btnStop->setEnabled(false);
    //        ui->pbInsert->setEnabled(false);
    //        ui->pbModify->setEnabled(false);
    //    }
}

void COptionsEarLedWidget::onPbAddclicked()
{
    AdjustDialogInfo();
    if (!m_pEvent)
    {
        return;
    }

    int nRunTime = ui->edit_exec_time->text().toInt();
    int nStopTime = ui->edit_stop_time->text().toInt();

    // 运行时间需大于或等于灯亮和灯灭时间之和
    if (nRunTime < m_LedLight + m_LedPutOut)
    {
        QMessageBox::warning(NULL, "", QObject::tr("Running time must be greater than the light time and light off time!"), QObject::tr("OK"));
        return;
    }

    CActionBlockData* pActionBlock = new CActionBlockData();
    if (pActionBlock == NULL)
        return;

    int nDefaultNameNum = m_pBlockListWidget->count();
    QString strNameNum = QString::number(nDefaultNameNum, 10);
    QString strDescription = "DefaultName" + strNameNum;

    std::wstring wsDescription = strDescription.toStdWString();
    pActionBlock->SetDescription((WCHAR *)wsDescription.c_str());

    pActionBlock->SetID(-1);
    pActionBlock->SetRunTime(nRunTime/* / nScaleTime*/);
    pActionBlock->SetStopTime(nStopTime/* / nScaleTime*/);

    char* pData = NULL;
    int nLen = 0;
    nLen = GetRawData(&pData);
    if (nLen && pData)
    {
        pActionBlock->SetExtBlockData(nLen, pData);
    }

    int nCurID = GetCurrenGroupManagerID();
    m_pEvent->ILayerOptionHandler_OnAddActionBlock(nCurID, pActionBlock, "", true);


    //        if (nCurID != -1)
    //        {
    //            int nItem = m_earBlockList.InsertItem(m_earBlockList.GetItemCount(), tempDescriptionInfo, -1);
    //            m_earBlockList.SetItemData(nItem, pActionBlock->GetID());
    //            SetCurSelectBlockItem(nItem);
    //        }

    CActionBlockGroupManager* pGroup = m_pGroupManager->GetAt(m_nCurrentGroupListSelectItem);
    if (pGroup)
    {
        UpdataBlockListByGroupManager(pGroup);
    }

    delete [] pData;
}

void COptionsEarLedWidget::onPbModifyclicked()
{
    AdjustDialogInfo();
    if(m_pBlockListWidget == NULL || m_pGroupManager == NULL)
        return;

    CActionBlockGroupManager* pGroupManager = m_pGroupManager->FindBlockData(m_nCurrentGroupID);
    if(pGroupManager == NULL)
        return;

    int nRunTime = ui->edit_exec_time->text().toInt();
    int nStopTime = ui->edit_stop_time->text().toInt();

    if (nRunTime < m_LedLight + m_LedPutOut)
    {
        QMessageBox::warning(NULL, "", QObject::tr("Running time must be greater than the light time and light off time!"), QObject::tr("OK"));
        return;
    }

    int nIndex = m_pBlockListWidget->currentRow();

    //    QListWidgetItem* pItem = m_pBlockListWidget->selectedIndexes()
    //    int nSelectBlockID = pItem->data(ActionBlockDataID).toInt();

    CActionBlockData* pActionBlock = pGroupManager->GetAt(nIndex);
    if (pActionBlock == NULL)
        return;

    // 帧时间修改
    int nTimeInc = (nRunTime + nStopTime) - (pActionBlock->GetRunTime() + pActionBlock->GetStopTime());

    // 保存修改数据
    WCHAR wstrDescription[MAX_DESCRIPTION_SIZE];
    memset(wstrDescription,0,MAX_DESCRIPTION_SIZE*sizeof(WCHAR));
    QString strDescription = ui->lEditEarDescription->text();
    if (!strDescription.isEmpty())
    {
        std::wstring wsDescription = strDescription.toStdWString();
        pActionBlock->SetDescription((WCHAR *)wsDescription.c_str());
        m_pBlockListWidget->item(nIndex)->setText(strDescription);
    }
    
    pActionBlock->SetRunTime(nRunTime);
    pActionBlock->SetStopTime(nStopTime);

    char* pData = NULL;
    int nLen = 0;
    nLen = GetRawData(&pData);
    if (nLen && pData)
    {
        pActionBlock->SetExtBlockData(nLen, pData);
    }

    if(m_pEvent)
    {
        m_pEvent->ILayerOptionHandler_OnActionBlockDataChange(m_nCurrentGroupID, pActionBlock->GetID(), nTimeInc);
    }

    delete [] pData;
}

void COptionsEarLedWidget::onPbInsertclicked()
{
    AdjustDialogInfo();
    if (m_pGroupManager == NULL || m_pBlockListWidget == NULL)
        return;

    CActionBlockGroupManager* pGroupManager = m_pGroupManager->FindBlockData(m_nCurrentGroupID);
    if(pGroupManager == NULL)
        return;

    int nCurRow = m_pBlockListWidget->currentRow();
    if (nCurRow == -1)
    {
        return;
    }
    QListWidgetItem* pItem = m_pBlockListWidget->item(nCurRow);
    int nSelectBlockID = pItem->data(ActionBlockDataID).toInt();

    int nRunTime = ui->edit_exec_time->text().toInt();
    int nStopTime = ui->edit_stop_time->text().toInt();

    // 运行时间需大于或等于灯亮和灯灭时间之和
    if (nRunTime < m_LedLight + m_LedPutOut)
    {
        QMessageBox::warning(NULL, "", QObject::tr("Running time must be greater than the light time and light off time!"), QObject::tr("OK"));
        return;
    }

//    int nScaleTime = m_pEvent->ILayerOptionHandler_GetScaleValue();//时间单位

    CActionBlockData* pActionBlockData = new CActionBlockData();
    pActionBlockData->SetID(pGroupManager->GetUnUserID());
    int nDefaultNameNum = m_pBlockListWidget->count();
    QString strNameNum = QString::number(nDefaultNameNum, 10);
    QString strDescription = "DefaultName" + strNameNum;
    std::wstring wsDescription = strDescription.toStdWString();
    pActionBlockData->SetDescription((WCHAR *)wsDescription.c_str());
    pActionBlockData->SetRunTime(nRunTime/* / nScaleTime*/);
    pActionBlockData->SetStopTime(nStopTime/* / nScaleTime*/);

    char* pData = NULL;
    int nLen = 0;
    nLen = GetRawData(&pData);
    if (nLen && pData)
    {
        pActionBlockData->SetExtBlockData(nLen, pData);
    }
    pGroupManager->InsertBeforeId(nSelectBlockID, pActionBlockData);

    int nCurrentShowID = pActionBlockData->GetID();
    SetShowID(&nCurrentShowID, 1);
    int nCurrentID = m_nCurrentGroupID;
    m_nCurrentGroupID = -1;
    g_IsUpdateUIByData = true;
    m_pEvent->ILayerOptionHandler_OnGroupManagerChange(nCurrentID, pActionBlockData->GetRunTime() + pActionBlockData->GetStopTime(), true);
    g_IsUpdateUIByData = false;

    delete[] pData;
    //    ShowSelectRow(nCurrentShowID);
}

void COptionsEarLedWidget::onGroupListDelete()
{
    //    QListWidgetItem* pItem = m_pGroupListWidget->currentItem();
    int nIndex = m_pGroupListWidget->currentRow();
    if(nIndex == -1)
    {
        return;
    }
    else
    {
        //        int nIndex = pItem->data(Qt::DecorationRole).toInt();
        CActionBlockGroupManager* pGroup = m_pGroupManager->GetAt(nIndex);
        if(pGroup == NULL)
        {
            qDebug()<<tr("new pGroup Fail in COptionsMotionWidget class")<<endl;
            return;
        }
        int nStartTime = pGroup->GetStartTime();
        m_pEvent->ILayerOptionHandler_SetSelectGroupBlock(nStartTime + 2);
    }
    m_pEvent->ILayerOptionHandler_OnDeleteGroupBlock();
    m_pEvent->ILayerOptionHandler_HasMotionData(m_pGroupListWidget->count() > 0, ACTION_LAYER_EAR);

    //    if(m_pGroupListWidget->count() > 0)
    //    {
    //        ui->btnUnitedSimulation->setEnabled(true);
    //        ui->btnStop->setEnabled(true);
    //        ui->pbInsert->setEnabled(true);
    //        ui->pbModify->setEnabled(true);
    //    }
    //    else
    //    {
    //        ui->btnUnitedSimulation->setEnabled(false);
    //        ui->btnStop->setEnabled(false);
    //        ui->pbInsert->setEnabled(false);
    //        ui->pbModify->setEnabled(false);
    //    }
}

/**************************************************************************
* 函数名:onGroupModifyName
* 功能:修改耳朵组名
* 参数:
* 返回值:
* 时间:2016/01/24 20:36
* 作者: 万向峰
* 改动日志
**************************************************************************/
void COptionsEarLedWidget::onGroupModifyName()
{
    DialogCommandBox dlg;
    QFile styleSheet(":/res/qss/motiondefault.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        dlg.setStyleSheet(styleSheet.readAll());
    }
    if (dlg.exec() == QDialog::Accepted)
    {
        QString strDescription = dlg.m_strCommandBoxName;
        if (strDescription.isEmpty())
        {
            return;
        }

        std::wstring wsDescription = strDescription.toStdWString();
        CActionBlockGroupManager* pGroup = m_pGroupManager->FindBlockData(GetCurrenGroupManagerID());
        if(pGroup)
        {
            pGroup->SetDescription((WCHAR *)wsDescription.c_str());
        }
        if (m_pEvent)
        {
            m_pEvent->ILayerOptionHandler_OnGroupManagerChange(m_nCurrentGroupID, 0, true);
        }

        g_IsModifyGroup = true;
        UpdateUIByData();
    }
}

/**************************************************************************
* 函数名:onGroupListCopy
* 功能:耳朵组拷贝
* 参数:
* 返回值:
* 时间:2016/01/24 20:36
* 作者: 万向峰
* 改动日志
**************************************************************************/
void COptionsEarLedWidget::onGroupListCopy()
{
    m_pEvent->ILayerOptionHandler_CopyGroup();
}

/**************************************************************************
* 函数名:onGroupListPaste
* 功能:耳朵组粘贴
* 参数:
* 返回值:
* 时间:2016/01/24 20:36
* 作者: 万向峰
* 改动日志
**************************************************************************/
void COptionsEarLedWidget::onGroupListPaste()
{
    CActionBlockGroupManager* pGroup = m_pGroupManager->FindBlockData(GetCurrenGroupManagerID());
    if(pGroup)
    {
        int nStartTime = pGroup->GetStartTime();
        m_pEvent->ILayerOptionHandler_PasteGroup(nStartTime + 1);
    }
}

/**************************************************************************
* 函数名：on_m_ListActionGroup_itemPressed
* 功能: 在耳朵灯组列表中选中行，进行鼠标操作
* 参数:
*    @[in ] item: 选中行数据项
* 返回值: 无
* 时间: 2017/3/27
* 作者: wanxf
*/
void COptionsEarLedWidget::on_m_ListActionGroup_itemPressed(QListWidgetItem *item)
{
    if(qApp->mouseButtons() == Qt::LeftButton)
    {
        int nRow = m_pGroupListWidget->row(item);
        if(nRow != -1 )
        {
            SetCurrentSelectGroupItem(item,nRow);
            if(m_pEvent)
            {
                m_pEvent->ILayerOptionHandler_OnSelectGroupManageChange(m_nCurrentGroupID,0);
            }
        }
    }
    else if(qApp->mouseButtons() == Qt::RightButton) //右键
    {
        if(item==Q_NULLPTR)
        {
            return;
        }
        else
        {
            int nRow = m_pGroupListWidget->row(item);
            if(nRow != -1 )
            {
                SetCurrentSelectGroupItem(item,nRow);
                if(m_pEvent)
                {
                    m_pEvent->ILayerOptionHandler_OnSelectGroupManageChange(m_nCurrentGroupID,0);
                }
            }

            CreateGroupListPopMenuActions();
        }
    }
}

/***********************************************************************
 * 函数名： createPopMenuActions
 * 功能：  创建弹出菜单
 * 返回值： void
 * 作者：  周志平
 ***********************************************************************/
void COptionsEarLedWidget::CreateGroupListPopMenuActions()
{
    //创建菜单、菜单项
    QMenu* pop_menu = new QMenu();
    QAction* DeleteAction = new QAction(tr("Delete"),this);
    QAction* ModifyAction = new QAction(tr("Modify group name"),this);
    QAction* CopyAction = new QAction(tr("Copy"),this);
    QAction* PasteAction = new QAction(tr("Paste"),this);

    pop_menu->addAction(DeleteAction);
    pop_menu->addAction(ModifyAction);
    pop_menu->addAction(CopyAction);
    pop_menu->addAction(PasteAction);
    QAbstractItemDelegate::connect(DeleteAction,&QAction::triggered,this,&COptionsEarLedWidget::onGroupListDelete);
    QAbstractItemDelegate::connect(ModifyAction,&QAction::triggered,this,&COptionsEarLedWidget::onGroupModifyName);
    QAbstractItemDelegate::connect(CopyAction,&QAction::triggered,this,&COptionsEarLedWidget::onGroupListCopy);
    QAbstractItemDelegate::connect(PasteAction,&QAction::triggered,this,&COptionsEarLedWidget::onGroupListPaste);
    //菜单出现的位置为当前鼠标的位置
    QFile styleSheet(":/res/qss/menudefault.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        pop_menu->setStyleSheet(styleSheet.readAll());
    }
    pop_menu->exec(QCursor::pos());
    pop_menu->deleteLater();
    //连接信号与槽
}

void COptionsEarLedWidget::AdjustDialogInfo()
{
    QString lightTime;
    QString turnOffTime;
    lightTime = ui->edit_led_on->text();
    turnOffTime = ui->edit_led_off->text();

    m_LedLight = lightTime.toInt();
    m_LedPutOut = turnOffTime.toInt();

    // 左耳LED
    m_Left = 0;
    m_Left = ui->widgetLedsLeft->getLedsSettings();

    // 右耳LED
    m_Right = 0;
    m_Right = ui->widgetLedsRight->getLedsSettings();

    int nCurIndex = ui->combo_bright->currentIndex();
    m_Bright = ui->combo_bright->itemText(nCurIndex).toInt();
}

void COptionsEarLedWidget::SetRawData(char* pData, int nLen)
{
    // 数据长度（INT） + DATA
    int nTotalLen = 0;
    UBX_MEMCPY_INC(nTotalLen, pData);
    if (nTotalLen != nLen)
        return;

    // 左眼LED
    UBX_MEMCPY_INC(m_Left, pData);
    // 右眼LED
    UBX_MEMCPY_INC(m_Right, pData);
    // 亮度
    UBX_MEMCPY_INC(m_Bright, pData);
    // 亮的时间
    UBX_MEMCPY_INC(m_LedLight, pData);
    // 灭的时间
    UBX_MEMCPY_INC(m_LedPutOut, pData);
}

int COptionsEarLedWidget::GetRawData(char** ppData)
{
    int nLen = 0;
    vector<char> DataSave;

    // 数据长度（INT） + DATA
    AddVectorData(&DataSave, (char*)&nLen, sizeof(nLen));
    // 左眼LED
    AddVectorData(&DataSave, (char*)&m_Left, sizeof(m_Left));
    // 右眼LED
    AddVectorData(&DataSave, (char*)&m_Right, sizeof(m_Right));
    // 亮度
    AddVectorData(&DataSave, (char*)&m_Bright, sizeof(m_Bright));
    // 亮的时间
    AddVectorData(&DataSave, (char*)&m_LedLight, sizeof(m_LedLight));
    // 灭的时间
    AddVectorData(&DataSave, (char*)&m_LedPutOut, sizeof(m_LedPutOut));

    nLen = DataSave.size();
    ModifyVertorData(DataSave, 0, (char*)&nLen, sizeof(nLen));
    if (nLen)
    {
        *ppData = new char[nLen];
        CopyVertorToPointer(DataSave, *ppData);
    }

    return nLen;
}

void COptionsEarLedWidget::onLeftLedsBtnToggled(int id, bool bChecked)
{
    if(bChecked)
    {
        ui->widgetLedsLeft->setBackgroundColor(id, eBlueColor);
    }
    else
    {
        ui->widgetLedsLeft->setBackgroundColor(id, eBlackColor);
    }
}

void COptionsEarLedWidget::onRightLedsBtnToggled(int id, bool bChecked)
{
    if(bChecked)
    {
        ui->widgetLedsRight->setBackgroundColor(id, eBlueColor);
    }
    else
    {
        ui->widgetLedsRight->setBackgroundColor(id, eBlackColor);
    }
}

void COptionsEarLedWidget::ImitationEarLed(int nRunTime, char leftLed, char rightLed, char bright, int nLedLight, int nLedOff)
{
    m_Left      = leftLed;
    m_Right     = rightLed;
    m_Bright    = bright;
    m_LedLight  = nLedLight;
    m_LedPutOut = nLedOff;

    ui->edit_exec_time->setText(QString::number(nRunTime));
    UpdateEarLedUI();
}

void COptionsEarLedWidget::onDeleteBlock()
{
    int nCurRow = m_pBlockListWidget->currentRow();
    QListWidgetItem* pItem = m_pBlockListWidget->item(nCurRow);
    int nSelectBlockID = pItem->data(ActionBlockDataID).toInt();
    if (nSelectBlockID == -1)
        return;

    int nTimeInc = 0;
    CActionBlockGroupManager* pGroup = m_pGroupManager->FindBlockData(m_nCurrentGroupID);
    if (pGroup == NULL)
        return;

    CActionBlockData* pActionBlockData = pGroup->FindBlockData(nSelectBlockID);
    if (pActionBlockData == NULL)
        return;

    nTimeInc += (pActionBlockData->GetRunTime()+pActionBlockData->GetStopTime());
    pGroup->DeleteLayerBlock(nSelectBlockID);

    g_IsUpdateUIByData = true;
    g_IsUpdateUIByData = false;
    m_pEvent->ILayerOptionHandler_OnGroupManagerChange(m_nCurrentGroupID, -nTimeInc, true);

    if (pGroup->GetActionCount() < 1) //判断是否还有数据
    {
        m_pEvent->ILayerOptionHandler_OnDeleteGroupBlock();
    } else {
        UpdataBlockListByGroupManager(pGroup);
    }
}

/**************************************************************************
* 函数名：on_m_ListActionBlock_itemPressed
* 功能: 在耳朵灯块列表中选中行，进行鼠标操作
* 参数:
*    @[in ] item: 选中行数据项
* 返回值: 无
* 时间: 2017/3/27
* 作者: wanxf
*/
void COptionsEarLedWidget::on_m_ListActionBlock_itemPressed(QListWidgetItem *item)
{
    if(qApp->mouseButtons() == Qt::LeftButton)
    {
        int nRow = m_pBlockListWidget->row(item);
        if(nRow != -1 )
        {
            if(m_pEvent)
            {
                m_pEvent->ILayerOptionHandler_OnSelectActionManageChange(m_nCurrentGroupID, nRow);
            }

            int nSelectBlockID = item->data(ActionBlockDataID).toInt();
            CActionBlockGroupManager* pGroup = m_pGroupManager->FindBlockData(m_nCurrentGroupID);
            if (pGroup == NULL)
                return;

            CActionBlockData* pActionBlockData = pGroup->FindBlockData(nSelectBlockID);
            if (pActionBlockData == NULL)
                return;
            UpdateBlockOptionUI(pActionBlockData);
        }
    }
    else if(qApp->mouseButtons() == Qt::RightButton) //右键
    {
        if(item==Q_NULLPTR)
        {
            return;
        }
        else
        {
            int nRow = m_pBlockListWidget->row(item);
            if(nRow != -1 )
            {
//                SetCurrentSelectGroupItem(item,nRow);
                if(m_pEvent)
                {
                    m_pEvent->ILayerOptionHandler_OnSelectActionManageChange(m_nCurrentGroupID, nRow);
                }
            }

            QMenu* pop_menu = new QMenu();
            QAction* DeleteAction = new QAction(tr("Delete"),this);
            pop_menu->addAction(DeleteAction);
            QAbstractItemDelegate::connect(DeleteAction, &QAction::triggered, this,&COptionsEarLedWidget::onDeleteBlock);

            //菜单出现的位置为当前鼠标的位置
            QFile styleSheet(":/res/qss/menudefault.qss");
            if(styleSheet.open(QIODevice::ReadOnly))
            {
                pop_menu->setStyleSheet(styleSheet.readAll());
            }
            pop_menu->exec(QCursor::pos());
            pop_menu->deleteLater();
        }
    }
}
