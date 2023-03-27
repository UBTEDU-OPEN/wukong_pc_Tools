

#include "OptionsExpressionWidget.h"
#include "ui_OptionsExpressionWidget.h"
#include "fileutils.h"
#include "formatfunc.h"
#include "configs.h"

#include <QFileDialog>
#include <QMenu>
#include <QMovie>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMessageBox>


bool CreateExpressionLayOptionHandler(ILayerOptionHandler** ppHandler)
{
    COptionsExpressionWidget* pProxy = new COptionsExpressionWidget();
    if (pProxy)
    {
        *ppHandler = pProxy;
        return true;
    }

    return false;
}

COptionsExpressionWidget::COptionsExpressionWidget(QWidget* parent) : QWidget(parent)
    , ui(new Ui::COptionsExpressionWidget)
    , m_pEvent(NULL)
    , m_pGroupListWidget(NULL)
    , m_pBlockListWidget(NULL)
    , m_pGroupManager(NULL)
    , m_nCurrentGroupID(-1)
    , m_nGroupSelectItem(-1)
    , m_strFilePath("")
    , m_nScaleTime(100)
{
    ui->setupUi(this);

    initWidget();
}


COptionsExpressionWidget::~COptionsExpressionWidget()
{
    delete ui;
}

/*************************************
* ��������initWidget
* ����:��ʼ������
* ����: ��
* ����ֵ: ��
* ʱ��: 2017/12/28
* ����: jianjie
*/
void COptionsExpressionWidget::initWidget()
{
    m_pGroupListWidget = ui->m_ListActionGroup;
    m_pBlockListWidget = ui->m_ListActionBlock;

    ui->lEPlayTime->setText("0");
    ui->lEStopTime->setText("0");

    ui->pbAdd->setEnabled(false);
    ui->pbModify->setEnabled(false);
    ui->pbInsert->setEnabled(false);

    connect(ui->pbChoosePath , SIGNAL(clicked()) , this , SLOT(slotChoosePath()));
    connect(ui->pbAdd, SIGNAL(clicked()) , this , SLOT(slotAddExpression()));
    connect(ui->pbInsert, SIGNAL(clicked()), this, SLOT(slotInsertExpression()));
    connect(ui->pbModify, SIGNAL(clicked()), this, SLOT(slotModifyExpression()));

    connect(m_pGroupListWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(slotGroupListItemPressed(QListWidgetItem*)));
    connect(m_pBlockListWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(slotBlockListItemPressed(QListWidgetItem*)));

    hide();
}

/*************************************
* ��������Connect
* ����: ���ص���������
* ����: 
* ����ֵ: 
* ʱ��: 2017/12/29
* ����: jianjie
*/
bool COptionsExpressionWidget::Connect(QWidget* pParent, ILayerOptionHandler_Event* pEvent)
{
    if (NULL == pParent || NULL == pEvent)
    {
        return false;
    }

    setParent(pParent);

    QLayout* layout = pParent->layout();
    layout->addWidget(this);

    m_pEvent = pEvent;

    return true;
}

/*************************************
* ��������ReleaseConnection
* ����: �ͷ���Դ
* ����: ��
* ����ֵ: ��
* ʱ��: 2017/12/29
* ����: jianjie
*/
void COptionsExpressionWidget::ReleaseConnection()
{
    disconnect();
    deleteLater();
}


QWidget* COptionsExpressionWidget::GetWidget()
{
    return this;
}

/*************************************
* ��������SetActionBlockDataManager
* ����: ���д�붯������
* ����: pManager          ��������
       nCurrentGroupID    ��ID
       nCurrentActionID   ֡ID
* ����ֵ: ��
* ʱ��: 2017/12/28
* ����: jianjie
*/
void COptionsExpressionWidget::SetActionBlockDataManager(CActionBlockDataManager* pManager, int nCurrentGroupID, int nCurrentActionID, int nOffset)
{
    if (NULL == pManager)
    {
        return;
    }

    m_nCurrentGroupID = nCurrentGroupID;
    m_pGroupManager = pManager;

    if (-1 == nCurrentGroupID)
    {
        m_nGroupSelectItem = -1;
    }

    updateUIByGroupManager(pManager);

    QListWidgetItem* pItem = NULL;
    for (int i = 0; i < m_pGroupListWidget->count(); ++i)
    {
        pItem = m_pGroupListWidget->item(i);
        if (NULL == pItem)
        {
            continue;
        }

        int nGroupID = pItem->data(GroupID).toInt();
        if (nGroupID == nCurrentGroupID)
        {
            m_nGroupSelectItem = i;
            break;
        }
    }
    setGroupSelectItem(pItem);

    if (-1 != nCurrentActionID && -1 != m_nGroupSelectItem)
    {
        CActionBlockGroupManager* pGroup = pManager->GetAt(m_nGroupSelectItem);
        if (pGroup)
        {
            CActionBlockData* pBlockData = pGroup->GetAt(nCurrentActionID);
            if (pBlockData)
            {
                m_pBlockListWidget->setCurrentRow(nCurrentActionID);
                updateBlockOptionUI(pBlockData);

                // ���»����˱���
                showExpression(pBlockData, (nOffset - 1) * m_nScaleTime);
            }
        }
    }
}

/*************************************
* ��������showExpression
* ����: ��ʾĳһʱ�̱���(�����ʱ������Ӧ)
* ����: pBlockData    ������
        nTime         ʱ���
* ����ֵ: ��
* ʱ��: 2018/01/06
* ����: jianjie
*/
void COptionsExpressionWidget::showExpression(CActionBlockData* pBlockData, int nTime)
{
    if (NULL == pBlockData || nTime < 0)
    {
        return;
    }

    WCHAR* pwchName = pBlockData->GetDescription();
    QString strFileName = QString::fromStdWString(pwchName);
    QString strFilePath = CConfigs::getUserDirPath(USERDIR_EXPRESS) + QDir::separator() + strFileName;

    int nCurTime = 0;
    int nCurFrame = 0;

    if (strFileName.contains(".gif"))
    {
        QMovie* pMovie = new QMovie(strFilePath);

        int nTotalFrames = pMovie->frameCount();

        int nTotalTime = pBlockData->GetRunTime() * m_nScaleTime;
        nCurFrame = nTotalFrames * nTime / nTotalTime;

        // 0��֡������ʱ��
        //pMovie->jumpToFrame(0);
        //for (int i = 0; i < nTotalFrames; ++i)
        //{
        //    nCurTime += pMovie->nextFrameDelay();
        //    pMovie->jumpToNextFrame();
        //    if (nCurTime >= nTime)
        //    {
        //        nCurFrame = i;
        //        break;
        //    }
        //}

        delete pMovie;
    }
    else if (strFileName.contains(".json"))
    {
        int nFps = 1;
        int nTotalFrames = 0;
        parseJsonFile(strFilePath, nFps, nTotalFrames);
        float fFrameTime = (float)1000 / nFps;
        for (int i = 0; i < nTotalFrames; ++i)
        {
            nCurTime = i * fFrameTime;
            if (nCurTime >= nTime)
            {
                nCurFrame = i;
                break;
            }
        }
    }
    else
    {
        return;
    }

    if (m_pEvent)
    {
        m_pEvent->ILayerOptionHandler_ShowExpression(strFileName, nCurFrame);
        qDebug() << "current frame:" << nCurFrame << endl;
    }
}

/*************************************
* ��������updateUIByGroupManager
* ����: ˢ�µ�ǰ������������
* ����: 
* ����ֵ: ��
* ʱ��: 2017/12/29
* ����: jianjie
*/
void COptionsExpressionWidget::updateUIByGroupManager(CActionBlockDataManager* pGroupManager)
{
    if (NULL == pGroupManager)
    {
        return;
    }

    int nCount = pGroupManager->GetCount();
    int nTemp = m_pGroupListWidget->count();

    if (pGroupManager->GetCount() == m_pGroupListWidget->count())
    {
        m_pGroupListWidget->setItemSelected(m_pGroupListWidget->item(m_nGroupSelectItem), true);
        return;
    }

    m_pGroupListWidget->clear();
    for (int i = 0; i < pGroupManager->GetCount(); ++i)
    {
        CActionBlockGroupManager* pGroup = pGroupManager->GetAt(i);
        if (NULL == pGroup)
        {
            continue;
        }

        QString strDescription = QString::fromStdWString(pGroup->GetDescription());
        QListWidgetItem* pItem = new QListWidgetItem(strDescription);
        m_pGroupListWidget->insertItem(i, pItem);
        int nGroupID = pGroup->GetID();
        pItem->setData(GroupID, nGroupID);
    }
    m_pGroupListWidget->show();
}

/*************************************
* ��������slotChoosePath
* ����: ѡ������ļ��ۺ���
* ����: ��
* ����ֵ: ��
* ʱ��: 2017/12/28
* ����: jianjie
*/
void COptionsExpressionWidget::slotChoosePath()
{
    //QString sFmt;
    //sFmt.sprintf("Chest File(*%s);;Chest File(*%s)", BIN_FILE_EXT, BIN_FILE_EXT);
    //QString sFilePath = QFileDialog::getOpenFileName(this, tr("Open File"), "./", sFmt);

    QString strExpressionFilePath = QFileDialog::getOpenFileName(this, tr("Choose a expression file"), "./", tr("Gif(*.gif);;Json(*.json)"));
    if (strExpressionFilePath.isEmpty())
    {
        return;
    }

    loadExpressionFile(strExpressionFilePath);

    CopyExpressionFile(strExpressionFilePath);

    ui->pbAdd->setEnabled(true);
    ui->pbInsert->setEnabled(true);
    ui->pbModify->setEnabled(true);

    ui->lEExpressionPath->setText(strExpressionFilePath);

    m_strFilePath = strExpressionFilePath;
}

/*************************************
* ��������loadExpressionFile
* ����: ���ء����������ļ�
* ����: ��
* ����ֵ: ��
* ʱ��: 2018/01/06
* ����: jianjie
*/
bool COptionsExpressionWidget::loadExpressionFile(QString strFilePath)
{
    if (strFilePath.isEmpty())
    {
        return false;
    }

    int nTotalTime = 0;
    int nTotalFrames = 0;

    if (strFilePath.contains(".gif"))               // gifͼ����
    {
        QMovie* pMovie = new QMovie();
        if (NULL == pMovie)
        {
            return false;
        }

        pMovie->setFileName(strFilePath);
        nTotalFrames = pMovie->frameCount();

        if (nTotalFrames > 500)
        {
            QMessageBox::warning(NULL, "", QObject::tr("The frame number of Gif is over 500 frames!"), QObject::tr("OK"));
            return false;
        }

        int nFrameTime = 0;
        while (1)
        {
            nFrameTime = pMovie->nextFrameDelay();
            nFrameTime = ((nFrameTime + 5) / 10) * 10;

            //qDebug() << "delaytime:" << nFrameTime << endl;

            nTotalTime += nFrameTime;

            if (!pMovie->jumpToNextFrame())
            {
                break;
            }
        }

        delete pMovie;
    } 
    else if (strFilePath.contains(".json"))         // json��ʽ�ļ�����
    {
        int nFps = 1;
        parseJsonFile(strFilePath, nFps, nTotalFrames);
        nTotalTime = nTotalFrames * 1000 / nFps;
    }

    if (m_pEvent)
    {
        m_nScaleTime = m_pEvent->ILayerOptionHandler_GetScaleValue();
    }

    int nTime = nTotalTime / m_nScaleTime + 1;

    ui->lEPlayTime->setText(QString::number(nTime));
}

/*************************************
* ��������parseJsonFile
* ����: ����Json�����ļ�
* ����: strFilePath    [IN]   �ļ�·��
        nFps           [OUT]  ֡��
        nTotalFrames   [OUT]  ��֡��
* ����ֵ: ��
* ʱ��: 2018/01/06
* ����: jianjie
*/
void COptionsExpressionWidget::parseJsonFile(QString strFilePath, int& nFps, int& nTotalFrames)
{
    QByteArray byteArray;
    QFile file(strFilePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    byteArray = file.readAll();
    file.close();

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(byteArray, &jsonError);
    if (!document.isNull() && jsonError.error == QJsonParseError::NoError)
    {
        if (document.isObject())
        {
            QJsonObject object = document.object();
            if (object.contains("op"))
            {
                QJsonValue value = object.value("op");
                if (value.isDouble())
                {
                    nTotalFrames = value.toInt();
                }
            }
            if (object.contains("fr"))
            {
                QJsonValue value = object.value("fr");
                if (value.isDouble())
                {
                    nFps = value.toInt();
                }
            }
        }
    }
}

/*************************************
* ��������CopyExpressionFile
* ����: ���Ʊ����ļ�����ʱ�ļ���·��
* ����: 
* ����ֵ: ��
* ʱ��: 2018/01/06
* ����: jianjie
*/
void COptionsExpressionWidget::CopyExpressionFile(QString strSrcPath)
{
    QString strFileName = FileUtils::getFileName(strSrcPath);
    QString strDstPath = CConfigs::getUserDirPath(USERDIR_EXPRESS) + QDir::separator() + strFileName;;

    QFile::copy(strSrcPath, strDstPath);
}

/*************************************
* ��������updateBlockListByGroup
* ����: ˢ��ָ����Ŀ�����
* ����: 
* ����ֵ: ��
* ʱ��: 2017/12/28
* ����: jianjie
*/
void COptionsExpressionWidget::updateBlockListByGroup(CActionBlockGroupManager* pGroup)
{
    if (NULL == pGroup)
    {
        return;
    }

    m_pBlockListWidget->clear();
    for (int i = 0; i < pGroup->GetActionCount(); ++i)
    {
        CActionBlockData* pBlockData = pGroup->GetAt(i);
        if (pBlockData)
        {
            QString strDescription = QString::fromStdWString(pBlockData->GetDescription());
            QListWidgetItem* pItem = new QListWidgetItem(strDescription);
            m_pBlockListWidget->insertItem(i, pItem);

            pItem->setData(ActionBlockDataID, pBlockData->GetID());
            pItem->setSelected(true);
        }
    }
    m_pBlockListWidget->show();
}

/*************************************
* ��������updateBlockOptionUI
* ����: ���ݿ�����ˢ��UI
* ����: 
* ����ֵ: ��
* ʱ��: 2017/12/28
* ����: jianjie
*/
void COptionsExpressionWidget::updateBlockOptionUI(CActionBlockData* pBlockData)
{
    if (NULL == pBlockData)
    {
        return;
    }

    QString strRunTime = QString::number(pBlockData->GetRunTime());
    ui->lEPlayTime->setText(strRunTime);

    QString strStopTime = QString::number(pBlockData->GetStopTime());
    ui->lEStopTime->setText(strStopTime);

    char* pData = NULL;
    int nLen = 0;
    pBlockData->GetExtBlockData(&pData, nLen);
    if (pData && nLen > 0)
    {
        SetRawData(pData, nLen);
        delete[] pData;
    }
    ui->lEExpressionPath->setText(m_strFilePath);
}

/*************************************
* ��������slotAddExpression
* ����: ��ӱ���ۺ���
* ����: ��
* ����ֵ: ��
* ʱ��: 2017/12/28
* ����: jianjie
*/
void COptionsExpressionWidget::slotAddExpression()
{
    int     nRunTime    = ui->lEPlayTime->text().toInt();
    int     nStopTime = ui->lEStopTime->text().toInt();

    if (m_strFilePath.isEmpty())
    {
        return;
    }

    QString strFileName = FileUtils::getFileName(m_strFilePath);
    std::wstring wstrDescription = strFileName.toStdWString();

    CActionBlockData* pActionBlockData = new CActionBlockData();
    if (NULL == pActionBlockData)
    {
        return;
    }

    pActionBlockData->SetID(-1);
    pActionBlockData->SetRunTime(nRunTime);
    pActionBlockData->SetStopTime(nStopTime);
    pActionBlockData->SetDescription((WCHAR*)wstrDescription.c_str());

    char* pData = NULL;
    int   nLen = 0;
    nLen = GetRawData(&pData);
    if (pData && nLen > 0)
    {
        pActionBlockData->SetExtBlockData(nLen, pData);

        delete[] pData;
    }

    int nCurrentGroupID = getCurrentGroupManagerID();
    if (m_pEvent)
    {
        m_pEvent->ILayerOptionHandler_OnAddActionBlock(nCurrentGroupID, pActionBlockData, "", true);
    }

    // ���Ʊ����ļ�

    if (-1 != nCurrentGroupID)
    {
        QListWidgetItem* pItem = new QListWidgetItem(strFileName);
        m_pBlockListWidget->insertItem(m_pBlockListWidget->count(), pItem);
        pItem->setData(ActionBlockDataID, pActionBlockData->GetID());
        pItem->setSelected(true);
    }

}

/*************************************
* ��������slotInsertExpression
* ����: �������ۺ���
* ����: ��
* ����ֵ: ��
* ʱ��: 2017/12/28
* ����: jianjie
*/
void COptionsExpressionWidget::slotInsertExpression()
{
    if (m_pBlockListWidget->count() <= 0 || NULL == m_pGroupManager)
    {
        return;
    }

    CActionBlockGroupManager* pGroup = m_pGroupManager->GetAt(m_nGroupSelectItem);
    if (NULL == pGroup)
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

    int     nRunTime = ui->lEPlayTime->text().toInt();
    int     nStopTime = ui->lEStopTime->text().toInt();

    if (m_strFilePath.isEmpty())
    {
        return;
    }

    QString strFileName = FileUtils::getFileName(m_strFilePath);
    std::wstring wstrDescription = strFileName.toStdWString();

    CActionBlockData* pActionBlockData = new CActionBlockData();
    if (NULL == pActionBlockData)
    {
        return;
    }

    pActionBlockData->SetID(-1);
    pActionBlockData->SetRunTime(nRunTime);
    pActionBlockData->SetStopTime(nStopTime);
    pActionBlockData->SetDescription((WCHAR*)wstrDescription.c_str());

    char* pData = NULL;
    int   nLen = 0;
    nLen = GetRawData(&pData);
    if (pData && nLen > 0)
    {
        pActionBlockData->SetExtBlockData(nLen, pData);

        delete[] pData;
    }

    pGroup->InsertBeforeId(nSelectBlockID, pActionBlockData);

    int nCurrentGroupID = getCurrentGroupManagerID();
    if (m_pEvent)
    {
        m_pEvent->ILayerOptionHandler_OnGroupManagerChange(nCurrentGroupID, pActionBlockData->GetRunTime() + pActionBlockData->GetStopTime(), true);
    }

    QListWidgetItem* pNewItem = new QListWidgetItem(strFileName);
    m_pBlockListWidget->insertItem(nCurRow, pNewItem);
    pNewItem->setData(ActionBlockDataID, pActionBlockData->GetID());
    pNewItem->setSelected(true);

}

/*************************************
* ��������slotModifyExpression
* ����: �޸ı������ݲۺ���
* ����: ��
* ����ֵ: ��
* ʱ��: 2017/12/28
* ����: jianjie
*/
void COptionsExpressionWidget::slotModifyExpression()
{
    if (NULL == m_pGroupManager)
    {
        return;
    }

    CActionBlockGroupManager* pGroup = m_pGroupManager->GetAt(m_nGroupSelectItem);
    if (NULL == pGroup)
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

    int     nRunTime = ui->lEPlayTime->text().toInt();
    int     nStopTime = ui->lEStopTime->text().toInt();
    QString strFilePath = ui->lEExpressionPath->text();
    if (strFilePath.isEmpty())
    {
        return;
    }

    QString strFileName = FileUtils::getFileName(strFilePath);
    std::wstring wstrDescription = strFileName.toStdWString();

    CActionBlockData* pActionBlockData = pGroup->FindBlockData(nSelectBlockID);
    if (NULL == pActionBlockData)
    {
        return;
    }

    pActionBlockData->SetRunTime(nRunTime);
    pActionBlockData->SetStopTime(nStopTime);
    pActionBlockData->SetDescription((WCHAR*)wstrDescription.c_str());

    char* pData = NULL;
    int   nLen = 0;
    nLen = GetRawData(&pData);
    if (pData && nLen > 0)
    {
        pActionBlockData->SetExtBlockData(nLen, pData);

        delete[] pData;
    }

    int nCurrentGroupID = getCurrentGroupManagerID();
    if (m_pEvent)
    {
        m_pEvent->ILayerOptionHandler_OnGroupManagerChange(nCurrentGroupID, pActionBlockData->GetRunTime() + pActionBlockData->GetStopTime(), true);
    }
}

/*************************************
* ��������setGroupSelectItem
* ����: ѡ����
* ����: 
* ����ֵ: ��
* ʱ��: 2017/12/28
* ����: jianjie
*/
void COptionsExpressionWidget::setGroupSelectItem(QListWidgetItem* pItem)
{
    if (NULL == pItem || -1 == m_nGroupSelectItem)
    {
        m_pBlockListWidget->clear();
        return;
    }

    m_pGroupListWidget->setItemSelected(pItem, true);
    m_nGroupSelectItem = m_pGroupListWidget->row(pItem);

    if (m_pGroupManager)
    {
        CActionBlockGroupManager* pGroup = m_pGroupManager->GetAt(m_nGroupSelectItem);
        if (pGroup)
        {
            m_nCurrentGroupID = pGroup->GetID();
            updateBlockListByGroup(pGroup);
        }
    }
}

/*************************************
* ��������slotDeleteGroupItem
* ����: ɾ��ָ����
* ����: ��
* ����ֵ: ��
* ʱ��: 2017/12/29
* ����: jianjie
*/
void COptionsExpressionWidget::slotDeleteGroupItem()
{
    if (NULL == m_pGroupManager)
    {
        return;
    }

    CActionBlockGroupManager* pGroup = m_pGroupManager->GetAt(m_nGroupSelectItem);
    if (pGroup)
    {
        int nStartTime = pGroup->GetStartTime();
        if (m_pEvent)
        {
            m_pEvent->ILayerOptionHandler_SetSelectGroupBlock(nStartTime + 2);
            m_pEvent->ILayerOptionHandler_OnDeleteGroupBlock();
            //m_pEvent->ILayerOptionHandler_HasMotionData(m_pGroupListWidget->count() > 0, ACTION_LAYER_EXPRESSION);
        }
    }

    if (m_pGroupListWidget->count() > 0)
    {
        ui->pbInsert->setEnabled(true);
        ui->pbModify->setEnabled(true);
    } 
    else
    {
        ui->pbInsert->setEnabled(false);
        ui->pbModify->setEnabled(false);
    }

    // deleteExpressionFile();
}

/*************************************
* ��������slotDeleteBlockItem
* ����: ɾ��ָ����
* ����: ��
* ����ֵ: ��
* ʱ��: 2017/12/29
* ����: jianjie
*/
void COptionsExpressionWidget::slotDeleteBlockItem()
{
    if (NULL == m_pGroupManager || NULL == m_pEvent)
    {
        return;
    }

    CActionBlockGroupManager* pGroup = m_pGroupManager->GetAt(m_nGroupSelectItem);
    if (pGroup)
    {
        int nBlockRow = m_pBlockListWidget->currentRow();
        QListWidgetItem* pItem = m_pBlockListWidget->item(nBlockRow);
        if (NULL == pItem)
        {
            return;
        }

        int nSelectBlockID = pItem->data(ActionBlockDataID).toInt();
        CActionBlockData* pBlockData = pGroup->FindBlockData(nSelectBlockID);
        if (NULL == pBlockData)
        {
            return;
        }

        pGroup->DeleteLayerBlock(nSelectBlockID);

        if (pGroup->GetActionCount() > 0)
        {
            updateBlockListByGroup(pGroup);
        }
        else
        {
            int nStartTime = pGroup->GetStartTime();
            m_pEvent->ILayerOptionHandler_SetSelectGroupBlock(nStartTime + 2);
            m_pEvent->ILayerOptionHandler_OnDeleteGroupBlock();
        }

        int nBlockTime = pBlockData->GetRunTime() + pBlockData->GetStopTime();
        m_pEvent->ILayerOptionHandler_OnGroupManagerChange(m_nCurrentGroupID, -nBlockTime, true);
    }

    // deleteExpressionFile();
}

/*************************************
* ��������slotGroupListItemPressed
* ����: ���б����ۺ���
* ����: ��
* ����ֵ: ��
* ʱ��: 2017/12/29
* ����: jianjie
*/
void COptionsExpressionWidget::slotGroupListItemPressed(QListWidgetItem* pItem)
{
    if (NULL == pItem)
    {
        return;
    }

    m_nGroupSelectItem = m_pGroupListWidget->row(pItem);
    setGroupSelectItem(pItem);

    if (m_pEvent)
    {
        m_pEvent->ILayerOptionHandler_OnSelectGroupManageChange(m_nCurrentGroupID, 0);
    }

    if (qApp->mouseButtons() == Qt::RightButton)
    {
        QMenu* pop_menu = new QMenu();
        QAction* deleteAction = new QAction(tr("Delete"), this);
        pop_menu->addAction(deleteAction);
        connect(deleteAction, SIGNAL(triggered()), this, SLOT(slotDeleteGroupItem()));

        QFile styleSheet(":/res/qss/menudefault.qss");
        if (styleSheet.open(QIODevice::ReadOnly))
        {
            pop_menu->setStyleSheet(styleSheet.readAll());
        }
        pop_menu->exec(QCursor::pos());
        pop_menu->deleteLater();
    }
}

/*************************************
* ��������slotBlockListItemPressed
* ����: ���б����ۺ���
* ����: ��
* ����ֵ: ��
* ʱ��: 2017/12/29
* ����: jianjie
*/
void COptionsExpressionWidget::slotBlockListItemPressed(QListWidgetItem* pItem)
{
    if (NULL == pItem)
    {
        return;
    }

    int nBlockRow = m_pBlockListWidget->row(pItem);
    if (m_pEvent)
    {
        m_pEvent->ILayerOptionHandler_OnSelectActionManageChange(m_nCurrentGroupID, nBlockRow);
    }

    if (m_pGroupManager)
    {
        CActionBlockGroupManager* pGroup = m_pGroupManager->GetAt(m_nGroupSelectItem);
        if (pGroup)
        {
            CActionBlockData* pBlockData = pGroup->GetAt(nBlockRow);
            if (pBlockData)
            {
                updateBlockOptionUI(pBlockData);
            }
        }
    }

    if (qApp->mouseButtons() == Qt::RightButton)
    {
        QMenu* pop_menu = new QMenu();
        QAction* deleteAction = new QAction(tr("Delete"), this);
        pop_menu->addAction(deleteAction);
        connect(deleteAction, SIGNAL(triggered()), this, SLOT(slotDeleteBlockItem()));

        QFile styleSheet(":/res/qss/menudefault.qss");
        if (styleSheet.open(QIODevice::ReadOnly))
        {
            pop_menu->setStyleSheet(styleSheet.readAll());
        }
        pop_menu->exec(QCursor::pos());
        pop_menu->deleteLater();
    }
}


int COptionsExpressionWidget::GetRawData(char** ppData)
{
    int nLen = 0;

    vector<char> DataSave;
    AddVectorData(&DataSave, (char*)&nLen, sizeof(nLen));

    QString strFileName = FileUtils::getFileName(m_strFilePath);
    const std::wstring wsPathInfo = strFileName.toStdWString();
    std::string strPathInfo = utils::unicode2ansi(wsPathInfo);
    const int nPathLen = strPathInfo.length();
    AddVectorData(&DataSave, (char *)&nPathLen, sizeof(nPathLen));
    AddVectorData(&DataSave, (char *)strPathInfo.c_str(), nPathLen);

    nLen = DataSave.size();
    ModifyVertorData(DataSave, 0, (char*)&nLen, sizeof(nLen));
    if (nLen > 0) 
    {
        *ppData = new char[nLen];
        CopyVertorToPointer(DataSave, *ppData);
    }
    return nLen;
}


void COptionsExpressionWidget::SetRawData(char* pData, int nLen)
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

        m_strFilePath = QString::fromStdWString(utils::ansi2unicode(pPathInfo));
        delete[] pPathInfo;
        pPathInfo = NULL;
    }
}

/*************************************
* ��������getCurrentGroupManagerID
* ����: ��ȡ��ǰ��ID
* ����: ��
* ����ֵ: int ID
* ʱ��: 2017/12/28
* ����: jianjie
*/
int COptionsExpressionWidget::getCurrentGroupManagerID()
{
    int nRet = -1;

    do 
    {
        if (-1 == m_nGroupSelectItem)
        {
            break;
        }

        if (NULL == m_pGroupManager)
        {
            break;
        }
        CActionBlockGroupManager* pGroupManager = m_pGroupManager->GetAt(m_nGroupSelectItem);
        if (pGroupManager)
        {
            nRet = pGroupManager->GetID();
        }

    } while (0);

    return nRet;
}