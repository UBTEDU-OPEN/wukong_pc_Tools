#include "UILayerImpl.h"
#include "stable.h"
#include "ActionBlockGroupManager.h"
#include "ActionBlockData.h"
#include "mp3projconstants.h"
#include <QPixmap>

CUILayerImpl::CUILayerImpl(ILayerCommonWidget *mp3Widget)
{
    m_pCUILayerCommonWidget = mp3Widget;
    m_bDrawBlockAction = true;
    m_pActionBlockDataManager = NULL;
    m_nCurStartTime = 0;
    m_nCurEndTime = 0;
    m_pPixmap = NULL;
    m_nCellHeight = Constants::DEFAULT_CELL_HEIGHT;
}

CUILayerImpl::~CUILayerImpl()
{

}

void CUILayerImpl::SetActionBlockDataManager(CActionBlockDataManager *pActionBlockDataManger)
{
    m_pActionBlockDataManager = pActionBlockDataManger;
}

CActionBlockDataManager *CUILayerImpl::GetActionBlockDataManager()
{
    return m_pActionBlockDataManager;
}

void CUILayerImpl::SetSelectGroupBlockID(int nID)
{
    if(!m_listGroupSelectID.contains(nID))
    {
        m_listGroupSelectID.append(nID);
    }
}

void CUILayerImpl::ClearSelectGroupBlock()
{
    m_listGroupSelectID.clear();
}

void CUILayerImpl::DrawGroup(/*QPixmap* pixmap*/)
{
    DrawBlockGroup(m_pPixmap);
    if(m_bDrawBlockAction)
    {
        DrawBlockAction(m_pPixmap);
    }
}

void CUILayerImpl::SetDrawBlockAction(bool bDraw)
{
    m_bDrawBlockAction = bDraw;
}

/**************************************************************************
* 函数名：DrawBase
* 功能:画该层基本部分
* 参数: 无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerImpl::DrawBase()
{
    if (m_pCUILayerCommonWidget == NULL)
    {
        return;
    }
    if(NULL != m_pPixmap)
    {
        delete m_pPixmap;
    }
    int nWidth = m_pCUILayerCommonWidget->GetWidget()->size().width();
//    int nCellHeigth = m_pCUILayerCommonWidget->GetCellHeight();
    m_pPixmap = new QPixmap(nWidth, m_nCellHeight);
    m_pPixmap->fill(Qt::transparent);
}

/************************************
* 名称: DrawBottomCell
* 功能: 绘制底部cell，参考原来windows下的界面
* 参数: [in]QPixmap* pixmap绘图对象指针
* 返回:   void
* 时间:   2016/01/07
* 作者:   hels
************************************/
void CUILayerImpl::DrawBottomCell(QPixmap* pixmap)
{
    if(NULL == pixmap)
    {
        return;
    }
    if (m_pCUILayerCommonWidget == NULL)
    {
        return;
    }

    int nCellEndIndex = m_pCUILayerCommonWidget->GetCellEndIndex();
    int nCellStartIndex = m_pCUILayerCommonWidget->GetCellStartIndex();
    int nCellCount = m_pCUILayerCommonWidget->GetCellCount();
    int nCellWidth = m_pCUILayerCommonWidget->GetCellWidth();
//    int nCellHigth = m_pCUILayerCommonWidget->GetCellHeight();
    int nLineThick = m_pCUILayerCommonWidget->GetLineThick();

    QPainter painter(pixmap);

//    int xTmp = m_nStartX;
    int xTmp = m_startPos.x();

    //绘制波形底部的方框
    int bottomStartY  = m_startPos.y();
//    int bottomStartY = m_nTimeCellHeight + m_nMusicCellHeight;//底部方框起始Y坐标
    for(int i = nCellStartIndex; i <= nCellEndIndex; i++)
    {
        if(i >= nCellCount) //判断音谱太短，rectWidget宽度都没占满时返回
            break;
        QPen penLine(Constants::COLOR_LINE, nLineThick, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);

        painter.setPen(penLine);
        //竖线
        painter.drawLine(xTmp, bottomStartY, xTmp, bottomStartY + m_nCellHeight);
        //横线
        painter.drawLine(xTmp, bottomStartY + m_nCellHeight,
                         xTmp + nCellWidth + nLineThick, bottomStartY + m_nCellHeight);
        //每五个，填充一次稍深得颜色，跟windows下vs版本保持一致
        if( (i + 1) % 5 == 0)
        {
            QRect rect(xTmp + 1, bottomStartY, nCellWidth, m_nCellHeight);
            QBrush brush(Constants::COLOR_BRUSH_CELL);
            painter.fillRect(rect, brush);
        }
        else
        {
            QRect rect(xTmp + 1, bottomStartY, nCellWidth, m_nCellHeight);
            QBrush brush(Constants::COLOR_DEF_BRUSH_CELL);
            painter.fillRect(rect, brush);
        }
        xTmp += nCellWidth;
        xTmp += nLineThick;
    }
    //竖线
    painter.drawLine(xTmp, bottomStartY, xTmp, bottomStartY + m_nCellHeight);
}

/**************************************************************************
* 函数名：GetPixmap
* 功能:获取该层像素图
* 参数: 无
* 返回值: QPixmap* 返回像素图，主要作为贴图供公共层直接贴，减少画图开销
* 时间: 2017/1/16
* 作者: Joker
*/
QPixmap *CUILayerImpl::GetPixmap()
{
    return m_pPixmap;
}

/**************************************************************************
* 函数名：GetCellHeight
* 功能:获取一格的高度
* 参数: 无
* 返回值: int 返回一格的高度
* 时间: 2017/1/16
* 作者: Joker
*/
int CUILayerImpl::GetCellHeight()
{
    return m_nCellHeight;
}

void CUILayerImpl::DrawBlockGroup(QPixmap *pixmap)
{
    if(NULL == pixmap ||
            NULL == m_pCUILayerCommonWidget ||
            NULL == m_pActionBlockDataManager)
    {
        return;
    }

    int nLineThick = m_pCUILayerCommonWidget->GetLineThick();
    int nCellWidth = m_pCUILayerCommonWidget->GetCellWidth();
    QPainter painter(pixmap);

    m_nCurStartTime = m_pCUILayerCommonWidget->GetCellStartIndex();
    m_nCurEndTime = m_pCUILayerCommonWidget->GetCellEndIndex();

    SetActionBlockDataType::const_iterator iter = m_pActionBlockDataManager->FirstGroupIndex(m_nCurStartTime);

    for(;iter != m_pActionBlockDataManager->EndIndex(); iter++)
    {
        CActionBlockGroupManager* pGroupManager = *iter;
        if(pGroupManager == NULL)
        {
            continue;
        }
        //ActionBlock存储的时间，这里的时间单位是格
        int nStartTime = pGroupManager->GetStartTime();
        int nEndTime = pGroupManager->GetEndTime();

        if( (nStartTime <= m_nCurEndTime) && (nEndTime >= m_nCurStartTime) )
        {
            QRect rect(0, 0, 0, pixmap->height());

            if (nStartTime - m_nCurStartTime > 0)
            {
                rect.setLeft( (nStartTime - m_nCurStartTime) * (nCellWidth + nLineThick));
            }
            else
            {
                rect.setLeft(0);
            }
            if(nEndTime < m_nCurEndTime)
            {
                rect.setRight( (nEndTime- m_nCurStartTime) * (nCellWidth + nLineThick) );
            }
            else
            {
                rect.setRight( (m_nCurEndTime - m_nCurStartTime)* (nCellWidth + nLineThick) );
            }

            if(IsSelectGroupBlock(pGroupManager->GetID()))
            {
                QBrush brush(Constants::COLOR_GROUP_BLOCK_SELECT);
                painter.fillRect(rect, brush);
            }
            else
            {
                QBrush brush(Constants::COLOR_GROUP_BLOCK_DEFAULT);
                painter.fillRect(rect, brush);
            }

            //            QBrush brush(QColor(120, 80, 100));
            //            painter.fillRect(rect, brush);

            //绘制组名称
            QFont font;
            font.setPointSize(12);
            painter.setFont(font);
            //QFontMetrics fm(font);
            QString strDescInfo = QString::fromStdWString(pGroupManager->GetDescription());
            //QString strDescInfo = QString::fromUtf16((WCHAR*)pGroupManager->GetDescription());
            //int width = fm.width(strDescInfo);
            //int height = fm.height();
            painter.setBackgroundMode(Qt::TransparentMode);//绘制文字设置背景透明
            //QRect rectFont(rect.topLeft(), QPoint(rect.right(), rect.top() + rect.height()));
            painter.setPen(Constants::COLOR_TIME_FONT);
            painter.drawText(rect, Qt::TextWrapAnywhere, " " + strDescInfo);
            //绘制底部cell实线
            QPen pen(Qt::black, m_pCUILayerCommonWidget->GetLineThick());
            painter.setPen(pen);
            painter.drawLine(rect.topLeft(), rect.topRight());//上面一条横线
            painter.drawLine(rect.topLeft(), rect.bottomLeft());//左边一条竖线
            painter.drawLine(rect.bottomLeft(), rect.bottomRight());//下面
            painter.drawLine(rect.topRight(), rect.bottomRight());//右边
        }
        else if(nStartTime > m_nCurEndTime)
        {
            return;
        }
    }
}

void CUILayerImpl::DrawBlockAction(QPixmap *pixmap)
{
    if(NULL == pixmap ||
            NULL == m_pCUILayerCommonWidget ||
            NULL == m_pActionBlockDataManager)
    {
        return;
    }
    //获取
    int nCol = m_pCUILayerCommonWidget->GetCurrentSelectCol();

    int nLineThick = m_pCUILayerCommonWidget->GetLineThick();
    int nCellWidth = m_pCUILayerCommonWidget->GetCellWidth();
    m_nCurStartTime = m_pCUILayerCommonWidget->GetCellStartIndex();
    m_nCurEndTime = m_pCUILayerCommonWidget->GetCellEndIndex();

    int nGroupIndex = 0;//动作组索引
    int nActionIndex = 0;//组中动作索引
    int nOffsetTime = 0;//时间线
    GetSelectGroupAction(nCol, nGroupIndex, nActionIndex, nOffsetTime);
    if(nGroupIndex == -1 || nActionIndex == -1)
    {
        return;
    }

    CActionBlockGroupManager* pGroupManager = m_pActionBlockDataManager->GetAt(nGroupIndex);
    if(NULL == pGroupManager)
    {
        return;
    }

    int nActionStartTime = pGroupManager->GetStartTime();
    int nActionEndTime = pGroupManager->GetStartTime();
    CActionBlockData* pBlockData = NULL;
    for(int i = 0; i <= nActionIndex; i++)
    {
        pBlockData = pGroupManager->GetAt(i);
        if(pBlockData == NULL)
        {
            //AddLog( "when DrawBlockAction, pBlockDate is NULL!");
            continue;
        }
        nActionEndTime += pBlockData->GetRunTime() + pBlockData->GetStopTime();
    }
    pBlockData = pGroupManager->GetAt(nActionIndex);
    if (NULL != pBlockData)
    {
        nActionStartTime = nActionEndTime - (pBlockData->GetRunTime() + pBlockData->GetStopTime());
    }
    //    //ActionBlock中存储的时间需要乘以时间粒度，才是真实ms时间
    //    nActionEndTime = nActionEndTime * m_pCUILayerCommonWidget->GetTimeScale();
    //    nActionStartTime = nActionStartTime * m_pCUILayerCommonWidget->GetTimeScale();
    QRect rect;

    //wanxf modify 2017/3/23  当用颜色填充选中块时，上下左右的边框不填充，显示自然一点
//    rect.setLeft( (nActionStartTime - m_nCurStartTime) * (nLineThick + nCellWidth));
//    rect.setRight( (nActionEndTime - m_nCurStartTime) * (nLineThick + nCellWidth) );
//    rect.setTop(1);
//    rect.setBottom(pixmap->height() - 1);
    rect.setLeft((nActionStartTime - m_nCurStartTime) * (nLineThick + nCellWidth) + 1);
    rect.setRight((nActionEndTime - m_nCurStartTime) * (nLineThick + nCellWidth) - 1);
    rect.setTop(1);
    rect.setBottom(pixmap->height() - 2);

    QPainter painter(pixmap);
    QBrush brush(Constants::COLOR_ACTION_BLOCK_SELECT);
    painter.fillRect(rect, brush);

    QRect rectFont;
    rectFont.setTop(0);
    rectFont.setLeft( (pGroupManager->GetStartTime() - m_nCurStartTime) * (nLineThick + nCellWidth));
    rectFont.setRight( (pGroupManager->GetEndTime() - m_nCurStartTime) * ((nLineThick + nCellWidth)));
    rectFont.setBottom(rect.bottom());
    QFont font;
    font.setPointSize(12);
    painter.setFont(font);
    QString strDescInfo = QString::fromStdWString(pGroupManager->GetDescription());
    //QString strDescInfo = QString::fromUtf16((WCHAR*)pGroupManager->GetDescription());
    painter.setBackgroundMode(Qt::TransparentMode);//绘制文字设置背景透明
    painter.setPen(Constants::COLOR_TIME_FONT);
    painter.drawText(rectFont, Qt::TextWrapAnywhere, " " + strDescInfo);
}

void CUILayerImpl::GetSelectGroupAction(int nCol, int &nGroupIndex, int &nActionIndex, int &nTimeOffset)
{
    CActionBlockGroupManager* pManager = NULL;
    bool bCursorInGroup = false;
    if(m_pActionBlockDataManager)
    {
        SetActionBlockDataType::const_iterator iterStart = m_pActionBlockDataManager->FirstGroupIndex(m_nCurStartTime);
        //从显示中的索引开始查找，不显示的即使被选中也无法绘制出来
        for(; iterStart != m_pActionBlockDataManager->EndIndex(); iterStart++)
        {
            pManager = *iterStart;
            if(pManager)
            {
                int nGroupStartTime = pManager->GetStartTime();
                int nGroupEndTime = pManager->GetEndTime();

                if(nGroupEndTime >= nCol && nCol > nGroupStartTime)
                {
                    nGroupIndex = m_pActionBlockDataManager->GetIndexByID(pManager->GetID());
                    bCursorInGroup = true;
                    break;
                }
            }
        }
    }

    if(!bCursorInGroup)
    {
        nGroupIndex = -1;
        nActionIndex = -1;
        nTimeOffset = -1;
        return;
    }
    CActionBlockData* pActionData = NULL;
    int nTimeSum = 0;
    if(pManager)
    {
        nTimeSum = pManager->GetStartTime();
        for(int i = 0; i < pManager->GetActionCount(); i++)
        {
            pActionData = pManager->GetAt(i);
            if(pActionData)
            {
                int nActionEndTime = pActionData->GetRunTime() + pActionData->GetStopTime();
                nTimeSum += nActionEndTime;
                if(nTimeSum > nCol)
                {
                    nActionIndex = i;
                    nTimeOffset = nCol - nTimeSum + nActionEndTime;
                    break;
                }
            }
        }
    }
}

bool CUILayerImpl::IsSelectGroupBlock(int nID)
{
    return m_listGroupSelectID.contains(nID);
}

/**************************************************************************
* 函数名：GetStartPosition
* 功能:获取该层起始坐标
* 参数: 无
* 返回值: 返回获得的坐标，用于绘制该层UI
* 时间: 2017/1/16
* 作者: Joker
*/
QPoint CUILayerImpl::GetStartPosition()
{
    return m_startPos;
}

/**************************************************************************
* 函数名：SetStartPosition
* 功能:设置该层起始坐标
* 参数:
*    @[in ] point: 当层的位置发现变化时，通过接口将起始位置保存起来
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUILayerImpl::SetStartPosition(QPoint point)
{
     m_startPos = point;
}
/**************************************************************************
* 函数名:   loadData
* 功能:     
* 参数:   
*....@[out] void
* 返回值:   
* 时间:     2017/03/21 16:25
* 作者:   Leo
*/
void CUILayerImpl::loadData()
{

}
