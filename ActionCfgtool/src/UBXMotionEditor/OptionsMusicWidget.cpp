#include "OptionsMusicWidget.h"
#include "ui_OptionsMusicWidget.h"
#include "util.h"
#include "fileutils.h"
#include <QFileDialog>
#include <QResource>
#include <QMessageBox>
#include <QMenu>
#include "formatfunc.h"
#include "configs.h"

bool CreateMusicLayOptionHandler(ILayerOptionHandler** ppHandler)
{
    COptionsMusicWidget * pProxy = new COptionsMusicWidget();
    if(pProxy)
    {
        *ppHandler = pProxy;
        return true;
    }
    return false;
}

COptionsMusicWidget::COptionsMusicWidget(QWidget *parent) :
    QWidget(parent)
  , ui(new Ui::COptionsMusicWidget)
  , m_nCurrentGroupListSelectItem(-1)
  , m_pGroupManager(NULL)
  , m_pGroupListWidget(NULL)
  , m_pBlockListWidget(NULL)
  , m_strMusicTitle("")
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    //    m_pMp3Container = /*new CUILayerCommonContainer(this);*/NULL;
    //    ui->horizontalLayout_mp3->addWidget(m_pMp3Container);
    m_pEvent = NULL;
    //    m_pCUILayerCommonWidget = /*m_pMp3Container->GetCUILayerCommonWidget();*/NULL;
    m_bMotionPlaying = false;
    m_bEarLedPlaying = false;
    m_bEyeLedPlaying = false;
    m_bMusicPlaying = false;
    m_bMusicPause = false;
    //    connect(this, &COptionsMusicWidget::SigMotionPlayStop, this, &COptionsMusicWidget::OnMotionPlayStop);
    //    connect(m_pCUILayerCommonWidget, &CUILayerCommonWidget::SigFinishMusicPlaying, this, &COptionsMusicWidget::OnFinishMusicPlaying);
    //    connect(this, &COptionsMusicWidget::SigSetControlState, this, &COptionsMusicWidget::OnSetControlState);
    //    connect(m_pCUILayerCommonWidget, &CUILayerCommonWidget::SigNotifyReleasedMusic, this, &COptionsMusicWidget::OnReleaseMusic);
    //    connect(m_pCUILayerCommonWidget, &CUILayerCommonWidget::SigNotifyLoadedMusic, this, &COptionsMusicWidget::OnLoadedMusic);

    //    ui->verticalLayout->setAlignment(ui->btnAddMusic, Qt::AlignHCenter);
    //    ui->verticalLayout->setAlignment(ui->btnDeleteMusic, Qt::AlignHCenter);
    //    ui->pauseMP3->hide();
    //    ui->pauseMotion->hide();
    //    ui->pauseEarLed->hide();
    //    ui->pauseEyeLed->hide();

    //    connect(ui->pauseMP3, &QPushButton::clicked, this, &COptionsMusicWidget::on_playMP3_clicked);
    //    connect(ui->pauseMotion, &QPushButton::clicked, this, &COptionsMusicWidget::on_playMotion_clicked);

    //    ui->btnDeleteMusic->setEnabled(false);
    //    ui->playMP3->setEnabled(false);
    //    ui->stopMP3->setEnabled(false);
    //    ui->playMotion->setEnabled(false);
    //    ui->stopMotion->setEnabled(false);

    //    ui->playEarLed->setEnabled(false);
    //    ui->stopEarLed->setEnabled(false);
    //    ui->playEyeLed->setEnabled(false);
    //    ui->stopEyeLed->setEnabled(false);

    m_bHasMotionData = false;
    initWidget();
}

COptionsMusicWidget::~COptionsMusicWidget()
{
    delete ui;
}

/**************************************************************************
* 函数名：initWidget
* 功能:初始化窗体
* 参数: 无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void COptionsMusicWidget::initWidget()
{
    ui->lEPlayTime->setText("0");
    ui->lEStopTime->setText("0");
    connect(ui->pbChoosePath, &QPushButton::clicked, this, &COptionsMusicWidget::onPbChoosePathClicked);
    connect(ui->pbAdd, &QPushButton::clicked, this, &COptionsMusicWidget::onPbAddClicked);
    connect(ui->pbInsert, &QPushButton::clicked, this, &COptionsMusicWidget::onPbInsertClicked);
    connect(ui->pbModify, &QPushButton::clicked, this, &COptionsMusicWidget::onPbModifyClicked);
    ui->pbAdd->setEnabled(false);
    ui->pbInsert->setEnabled(false);
    ui->pbModify->setEnabled(false);
    hide();
    m_pGroupListWidget = ui->m_ListActionGroup;
    m_pBlockListWidget = ui->m_ListActionBlock;
}

bool COptionsMusicWidget::Connect(QWidget *pParent, ILayerOptionHandler_Event* pEvent)
{
    setParent(pParent);
    QLayout* layout = pParent->layout();
    layout->addWidget(this);
    //    m_pCUILayerCommonWidget->Connect(pParent, pEvent);
    //    connect(m_pCUILayerCommonWidget, &CUILayerCommonWidget::SigLButtonDoubleClick, this, &COptionsMusicWidget::OnLButtonDoubleClick);
    m_pEvent = pEvent;

    //    QString rccPath = getAppSubDirPath(SUBDIR_RES) + QDir::separator() + "mp3proj.rcc";
    //    QResource::registerResource(rccPath);
    //    setDefaultStyle();

    return true;
}

void COptionsMusicWidget::ReleaseConnection()
{
    disconnect();
//    delete this;
    m_nCurrentGroupListSelectItem = -1;
    deleteLater();
}

QWidget *COptionsMusicWidget::GetWidget()
{
    return this;
}

/**************************************************************************
* 函数名：SetActionBlockDataManager
* 功能:设置动作数据，当动作组发生变化时，通过该接口更新数据
* 参数:
*    @[in ] pManager: 需要更新的动作组
*    @[in ] nCurrentGroupID: 动作组ID
*    @[in ] pManager: 动作帧ID
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void COptionsMusicWidget::SetActionBlockDataManager(CActionBlockDataManager *pManager, int nCurrentGroupID, int nCurrentActionID, int nOffset)
{
    if (NULL == pManager)
    {
        return;
    }

    m_nCurrentGroupID = nCurrentGroupID;
    m_pGroupManager = pManager;

    if (nCurrentGroupID == -1)
    {
        m_nCurrentGroupListSelectItem = -1;
//        m_ShowId.clear();
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
        SetCurrentSelectItem(item,m_nCurrentGroupListSelectItem);
        g_IsUpdateUIByData = false;
        g_IsModifyGroup  = false;
    }

    if (nCurrentActionID != -1 && m_nCurrentGroupListSelectItem != -1)
    {
        CActionBlockGroupManager* pGroup = pManager->GetAt(m_nCurrentGroupListSelectItem);
        if (pGroup)
        {
            CActionBlockData *pBlockData = pGroup->GetAt(nCurrentActionID);
            if (pBlockData)
            {
                ShowSelectRow(nCurrentActionID);
                UpdateBlockOptionUI(pBlockData);
            }
        }
    }
}

/**************************************************************************
* 函数名：UpdateUIByData
* 功能:根据Data更新动作组列表
* 参数: 无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void COptionsMusicWidget::UpdateUIByData()
{
    if(m_pGroupManager ==NULL)
    {
        return;
    }
    if(m_pGroupListWidget ==NULL)
    {
        return;
    }
    int nCout = m_pGroupManager->GetCount();
    int nCount = m_pGroupListWidget->count();
    if((m_pGroupListWidget->count() == m_pGroupManager->GetCount()) && !g_IsModifyGroup)
    {
        m_pGroupListWidget->setItemSelected(m_pGroupListWidget->item(m_nCurrentGroupListSelectItem),true);
        return;
    }
    m_pGroupListWidget->clear();
    for(int i = 0;i<m_pGroupManager->GetCount();i++)
    {
        CActionBlockGroupManager* pGroup = m_pGroupManager->GetAt(i);
        //QString strDescription = WCHARToQString(pGroup->GetDescription(), MAX_DESCRIPTION_SIZE*sizeof(WCHAR));
        QString strDescription = QString::fromStdWString(pGroup->GetDescription());
        //QString strDescription = QString::fromUtf16((WCHAR *)pGroup->GetDescription());
        QListWidgetItem * pItem = new QListWidgetItem(strDescription);
        m_pGroupListWidget->insertItem(i,pItem);
        int nID = pGroup->GetID();
        pItem->setData(GroupID,nID);
    }
    m_pGroupListWidget->show();

    //m_pEvent->ILayerOptionHandler_HasMotionData(m_pGroupListWidget->count() > 0, ACTION_LAYER_EAR);
}

/**************************************************************************
* 函数名：SetCurrentSelectItem
* 功能:设置当前动作组动作帧列表
* 参数:
*    @[in ] item: 动作组item
*    @[in ] nIndex: 动作组item索引
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void COptionsMusicWidget::SetCurrentSelectItem(QListWidgetItem * item, int nIndex)
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
            UpdataActionByGroupManager(pGroup);
        }
    }
}

/**************************************************************************
* 函数名：UpdataActionByGroupManager
* 功能:动作组发生变化时，需要更新动作帧数据
* 参数:
*    @[in ] pGroup: 需要更新的动作组
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void COptionsMusicWidget::UpdataActionByGroupManager(CActionBlockGroupManager *pGroup)
{
    if(m_pBlockListWidget == NULL)
        return;

    m_pBlockListWidget->clear();

    for(int i = 0; i<pGroup->GetActionCount(); i++)
    {
        CActionBlockData* pBlockData = pGroup->GetAt(i);
        if(pBlockData)
        {
            char* pData = NULL;
            int nLen = 0;
            pBlockData->GetExtBlockData(&pData, nLen);

            QString strDestription = QString::fromStdWString(pBlockData->GetDescription());
            //QString strDescription = QString::fromUtf16((WCHAR *)pBlockData->GetDescription());
            QListWidgetItem * pItem = new QListWidgetItem(strDestription);
            m_pBlockListWidget->insertItem(i,pItem);

            pItem->setData(ActionBlockDataID, pBlockData->GetID());
            pItem->setSelected(true);
            //           m_pBlockListWidget->setItemSelected(pItem, true);
            m_pBlockListWidget->show();

            delete[] pData;
        }
    }
}

 // 显示选中的ID
void COptionsMusicWidget::ShowSelectRow(int row)
{
    if(m_pBlockListWidget)
    {
        m_pBlockListWidget->setCurrentRow(row);
    }
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
void COptionsMusicWidget::UpdateBlockOptionUI(CActionBlockData *pBlockData)
{
    if (pBlockData == NULL)
    {
        return;
    }
    QString strRunTime = QString::number(pBlockData->GetRunTime());
    ui->lEPlayTime->setText(strRunTime);

    QString strStopTime  = QString::number(pBlockData->GetStopTime());
    ui->lEStopTime->setText(strStopTime);

    char* pData = NULL;
    int nLen = 0;
    pBlockData->GetExtBlockData(&pData, nLen);
    SetRawData(pData, nLen);
    //leo modified 20170320
    //ui->lEMusicPath->setText(m_strEditFilePath);
    QString qsMusicName = FileUtils::getFileName(m_strEditFilePath);
    ui->lEMusicPath->setText(qsMusicName);

    SAFE_DELETE_ARRAY(pData);
}



bool COptionsMusicWidget::IsMusicLoaded()
{
    //    return m_pCUILayerCommonWidget->IsMusicLoaded();
    return true;
}


void COptionsMusicWidget::SetControlState(bool controlState, int nLayerType)
{
    //    emit SigSetControlState(controlState, nLayerType);
}

void COptionsMusicWidget::InsertGroupByMenu(int nCol)
{

}

/**************************************************************************
* 函数名：onPbAddClicked
* 功能:添加音乐动作
* 参数: 无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void COptionsMusicWidget::onPbAddClicked()
{
    int nRunTime = ui->lEPlayTime->text().toInt();
    int nStopTime = ui->lEStopTime->text().toInt();
    QString strFilePathName = ui->lEMusicPath->text();
    if (strFilePathName.isEmpty())
    {
        return;
    }

    if (!m_pEvent)
    {
        return;
    }

    CActionBlockData* pActionBlockData = new CActionBlockData();
    if (!pActionBlockData)
    {
        //AddLog( "new Memory fail");
        return;
    }
//    int nScaleTime = m_pEvent->ILayerOptionHandler_GetScaleValue();
    pActionBlockData->SetID(-1);
    pActionBlockData->SetRunTime(nRunTime /*/ nScaleTime*/);
    pActionBlockData->SetStopTime(nStopTime/*/ nScaleTime*/);

    //leo add 20170313 解决windows下copy路径问题 20170313
    // QString strFileName = CUtil::GetShortFileName(strFilePathName);
    QString strFileName = FileUtils::getFileName(strFilePathName);

    //WCHAR wstrDescription[MAX_DESCRIPTION_SIZE];
    //memset(wstrDescription, 0, MAX_DESCRIPTION_SIZE*sizeof(WCHAR));
    //int nDescriptionLen = QStringToWCHAR(wstrDescription, strFileName);
    std::wstring wstrDescription= strFileName.toStdWString();
    pActionBlockData->SetDescription((WCHAR *)(wstrDescription.c_str()));

    char* pData = NULL;
    int nLen = 0;
    nLen = GetRawData(&pData);
    if (nLen && pData)
    {
        pActionBlockData->SetExtBlockData(nLen, pData);
    }

    SAFE_DELETE_ARRAY(pData);

    int nCurrentID = GetCurrenGroupManagerID(); //需要调试
    m_pEvent->ILayerOptionHandler_OnAddActionBlock(nCurrentID, pActionBlockData, "", true);


    CopyMusicFileTo(strFilePathName, strFileName);

    //这里只添加数据，不拷贝文件
    if (nCurrentID != -1)
    {
        QListWidgetItem * pItem = new QListWidgetItem(strFileName);
        m_pBlockListWidget->insertItem(m_pBlockListWidget->count(), pItem);
        pItem->setData(ActionBlockDataID, pActionBlockData->GetID());
        pItem->setSelected(true);
    }
    //if(m_pGroupManager)
    //{
    //    CActionBlockGroupManager* pGroup = m_pGroupManager->GetAt(m_nCurrentGroupID);
    //    if (pGroup)
    //    {
    //        UpdateBlockList(pGroup);
    //    }
    //}
}


void COptionsMusicWidget::onPbInsertClicked()
{
    int nRunTime = ui->lEPlayTime->text().toInt();
    int nStopTime = ui->lEStopTime->text().toInt();

    QString strFilePathName = ui->lEMusicPath->text();
    if (strFilePathName.isEmpty())
    {
        return;
    }

    if (!m_pEvent)
    {
        return;
    }

    if (m_pBlockListWidget->count() <= 0)
    {
        return;
    }

    int nCurRow = m_pBlockListWidget->currentRow();
    QListWidgetItem* pItem = m_pBlockListWidget->item(nCurRow);
    if (NULL == pItem)
    {
        return;
    }
    int nSelectBlockID = pItem->data(ActionBlockDataID).toInt();
    CActionBlockGroupManager* pGroup = m_pGroupManager->FindBlockData(m_nCurrentGroupID);
    if (pGroup)
    {
        CActionBlockData* pActionBlockData = new CActionBlockData();
        if (!pActionBlockData)
        {
            //AddLog( "new Memory fail");
            return;
        }
        pActionBlockData->SetID(-1);
        pActionBlockData->SetRunTime(nRunTime);
        pActionBlockData->SetStopTime(nStopTime);
        QString strFileName = FileUtils::getFileName(strFilePathName);
        std::wstring wstrDescription = strFileName.toStdWString();
        pActionBlockData->SetDescription((WCHAR *)wstrDescription.c_str());
        char* pData = NULL;
        int nLen = 0;
        nLen = GetRawData(&pData);
        if (nLen && pData)
        {
            pActionBlockData->SetExtBlockData(nLen, pData);
        }

        SAFE_DELETE_ARRAY(pData);

        pGroup->InsertBeforeId(nSelectBlockID, pActionBlockData);

        int nCurrentID = m_nCurrentGroupID; //需要调试
        m_pEvent->ILayerOptionHandler_OnGroupManagerChange(nCurrentID,  pActionBlockData->GetRunTime() + pActionBlockData->GetStopTime(), true);
        //这里只添加数据，不拷贝文件

        QListWidgetItem * pNewItem = new QListWidgetItem(strFileName);
        m_pBlockListWidget->insertItem(nCurRow, pNewItem);
        pNewItem->setData(ActionBlockDataID, pActionBlockData->GetID());
        pNewItem->setSelected(true);

        //if(m_pGroupManager)
        //{
        //    CActionBlockGroupManager* pGroup = m_pGroupManager->GetAt(m_nCurrentGroupListSelectItem);
        //    if (pGroup)
        //    {
        //        UpdateBlockList(pGroup);
        //    }
        //}
    }
}

/**************************************************************************
* 函数名：onPbModifyClicked
* 功能: 插入音乐动作帧
* 参数: 无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void COptionsMusicWidget::onPbModifyClicked()
{
    int nRunTime = ui->lEPlayTime->text().toInt();
    int nStopTime = ui->lEStopTime->text().toInt();

    QString strFilePathName = ui->lEMusicPath->text();
    if (strFilePathName.isEmpty())
    {
        return;
    }

    if (!m_pEvent)
    {
        return;
    }

    if (m_pBlockListWidget->count() <= 0)
    {
        return;
    }

    int nCurRow = m_pBlockListWidget->currentRow();
    QListWidgetItem* pItem = m_pBlockListWidget->item(nCurRow);
    if (NULL == pItem)
    {
        return;
    }
    int nSelectBlockID = pItem->data(ActionBlockDataID).toInt();
    CActionBlockGroupManager* pGroup = m_pGroupManager->GetAt(m_nCurrentGroupID);
    if (pGroup)
    {
        CActionBlockData* pActionBlockData = pGroup->FindBlockData(nSelectBlockID);
        if (!pActionBlockData)
        {
            //AddLog( "can' find ActionBlockData!");
            return;
        }

        pActionBlockData->SetRunTime(nRunTime);
        pActionBlockData->SetStopTime(nStopTime);
        QString strFileName = FileUtils::getFileName(strFilePathName);
        std::wstring wsFileName = strFilePathName.toStdWString();
        pActionBlockData->SetDescription((WCHAR *)wsFileName.c_str());

        char* pData = NULL;
        int nLen = 0;
        nLen = GetRawData(&pData);
        if (nLen && pData)
        {
            pActionBlockData->SetExtBlockData(nLen, pData);
        }

        SAFE_DELETE_ARRAY(pData);

        int nCurrentID = m_nCurrentGroupID; //需要调试
        m_pEvent->ILayerOptionHandler_OnGroupManagerChange(nCurrentID,  pActionBlockData->GetRunTime() + pActionBlockData->GetStopTime(), true);
        //这里只添加数据，不拷贝文件
    }
}

/**************************************************************************
* 函数名：onPbChoosePathClicked
* 功能:选择音乐路径按钮
* 参数: 无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void COptionsMusicWidget::onPbChoosePathClicked()
{
//    int nMusicLegth = 0;
    QString sMusicFilePath = QFileDialog::getOpenFileName(this, tr("Choose a music file"), ".", tr("Music File(*.mp3);;Music File(*.mp3)"));
    if(!sMusicFilePath.isEmpty())
    {
        if(m_pEvent)
        {
            if (!m_pEvent->ILayerOptionHandler_LoadMusic(sMusicFilePath))
            {
                // 音乐格式不合格，加载失败
                return;
            }
        }
        //        m_pCUILayerCommonWidget->LoadMusic(sMusicFilePath);
        //        emit m_pCUILayerCommonWidget->SigReDrawAll();
        m_strEditFilePath = sMusicFilePath;
        ui->pbAdd->setEnabled(true);
        ui->pbInsert->setEnabled(true);
        ui->pbModify->setEnabled(true);
        //leo modified 20170320
        //ui->lEMusicPath->setText(sMusicFilePath);
        QString qsMusicName = FileUtils::getFileName(sMusicFilePath);
        ui->lEMusicPath->setText(sMusicFilePath);
    }
}

//void COptionsMusicWidget::OnMotionPlayStop()
//{
//    if(!m_bMusicPlaying)//直接动作仿真结束和全局仿真但音乐没有加载时都会进入，此时需要通知Motion模块更新按钮状态
//    {
//        //ui->playMotion->setText(tr("Simulate"));
//        ui->pauseMotion->hide();
//        ui->playMotion->show();
//        ui->btnAddMusic->setEnabled(true);
//        if(m_pCUILayerCommonWidget->IsMusicLoaded())
//        {
//            ui->btnDeleteMusic->setEnabled(true);
//            ui->playMP3->setEnabled(true);
//            ui->stopMP3->setEnabled(true);
//        }
//        if(m_bHasMotionData)
//        {
//            ui->playMotion->setEnabled(true);
//            ui->stopMotion->setEnabled(true);
//        }
//    }
//    m_bMotionPlaying = false;
//    m_pCUILayerCommonWidget->SetSinglePlaying(false);
//    m_pCUILayerCommonWidget->SetPlaying(false);
//}

//void COptionsMusicWidget::OnSetControlState(bool controlState, int nLayerType)
//{
//    if(controlState)
//    {
//        ui->btnAddMusic->setEnabled(true);
//        if(m_pCUILayerCommonWidget->IsMusicLoaded())
//        {
//            ui->btnDeleteMusic->setEnabled(true);
//            ui->playMP3->setEnabled(true);
//            ui->stopMP3->setEnabled(true);
//        }
//        ui->pauseMotion->hide();
//        ui->playMotion->show();

//        //wanxf 06-09-19 add
//        ui->pauseEarLed->hide();
//        ui->playEarLed->show();
//        ui->pauseEyeLed->hide();
//        ui->playEyeLed->show();
//        //add end

//        if(m_bHasMotionData)
//        {
//            ui->playMotion->setEnabled(true);
//            ui->stopMotion->setEnabled(true);
//        }
//        m_bMotionPlaying = false;
//        m_bMusicPlaying = false;

//        //wanxf 06-09-19 add
//        m_bEarLedPlaying = false;
//        m_bEyeLedPlaying = false;
//        //add end

//        m_pCUILayerCommonWidget->SetSinglePlaying(false);
//        m_pCUILayerCommonWidget->SetPlaying(false);
//    }
//    else
//    {
//        switch(nLayerType)
//        {
//        case 1:
//        {
//            //ui->playMotion->setEnabled(false);
//            //ui->stopMotion->setEnabled(false);
//            m_bMotionPlaying = true;
//            m_pCUILayerCommonWidget->SetSinglePlaying(true);
//            m_pCUILayerCommonWidget->SetPlaying(false);
//            ui->pauseMotion->show();
//            ui->playMotion->hide();
//        }
//            break;
//        case 4:
//        {
//            //ui->playMP3->setEnabled(false);
//            //ui->stopMP3->setEnabled(false);
//        }
//            break;
//        case 5:
//        {
//            ui->btnAddMusic->setEnabled(false);
//            ui->btnDeleteMusic->setEnabled(false);
//            ui->playMotion->setEnabled(false);
//            ui->playMP3->setEnabled(false);
//            ui->stopMotion->setEnabled(false);
//            ui->stopMP3->setEnabled(false);
//            m_pCUILayerCommonWidget->SetSinglePlaying(false);
//            m_pCUILayerCommonWidget->SetPlaying(true);
//            m_bMotionPlaying = true;
//        }
//            break;
//        }
//    }
//}

//void COptionsMusicWidget::OnReleaseMusic()
//{
//    if(!m_pCUILayerCommonWidget->IsMusicLoaded())
//    {
//        ui->btnDeleteMusic->setEnabled(false);
//        ui->playMP3->setEnabled(false);
//        ui->stopMP3->setEnabled(false);
//    }
//}

void COptionsMusicWidget::OnLoadedMusic()
{
    //    if(m_pCUILayerCommonWidget->IsMusicLoaded())
    //    {
    //        ui->btnDeleteMusic->setEnabled(true);
    //        ui->playMP3->setEnabled(true);
    //        ui->stopMP3->setEnabled(true);
    //    }
}

//void COptionsMusicWidget::OnFinishMusicPlaying()
//{
//    m_bMusicPlaying = false;
//    ui->btnAddMusic->setEnabled(true);
//    ui->pauseMP3->hide();
//    ui->playMP3->show();
//    if(m_pCUILayerCommonWidget->IsMusicLoaded())
//    {
//        ui->btnDeleteMusic->setEnabled(true);
//        ui->playMP3->setEnabled(true);
//        ui->stopMP3->setEnabled(true);
//    }
//    if(m_bHasMotionData && !m_bMotionPlaying)
//    {
//        ui->playMotion->setEnabled(true);
//        ui->stopMotion->setEnabled(true);
//    }
//    if(m_pEvent)
//    {
//        m_pEvent->ILayerOptinHandler_FinishMusicPlaying();
//    }
//}
//void COptionsMusicWidget::OnLButtonDoubleClick(int nCol)
//{
//    if(m_pEvent)
//    {
//        m_pEvent->ILayerOptinHandler_OnLButtonDBClick(nCol);
//        //ui->playMotion->setEnabled(false);
//        //ui->stopMotion->setEnabled(false);
//    }
//}

//void COptionsMusicWidget::on_btnAddMusic_clicked()
//{
//    QString sMusicFile = QFileDialog::getOpenFileName(this, tr("Choose a music file"), ".", tr("Music File(*.mp3);;Music File(*.mp3)"));
//    if(!sMusicFile.isEmpty())
//    {
//        m_pCUILayerCommonWidget->LoadMusic(sMusicFile);
//        emit m_pCUILayerCommonWidget->SigReDrawAll();
//    }
//}

//void COptionsMusicWidget::on_playMP3_clicked()
//{
//    //m_pCUILayerCommonWidget->StoreCurrentX();
//    if(!m_bMusicPlaying && m_pCUILayerCommonWidget->IsMusicLoaded())
//    {
//        if(m_bMusicPause)
//        {
//            m_pCUILayerCommonWidget->OnResumeMP3();
//            m_bMusicPause = false;
//        }
//        else
//                    m_pCUILayerCommonWidget->OnPlayMP3FromCur();
//        }
//        m_bMusicPlaying = true;
//        //ui->playMP3->setText(tr("Pause"));
//        ui->pauseMP3->show();
//        ui->playMP3->hide();
//        ui->btnAddMusic->setEnabled(false);
//        ui->btnDeleteMusic->setEnabled(false);
//    }
//    else if(m_bMusicPlaying)
//    {
//        m_pCUILayerCommonWidget->OnPauseMP3();
//        m_bMusicPause = true;
//        m_bMusicPlaying = false;
//        //ui->playMP3->setText(tr("Simulate"));
//        ui->pauseMP3->hide();
//        ui->playMP3->show();
//        ui->btnAddMusic->setEnabled(true);
//        if(m_pCUILayerCommonWidget->IsMusicLoaded())
//            ui->btnDeleteMusic->setEnabled(true);
//    }
//}

//void COptionsMusicWidget::on_stopMP3_clicked()
//{
//    if(m_bMusicPlaying || m_bMusicPause)
//    {
//        m_pCUILayerCommonWidget->UpdateLayerAction(0);
//        ui->btnAddMusic->setEnabled(true);
//        if(m_pCUILayerCommonWidget->IsMusicLoaded())
//            ui->btnDeleteMusic->setEnabled(true);
//    }
//    m_pCUILayerCommonWidget->OnStopMP3();
//    //m_pCUILayerCommonWidget->RestoreCurrentX();
//    m_bMusicPlaying = false;
//    m_bMusicPause = false;
//    //ui->playMP3->setText(tr("Simulate"));
//    ui->pauseMP3->hide();
//    ui->playMP3->show();
//    if(m_pEvent)
//    {
//        m_pEvent->ILayerOptinHandler_FinishMusicPlaying();
//    }
//}

//void COptionsMusicWidget::on_playMotion_clicked()
//{
//    if(m_pEvent)
//    {
//        if(m_bMotionPlaying)
//        {
//            m_pEvent->ILayerOptionHanlder_LayerPause();
//            //ui->playMotion->setText(tr("Simulate"));
//            ui->pauseMotion->hide();
//            ui->playMotion->show();
//            m_bMotionPlaying = false;
//        }
//        else
//        {
//            m_pEvent->ILayerOptionHanlder_LayerPause();
//            m_pEvent->ILayerOptionHanlder_LayerPlaying(ACTION_LAYER_MOTION);
//            //ui->playMotion->setText(tr("Pause"));
//            ui->pauseMotion->show();
//            ui->playMotion->hide();
//            m_bMotionPlaying = true;
//        }
//        m_pCUILayerCommonWidget->SetPlaying(m_bMotionPlaying);
//    }
//}

//void COptionsMusicWidget::on_stopMotion_clicked()
//{
//    if(m_pEvent)
//    {
//        m_pEvent->ILayerOptionHanlder_OnStopPlay();
//        //ui->playMotion->setText(tr("Simulate"));
//        ui->pauseMotion->hide();
//        ui->playMotion->show();
//        m_bMotionPlaying = false;
//        m_pCUILayerCommonWidget->SetPlaying(m_bMotionPlaying);
//    }
//}

//void COptionsMusicWidget::setDefaultStyle()
//{
//    QFile styleSheet(":/res/qss/mp3projdefault.qss");
//    if(styleSheet.open(QIODevice::ReadOnly))
//    {
//        setStyleSheet(styleSheet.readAll());
//    }
//}

//void COptionsMusicWidget::on_btnDeleteMusic_clicked()
//{
//    if(m_pCUILayerCommonWidget)
//    {
//        m_pCUILayerCommonWidget->OnDeleteSelectedMusic();
//    }
//}

//void COptionsMusicWidget::on_playEarLed_clicked()
//{
//    if(m_pEvent)
//    {
//        if(m_bEarLedPlaying)
//        {
//            m_pEvent->ILayerOptionHanlder_LayerPause();
//            ui->pauseEarLed->hide();
//            ui->playEarLed->show();
//            m_bEarLedPlaying = false;
//        }
//        else
//        {
//            m_pEvent->ILayerOptionHanlder_LayerPause();
//            m_pEvent->ILayerOptionHanlder_LayerPlaying(ACTION_LAYER_EAR);
//            ui->pauseEarLed->show();
//            ui->playEarLed->hide();
//            m_bEarLedPlaying = true;
//        }
//        m_pCUILayerCommonWidget->SetPlaying(m_bEarLedPlaying);
//    }
//}

//void COptionsMusicWidget::on_stopEarLed_clicked()
//{
//    if(m_pEvent)
//    {
//        m_pEvent->ILayerOptionHanlder_OnStopPlay();
//        ui->pauseEarLed->hide();
//        ui->playEarLed->show();
//        m_bEarLedPlaying = false;
//        m_pCUILayerCommonWidget->SetPlaying(m_bEarLedPlaying);
//    }
//}

//void COptionsMusicWidget::on_playEyeLed_clicked()
//{
//    if(m_pEvent)
//    {
//        if(m_bEyeLedPlaying)
//        {
//            m_pEvent->ILayerOptionHanlder_LayerPause();
//            ui->pauseEyeLed->hide();
//            ui->playEyeLed->show();
//            m_bEyeLedPlaying = false;
//        }
//        else
//        {
//            m_pEvent->ILayerOptionHanlder_LayerPause();
//            m_pEvent->ILayerOptionHanlder_LayerPlaying(ACTION_LAYER_EYE);
//            ui->pauseEyeLed->show();
//            ui->playEyeLed->hide();
//            m_bEyeLedPlaying = true;
//        }
//        m_pCUILayerCommonWidget->SetPlaying(m_bEyeLedPlaying);
//    }
//}

//void COptionsMusicWidget::on_stopEyeLed_clicked()
//{
//    if(m_pEvent)
//    {
//        m_pEvent->ILayerOptionHanlder_OnStopPlay();
//        ui->pauseEyeLed->hide();
//        ui->playEyeLed->show();
//        m_bEyeLedPlaying = false;
//        m_pCUILayerCommonWidget->SetPlaying(m_bEyeLedPlaying);
//    }
//}


int COptionsMusicWidget::GetRawData(char** ppData)
{
    int nLen = 0;
    vector<char> DataSave;
    AddVectorData(&DataSave, (char*)&nLen, sizeof(nLen));
    char* pPathInfo = NULL;
    QString qsMusicName = FileUtils::getFileName(m_strEditFilePath);
    const std::wstring wsPathInfo = qsMusicName.toStdWString();
    std::string strPathInfo = utils::unicode2ansi(wsPathInfo);
    const int nPathLen = strPathInfo.length();
    AddVectorData(&DataSave, (char *)&nPathLen, sizeof(nPathLen));
    AddVectorData(&DataSave, (char *)strPathInfo.c_str(), nPathLen);
    SAFE_DELETE_ARRAY(pPathInfo);

    nLen = DataSave.size();
    ModifyVertorData(DataSave, 0, (char*)&nLen, sizeof(nLen));
    if (nLen > 0) {
        *ppData = new char[nLen];
        CopyVertorToPointer(DataSave, *ppData);
    }
    return nLen;
}

void COptionsMusicWidget::SetRawData(char* pData, int nLen)
{
    int nTotalLen = 0;
    UBX_MEMCPY_INC(nTotalLen, pData);
    if (nTotalLen != nLen)
        return;

    int nInfoLen = 0;
    UBX_MEMCPY_INC(nInfoLen, pData);
    if (nInfoLen)
    {
        char* pPathInfo = new char[nInfoLen + 1];
        memset(pPathInfo, 0, nInfoLen + 1);
        memcpy(pPathInfo, pData, nInfoLen);
        pData += nInfoLen;

        m_strEditFilePath = QString::fromStdWString(utils::ansi2unicode(pPathInfo));
        delete [] pPathInfo;
        pPathInfo = NULL;
    }
}

/**************************************************************************
* 函数名: GetCurrenGroupManagerID
* 功能:   获取当前的动作组ID
* 参数:
* 返回值:
* 时间:   2015/12/25 16:36
* 作者: 周志平
**************************************************************************/
int COptionsMusicWidget::GetCurrenGroupManagerID()
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
* 函数名：UpdateBlockList
* 功能:更新动作列表
* 参数:
*    @[in ] pGroup: 动作组数据指针
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void COptionsMusicWidget::UpdateBlockList(CActionBlockGroupManager* pGroup)
{
    if (!pGroup)
    {
        return;
    }

    m_pBlockListWidget->clear();
    for(int i = 0; i<pGroup->GetActionCount(); i++)
    {
        CActionBlockData* pBlockData = pGroup->GetAt(i);
        if(pBlockData)
        {
            QString strDescription = QString::fromStdWString(pBlockData->GetDescription());
            //QString strDescription = QString::fromUtf16((WCHAR *)pBlockData->GetDescription());
            QListWidgetItem * pItem = new QListWidgetItem(strDescription);
            m_pBlockListWidget->insertItem(i,pItem);

            pItem->setData(ActionBlockDataID, pBlockData->GetID());
            pItem->setSelected(true);
            //           m_pBlockListWidget->setItemSelected(pItem, true);
        }
    }
    SetCurSelectBlockItem(pGroup->GetActionCount());
    m_pBlockListWidget->show();
}

void COptionsMusicWidget::SetCurSelectBlockItem(int nBlockItem)
{
    //    if (m_nCurrentSelectItem = -1)
    //    {
    //        return
    //    }
}

/**************************************************************************
* 函数名：slotDeleteGroup
* 功能: 删除音乐组
* 参数:无
* 返回值: 无
* 时间: 2017/11/08
* 作者: jianjie
*/
void COptionsMusicWidget::slotDeleteGroup()
{
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
    //m_pEvent->ILayerOptionHandler_HasMotionData(m_pGroupListWidget->count() > 0, ACTION_LAYER_MUSIC);
    if(m_pGroupListWidget->count() > 0)
    {
        ui->pbInsert->setEnabled(true);
        ui->pbModify->setEnabled(true);
    }
    else
    {
        ui->pbInsert->setEnabled(false);
        ui->pbModify->setEnabled(false);
    }

    deleteMusicFile();
}

/**************************************************************************
* 函数名：DeleteLayerBlock
* 功能: 删除动作块
* 参数:无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void COptionsMusicWidget::slotDeleteLayerBlock()
{
    if (m_pGroupManager == NULL)
    {
        return;
    }
    int nAllTime = 0;
    CActionBlockGroupManager* pGroupManager = m_pGroupManager->FindBlockData(m_nCurrentGroupID);
    if (pGroupManager)
    {
        int nCurRow = m_pBlockListWidget->currentRow();
        QListWidgetItem* pItem = m_pBlockListWidget->item(nCurRow);
        if (NULL == pItem)
        {
            return;
        }
        int nSelectBlockID = pItem->data(ActionBlockDataID).toInt();
        if (-1 == nSelectBlockID)
        {
            return;
        }

        CActionBlockData* pActionBlockData = pGroupManager->FindBlockData(nSelectBlockID);
        if (NULL == pActionBlockData)
        {
            return;
        }

        nAllTime = pActionBlockData->GetRunTime() + pActionBlockData->GetStopTime();
        pGroupManager->DeleteLayerBlock(nSelectBlockID);

        // Modify by jianjie 2017/11/29
        if (pGroupManager->GetActionCount() < 1) //判断是否还有数据
        {
            int nStartTime = pGroupManager->GetStartTime();
            m_pEvent->ILayerOptionHandler_SetSelectGroupBlock(nStartTime + 2);
            m_pEvent->ILayerOptionHandler_OnDeleteGroupBlock();
        }
        else
        {
            UpdateBlockList(pGroupManager);
        }

        if (m_pEvent)
        {
            m_pEvent->ILayerOptionHandler_OnGroupManagerChange(m_nCurrentGroupID, -nAllTime, true);
        }

        deleteMusicFile();
    }
}

/**************************************************************************
* 函数名：deleteMusicFile
* 功能: 删除音乐文件
* 参数:无
* 返回值: 无
* 时间: 2017/12/06
* 作者: jianjie
*/
void COptionsMusicWidget::deleteMusicFile()
{
    // 遍历音乐组中音乐文件
    QStringList strMusicList;
    for (int i = 0; i < m_pGroupManager->GetCount(); ++i)
    {
        CActionBlockGroupManager* pGroupManager = m_pGroupManager->GetAt(i);
        if (!pGroupManager)
        {
            continue;
        }

        for (int j = 0; j < pGroupManager->GetActionCount(); ++j)
        {
            CActionBlockData* pBlockData = pGroupManager->GetAt(j);
            QString strDescription = QString::fromStdWString(pBlockData->GetDescription());
            if (!strMusicList.contains(strDescription))
            {
                strMusicList.append(strDescription);
            }
        }
    }

    // 遍历music文件夹中音乐文件，若已不存在于音乐组中，则删除本地音乐文件
    QString strMusicPath = CConfigs::getUserDirPath(USERDIR_MUSIC);

    QDir dir(strMusicPath);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    for (int i = 0; i < fileList.count(); ++i)
    {
        QFileInfo file = fileList.at(i);
        if (file.isFile() && !strMusicList.contains(file.fileName()))
        {
            QFile::remove(file.absoluteFilePath());
            //file.dir().remove(file.fileName());

            // 删除音乐文件，相关参数复位，此文件不可再次添加(需重新载入)
            ui->lEPlayTime->setText("0");
            ui->lEStopTime->setText("0");
            ui->lEMusicPath->setText("");
        }
    }
}

/**************************************************************************
* 函数名：MusicLoaded
* 功能:更新音乐名称和时长
* 参数:
*    @[in ] strTitle: 音乐标题
*    @[in ] nTime: 音乐总时间，单位为s
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void COptionsMusicWidget::MusicLoaded(const QString& strTitle, int nTime)
{
    int milliTime = nTime;
    int nScaleTime = 50;
    if (m_pEvent)
    {
       nScaleTime = m_pEvent->ILayerOptionHandler_GetScaleValue();
    }
    int nRunTime = milliTime / nScaleTime + 2;
    QString strTime = QString::number(nRunTime);
    ui->lEPlayTime->setText(strTime);
    m_strMusicTitle = strTitle;
//    m_strTotalTime = QString().sprintf("%02d:%02d:%02d",
//                                       (int)(time) / 3600,
//                                       ( (int)(time) % 3600 ) / 60,
//                                       (int)(time) % 60);
//    ui->labelMusicTime->setText(m_strTotalTime);
//    m_nTotalTime = time;
//    ui->musicSlider->setRange(0, time);
//    ui->musicSlider->setSliderPosition(0);
}

/**************************************************************************
* 函数名：CopyMusicFileTo
* 功能: 拷贝加载的音乐文件到特定目录
* 参数:
*    @[in ] strSourcePath: 待拷贝音乐的源路径
*    @[in ] strMusicName: 音乐文件名称
* 返回值: 无
* 时间: 2017/1/16
* 作者: wanxf
*/
void COptionsMusicWidget::CopyMusicFileTo(QString strSourcePath, QString strMusicName)
{
    QString strMusicPath = CConfigs::getUserDirPath(USERDIR_MUSIC);
    strMusicPath += "/" + strMusicName;
    QFile::copy(strSourcePath, strMusicPath);
}

/**************************************************************************
* 函数名：on_m_ListActionGroup_itemPressed
* 功能: 在音乐组列表中选中行，进行鼠标操作
* 参数:
*    @[in ] item: 选中行数据项
* 返回值: 无
* 时间: 2017/3/27
* 作者: wanxf
*/
void COptionsMusicWidget::on_m_ListActionGroup_itemPressed(QListWidgetItem *item)
{
    if(qApp->mouseButtons() == Qt::LeftButton)
    {
        int nRow = m_pGroupListWidget->row(item);
        if(nRow != -1 )
        {
            SetCurrentSelectItem(item, nRow);
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
                SetCurrentSelectItem(item, nRow);
                if(m_pEvent)
                {
                    m_pEvent->ILayerOptionHandler_OnSelectGroupManageChange(m_nCurrentGroupID , nRow);
                }
            }

            QMenu* pop_menu = new QMenu();
            QAction* DeleteAction = new QAction(tr("Delete"),this);
            pop_menu->addAction(DeleteAction);
            QAbstractItemDelegate::connect(DeleteAction, &QAction::triggered, this,&COptionsMusicWidget::slotDeleteGroup);

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

/**************************************************************************
* 函数名：on_m_ListActionBlock_itemPressed
* 功能: 在音乐块列表中选中行，进行鼠标操作
* 参数:
*    @[in ] item: 选中行数据项
* 返回值: 无
* 时间: 2017/3/27
* 作者: wanxf
*/
void COptionsMusicWidget::on_m_ListActionBlock_itemPressed(QListWidgetItem *item)
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

            //int nSelectBlockID = item->data(ActionBlockDataID).toInt();
            CActionBlockGroupManager* pGroup = m_pGroupManager->FindBlockData(m_nCurrentGroupID);
            if (pGroup == NULL)
                return;

            CActionBlockData* pActionBlockData = pGroup->GetAt(nRow);
            if (pActionBlockData == NULL)
                return;
            UpdateBlockOptionUI(pActionBlockData);
        }
    }
    else if(qApp->mouseButtons() == Qt::RightButton) //右键
    {
        if(item == Q_NULLPTR)
        {
            return;
        }
        else
        {
            int nRow = m_pBlockListWidget->row(item);
            if(nRow != -1 )
            {
                if(m_pEvent)
                {
                    m_pEvent->ILayerOptionHandler_OnSelectActionManageChange(m_nCurrentGroupID, nRow);
                }
            }

            QMenu* pop_menu = new QMenu();
            QAction* DeleteAction = new QAction(tr("Delete"),this);
            pop_menu->addAction(DeleteAction);
            QAbstractItemDelegate::connect(DeleteAction, &QAction::triggered, this,&COptionsMusicWidget::slotDeleteLayerBlock);

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
