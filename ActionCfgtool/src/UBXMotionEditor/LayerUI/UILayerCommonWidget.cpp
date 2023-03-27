/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CUILayerCommonWidget.cpp
* 创建时间：2017/01/17 10:15
* 文件标识：
* 文件摘要：公共层UI创建
*
* 当前版本：1.0.0.0
* 作    者：Joker
* 完成时间：2017/01/17
* 版本摘要：
*/
#include "UILayerCommonWidget.h"
#include "ui_UILayerCommonWidget.h"
#include "IUILayerManager.h"
#include "UILayerImpl.h"
#include "UIMotionLayer.h"
#include "UIEarLedLayer.h"
#include "UIEyeLedLayer.h"
#include "UIMusicLayer.h"
#include "UIExpressionLayer.h"
#include <QPen>
#include <QBrush>
#include <QScrollBar>
#include <QMenu>
#include <QDir>
#include <QMessageBox>
#include "util.h"
#include "configs.h"

CUILayerCommonWidget::CUILayerCommonWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LayerCommonWidget)
    //    , m_waveLayer(this)
    //    , m_actionLayer(this)
    //    , m_earLedLayer(this)
    //    , m_eyeLedLayer(this)
    //    , m_musicLayer(this)
{    
    ui->setupUi(this);

    m_pPixmapMem = NULL;
    //    m_pPixmapGroup = NULL;
    //    m_pPixmapWave = NULL;
    m_pPixmapBackGround = NULL;
    //    m_pEarPixmapGroup = NULL;
    //    m_pEyePixmapGroup = NULL;

    //    m_pActionDataManager = NULL;
    m_nCurrentGroupID = -1;
    m_nCurrentActionID = -1;
    m_pEvent = NULL;

    m_nCellCount = Constants::DEFAULT_CELL_COUNT;
    m_nCellWidth = Constants::DEFAULT_CELL_WIDTH;
    m_nLineThick = Constants::DEFAULT_LINE_THICK;
    //    m_nCellHeight = Constants::DEFAULT_CELL_HEIGHT;
    m_nCellTotalSeconds = Constants::DEFAULT_CELL_COUNT;//初始cellcount以秒为单位

    m_nTimeCellWidth = Constants::DEFAULT_CELL_WIDTH;
    m_nTimeCellHeight = Constants::DEFAULT_CELL_TIME_HEIGHT;
    m_nMusicCellWidth = Constants::DEFAULT_CELL_WIDTH;
    m_nMusicCellHeight = Constants::DEFAULT_CELL_MUSIC_HEIGHT;

    m_nStartX = 0;
    m_nStartY = 0;
    m_nCellStartIndex = 0;

    m_nNewUILayerStartY = m_nTimeCellHeight + m_nMusicCellHeight;

    m_nCurCol = 1;
    m_nSelLineLength = 500;

//    m_nPixelPerSec = m_nCellWidth + m_nLineThick;
    m_timeLevel = ss_1;
    m_nStartMilliSecs = 0;
    m_nEndMilliSecs = 0;
    m_pMP3Player = NULL;
    //m_pMP3Thread = NULL;

    m_timeScale = 0; //m_timeScale这里的初始化没有意义，需要从外面传进来

    m_bForwards = false;
    m_nWheelCount = 0;

    connect(this, &CUILayerCommonWidget::SigReDrawAll, this, &CUILayerCommonWidget::ReDrawAll);
    connect(this, &CUILayerCommonWidget::SigRedDrawTimeAndAction, this, &CUILayerCommonWidget::OnReDrawTimeAndAction);

    m_bIsPlaying = false;
    m_bIsSinglePlaying = false;
    m_nCurChosenAction = NO_ACTION_CHOSEN;
    m_nPasteType = NO_PASTE;

    m_strMp3Path = "";
    m_strWavPath = "";
    m_pIUILayerManager = NULL;
    m_nCurSelXStore = 0;

    setAcceptDrops(true);
    m_dragMode = -1;
    //时间层专用变量
    m_nDivisor = 5;
    ui->btnZoomIn->hide();
    ui->btnZoomOut->hide();
}

CUILayerCommonWidget::~CUILayerCommonWidget()
{    
    SAFE_DELETE(ui);
    SAFE_DELETE(m_pMP3Player);
    //暂时还不清楚要不要删除，有可能会崩溃
    DeleteAllUILayer();
}

/**************************************************************************
* 函数名: DeleteAllUILayer
* 功能:删除所有层的UI
* 参数:
* 返回值:
* 时间:2015/12/25 18:13
* 作者: 周志平
**************************************************************************/
void CUILayerCommonWidget::DeleteAllUILayer()
{
    for (int i=0; i<m_listUILayer.count(); i++)
    {
        CUILayerImpl* pUILayer = m_listUILayer.at(i);
        if (pUILayer)
        {
            SAFE_DELETE(pUILayer);
        }
    }
    m_listUILayer.clear();
    //刷新新建层开始位置
    m_nNewUILayerStartY = m_nTimeCellHeight + Constants::DEFAULT_CELL_MUSIC_HEIGHT;
}

bool CUILayerCommonWidget::Connect(IUILayerManager* pParent, IUILayerManager_Event* pEvent)
{
    m_pIUILayerManager = pParent;
    m_pEvent = pEvent;
    //    initUILayer();
    return true;
}

void CUILayerCommonWidget::ReleaseConnection()
{
    //to do
}

///**************************************************************************
//* 函数名：SetOptionHandlerEvent
//* 功能:设置编辑选项事件对象指针
//* 参数:
//*    @[in ] pEvent:编辑选项事件对象指针
//* 返回值: 无
//* 时间: 2017/1/16
//* 作者: Joker
//*/
//void CUILayerCommonWidget::SetOptionHandlerEvent(ILayerOptionHandler_Event *pEvent)
//{
//    m_pEvent = pEvent;
//}

/**************************************************************************
* 函数名：GetWidget
* 功能:获取窗体指针
* 参数: 无
* 返回值: QWidget 返回窗体的指针
* 时间: 2017/1/16
* 作者: Joker
*/
QWidget *CUILayerCommonWidget::GetWidget()
{
    return this;
}

/**************************************************************************
* 函数名：GetTimeScale
* 功能: 获取单刻度时间粒度
* 参数: 无
* 返回值: 返回时间粒度
* 时间: 2016/12/09
* 作者: Joker
*/
int CUILayerCommonWidget::GetTimeScale()
{
    return m_timeScale;
}

/**************************************************************************
* 函数名：UpdateLayerAction
* 功能: 根据组ID和总时间更新LayerUI
* 参数:
*    @[in ] nGroupID: 组ID
*    @[in ] nStartTime: 组开始时间
*    @[in ] nType: 层的类型
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerCommonWidget::UpdateLayerAction(int nGroupID, int nStartTime, emActionLayerType nType)
{
    //先清除所有选中状态
    for (int i = 0; i< m_listUILayer.count(); i++)
    {
        CUILayerImpl* pUILayer = m_listUILayer.at(i);
        if (pUILayer)
        {
            pUILayer->ClearSelectGroupBlock();
            pUILayer->SetDrawBlockAction(false);
        }
    }

    m_nCurrentGroupID = nGroupID;
    CUILayerImpl* pUILayer = FindUILayerByLayerType(nType);
    if (pUILayer)
    {
        pUILayer->ClearSelectGroupBlock();
        pUILayer->SetSelectGroupBlockID(nGroupID);
        pUILayer->SetDrawBlockAction(true);
    }
    //    m_actionLayer.ClearSelectGroupBlock();
    //    m_actionLayer.SetSelectGroupBlockID(nGroupID);
    //    m_actionLayer.SetDrawBlockAction(true);
    if(NULL == m_pMP3Player ||!m_pMP3Player->IsPlaying())
    {
        int nEndTime = GetCellEndIndex();
        m_nCurCol = nStartTime + 1;
        //往时间增加的方向选择
        if(nStartTime >= nEndTime)
        {
            m_nCellStartIndex = nStartTime;
            emit SigCellIndexChanged(m_nCellStartIndex);
            emit SigReDrawAll();
        }
        //往时间小的方向选择
        else if(nStartTime <= m_nCellStartIndex)
        {
            m_nCellStartIndex = nStartTime;
            emit SigCellIndexChanged(m_nCellStartIndex);
            emit SigReDrawAll();
        }
        else//其他情况只需要重绘时间线和动作组选中状态
        {
            emit SigRedDrawTimeAndAction();
        }
    }
}

/**************************************************************************
* 函数名：UpdateLayerUIByCol
* 功能: 根据当前时间更新LayerUI
* 参数:
*    @[in ] nCol: 当前时间刻度
*    @[in ] nType: 层类型
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerCommonWidget::UpdateLayerUIByCol(int nCol)
{
    //    if(NULL == m_pMP3Player || !m_pMP3Player->IsPlaying())
    //    {
    //        int startIndex = (nCol * m_timeScale * GetTimeScale()) / (m_nCellWidth + m_nLineThick);
    //        m_nCurCol = nCol * m_timeScale * GetTimeScale();
    //        if(startIndex >= GetCellEndIndex())
    //        {
    //            m_nCellStartIndex = GetCellEndIndex();
    //            emit SigCellIndexChanged(m_nCellStartIndex);
    //            emit SigReDrawAll();
    //        }
    //        else if(startIndex <= m_nCellStartIndex)
    //        {
    //            m_nCellStartIndex = startIndex;
    //            emit SigCellIndexChanged(m_nCellStartIndex);
    //            emit SigReDrawAll();
    //        }
    //        else
    //        {
    //            emit SigRedDrawTimeAndAction();
    //        }
    //    }

    int startIndex = nCol-1;//开始时间是格数减1，即第一格开始时间是0~1
    m_nCurCol = nCol;
    if(startIndex >= GetCellEndIndex())
    {
        m_nCellStartIndex = GetCellEndIndex();
        emit SigCellIndexChanged(m_nCellStartIndex);
        emit SigReDrawAll();
    }
    else if(startIndex <= m_nCellStartIndex)
    {
        m_nCellStartIndex = startIndex;
        emit SigCellIndexChanged(m_nCellStartIndex);
        emit SigReDrawAll();
    }
    else
    {
        emit SigRedDrawTimeAndAction();
    }

}

/**************************************************************************
* 函数名：UpdateLayerWidget
* 功能: 通知LayerUI更新界面
* 参数:
*    @[in ] pActionBlockDataManger: 层数据管理
*    @[in ] nCurrentGroupID: 当前组ID
*    @[in ] nCurrentActionID: 当前块ID
*    @[in ] nLayerType: 层类型
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerCommonWidget::UpdateLayerWidget(CActionBlockDataManager *pActionBlockDataManger, int nCurrentGroupID, int nCurrentActionID, emActionLayerType nLayerType)
{
    //先清除所有选中状态
    for (int i = 0; i< m_listUILayer.count(); i++)
    {
        CUILayerImpl* pUILayer = m_listUILayer.at(i);
        if (pUILayer)
        {
            pUILayer->ClearSelectGroupBlock();
            pUILayer->SetDrawBlockAction(false);
        }
    }

    CUILayerImpl* pUILayer = FindUILayerByLayerType(nLayerType);
    if (pUILayer)
    {
        pUILayer->SetActionBlockDataManager(pActionBlockDataManger);
        pUILayer->ClearSelectGroupBlock();
        if (nCurrentGroupID != -1)
        {
            pUILayer->SetSelectGroupBlockID(nCurrentGroupID);
        }
        pUILayer->SetDrawBlockAction(true);
    }
    m_nCurrentGroupID = nCurrentGroupID;
    m_nCurrentActionID = nCurrentActionID;

    //根据m_nCellCount修改左右拉动条范围
    emitRangeChanged();

    emit SigReDrawAll();
}

void CUILayerCommonWidget::SetCellCount(int nCount)
{
    m_nCellCount = nCount;
}

int CUILayerCommonWidget::GetCellCount()
{
    return m_nCellCount;
}

//bool CUILayerCommonWidget::IsMusicLoaded()
//{
//    return m_waveLayer.IsLoaded();
//}

void CUILayerCommonWidget::SetPlaying(bool play)
{
    m_bIsPlaying = play;
}

void CUILayerCommonWidget::SetSinglePlaying(bool play)
{
    m_bIsSinglePlaying = play;
}

///**************************************************************************
//* 函数名：initUILayer
//* 功能:初始化各个层单独的UI,这里本应该是根据Layer的实际数量增加的，但是现在
//      暂时为了简单起见，直接把所有层都添加了
//* 参数: 无
//* 返回值: 无
//* 时间: 2017/1/16
//* 作者: Joker
//*/
//void CUILayerCommonWidget::initUILayer()
//{
////    if (!m_pIUILayerManager)
////    {
////        return;
////    }
////    for (int i =0; i< m_pIUILayerManager->GetLayerCount(); i++)
////    {
////        emActionLayerType nActionType = m_pIUILayerManager->GetLayerTypeByIndex(i);
////        if (nActionType == ACTION_LAYER_UNKNOWN || nActionType == ACTION_LAYER_TIME)
////        {
////            continue;
////        }
////        AddUILayer(nActionType);
////    }
//}

/**************************************************************************
* 函数名：AddUILayer
* 功能:根据层的类型创建层UI
* 参数:
*    @[in ] nType: 层类型
* 返回值: 创建成功返回true，否则返回false
* 时间: 2017/1/16
* 作者: Joker
*/
bool CUILayerCommonWidget::AddUILayer(emActionLayerType nType)
{
    CUILayerImpl* pUILayer = NULL;

    if (nType == ACTION_LAYER_MOTION)
    {
        pUILayer = new CUIMotionLayer(this);
    }
    else if (nType == ACTION_LAYER_EYE)
    {
        pUILayer = new CUIEyeLedLayer(this);
    }
    else if (nType == ACTION_LAYER_EAR)
    {
        pUILayer = new CUIEarLedLayer(this);
    }
    else if (nType == ACTION_LAYER_MUSIC)
    {
        pUILayer = new CUIMusicLayer(this);
    }
    else if (nType == ACTION_LAYER_HAND)
    {
        pUILayer = new CUIHandMotionLayer(this);
    }
    else if (nType == ACTION_LAYER_LEG)
    {
        pUILayer = new CUILegMotionLayer(this);
    }
    else if (nType == ACTION_LAYER_HEAD)
    {
        pUILayer = new CUIHeadMotionLayer(this);
    }
    else if (nType == ACTION_LAYER_EXPRESSION)
    {
        pUILayer = new CUIExpressionLayer(this);
    }
    else
    {
        return false;
    }

    if (pUILayer == NULL)
        return false;
    //这里暂时没有做层重复性进行判断
    return AddUILayer(pUILayer);
}

/**************************************************************************
* 函数名：FindUILayerByLayerType
* 功能:根据层的类型获取相应的UILayer
* 参数:
*    @[in ] nType: 层类型
* 返回值: CUILayerImpl* 返回相应的UILayer对象
* 时间: 2017/1/16
* 作者: Joker
*/
CUILayerImpl *CUILayerCommonWidget::FindUILayerByLayerType(emActionLayerType nType)
{
    CUILayerImpl* itemFound = NULL;

    for (int i=0; i<m_listUILayer.count(); i++)
    {
        itemFound = m_listUILayer.at(i);
        if (itemFound && itemFound->GetType() == nType)
        {
            return itemFound;
        }
    }
    return NULL;
}

void CUILayerCommonWidget::OnScrollBarValueChanged(int value)
{
    m_nCellStartIndex = value/* / (m_nCellWidth + m_nLineThick)*/;
    emit SigReDrawAll();
}

void CUILayerCommonWidget::OnPositionChanged(quint64 position)
{
    UpdateTimeLineWidthMusic(position);
    //    if(m_pEvent)
    //    {
    //        m_pEvent->ILayerOptionHandler_UpdateMusicPosition(position);
    //    }
}

/**************************************************************************
* 函数名: LoadMusic
* 功能: 加载音乐数据
* 参数:
*    @[in ] const QString& strFilePath: 音乐文件路径
* 返回值: 成功返回true，失败返回false
* 时间: 2015/12/08 16:16
* 作者: ZDJ
*/
bool CUILayerCommonWidget::LoadMusic(const QString& strFilePath)
{
    bool bRet = false;
    //leo add 20170314 uncertainty
    m_strMp3Path = strFilePath;
//    m_strMp3Path = pszFilePath;
    CUILayerImpl* pUILayer = FindUILayerByLayerType(ACTION_LAYER_MUSIC);
    if (pUILayer)
    {
        bRet = ((CUIMusicLayer*)pUILayer)->LoadMusic(strFilePath);
    }
    if (bRet)
    {
        m_strMp3Path = strFilePath;
    }

    return bRet;
}

/**************************************************************************
* 函数名：MusicHasLoaded
* 功能:音乐加载完成后界面处理
* 参数:
*    @[in ] nTime: 波形总时间
*    @[in ] strWavePath: 波形保存的路径
*    @[in ] strTitle: 音乐标题
* 返回值: bool 处理完返回true，处理出现错误返回false
* 时间: 2017/1/16
* 作者: Joker
*/
bool CUILayerCommonWidget::MusicHasLoaded(int nTime, QString strWavePath, QString strTitle)
{
    //    m_nCellCount = nTime + 1;
    m_strWavPath = strWavePath;
    //    m_nCellTotalSeconds = m_nCellCount;
    //    m_timeLevel = ss_1;
    //    emitRangeChanged();
    //    m_nEndMilliSecs = strWavePath * 1000;//转为毫秒
    //    updateCellCount();
    if(m_pEvent)
    {
        m_pEvent->LayerUIEvent_MusicHasLoaded(m_strMp3Path, strTitle, nTime);
    }
    QFileInfo fileInfo(m_strMp3Path);
    QString strMusicPathFile = CConfigs::getUserDirPath(USERDIR_MUSIC) + QDir::separator() + fileInfo.fileName();
    if(strMusicPathFile.compare(m_strMp3Path) != 0)
    {
        // ClearDir(getUserDirPath(USERDIR_MUSIC));
        QFile::copy(m_strMp3Path, CConfigs::getUserDirPath(USERDIR_MUSIC) + QDir::separator() + fileInfo.fileName());
    }
    //emit SigNotifyLoadedMusic();
    return true;
}

/**************************************************************************
* 函数名：MusicHasReleased
* 功能:音乐资源释放后界面处理
* 参数:
*    @[in ] sucessed: 资源是否释放
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerCommonWidget::MusicHasReleased()
{
    ClearDir(CConfigs::getUserDirPath(USERDIR_MUSIC));
    m_nCellCount = 0;
    m_nCellTotalSeconds = 0;
    m_timeLevel = ss_1;
    emitRangeChanged();
    m_nEndMilliSecs = 0;//转为毫秒
    //        if(m_pEvent)
    //        {
    //            m_pEvent->ILayerOptionHandler_DeleteMusic();
    //            m_pEvent->ILayerOptionHandler_MusicLoaded("", "", 0);
    //        }
    emit SigNotifyReleasedMusic();
}

//int CUILayerCommonWidget::GetCellWidth()
//{
//    return m_nCellWidth;
//}

/**************************************************************************
* 函数名：GetLineThick
* 功能: 获取线的厚度
* 参数: 无
* 返回值: 返回线的厚度
* 时间: 2017/1/16
* 作者: Joker
*/
int CUILayerCommonWidget::GetLineThick()
{
    return m_nLineThick;
}

//int CUILayerCommonWidget::GetCurSelectX()
//{
//    return m_nCurCol;
//}

/**************************************************************************
* 函数名：GetCurrentSelectCol
* 功能: 根据当前位置获取当前时间(单位是格)
* 参数: 无
* 返回值: 返回当前时间刻度
* 时间: 2017/1/16
* 作者: Joker 使用该函数时，请确保m_nCurSelX是最新的，如果不是最新，需要先调用GetCurCellByPos
*/
int CUILayerCommonWidget::GetCurrentSelectCol()
{
    //    m_nCurCol = m_nCellStartIndex * (m_nCellWidth + m_nLineThick) + point.x();
    //    double endIndex = (double)m_nCurCol / (double)(m_nCellWidth + m_nLineThick);
    //    int ret = (double)GetCurrentms(endIndex) / (double)m_timeScale;
    //    return ret;
    return m_nCurCol;
}

/**************************************************************************
* 函数名：SetCurrentSelectCol
* 功能: 设置当前位置的时间(单位是格)
* 参数: 无
* 返回值: 返回当前时间刻度
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerCommonWidget::SetCurrentSelectCol(int nCol)
{
    m_nCurCol = nCol;
}

//long CUILayerCommonWidget::GetCurrentms(int nCol)
//{
//    //转换成ms然后返回
//    switch(m_timeLevel)
//    {
//    case ddd_10:
//        return curIndex * 10.0;
//    case ddd_50:
//        return curIndex * 50.0;
//    case ddd_100:
//        return curIndex * 100 ;
//    case ss_1:
//        return curIndex * 1000.0;
//    case ss_5:
//        return curIndex * 5000.0;
//    case ss_10:
//        return curIndex * 10000.0;
//    case mm_1:
//        return curIndex * 60000.0;
//    case mm_5:
//        return curIndex * 300000.0 ;
//    case mm_10:
//        return curIndex * 600000.0;
//    default:
//        break;
//    }
//    return nCol * m_timeScale;
//}

void CUILayerCommonWidget::paintEvent(QPaintEvent *event)
{
    if(NULL != m_pPixmapMem)
    {
        QPainter painter(this);
        QRect rcWnd(0, 0, this->width(), this->height());
        painter.drawPixmap(rcWnd, *m_pPixmapMem, rcWnd);
    }
}

void CUILayerCommonWidget::wheelEvent(QWheelEvent *event)
{
    event->ignore();//不处理，让父窗体处理，实现滚动条功能
}

void CUILayerCommonWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //AdjustCellCount();
    emitRangeChanged();
    emit SigReDrawAll();
}

void CUILayerCommonWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint curPoint = event->pos();
    emActionLayerType nLayerType = GetLayerTypeByPos(curPoint);
    if (nLayerType == ACTION_LAYER_UNKNOWN || nLayerType == ACTION_LAYER_TIME)
    {
        return;
    }
    int nCol = GetCurCellByPos(curPoint);
    emit SigUpdateScaleAndRealTime(m_timeScale, nCol);
    if (m_pIUILayerManager == NULL)
    {
        return;
    }
    if(event->button() == Qt::LeftButton)
    {
        //通知LayerManager
        m_pIUILayerManager->OnLayerLButtonDown(nLayerType, nCol);
    }
    else if(event->button() == Qt::RightButton)
    {
        m_pIUILayerManager->OnLayerRButtonDown(nLayerType, nCol);
        m_nCurChosenAction =  NO_ACTION_CHOSEN;
        CreateMotionLayerGroupListPopMenuActions();
        if (m_nCurChosenAction != NO_ACTION_CHOSEN)
        {
            m_pIUILayerManager->OnDoMenuCommand(nLayerType, m_nCurChosenAction);
        }
        ////                    m_pEvent->Ilayeroptionhandler_MotionlayerActionCommand(m_nCurChosenAction, nCol);

    }
}

void CUILayerCommonWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPoint curPoint = event->pos();
    emActionLayerType nLayerType = GetLayerTypeByPos(curPoint);

    int nCol = GetCurCellByPos(curPoint);

    if (m_pIUILayerManager)
    {
        m_pIUILayerManager->OnLayerMouseMove(nLayerType , nCol);
    }

}

void CUILayerCommonWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPoint curPoint = event->pos();
    if(event->button() == Qt::LeftButton)
    {
        emit SigLButtonDoubleClick(GetCurCellByPos(curPoint));
    }
}

//leo add 20170228 test
emActionLayerType CUILayerCommonWidget::getDropDataType(const char* pData, const int nLen)
{
    emActionLayerType eActionType = ACTION_LAYER_UNKNOWN;
    int nActionTypePos = sizeof(int) * 2 + strlen(UBX_TEMPLATE_ACTION_TAG); //文件总长度，文件头长度，文件头
    if (pData == NULL || nLen < (sizeof(nActionTypePos) + nActionTypePos))
        return eActionType;
    memcpy((int*)&eActionType, pData + nActionTypePos, sizeof(eActionType));
    return eActionType;
}

void CUILayerCommonWidget::dragEnterEvent(QDragEnterEvent *event)
{
    //qDebug("dragEnterEvent: enter drag mode ............");

    if(event->mimeData()->hasFormat(UBX_TEMPLATE_ACTION_TAG))
    {
        m_dragMode = 1;
        event->setDropAction(Qt::MoveAction);
        event->accept();
        return;
    }
    event->ignore(); 
}

void CUILayerCommonWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasFormat(UBX_TEMPLATE_ACTION_TAG))
    {
        m_dragMode = 1;
        // 设置可放下图片
        //        QPixmap drag_img(20, 18);
        //        QPainter painter(&drag_img);
        //        painter.drawText(QRectF(0,0, 20,18), NULL , QTextOption(Qt::AlignVCenter));
        //        drag->setPixmap(drag_img);
        QPoint curPoint = event->pos();
        emActionLayerType eLayerType = GetLayerTypeByPos(curPoint);
        QByteArray data = event->mimeData()->data(UBX_TEMPLATE_ACTION_TAG);
        emActionLayerType eDragDataType = getDropDataType(data.data(), data.length());
        if (eLayerType != ACTION_LAYER_UNKNOWN && eLayerType == eDragDataType)
        {
            event->acceptProposedAction();
            return;
        }
    }
    event->ignore(); 
}

void CUILayerCommonWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    //qDebug("exit topotree, exit drag mode ............");
    m_dragMode = 0;
    QWidget::dragLeaveEvent(event);
}

void CUILayerCommonWidget::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasFormat(UBX_TEMPLATE_ACTION_TAG))
    {
        QByteArray data = event->mimeData()->data(UBX_TEMPLATE_ACTION_TAG);
        //leo add 20170228 test
        QPoint curPoint = event->pos();
        emActionLayerType eLayerType = GetLayerTypeByPos(curPoint);
        emActionLayerType eDragDataType = getDropDataType(data.data(), data.length());
        if (eLayerType == ACTION_LAYER_UNKNOWN || eLayerType != eDragDataType)
        {
            return;
        }

        QString strName = "";
        if(event->mimeData()->hasText())
        {
            strName = event->mimeData()->text();
        }
        //leo add 20170228
        int nLen = data.length();
        char* pData = new char[nLen];
        // pData = data.data(); 注释老代码，这里并没有复制，只是copy一个指针

        //leo add 20170228 //修改后的附加数据
        memcpy(pData, data.data(), nLen);
        data.clear();

        int nSumLen = 0;
        UBX_MEMCPY_INC(nSumLen, pData);
        int nTagLen = 0;
        UBX_MEMCPY_INC(nTagLen, pData);
        //去Tag12
        char *pTag = new char[nTagLen + 1];
        memcpy(pTag, pData, nTagLen);
        pTag[nTagLen] = 0;
        int nRet = strncmp(pTag, UBX_TEMPLATE_ACTION_TAG, nTagLen + 1);
        SAFE_DELETE_ARRAY(pTag);

        if (nRet != 0)
            return ;
        pData += nTagLen;
        //解文件类型
        int nActionType = 0;
        UBX_MEMCPY_INC(nActionType, pData);
        nLen = nLen - sizeof(nSumLen) - sizeof(nTagLen) - nTagLen -sizeof(nActionType);
        //int nBlockLen = 0;
        //UBX_MEMCPY_INC(nBlockLen, pData);
        //leo end end之后应该是原生数据,
        //实测这里多了一个长度，数据需要再验证？？？
        //int nBlockLenRedundant = 0;
        //UBX_MEMCPY_INC(nBlockLenRedundant, pData);
        // end---------------------------------------------------------

        m_nCurCol = GetCurCellByPos(curPoint);
        //if(m_pEvent)
        //{
        //    //m_pEvent->Ilayeroptionhandler_TreeToView(pData, nLens, m_nCurCol, strName );
        //}

        if (m_pIUILayerManager)
        {
            m_pIUILayerManager->TreeToView(pData, nLen, eLayerType, m_nCurCol, strName);
        }

        return;
    }

    event->ignore();
    //return QWidget::dropEvent(event);
    //e/        QTreeWidget::dropEvent(event);
}

void CUILayerCommonWidget::DrawBackGround(QPixmap* pixmap)
{
    if(NULL == pixmap)
    {
        return;
    }

    QRect rectWidget = this->geometry();
    QRect rectTimeBG = rectWidget;
    rectTimeBG.setHeight(Constants::DEFAULT_CELL_TIME_HEIGHT);
    QBrush bgBrush(Constants::COLOR_MP3_BG);
    QPainter painter;
    painter.begin(pixmap);
    painter.fillRect(rectWidget, bgBrush);
    painter.fillRect(rectTimeBG, QBrush(Constants::COLOR_TIME_BG));
    painter.end();
    DrawTimeLayer(pixmap);
    //    DrawBottomCell(pixmap);
    //    DrawEarLedCell(pixmap);
    //    DrawEyeLedCell(pixmap);
}

//绘制显示的时间
void CUILayerCommonWidget::DrawTimeLayer(QPixmap* pixmap)
{
    if(NULL == pixmap)
    {
        return;
    }
    QPainter painter(pixmap);
    int hTmp3 = m_nTimeCellHeight / 3;
    int xTmp = m_nStartX;
    //绘制选择线
    int nCount = GetCellEndIndex();

    //绘制时间刻度
    for(int i = m_nCellStartIndex + 1; i <= nCount; i++)
    {
        if (i > m_nCellCount)
        {
            break;
        }
        QPen pen(Constants::COLOR_LINE, m_nLineThick);
        painter.setPen(pen);

        //绘制时间文字
        //if((i + 1) % 2 == 0)
        {
            //刻度线
            painter.drawLine(xTmp, m_nTimeCellHeight - hTmp3,
                             xTmp, m_nTimeCellHeight);
            //绘制横线
            painter.drawLine(xTmp, m_nTimeCellHeight -1,
                             xTmp + m_nTimeCellWidth + m_nLineThick, m_nTimeCellHeight -1);

            //少画一个时间值，给放大和缩放，TODO:Joker 放大缩小已暂时隐藏
            if((i < nCount) && (i % m_nDivisor == 0))
            {
                QFont font;
                font.setPointSize(12);
                painter.setFont(font);
                QFontMetrics fm(font);

                //                QString strTime = GetTimeString(i+1);
                QString strTime = QString::number(i);
                int width = fm.width(strTime);
                int height = fm.height();
                QRect rect(xTmp + m_nTimeCellWidth +  m_nLineThick - width, 0,
                           width, height);
                painter.setPen(Constants::COLOR_TIME_FONT);
                painter.drawText(rect, strTime);
            }
        }
        xTmp += m_nLineThick + m_nTimeCellWidth;
    }
    //刻度线
    QPen pen(Constants::COLOR_LINE, m_nLineThick);
    painter.setPen(pen);
    painter.drawLine(xTmp, m_nTimeCellHeight - hTmp3,
                     xTmp, m_nTimeCellHeight);
}

//m_wavedata中是以s为单位存储波形点，因此需要根据TimeLevel进行换算
long CUILayerCommonWidget::GetDisplayStartTime()
{
    //    return GetCurrentms(m_nCellStartIndex);
    return m_nCellStartIndex * m_timeScale;
}

//m_wavedata中是以s为单位存储波形点，因此需要根据TimeLevel进行换算
long CUILayerCommonWidget::GetDisplayEndTime()
{
    //    int width = GetContentWidth();
    //    long nStartTime= GetDisplayStartTime();
    //    //当前width对应的cell个数
    //    long cellCount = (long)width / (long)(m_nCellWidth + m_nLineThick);
    //    return nStartTime + GetCurrentms(cellCount);
    int nEndCol = GetCellEndIndex();
    return nEndCol * m_timeScale;
}

/**************************************************************************
* 函数名：SetActionBlockDataManager
* 功能:设置UI层组的数据，这里只添加数据，不做画图
* 参数:
*    @[in ] pActionBlockDataManger: 组数据
*    @[in ] emActionLayerType: 层类型
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerCommonWidget::SetActionBlockDataManager(CActionBlockDataManager *pActionBlockDataManger, emActionLayerType nLayerType)
{
    CUILayerImpl* pUILayer = FindUILayerByLayerType(nLayerType);
    if (pUILayer)
    {
        pUILayer->SetActionBlockDataManager(pActionBlockDataManger);
    }
}

///**************************************************************************
//* 函数名：GetDrawInterval
//* 功能:根据TimeLevel计算波形取点间隔数,提供给WaveLayer使用
//* 参数:
//*    @[in ] nSampleRate: 采样率
//* 返回值: 无
//* 时间: 2017/1/16
//* 作者: hels ，modify by Joker
//*/
//int CUILayerCommonWidget::GetDrawInterval(int nSampleRate)
//{
//    //计算每秒取的采样点数
//    int nDrawInterval = (int)nSampleRate / m_nPixelPerSec;//由于一格中只有m_nPixelPerSec个像素
//    return nDrawInterval * m_timeScale / 1000;
//    //因此在采样点中每隔nDrawInterval取一个样本绘制
//}



/**************************************************************************
* 函数名：SetTimeScale
* 功能:设置时间粒度
* 参数:
*    @[in ] nScale: 时间粒度，由外部传进来
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerCommonWidget::SetTimeScale(int nScale)
{
    m_timeScale = nScale;
    emit SigUpdateScaleAndRealTime(nScale, m_nCurCol);
}

//绘制时间选择线,由于鼠标点击切换时间点时，不需要重绘全部内容，为提高效率，将时间线专门绘制
void CUILayerCommonWidget::DrawTimeSelLine(QPixmap* pixmap)
{
    if(NULL != pixmap)
    {
        QPainter painter;
        painter.begin(pixmap);
        int xPosition = (m_nCurCol - m_nCellStartIndex) * (m_nCellWidth + m_nLineThick) - m_nCellWidth/2;
        QPen penSelLine(Constants::COLOR_SEL_LINE, m_nLineThick);
        painter.setPen(penSelLine);
        QPixmap pixmapPointer(":/res/images/icon_pointer.png");
        painter.drawPixmap(xPosition - pixmapPointer.width() / 2, 0,
                           pixmapPointer.width(), pixmapPointer.height(),
                           pixmapPointer);
        painter.drawLine(xPosition, pixmapPointer.height(), xPosition, m_nTimeCellHeight + m_nSelLineLength);
        painter.end();
    }
}

/************************************
* 名称: GetCellEndIndex
* 功能: 获取widget能显示到的cell的索引，会根据widget宽度和startindex计算
* 返回:   int 返回索引值
* 时间:   2016/01/08
* 作者:   hels
************************************/
int CUILayerCommonWidget::GetCellEndIndex()
{
    QRect rectWidget = this->geometry();
    //widget一屏幕能显示的cell的个数 + 开始的cell index
    return rectWidget.width() / (m_nLineThick + m_nCellWidth) + m_nCellStartIndex;
}

/************************************
* 名称: GetTimeString
* 功能: 获取显示的时间字符串，根据hh:mm:ss和mm:ss.ddd的精度进行计算，参考Adobe Audtion软件
* 参数: [in]int nCurIndex当前的cell索引
* 返回:   QString 时间字符串
* 时间:   2016/01/08
* 作者:   hels
************************************/
QString CUILayerCommonWidget::GetTimeString(int nCurIndex)
{
    //默认处于显示秒级别 间隔一个刻度显示时间
    //毫秒级别      10  50  100
    //秒级别        1   5   10
    //分钟级别      1  5   10
    switch(m_timeLevel)
    {
    case ddd_10:
        return QString().sprintf("%02d:%02d:%02d.%03d",
                                 (int)(nCurIndex / 100) / 3600,
                                 ( (int)(nCurIndex / 100) % 3600 ) / 60,
                                 (int)(nCurIndex / 100) % 60,
                                 (int)(nCurIndex) % 100 * 10);
        break;
    case ddd_50:
        return QString().sprintf("%02d:%02d:%02d.%03d",
                                 (int)(nCurIndex / 20) / 3600,
                                 ( (int)(nCurIndex / 20) % 3600 ) / 60,
                                 (int)(nCurIndex / 20) % 60,
                                 (int)(nCurIndex) % 20 * 50);
        break;
    case ddd_100:
        return QString().sprintf("%02d:%02d:%02d.%03d",
                                 (int)(nCurIndex / 10) / 3600,
                                 ( (int)(nCurIndex / 10) % 3600 ) / 60,
                                 (int)(nCurIndex / 10) % 60,
                                 (int)(nCurIndex) % 10 * 100);
        break;
    case ss_1:
        return QString().sprintf("%02d:%02d:%02d",
                                 (int)(nCurIndex) / 3600,
                                 ( (int)(nCurIndex) % 3600 ) / 60,
                                 (int)(nCurIndex) % 60);
    case ss_5:
        return QString().sprintf("%02d:%02d:%02d",
                                 (int)(nCurIndex * 5) / 3600,
                                 ( (int)(nCurIndex * 5) % 3600 ) / 60,
                                 (int)(nCurIndex * 5) % 60);
    case ss_10:
        return QString().sprintf("%02d:%02d:%02d",
                                 (int)(nCurIndex * 10) / 3600,
                                 ( (int)(nCurIndex * 10) % 3600 ) / 60,
                                 (int)(nCurIndex * 10) % 60);
    case mm_1:
        return QString().sprintf("%02d:%02d:%02d",
                                 (int)(nCurIndex * 60) / 3600,
                                 ( (int)(nCurIndex * 60) % 3600 ) / 60,
                                 0);
    case mm_5:
        return QString().sprintf("%02d:%02d:%02d",
                                 (int)(nCurIndex * 60 * 5) / 3600,
                                 ( (int)(nCurIndex * 60 * 5) % 3600 ) / 60,
                                 0);
    case mm_10:
        return QString().sprintf("%02d:%02d:%02d",
                                 (int)(nCurIndex * 60 * 10) / 3600,
                                 ( (int)(nCurIndex * 60 * 10) % 3600 ) / 60,
                                 0);
    default:
        break;
    }
    return QString("");
}

int CUILayerCommonWidget::GetContentWidth()
{
    int nEndIndex = GetCellEndIndex();
    int width;
    if(nEndIndex >= m_nCellCount)
    {
        width = (m_nCellWidth + m_nLineThick) * (m_nCellCount - m_nCellStartIndex);
    }
    else
    {
        width = geometry().width();
    }
    return width;
}

//void CUILayerCommonWidget::TimeLevelChanged(TimeLevel timeLevel)
//{
//    //if(m_timeLevel != timeLevel)
//    {
//        //根据时间变化前需要先获取当前的时间点真实数据
//        //double time = GetCurrentms(m_nCellCount);
//        double time = m_nCellTotalSeconds * 1000;//转为ms
//        double endIndexTime = GetCurrentms((double)m_nCurCol / (double)(m_nCellWidth + m_nLineThick));

//        m_timeLevel = timeLevel;
//        switch(timeLevel)
//        {
//        case ddd_10:
//            m_nCellCount = qCeil(time / 10.0);
//            break;
//        case ddd_50:
//            m_nCellCount = qCeil(time / 50.0);
//            break;
//        case ddd_100:
//            m_nCellCount = qCeil(time / 100.0);
//            break;
//        case ss_1:
//            m_nCellCount = qCeil(time / 1000.0);
//            break;
//        case ss_5:
//            m_nCellCount = qCeil(time / 5000.0);
//            break;
//        case ss_10:
//            m_nCellCount = qCeil(time / 10000.0);
//            break;
//        case mm_1:
//            m_nCellCount = qCeil(time / 60000.0);
//            break;
//        case mm_5:
//            m_nCellCount = qCeil(time / 300000.0);
//            break;
//        case mm_10:
//            m_nCellCount = qCeil(time / 600000.0);
//            break;
//        default:
//            break;
//        }
//        //根据要缩放的时间做相关的变化
//        m_nCurCol = endIndexTime * GetXScalePerMilliSecond();
//        //m_nCellCount = (m_nCellCount == 0 ? 1 : m_nCellCount);
//        AdjustCellCount();
//        emitRangeChanged();
//        emit SigReDrawAll();
//    }
//}

///**************************************************************************
//* 函数名: PrepareMP3
//* 功能:
//* 参数:
//* 返回值:
//* 时间:
//* 作者:
//* 改动日志
//**************************************************************************/
//void CUILayerCommonWidget::PrepareMP3()
//{
//    if(m_waveLayer.IsLoaded())
//    {
//        if(NULL == m_pMP3Player)
//        {
//            m_pMP3Player = new MP3Player();
//            connect(m_pMP3Player, &MP3Player::SigPositionChanged, this, &CUILayerCommonWidget::OnPositionChanged);
//            connect(m_pMP3Player, &MP3Player::SigFinishPlaying, this, &CUILayerCommonWidget::SigFinishMusicPlaying);
//        }
//        m_pMP3Player->SetFileName(m_strWavPath);
//    }
//}

///**************************************************************************
//* 函数名: ResetMP3
//* 功能:删除MP3后，重置
//* 参数:
//* 返回值:
//* 时间:
//* 作者:周志平
//* 改动日志
//**************************************************************************/
//void CUILayerCommonWidget::ResetMP3()
//{

//    if(m_pMP3Player)
//    {
//        SAFE_DELETE(m_pMP3Player);
//    }
//    m_pMP3Player->SetFileName("");

//}


/**************************************************************************
* 函数名: emitRangeChanged
* 功能:
* 参数:
* 返回值:
* 时间:
* 作者: 周志平
* 改动日志
**************************************************************************/
void CUILayerCommonWidget::emitRangeChanged()
{
    //    emit SigRangeChanged(0, ((m_nCellCount - width() / (m_nCellWidth + m_nLineThick)) + 1) * (m_nCellWidth + m_nLineThick), m_nCellWidth + m_nLineThick);
    emit SigRangeChanged(0, (m_nCellCount - width() / (m_nCellWidth + m_nLineThick) + 1), 1);
}

/**************************************************************************
* 函数名: ClearDir
* 功能:
* 参数:
* 返回值:
* 时间:
* 作者: 周志平
* 改动日志
**************************************************************************/
void CUILayerCommonWidget::ClearDir(const QString &fullPath)
{
    if(fullPath.isEmpty())
    {
        return;
    }

    QDir dir(fullPath);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fi, fileList)
    {
        if(fi.isFile())
        {
            fi.dir().remove(fi.fileName());
        }
        else
        {
            ClearDir(fi.absoluteFilePath());
        }
    }
    //dir.rmpath(dir.absolutePath());
}

/**************************************************************************
* 函数名：GetCellWidth
* 功能:获取一格的宽度
* 参数: 无
* 返回值: int 返回一格的宽度
* 时间: 2017/1/16
* 作者: Joker
*/
int CUILayerCommonWidget::GetCellWidth()
{
    return m_nCellWidth;
}

/**************************************************************************
* 函数名：GetCellHeight
* 功能:获取一格的高度
* 参数: 无
* 返回值: int 返回一格的高度
* 时间: 2017/1/16
* 作者: Joker
*/
//int CUILayerCommonWidget::GetCellHeight()
//{
//    return m_nCellHeight;
//}

/**************************************************************************
* 函数名：GetMusicCellHeight
* 功能:获取音乐层一格所占的高度
* 参数: 无
* 返回值: int 返回音乐层高度
* 时间: 2017/1/16
* 作者: Joker
*/
//int CUILayerCommonWidget::GetMusicCellHeight()
//{
//    return m_nMusicCellHeight;
//}

/************************************
* 名称: GetCellStartIndex()
* 功能: 获取widget开始的cell的索引
* 返回:   int 返回索引值
* 时间:   2016/01/08
* 作者:   Joker
************************************/
int CUILayerCommonWidget::GetCellStartIndex()
{
    return m_nCellStartIndex;
}

//新UI如果cellcount太小，背景纯黑不好看，因此调整cellcount保证大于等于widget width
void CUILayerCommonWidget::AdjustCellCount()
{
    QRect rectWidget = this->geometry();
    int cellCount = rectWidget.width() / (m_nLineThick + m_nCellWidth);
    m_nCellCount = m_nCellCount > cellCount? m_nCellCount : cellCount;
}

void CUILayerCommonWidget::onReDrawAction()
{
    for (int i = 0; i < m_listUILayer.count(); i++)
    {
        CUILayerImpl* pUILayer = m_listUILayer.at(i);
        if(pUILayer)
        {
            pUILayer->DrawGroup();
        }

    }
}

/**************************************************************************
* 函数名: OnCopy
* 功能:动作组复制响应槽
* 参数:
* 返回值:
* 时间:
* 作者: 周志平
* 改动日志
**************************************************************************/
void CUILayerCommonWidget::OnCopy()
{
    m_nPasteType = PASTE_AFTER_COPY;
    m_nCurChosenAction = COPY_ACTION_CHOSEN;
}

/**************************************************************************
* 函数名: OnSelectedBlockPlay
* 功能:动作组粘贴响应槽
* 参数:
* 返回值:
* 时间:
* 作者: 周志平
* 改动日志
**************************************************************************/
void CUILayerCommonWidget::OnPaste()
{
    if(m_nPasteType == PASTE_AFTER_CUT)
    {
        m_nPasteType = NO_PASTE;
    }
    m_nCurChosenAction = PASTE_ACTION_CHOSEN;
}

/**************************************************************************
* 函数名: OnSelectedBlockPlay
* 功能:动作组剪切响应槽
* 参数:
* 返回值:
* 时间:
* 作者: 周志平
* 改动日志
**************************************************************************/
void CUILayerCommonWidget::OnCut()
{
    m_nPasteType = PASTE_AFTER_CUT;
    m_nCurChosenAction = CUT_ACTION_CHOSEN;
}

/**************************************************************************
* 函数名: OnSelectedBlockPlay
* 功能:删除动作组响应槽
* 参数:
* 返回值:
* 时间:
* 作者: 周志平
* 改动日志
**************************************************************************/
void CUILayerCommonWidget::OnDeleteSelectGroup()
{
    m_nCurChosenAction = DELETE_ACTION_CHOSEN;
}

/**************************************************************************
* 函数名: OnSelectedBlockPlay
* 功能:单个动作组仿真响应槽
* 参数:
* 返回值:
* 时间:
* 作者: 周志平
* 改动日志
**************************************************************************/
void CUILayerCommonWidget::OnSelectedBlockPlay()
{
    m_nCurChosenAction = SELECTEDBLOCKPLAY_ACTION_CHOSEN;
}

/**************************************************************************
* 函数名: OnCancelBlockPlay
* 功能:取消动作组播放响应槽
* 参数:
* 返回值:
* 时间:
* 作者: 周志平
* 改动日志
**************************************************************************/
void CUILayerCommonWidget::OnCancelBlockPlay()
{
    m_nCurChosenAction = CANCELBLOCKPLAY_ACTION_CHOSEN;
}
/**************************************************************************
* 函数名: OnInsertBlankTime
* 功能:动作组复制响应槽
* 参数:
* 返回值:
* 时间:
* 作者: 周志平
* 改动日志
**************************************************************************/
void CUILayerCommonWidget::OnInsertBlankTime()
{
    m_nCurChosenAction = INSERTBLANKTIME_ACTION_CHOSEN;
}
/**************************************************************************
* 函数名: OnSaveToCommandBox
* 功能:保存响应槽
* 参数:
* 返回值:
* 时间:
* 作者: 周志平
* 改动日志
**************************************************************************/
void CUILayerCommonWidget::OnSaveToCommandBox()
{
    m_nCurChosenAction = SAVETOCOMMANDBOX_ACTION_CHOSEN;
}
/**************************************************************************
* 函数名: OnModifyGroup
* 功能:修改组名响应槽
* 参数:
* 返回值:
* 时间:
* 作者: 周志平
* 改动日志
**************************************************************************/
void CUILayerCommonWidget::OnModifyGroup()
{
    m_nCurChosenAction = MODIFYGROUP_ACTION_CHOSEN;
}
/**************************************************************************
* 函数名:OnSelectedMusicDel
* 功能: 删除Music响应槽
* 参数:
* 返回值:
* 时间:
* 作者: 周志平
* 改动日志
**************************************************************************/
void CUILayerCommonWidget::OnDeleteSelectedMusic()
{
    if(!m_strMp3Path.isEmpty())
    {
        CUILayerImpl* pUILayer = FindUILayerByLayerType(ACTION_LAYER_MUSIC);
        if (pUILayer)
        {
            ((CUIMusicLayer*)pUILayer)->ReleaseMusic(m_strMp3Path);
        }
        m_strMp3Path = "";
    }
}

void CUILayerCommonWidget::ReDrawAll()
{
    QSize size = this->size();
    if(NULL != m_pPixmapMem)
    {
        delete m_pPixmapMem;
    }
    m_pPixmapMem = new QPixmap(size.width(), size.height());
    m_pPixmapMem->fill(Qt::transparent);

    if(NULL != m_pPixmapBackGround)
    {
        delete m_pPixmapBackGround;
    }
    m_pPixmapBackGround = new QPixmap(size.width(), size.height());
    m_pPixmapBackGround->fill(Qt::transparent);


    DrawBackGround(m_pPixmapBackGround);

    for (int i = 0; i < m_listUILayer.count(); i++)
    {
        CUILayerImpl* pUILayer = m_listUILayer.at(i);
        if (pUILayer)
        {
            pUILayer->DrawBottomCell(m_pPixmapBackGround);//将各个层的背景加上
            pUILayer->DrawBase();
            pUILayer->DrawGroup();
        }

    }

    DrawPixmapExcludeSelLine(m_pPixmapMem);
    DrawTimeSelLine(m_pPixmapMem);
    update();
}

void CUILayerCommonWidget::OnReDrawTimeAndAction()
{
    onReDrawAction();
    //    m_actionLayer.DrawGroup(m_pPixmapGroup);
    //    m_earLedLayer.DrawGroup(m_pEarPixmapGroup);
    //    m_eyeLedLayer.DrawGroup(m_pEyePixmapGroup);
    //    m_musicLayer.DrawGroup(m_pPixmapWave);

    DrawPixmapExcludeSelLine(m_pPixmapMem);
    DrawTimeSelLine(m_pPixmapMem);
    update();
}

void CUILayerCommonWidget::UpdateTimeLineWidthMusic(quint64 position)
{
    //position单位是毫秒，需要做相应转换
    double nCurPos = position;
    switch(m_timeLevel)
    {
    case ddd_10:
        nCurPos = nCurPos / 10.0;
        break;
    case ddd_50:
        nCurPos = nCurPos / 50.0;
        break;
    case ddd_100:
        nCurPos = nCurPos / 100.0;
        break;
    case ss_1:
        nCurPos = nCurPos / 1000.0;
        break;
    case ss_5:
        nCurPos = nCurPos / 5000.0 ;
        break;
    case ss_10:
        nCurPos = nCurPos / 10000.0;
        break;
    case mm_1:
        nCurPos = nCurPos / 60000.0;
        break;
    case mm_5:
        nCurPos = nCurPos / 300000.0;
        break;
    case mm_10:
        nCurPos = nCurPos / 600000.0;
        break;
    default:
        break;
    }
    int nEndIndex = GetCellEndIndex();
    m_nCurCol = nCurPos * (m_nCellWidth + m_nLineThick);
    if(nCurPos >= nEndIndex)
    {
        m_nCellStartIndex = nEndIndex;
        emit SigReDrawAll();
        emit SigCellIndexChanged(m_nCellStartIndex);
    }
    else
    {
        //只是更新时间线，不需要重新绘制所有图形
        if(m_bIsPlaying)
        {
            OnReDrawTimeAndAction();
        }
        else
        {
            DrawPixmapExcludeSelLine(m_pPixmapMem);
            DrawTimeSelLine(m_pPixmapMem);
            update();
        }
    }
}

//void CUILayerCommonWidget::StoreCurrentX()
//{
//    m_nCurSelXStore = m_nCurCol;
//}

//void CUILayerCommonWidget::RestoreCurrentX()
//{
//    m_nCurCol = m_nCurSelXStore;
//    int nCol = GetCurrentSelectCol();
//    emit SigReDrawAll();
//    emit SigCellIndexChanged(m_nCellStartIndex);
//}

//将除时间线外的其他pixmap绘制到m_pPixmapMem中,贴图的操作，很快,主要给鼠标点击时间线时使用，其他时候用不到
void CUILayerCommonWidget::DrawPixmapExcludeSelLine(QPixmap* pixmap)
{
    if(NULL != pixmap)
    {
        QPainter painter;
        painter.begin(pixmap);
        painter.drawPixmap(0, 0, m_pPixmapBackGround->width(), m_pPixmapBackGround->height(), *m_pPixmapBackGround);
        for (int i = 0; i < m_listUILayer.count(); i++)
        {
            CUILayerImpl* pUILayer = m_listUILayer.at(i);
            if(pUILayer)
            {
                QPixmap* pPixmap = pUILayer->GetPixmap();
                if(pPixmap == NULL)
                {
                    //AddLog( "PixMap Created failed，Please check if UILayer is correct");
                }
                else
                {
                    painter.drawPixmap(0, pUILayer->GetStartPosition().y(), pPixmap->width(), pPixmap->height(), *pPixmap);
                }

            }

        }
        //        painter.drawPixmap(0, m_nTimeCellHeight, m_pPixmapWave->width(), m_pPixmapWave->height(), *m_pPixmapWave);
        //        painter.drawPixmap(0, m_nTimeCellHeight + m_nMusicCellHeight, m_pPixmapGroup->width(), m_pPixmapGroup->height(), *m_pPixmapGroup);
        //        painter.drawPixmap(0, m_nTimeCellHeight + m_nMusicCellHeight + m_nCellHeight+3, m_pPixmapGroup->width(), m_pPixmapGroup->height(), *m_pEarPixmapGroup);
        //        painter.drawPixmap(0, m_nTimeCellHeight + m_nMusicCellHeight + m_nCellHeight*2+6, m_pPixmapGroup->width(), m_pPixmapGroup->height(), *m_pEyePixmapGroup);
        painter.end();
    }
}

/**************************************************************************
* 函数名:CreateMotionLayerGroupListPopMenuActions
* 功能:创建动作组右键动作组菜单
* 参数:
* 返回值:
* 时间:2016/01/28 17:19
* 作者: 周志平
* 改动日志
**************************************************************************/
void CUILayerCommonWidget::CreateMotionLayerGroupListPopMenuActions()
{
    //创建菜单、菜单项
    QMenu* LayoutMenu = new QMenu();
    QAction* CopyAction = new QAction(QObject::tr("Copy"),this);
    QAction* PasteAction = new QAction(QObject::tr("Paste"),this);
    QAction* CutAction = new QAction(QObject::tr("Cut"),this);
    QAction* DeleteAction = new QAction(QObject::tr("Delete"),this);
    QAction* SinglePlayingAction = new QAction(QObject::tr("Single block simulation"),this);
    QAction* CancelPlayAction = new QAction(QObject::tr("Cancel simulation"),this);
    QAction* InsertGroupAction = new QAction(QObject::tr("Insert blank group"),this);
    QAction* SaveToBoxAction = new QAction(QObject::tr("Save to direct box"),this);
    QAction* ModifyGroupAction = new QAction(QObject::tr("Modify group info"),this);

    LayoutMenu->addAction(CopyAction);
    LayoutMenu->addAction(PasteAction);
    LayoutMenu->addAction(CutAction);
    LayoutMenu->addAction(DeleteAction);
    LayoutMenu->addAction(SinglePlayingAction);
    LayoutMenu->addAction(CancelPlayAction);
    LayoutMenu->addAction(InsertGroupAction);
    LayoutMenu->addAction(SaveToBoxAction);
    LayoutMenu->addAction(ModifyGroupAction);

    //连接信号与槽
    connect(CopyAction,&QAction::triggered,this,&CUILayerCommonWidget::OnCopy);
    connect(PasteAction,&QAction::triggered,this,&CUILayerCommonWidget::OnPaste);
    connect(CutAction,&QAction::triggered,this,&CUILayerCommonWidget::OnCut);
    connect(DeleteAction,&QAction::triggered,this,&CUILayerCommonWidget::OnDeleteSelectGroup);
    connect(SinglePlayingAction,&QAction::triggered,this,&CUILayerCommonWidget::OnSelectedBlockPlay);
    connect(CancelPlayAction,&QAction::triggered,this,&CUILayerCommonWidget::OnCancelBlockPlay);
    connect(InsertGroupAction,&QAction::triggered,this,&CUILayerCommonWidget::OnInsertBlankTime);
    connect(SaveToBoxAction,&QAction::triggered,this,&CUILayerCommonWidget::OnSaveToCommandBox);
    connect(ModifyGroupAction,&QAction::triggered,this,&CUILayerCommonWidget::OnModifyGroup);
    //菜单出现的位置为当前鼠标的位置
    if(m_pEvent)
    {

        if( m_nCurrentGroupID == -1 || m_bIsPlaying || m_bIsSinglePlaying)
        {
            CopyAction->setEnabled(false);
            CutAction->setEnabled(false);
            DeleteAction->setEnabled(false);
            SinglePlayingAction->setEnabled(false);
            CancelPlayAction->setEnabled(false);
            InsertGroupAction->setEnabled(false);
            SaveToBoxAction->setEnabled(false);
            ModifyGroupAction->setEnabled(false);
        }
        if(m_bIsPlaying)
        {
            PasteAction->setEnabled(false);
        }
        if(m_bIsSinglePlaying)
        {
            CancelPlayAction->setEnabled(true);
        }
        if(m_nPasteType == NO_PASTE)
        {
            PasteAction->setEnabled(false);
        }

    }

    QFile styleSheet(":/res/qss/mp3projmenudefault.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        LayoutMenu->setStyleSheet(styleSheet.readAll());
    }

    LayoutMenu->exec(QCursor::pos());
    LayoutMenu->deleteLater();
}
/**************************************************************************
* 函数名:CreateMusicLayerGroupListPopMenuActions
* 功能:创建音乐右键动作组菜单
* 参数:
* 返回值:
* 时间:2016/01/28 17:19
* 作者: 周志平
* 改动日志
**************************************************************************/
void CUILayerCommonWidget::CreateMusicLayerGroupListPopMenuActions()
{
    //创建菜单、菜单项
    QMenu* LayoutMenu = new QMenu();
    QAction* DeleteAction = new QAction(QObject::tr("Delete"),this);
    LayoutMenu->addAction(DeleteAction);

    //连接信号与槽

    connect(DeleteAction,&QAction::triggered,this,&CUILayerCommonWidget::OnDeleteSelectedMusic);

    //菜单出现的位置为当前鼠标的位置
    if(m_pEvent)
    {

        if(m_strMp3Path.isEmpty() || (m_pMP3Player && m_pMP3Player->IsPlaying()))
        {
            DeleteAction->setEnabled(false);
        }
        else
        {
            DeleteAction->setEnabled(true);
        }

    }

    QFile styleSheet(":/res/qss/mp3projmenudefault.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        LayoutMenu->setStyleSheet(styleSheet.readAll());
    }

    LayoutMenu->exec(QCursor::pos());
    LayoutMenu->deleteLater();
}

//void CUILayerCommonWidget::SetActStartTime(int nActStartTime)
//{

//}

//void CUILayerCommonWidget::SetActEndTime(int nActEndTime)
//{

//}

/**************************************************************************
* 函数名：GetLayerTypeByPos
* 功能:根据当前位置获取所在层
* 参数:
*    @[in ] point  位置
* 返回值: int 返回所在层
* 时间: 2017/1/16
* 作者: Joker
*/

emActionLayerType CUILayerCommonWidget::GetLayerTypeByPos(QPoint point)
{

    emActionLayerType nLayerType = ACTION_LAYER_UNKNOWN;
    if (m_listUILayer.count() == 0)
    {
        return nLayerType;
    }

    for (int i = 0; i<m_listUILayer.count(); i++)
    {
        CUILayerImpl* pUILayer = m_listUILayer.at(i);
        if (pUILayer == NULL)
        {
            continue;
        }
        QPoint startPoint = pUILayer->GetStartPosition();
        if (point.y() > startPoint.y() && point.y() < startPoint.y() + pUILayer->GetCellHeight() + Constants::DEFAULT_CELL_GAP_HEIGHT)
        {
            nLayerType = pUILayer->GetType();
            return nLayerType;
        }
    }
    return nLayerType;

    //    QRect rectContain(0,0,0,0);
    //    rectContain.setRight(GetContentWidth());
    //    rectContain.setBottom(m_nTimeCellHeight + m_nMusicCellHeight + m_nCellHeight*3 + 6);

    //    //Time layer
    //    QRect rectTimeContain(0,0,0,0);
    //    rectTimeContain.setRight(GetContentWidth());
    //    rectTimeContain.setBottom(m_nTimeCellHeight);

    //    //Music layer
    //    QRect rectMusicContain(0,0,0,0);
    //    rectMusicContain.setRight(GetContentWidth());
    //    rectMusicContain.setTop(m_nTimeCellHeight);
    //    rectMusicContain.setBottom(m_nTimeCellHeight + m_nMusicCellHeight);


    //    //Motion Layer
    //    QRect rectMotionContain(0,0,0,0);
    //    rectMotionContain.setRight(GetContentWidth());
    //    rectMotionContain.setTop(m_nTimeCellHeight + m_nMusicCellHeight);
    //    rectMotionContain.setBottom(m_nTimeCellHeight + m_nMusicCellHeight + m_nCellHeight);

    //    //EarLed Layer
    //    QRect rectEarLedContain(0,0,0,0);
    //    rectEarLedContain.setRight(GetContentWidth());
    //    rectEarLedContain.setTop(m_nTimeCellHeight + m_nMusicCellHeight + m_nCellHeight);
    //    rectEarLedContain.setBottom(m_nTimeCellHeight + m_nMusicCellHeight + m_nCellHeight*2);

    //    //EyeLed Layer
    //    QRect rectEyeLedContain(0,0,0,0);
    //    rectEyeLedContain.setRight(GetContentWidth());
    //    rectEyeLedContain.setTop(m_nTimeCellHeight + m_nMusicCellHeight + m_nCellHeight*2);
    //    rectEyeLedContain.setBottom(m_nTimeCellHeight + m_nMusicCellHeight + m_nCellHeight*3);
    //    if(rectContain.contains(point))
    //    {
    //        //        m_nCurCol = m_nCellStartIndex * (m_nCellWidth + m_nLineThick) + point.x();
    //        if (rectTimeContain.contains(point))
    //        {
    //            nLayerType = ACTION_LAYER_TIME;
    //        }
    //        if (rectMusicContain.contains(point))
    //        {
    //            nLayerType = ACTION_LAYER_MUSIC;
    //        }
    //        if (rectMotionContain.contains(point))
    //        {
    //            nLayerType = ACTION_LAYER_MOTION;
    //        }
    //        else if (rectEarLedContain.contains(point))
    //        {
    //            nLayerType = ACTION_LAYER_EAR;
    //        } else if (rectEyeLedContain.contains(point))
    //        {
    //            nLayerType = ACTION_LAYER_EYE;
    //        }
    //    }
    //    return nLayerType;
}

int CUILayerCommonWidget::GetCurCellByPos(QPoint point)
{
    int x = point.x();

    int nEndCountIndex = GetCellEndIndex();

    int iCurPos = 0;
    int nCurCell = 0;
    //绘制时间刻度
    for(int i = m_nCellStartIndex; i <= nEndCountIndex; i++)
    {
        if (x >= iCurPos && x < iCurPos + m_nCellWidth + m_nLineThick)
        {
            nCurCell = i + 1;
            break;
        }
        iCurPos += m_nCellWidth + m_nLineThick;
    }
    //将当前列表保存起来，以便后面使用
    m_nCurCol = nCurCell;
    return nCurCell;
}

/************************************
* 名称: DrawBottomCell
* 功能: 绘制底部cell，参考原来windows下的界面
* 参数: [in]QPixmap* pixmap绘图对象指针
* 返回:   void
* 时间:   2016/01/07
* 作者:   hels
************************************/
//void CUILayerCommonWidget::DrawBottomCell(QPixmap* pixmap)
//{
//    if(NULL == pixmap)
//    {
//        return;
//    }
//    QPainter painter(pixmap);
//    int nCount = GetCellEndIndex();
//    int xTmp = m_nStartX;
//    //绘制波形底部的方框
//    int bottomStartY = m_nTimeCellHeight + m_nMusicCellHeight;//底部方框起始Y坐标
//    for(int i = m_nCellStartIndex; i <= nCount; i++)
//    {
//        if(i >= m_nCellCount) //判断音谱太短，rectWidget宽度都没占满时返回
//            break;
//        QPen penLine(Constants::COLOR_LINE, m_nLineThick, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
//        painter.setPen(penLine);

//        //竖线
//        painter.drawLine(xTmp, bottomStartY, xTmp, bottomStartY + m_nCellHeight);
//        //横线
//        painter.drawLine(xTmp, bottomStartY + m_nCellHeight,
//                         xTmp + m_nCellWidth + m_nLineThick, bottomStartY + m_nCellHeight);
//        //每五个，填充一次稍深得颜色，跟windows下vs版本保持一致
//        if( (i + 1) % 5 == 0)
//        {
//            QRect rect(xTmp + 1, bottomStartY, m_nCellWidth, m_nCellHeight);
//            QBrush brush(Constants::COLOR_BRUSH_CELL);
//            painter.fillRect(rect, brush);
//        }
//        else
//        {
//            QRect rect(xTmp + 1, bottomStartY, m_nCellWidth, m_nCellHeight);
//            QBrush brush(Constants::COLOR_DEF_BRUSH_CELL);
//            painter.fillRect(rect, brush);
//        }
//        xTmp += m_nCellWidth;
//        xTmp += m_nLineThick;
//    }
//    //竖线
//    painter.drawLine(xTmp, bottomStartY, xTmp, bottomStartY + m_nCellHeight);
//}

//void CUILayerCommonWidget::DrawEarLedCell(QPixmap* pixmap)
//{
//    if(NULL == pixmap)
//    {
//        return;
//    }
//    QPainter painter(pixmap);
//    int nCount = GetCellEndIndex();
//    int xTmp = m_nStartX;
//    //绘制波形底部的方框 。。 与动作层相比，只有这一行不同
//    int bottomStartY = m_nTimeCellHeight + m_nMusicCellHeight + m_nCellHeight + 3;//底部方框起始Y坐标
//    for(int i = m_nCellStartIndex; i <= nCount; i++)
//    {
//        if(i >= m_nCellCount) //判断音谱太短，rectWidget宽度都没占满时返回
//            break;
//        QPen penLine(Constants::COLOR_LINE, m_nLineThick, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
//        painter.setPen(penLine);

//        //竖线
//        painter.drawLine(xTmp, bottomStartY, xTmp, bottomStartY + m_nCellHeight);
//        //横线
//        painter.drawLine(xTmp, bottomStartY + m_nCellHeight,
//                         xTmp + m_nCellWidth + m_nLineThick, bottomStartY + m_nCellHeight);
//        //每五个，填充一次稍深得颜色，跟windows下vs版本保持一致
//        if( (i + 1) % 5 == 0)
//        {
//            QRect rect(xTmp + 1, bottomStartY, m_nCellWidth, m_nCellHeight);
//            QBrush brush(Constants::COLOR_BRUSH_CELL);
//            painter.fillRect(rect, brush);
//        }
//        else
//        {
//            QRect rect(xTmp + 1, bottomStartY, m_nCellWidth, m_nCellHeight);
//            QBrush brush(Constants::COLOR_DEF_BRUSH_CELL);
//            painter.fillRect(rect, brush);
//        }
//        xTmp += m_nCellWidth;
//        xTmp += m_nLineThick;
//    }
//    //竖线
//    painter.drawLine(xTmp, bottomStartY, xTmp, bottomStartY + m_nCellHeight);
//}

//void CUILayerCommonWidget::DrawEyeLedCell(QPixmap* pixmap)
//{
//    if(NULL == pixmap)
//    {
//        return;
//    }
//    QPainter painter(pixmap);
//    int nCount = GetCellEndIndex();
//    int xTmp = m_nStartX;
//    //绘制波形底部的方框 。。 与动作层相比，只有这一行不同
//    int bottomStartY = m_nTimeCellHeight + m_nMusicCellHeight + m_nCellHeight*2 + 6;//底部方框起始Y坐标
//    for(int i = m_nCellStartIndex; i <= nCount; i++)
//    {
//        if(i >= m_nCellCount) //判断音谱太短，rectWidget宽度都没占满时返回
//            break;
//        QPen penLine(Constants::COLOR_LINE, m_nLineThick, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
//        painter.setPen(penLine);

//        //竖线
//        painter.drawLine(xTmp, bottomStartY, xTmp, bottomStartY + m_nCellHeight);
//        //横线
//        painter.drawLine(xTmp, bottomStartY + m_nCellHeight,
//                         xTmp + m_nCellWidth + m_nLineThick, bottomStartY + m_nCellHeight);
//        //每五个，填充一次稍深得颜色，跟windows下vs版本保持一致
//        if( (i + 1) % 5 == 0)
//        {
//            QRect rect(xTmp + 1, bottomStartY, m_nCellWidth, m_nCellHeight);
//            QBrush brush(Constants::COLOR_BRUSH_CELL);
//            painter.fillRect(rect, brush);
//        }
//        else
//        {
//            QRect rect(xTmp + 1, bottomStartY, m_nCellWidth, m_nCellHeight);
//            QBrush brush(Constants::COLOR_DEF_BRUSH_CELL);
//            painter.fillRect(rect, brush);
//        }
//        xTmp += m_nCellWidth;
//        xTmp += m_nLineThick;
//    }
//    //竖线
//    painter.drawLine(xTmp, bottomStartY, xTmp, bottomStartY + m_nCellHeight);
//}

void CUILayerCommonWidget::on_btnZoomOut_clicked()
{
    //    //mp3正在播放，不响应缩放操作
    //    if(m_pMP3Player && m_pMP3Player->IsPlaying())
    //    {
    //        return;
    //    }

    //    if(m_timeLevel < mm_10)
    //    {
    //        TimeLevelChanged((TimeLevel)((int)m_timeLevel + 1));
    //    }
}

void CUILayerCommonWidget::on_btnZoomIn_clicked()
{
    //    //mp3正在播放，不响应缩放操作
    //    if(m_pMP3Player && m_pMP3Player->IsPlaying())
    //    {
    //        return;
    //    }

    //    if(m_timeLevel > ddd_10)
    //    {
    //        TimeLevelChanged((TimeLevel)((int)m_timeLevel - 1));
    //    }
}

/**************************************************************************
* 函数名：AddUILayer
* 功能: 添加UILayer,同时修改UILayer所在层的位置，由于Music Layer是画波形的，需要特殊处理
* 参数:
*    @[in ] pUILayer: 需要添加的Layer指针
* 返回值: bool
* 时间: 2017/1/16
* 作者: Joker
*/
bool CUILayerCommonWidget::AddUILayer(CUILayerImpl* pUILayer)
{
    CUILayerImpl* itemFound = NULL; /*new IActionLayer();*/

    if (pUILayer == NULL)
        return false;

    //判断该层是否已经存在
    for (int i=0; i<m_listUILayer.count(); i++)
    {
        itemFound = m_listUILayer.at(i);
        if (itemFound && itemFound->GetType() == pUILayer->GetType())
        {
            return false;
        }
    }

    int nStartX = m_nStartX;
    int nStartY = 0;

    //先处理音乐层,无论音乐层什么添加，都在时间层下面
    if (pUILayer->GetType() == ACTION_LAYER_MUSIC)
    {
        nStartY = m_nTimeCellHeight;
    }
    else
    {
        //        CUILayerImpl* pLayerLast = NULL;

        //        if (m_listUILayer.count())
        //        {
        //            pLayerLast = GetLastLayer();
        //        }

        //        // 更新该层的起始位置
        //        QPoint pointLast(0,0);
        //        if (pLayerLast)
        //        {
        //            if(pLayerLast->GetType() == ACTION_LAYER_MUSIC)
        //            {
        //                //如果最后添加的是Music层，不符合本规则，取再前一次添加的层；
        //                pLayerLast = GetPreLastLayer();
        //            }
        //            //后面添加的层放在最后一个层后面
        //            pointLast = pLayerLast->GetStartPosition();
        //            nStartY = pointLast.y() + pLayerLast->GetCellHeight() + Constants::DEFAULT_CELL_GAP_HEIGHT;
        //        }
        //        else
        //        {
        //            //如果分支走到这里，说明前面音乐层没有添加，没关系，这里预留音乐层的位置
        //            nStartY =  m_nTimeCellHeight + Constants::DEFAULT_CELL_MUSIC_HEIGHT + Constants::DEFAULT_CELL_GAP_HEIGHT;
        //        }
        nStartY = m_nNewUILayerStartY;
        m_nNewUILayerStartY += pUILayer->GetCellHeight() + Constants::DEFAULT_CELL_GAP_HEIGHT;
    }

    pUILayer->SetStartPosition(QPoint(nStartX, nStartY));
    m_listUILayer.append(pUILayer);

    return true;
}

void CUILayerCommonWidget::AdjustLayerPosition()
{
    CUILayerImpl* itemFound = NULL;
    int nStartX = m_nStartX;
    int nStartY = 0;
    //音乐层位置永远不变
    bool bIsHasMusic = false;

    //开始画层的起始位置，先跳过音乐层
    m_nNewUILayerStartY = m_nTimeCellHeight + Constants::DEFAULT_CELL_MUSIC_HEIGHT;
    for (int i= 0; i<m_listUILayer.count(); i++)
    {
        itemFound = m_listUILayer.at(i);
        if(itemFound->GetType() == ACTION_LAYER_MUSIC)
        {
            //音乐层没删除不需要调整
            bIsHasMusic = true;
            continue;
        }
        nStartY = m_nNewUILayerStartY;
        itemFound->SetStartPosition(QPoint(nStartX, nStartY));
        m_nNewUILayerStartY += itemFound->GetCellHeight() + Constants::DEFAULT_CELL_GAP_HEIGHT;
    }
    emit ReDrawAll();
}

/**************************************************************************
* 函数名:DeleteActionLayer
* 功能:根据ID删除响应层
* 参数: int nTypeID   层ID
* 返回值: bool 当删除成功时，返回真，否则返回假
* 时间:
* 作者: 周志平
* 改动日志
**************************************************************************/
bool CUILayerCommonWidget::DeleteUILayerByID(int nTypeID)
{
    CUILayerImpl* itemFound = NULL; /*new IActionLayer();*/

    //判断该层是否已经存在
    for (int i=0; i<m_listUILayer.count(); i++)
    {
        itemFound = m_listUILayer.at(i);
        if (itemFound && itemFound->GetType() == nTypeID)
        {
            SAFE_DELETE(itemFound);
            m_listUILayer.removeAt(i);
        }
    }
    AdjustLayerPosition();
    return true;
}

CUILayerImpl* CUILayerCommonWidget::FindLayerByID(int nTypeID)
{
    CUILayerImpl* itemFound = NULL;

    for (int i=0; i<m_listUILayer.count(); i++)
    {
        itemFound = m_listUILayer.at(i);
        if (itemFound && itemFound->GetType() == nTypeID)
        {
            return itemFound;
        }
    }
    return NULL;
}
/**************************************************************************
* 函数名:FindLayerByIndex
* 功能:根据层索引查找层
* 参数:int nIndex   索引值
* 返回值: CUILayerImpl* 返回获取的层
* 时间:2015/12/22 10:57
* 作者: 周志平
* 改动日志：2015/12/22 10:57 修改了查找方法
**************************************************************************/
CUILayerImpl* CUILayerCommonWidget::FindLayerByIndex(int nIndex)
{
    CUILayerImpl* itemFound = NULL;

    itemFound = m_listUILayer.at(nIndex);
    return itemFound;
}

/**************************************************************************
* 函数名：GetLastLayer
* 功能:获取最后添加层
* 参数: 无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
//CUILayerImpl* CUILayerCommonWidget::GetLastLayer()
//{
//    //    return m_listActionLayer.takeLast();
//    return m_listUILayer.last();
//}

/**************************************************************************
* 函数名：GetPreLastLayer
* 功能:获取最后添加层之前的那层，主要用来处理最后添加的层是Music层的情况
* 参数: 无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
//CUILayerImpl* CUILayerCommonWidget::GetPreLastLayer()
//{
//    int nLastIndex = m_listUILayer.count() - 1;

//    if (nLastIndex > 0)
//    {
//        //取前一次添加的
//        return m_listUILayer.at(nLastIndex - 1);
//    }
//    return NULL;
//}

/**************************************************************************
* 函数名：loadData
* 参数:
*    @[in ] emActionLayerType: 层类型
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerCommonWidget::loadData(emActionLayerType nLayerType)
{
    CUILayerImpl* pUILayer = FindUILayerByLayerType(nLayerType);
    if (pUILayer)
    {
        pUILayer->loadData();
    }
}

