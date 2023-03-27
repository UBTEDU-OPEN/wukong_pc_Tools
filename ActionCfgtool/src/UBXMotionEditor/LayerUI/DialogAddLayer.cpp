
#include <QCheckBox>
#include "DialogAddLayer.h"
#include "ui_DialogAddLayer.h"
#include "stable.h"
#include "util.h"
#include "robotdef.h"

CDialogAddLayer::CDialogAddLayer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDialogAddLayer)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    for (int i = ACTION_LAYER_MOTION; i < ACTION_MAX_LAYER_COUNT; i++)
    {
        // Add by jianjie 2017/10/30 mini暂时不加眼睛、耳朵、嘴巴层
        if (   ACTION_LAYER_EYE     == i
            || ACTION_LAYER_EAR     == i
            || ACTION_LAYER_MOUTH   == i)
        {
            continue;
        }
        QString strLayerName = GetLayerNameByType(i);
        QCheckBox* pCheckBox = new QCheckBox(strLayerName);
        connect(pCheckBox , SIGNAL(stateChanged(int)) , this , SLOT(slotCheckState(int)));
        //        bool bRet = IsLayerExist(i);
        //        pCheckBox->setChecked(bRet);
        ui->vLayoutLayer->addWidget(pCheckBox);
        m_mapCheckBox.insert(pCheckBox, i);
    }
    //    ui->listWidgetLayer->setSelectionMode(QAbstractItemView::ContiguousSelection);

    //    ui->listWidgetLayer->addItem(tr("Music Layer"));
    //    ui->listWidgetLayer->addItem(tr("Eye Layer"));
    //    ui->listWidgetLayer->addItem(tr("ear Layer"));
    //    QListWidgetItem * pItem = ui->listWidgetLayer->item(0);
    //    if (pItem)
    //    {
    //        pItem->setData(LayerType, 1);
    //    }
}

CDialogAddLayer::~CDialogAddLayer()
{
    for (int i = 0; i < ui->vLayoutLayer->count(); i++)
    {
        QCheckBox* pCheckBox =  (QCheckBox*)(ui->vLayoutLayer->itemAt(i)->widget());
        if (pCheckBox)
        {
            ui->vLayoutLayer->removeWidget(pCheckBox);
            m_mapCheckBox.remove(pCheckBox);
            SAFE_DELETE(pCheckBox);
        }

    }

    delete ui;
}

/**************************************************************************
* 函数名：InitLayerList
* 功能:初始化层列表
* 参数: wu
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CDialogAddLayer::InitLayerList(int** pLayerID, int& nLayerCount)
{
    while (!m_vUnUseID.isEmpty())
    {
        m_vUnUseID.removeFirst();
    }

    for (int i = 0; i < nLayerCount; i ++)
    {
        m_vUnUseID.push_back((*pLayerID)[i]);

    }

    for (int i = 0; i < ui->vLayoutLayer->count(); i++)
    {
        QCheckBox* pCheckBox =  (QCheckBox*)(ui->vLayoutLayer->itemAt(i)->widget());
        int nLayerType = m_mapCheckBox.value(pCheckBox);
        bool bRet = IsLayerExist(nLayerType);
        pCheckBox->setChecked(bRet);
    }
    //        for (int i = ACTION_LAYER_EYE; i < ACTION_MAX_LAYER_COUNT; i++)
    //        {
    //            QString strLayerName = GetLayerNameByType(i);
    //            QCheckBox* pCheckBox = new QCheckBox(strLayerName);
    //            bool bRet = IsLayerExist(i);
    //            pCheckBox->setChecked(bRet);
    //            ui->vLayoutLayer->addWidget(pCheckBox);
    //        }


    if (*pLayerID)
    {
        SAFE_DELETE_ARRAY(*pLayerID);
    }
}

/**************************************************************************
* 函数名：GetSelectLayer
* 功能:获取选择的层
* 参数:
*    @[in ] ppIDs: 具体选择的层
*    @[in ] nCount: 选择层的数量
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CDialogAddLayer::GetSelectLayer(int **ppIDs, int &nCount)
{
    vector <int> selectLayer;

    // 全身层动作和分层动作是否被选中
    bool bTotalAction = false;
    bool bLayerAction = false;

    for (int i = 0; i < ui->vLayoutLayer->count(); i++)
    {
        QCheckBox* pCheckBox =  (QCheckBox*)(ui->vLayoutLayer->itemAt(i)->widget());
        if (pCheckBox->isChecked())
        {
            int nLayerID = m_mapCheckBox.value(pCheckBox);
            selectLayer.push_back(nLayerID);
        }
    }
    nCount = selectLayer.size();
    if (nCount)
    {
        *ppIDs = new int[nCount];
    }
    else
    {
        *ppIDs = NULL;
    }
    for (int i = 0; i< selectLayer.size(); i++)
    {
        (*ppIDs)[i] = selectLayer[i];
    }
}

/**************************************************************************
* 函数名：GetLayerNameByType
* 功能:根据层的类型获取Layer名字
* 参数:
*    @[in ] nType: 层的类型
* 返回值: QString
* 时间: 2017/1/16
* 作者: Joker
*/
QString CDialogAddLayer::GetLayerNameByType(int nType)
{
    QString strLayerName = "";
    switch (nType)
    {
    case ACTION_LAYER_MOTION:
        strLayerName = tr("Motion Layer");
        break;
    case ACTION_LAYER_HEAD:
        strLayerName = tr("Head Layer");
        break;
    case ACTION_LAYER_HAND:
        strLayerName = tr("Hand Layer");
        break;
    case ACTION_LAYER_LEG:
        strLayerName = tr("Leg Layer");
        break;
    case ACTION_LAYER_MUSIC:
        strLayerName = tr("Music Layer");
        break;
    case ACTION_LAYER_EAR:
        strLayerName = tr("Ear Layer");
        break;
    case ACTION_LAYER_EYE:
        strLayerName = tr("Eye Layer");
        break;
    case ACTION_LAYER_MOUTH:
        strLayerName = tr("Mouth Layer");
        break;
    case ACTION_LAYER_EXPRESSION:
        strLayerName = tr("Expression Layer");
        break;
    default:
        break;
    }
    return strLayerName;
}

bool CDialogAddLayer::IsLayerExist(int nType)
{
    for(int i = 0; i < m_vUnUseID.count(); i++)
    {
        if (nType == m_vUnUseID[i])
        {
            return false;
        }
    }
    return true;
}

/*************
* 函数名:paintEvent
* 功能:重绘事件
* 参数:
* 返回值:
* 时间: 2016/06/01
* 作者: 周志平
*****************/
void CDialogAddLayer::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CDialogAddLayer::on_pbClose_clicked()
{
    this->reject();
}

void CDialogAddLayer::on_pbCancel_clicked()
{
    this->reject();
}

void CDialogAddLayer::on_pbOK_clicked()
{
    this->accept();
}


QCheckBox* CDialogAddLayer::findCheckBoxByID(int nLayerID)
{
    QCheckBox* pCheckBox = NULL;
    QMap <QCheckBox*, int>::const_iterator itor = m_mapCheckBox.begin();
    for (; itor != m_mapCheckBox.end(); ++itor)
    {
        if (nLayerID == itor.value())
        {
            pCheckBox = itor.key();
            break;
        }
    }

    return pCheckBox;
}

// Add by jianjie 2017/10/24 全身动作和分层动作进行互斥
void CDialogAddLayer::slotCheckState(int nCheckState)
{
    QCheckBox* pCheckBox = qobject_cast<QCheckBox*>(sender());
    if (!pCheckBox)
    {
        return;
    }

    int nLayerType = m_mapCheckBox.value(pCheckBox);

    bool bMotionChecked = false;
    bool bHeadChecked = false;
    bool bHandChecked = false;
    bool bLegChecked  = false;

    QCheckBox* pMotionCheck = findCheckBoxByID(ACTION_LAYER_MOTION);
    if (pMotionCheck)
    {
        bMotionChecked = pMotionCheck->isChecked();
    }

    QCheckBox* pHeadCheck = findCheckBoxByID(ACTION_LAYER_HEAD);
    if (pCheckBox)
    {
        bHeadChecked = pHeadCheck->isChecked();
    }

    QCheckBox* pHandCheck = findCheckBoxByID(ACTION_LAYER_HAND);
    if (pHandCheck)
    {
        bHandChecked = pHandCheck->isChecked();
    }

    QCheckBox* pLegCheck = findCheckBoxByID(ACTION_LAYER_LEG);
    if (pLegCheck)
    {
        bLegChecked = pLegCheck->isChecked();
    }

    // 去掉动作层(复原操作)
    //if (ACTION_LAYER_MOTION == nLayerType && !bMotionChecked)
    //{
    //    if (pCheckBox)
    //    {
    //        pCheckBox->setChecked(true);
    //    }
    //}
    if (!bMotionChecked && !bHeadChecked && !bHandChecked && !bLegChecked)
    {
        if (pMotionCheck)
        {
            pMotionCheck->setChecked(true);
        }
    }

    // 添加分层动作时，删掉全动作层
    if (bMotionChecked
        && nCheckState
        && (ACTION_LAYER_HEAD == nLayerType || ACTION_LAYER_HAND == nLayerType || ACTION_LAYER_LEG == nLayerType))
    {
        if (pMotionCheck)
        {
            pMotionCheck->setChecked(false);
        }
    }

    // 添加全动作层时，删掉分层动作
    if ((bHeadChecked || bHandChecked || bLegChecked)
        && nCheckState
        && ACTION_LAYER_MOTION == nLayerType)
    {
        if (pHeadCheck)
        {
            pHeadCheck->setChecked(false);
        }

        if (pHandCheck)
        {
            pHandCheck->setChecked(false);
        }

        if (pLegCheck)
        {
            pLegCheck->setChecked(false);
        }
    }
}