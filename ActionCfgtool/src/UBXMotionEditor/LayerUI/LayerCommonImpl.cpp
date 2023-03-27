#include "stable.h"
#include "LayerCommonImpl.h"
#include "UBXModifyLayoutBlockInfo.h"
#include "IUILayerManager.h"
#include "MotionDataPlay.h"
#include "MusicDataPlay.h"
#include "UBXAddBlankBlockDlg.h"
#include "UBXCommandBoxDlg.h"
#include "UILayerManager.h"
#include "../UIDialogBlockDescription.h"
#include "../dialogcommandbox.h"
#include "UILayerCommonContainer.h"
#include <QMessageBox>
#include <QClipboard>
#include <QMimeData>
#include <QApplication>
#include <QDir>
#include <QFile>
//#include <QMenu>
CLayerCommonImpl::CLayerCommonImpl(void)
    : m_pBlockDataManager(NULL)
    , m_pOptionHandler(NULL)
    , m_pLayerManager(NULL)
    , m_pGroupDataPlay(NULL)
    , m_IsPlaying(false)
    , m_IsSinglePlaying(false)
    //    , m_bMouseDown(false)
    , m_treeView(NULL)
    , m_IsDragMouse(false)
    , m_IsHaveCopyData(false)
    , m_nCurCol(1)
    //    , (NULL)
//    , m_curLayer(ACTION_LAYER_UNKNOWN)
{
    //    x = 0;
    //    y = 0;
    //    //w = 8;
    //    h = 20;
    //    m_divisor = 5;
    //    m_lineColor = qRgb( 128, 128, 128 );
    //    //m_lineThick = 1;
    //    m_HScrollOffset = 0;

    //    m_CellStartIndex = 0;
    //m_CellCount = 300;
    m_positionDragMouse = 0;

    //    this->m_fontSize = 80;
    //    this->m_fontFamily = QObject::tr("Arial");
    //    this->m_fontColor = qRgb( 10, 10, 10 );

    //    m_GroupBlockDefColor = qRgb(213, 213, 213);
    //    m_GroupBlockSelColor = qRgb(213, 215, 251);
    //m_GroupBlockSelColor = RGB(51, 153, 255);
    // 	m_GroupBlockSelSetColor = RGB(13,14,210);
    // 	m_GroupBlockDefSetColor = RGB(213,14,210);

    //m_CellCount = 300;
    //m_lineThick = 1;
    //m_nCellWidth = 8;
    //m_scaleValue = 50;
}


CLayerCommonImpl::~CLayerCommonImpl(void)
{
    while(!m_GroupSelectID.isEmpty())
    {
        m_GroupSelectID.removeFirst();
    }
    //	m_GroupSelectID.RemoveAll();
}

bool CLayerCommonImpl::Connect(QWidget* managerHandle)
{
    m_pBlockDataManager = new CActionBlockDataManager();
    m_pBlockDataManager->Connect();
    m_ManagerHandle = managerHandle;
    CreateOptionHandler();
    //    if (m_pILayerCommonWidget)
    //    {
    //        m_pILayerCommonWidget->SetOptionHandlerEvent(this);
    //    }
    return true;
}
/**************************************************************************
* 函数名: ReleaseConnection
* 功能:
* 参数:
* 返回值:
* 时间:b
* 作者: 周志平
* 改动日志  保留 m_pOptionHandler  m_pBlockDataManager   不置空
**************************************************************************/
bool CLayerCommonImpl::ReleaseConnection()
{
    if (m_pGroupDataPlay)
    {
        m_pGroupDataPlay->ReleaseConnection();
        m_pGroupDataPlay = NULL;
    }

    if (m_pBlockDataManager)
    {
        m_pBlockDataManager->ReleaseConnection();
        delete m_pBlockDataManager;

        m_pBlockDataManager = NULL;
    }

    if (m_pOptionHandler)
    {
        m_pOptionHandler->ReleaseConnection();
        m_pOptionHandler = NULL;
    }
    m_GroupSelectID.clear();

    return true;
}

int CLayerCommonImpl::GetNewActionBalockDataID()
{
    int BlockID = 1;
    while(1)
    {
        if (m_pBlockDataManager->FindBlockData(BlockID) == NULL)
            break;

        BlockID++;
    }

    return BlockID;
}

bool CLayerCommonImpl::IsSelectGroupBlock(int nID)
{
    for (int i=0; i<m_GroupSelectID.count(); i++)
    {
        int nFind = m_GroupSelectID.at(i);
        if (nFind == nID)
            return true;
    }

    return false;
}

void CLayerCommonImpl::ClearSelectGroupBlock()
{
    m_GroupSelectID.clear();
    //while(!m_GroupSelectID.isEmpty())
    //{
    //    m_GroupSelectID.removeFirst();
    //}
}

void CLayerCommonImpl::ClearSelectState()
{
    ClearSelectGroupBlock();
}
// int CLayerCommonImpl::GetSelectGroupAction(int nCol)
// {
// 	SetSelectGroupBlock(nCol);
// 	int nGroupId = GetSelectGroupBlock();
// 	int nTimeSum = 0;
// 	//m_p
// 	return 1;
//
// }

void CLayerCommonImpl::ILayerOptionHandler_SetSelectGroupBlock(int nCol)
{
    ClearSelectGroupBlock();
    SetSelectGroupBlock(nCol);
}

void CLayerCommonImpl::SetSelectGroupBlock(int nCol)
{
    if (m_pBlockDataManager == NULL)
        return;

    SetActionBlockDataType::const_iterator iterStart = m_pBlockDataManager->FirstGroupIndex(nCol - 1);
    for(; iterStart != m_pBlockDataManager->EndIndex(); iterStart++)
    {
        CActionBlockGroupManager* pManager = NULL;
        if(iterStart != m_pBlockDataManager->EndIndex())
        {
            pManager = *iterStart;
        }
        if (pManager == NULL)
            continue;

        int nStartTime = pManager->GetStartTime();
        int nEndTime = pManager->GetEndTime();

        //wanxf modify 组的时间是时间刻度，开始时间等于格数-1，故nCol-1后再与组的开始时间进行判断
        //if (nCol >= nStartTime && nCol<=nEndTime)
        if ((nCol-1) >= nStartTime && nCol <= nEndTime)
        {
            int nId = pManager->GetID();
            bool IsIdExist = false;
            for(int j = 0; j < m_GroupSelectID.count(); j++)
            {
                if (nId == m_GroupSelectID.at(j))
                {
                    IsIdExist = true;
                    break;
                }
            }
            if (!IsIdExist)
            {
                m_GroupSelectID.append(nId);
            }
            return;
        }
    }
}

int CLayerCommonImpl::GetSelectGroupBlock()
{
    int nSelectID = -1;

    if (m_GroupSelectID.count())
    {
        nSelectID = m_GroupSelectID.at(0);
    }

    return nSelectID;
}

void CLayerCommonImpl::MoveAllGourpTime(CActionBlockGroupManager* pManager, int nOffset)
{
    if (    NULL == pManager
        ||  NULL == m_pBlockDataManager)
        return;

    // 移动组数据时防止当前组覆盖前一组数据
    int nPreEndTime = 0;
    int nNewStartTime = pManager->GetStartTime() + nOffset;

    int nCurIndex = m_pBlockDataManager->GetIndexByID(pManager->GetID());
    if (nCurIndex > 0)
    {
        CActionBlockGroupManager* pPreManager = m_pBlockDataManager->GetAt(nCurIndex - 1);
        if (pPreManager)
        {
            nPreEndTime = pPreManager->GetEndTime();
        }
    }

    if (nNewStartTime < nPreEndTime)
    {
        return;
    }

    pManager->SetStartTime(nNewStartTime);
    pManager->SetEndTime(pManager->GetEndTime() + nOffset);
    AdjustAllGroupTime(pManager, nOffset);
}

void CLayerCommonImpl::AdjustAllGroupTime(CActionBlockGroupManager* pManager, int nAdd)
{
    if (pManager == NULL)
        return;

    //如果不是最后一块数据，需要移动后面所有块
    int nIndex = m_pBlockDataManager->GetIndexByID(pManager->GetID());
    if ( nIndex > -1 &&  nIndex < m_pBlockDataManager->GetCount()-1)
    {
        // 需要移动后面所有的块（由于计算误差，后面块的开始时间以前一个快的结束时间计）
        for (int i=nIndex+1; i<m_pBlockDataManager->GetCount(); i++)
        {
            CActionBlockGroupManager* pMove = m_pBlockDataManager->GetAt(i);
            if (pMove)
            {
                pMove->SetStartTime(pMove->GetStartTime() + nAdd);
                pMove->SetEndTime(pMove->GetEndTime() + nAdd);
            }
        }
    }

    // 调整总的时间
    CActionBlockGroupManager* pLast = m_pBlockDataManager->GetAt(m_pBlockDataManager->GetCount()-1);
    //if (pLast->GetEndTime() >= GetCellCount())
    {
        int nCellCount = m_pLayerManager->GetNewMaxCellCount(pLast->GetEndTime()) + Constants::ADD_OFFSETS;
        m_pLayerManager->SetCellCount( nCellCount);
    }
      //这里不再更新UI，需要的时候再使用
//    int nSelectGroupID = GetSelectGroupBlock();

//    if(m_pLayerManager)
//    {
//        m_pLayerManager->UpdateLayerWidget(m_pBlockDataManager, nSelectGroupID, 0, GetType());
//    }
}

void CLayerCommonImpl::OnLButtonDBClick(int nCol)
{
#if 0
    qDebug()<<(QObject::tr("dbCLick\n"))<<endl;
    if (m_IsPlaying || !m_pLayerManager)
        return;
    DialogBlockDescription dlg;
    QFile styleSheet(":/res/qss/motiondefault.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        dlg.setStyleSheet(styleSheet.readAll());
    }
    //    CAutolockRes res;
    ClearSelectGroupBlock();
    SetSelectGroupBlock(nCol);
    int nGroupID = GetSelectGroupBlock();

    if (nGroupID != -1)
    {
        CActionBlockGroupManager* pGroupManager = m_pBlockDataManager->FindBlockData(nGroupID);

        int nStartTime = ceil(pGroupManager->GetStartTime() * m_pLayerManager->GetTimeScaleValues());
        dlg.SetStartTime(nStartTime);

        int nEndTime = ceil(pGroupManager->GetEndTime() * m_pLayerManager->GetTimeScaleValues());
        dlg.SetEndTime(nEndTime);

        QString strDescription = WCHARToQString(pGroupManager->GetDescription(), MAX_DESCRIPTION_SIZE*sizeof(WCHAR));
        dlg.SetDescription(strDescription);

        if(dlg.exec() == QDialog::Accepted)
        {
            if(!(dlg.m_strBlockDescriptionInfo.isEmpty()))
            {
                int nStartTime = dlg.m_nStartTime / m_pLayerManager->GetTimeScaleValues();
                int nEndTime = dlg.m_nEndTime/ m_pLayerManager->GetTimeScaleValues();
                //设置开始时间
                pGroupManager->SetStartTime(nStartTime);
                //设置结束时间
                pGroupManager->SetEndTime(nEndTime);

                WCHAR wstrDescription[MAX_DESCRIPTION_SIZE];
                memset(wstrDescription,0,MAX_DESCRIPTION_SIZE*sizeof(WCHAR));
                QString strDescriptionTmp = dlg.m_strBlockDescriptionInfo;
                int nLen = strDescriptionTmp.length();
                int nDescriptionLen = QStringToWCHAR(wstrDescription, dlg.m_strBlockDescriptionInfo);
                //                wstrDescription[nDescriptionLen/sizeof(WCHAR)-1] ='\0';
                //设置描述
                pGroupManager->SetDescription(wstrDescription);

                g_IsUpdateUIByData = true;
                g_IsModifyGroup = true;

                if (m_pLayerManager)
                {
                    m_pLayerManager->SetActionDataGroupManagerToOptions(GetBlockDataManager(), nGroupID,0);
                    m_pLayerManager->GetWidget()->update();
                    m_pLayerManager->UpdateLayerWidget(GetBlockDataManager(), nGroupID,0, GetType());
                    //                    m_pLayerManager->UpdateScrollBar();
                }
                g_IsUpdateUIByData  = false;
                if (m_pLayerManager)
                {
                    m_pLayerManager->IUILayerManager_SetModifiedState(true);
                }
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setText(QObject::tr("the name of the bolck can't be empty!"));
                msgBox.exec();
            }
        }
    }
#endif
    ClearSelectGroupBlock();
    SetSelectGroupBlock(nCol);
    if (!m_IsPlaying)
    {
        int nGroupIndex = 0;
        int nActionIndex = 0;
        int nOffsetTime = 0;
        GetSelectGroupAction(nCol,nGroupIndex,nActionIndex,nOffsetTime);
        if (nGroupIndex == -1 || nActionIndex == -1)
        {
            return;
        }
        CActionBlockGroupManager *pManger = NULL;
        CActionBlockData *pActionBlock = NULL;
        if (m_pBlockDataManager)
        {
            pManger = m_pBlockDataManager->GetAt(nGroupIndex);
        }
        if (pManger)
        {
            pActionBlock = pManger->GetAt(nActionIndex);
        }
        if (pActionBlock)
        {
            ILayerOptionHandler_OnDataBlockPlaying(pActionBlock);
        }
    }
}

void CLayerCommonImpl::OnLButtonDown(int nCol, QPoint point, int nStartTime)
{
    m_nCurCol = nCol;
    ClearSelectGroupBlock();
    SetSelectGroupBlock(nCol);
    int nGroupIndex = 0;
    int nActionIndex = 0;
    int nOffset = 0;
    GetSelectGroupAction(nCol,nGroupIndex,nActionIndex,nOffset, nStartTime);
    int nSelGroupID = GetSelectGroupBlock();
    //int nSelGroupID = nGroupIndex + 1;

    if (m_pLayerManager)
    {
        g_IsUpdateUIByData = true;

        // 刷新3D图
        m_pLayerManager->SetActionDataGroupManagerToOptions(GetBlockDataManager(), nSelGroupID, nActionIndex , nOffset);

        // 刷新UILayerCommonWidget (动作帧游标)
        m_pLayerManager->UpdateLayerWidget(GetBlockDataManager(), nSelGroupID, nActionIndex, GetType());
        g_IsUpdateUIByData = false;
    }
}

/**************************************************************************
* 函数名：OnMouseMove
* 功能:Layer层鼠标移动事件(调整组数据位置)
* 参数:
*    @[in ] nCol: 第几列
* 返回值: 无
* 时间: 2017/10/30
* 作者: jianjie
*/
void CLayerCommonImpl::OnMouseMove(int nCol)
{
    int nOffset = nCol - m_nCurCol;
    int nSelGroupID = GetSelectGroupBlock();

    CActionBlockGroupManager* pCurGroupManager = m_pBlockDataManager->FindBlockData(GetSelectGroupBlock());

    MoveAllGourpTime(pCurGroupManager , nOffset);
    m_pLayerManager->UpdateLayerWidget(GetBlockDataManager(), nSelGroupID, 0, GetType());

    m_nCurCol = nCol;
}

//TODO:Joker 这个函数改动很多，可能要重写
void CLayerCommonImpl::OnLButtonUp(QPoint point)
{
    //    m_bMouseDown = false;
    //    if (m_IsDragMouse)
    //    {
    //        g_IsModifyGroup = true;
    //        CActionBlockGroupManager* pGroupManager = NULL;
    //        for (int j = 0; j < m_pBlockDataManager->GetCount(); j++)
    //        {
    //            pGroupManager = m_pBlockDataManager->GetAt(j);
    //            if (m_positionDragMouse == (int)pGroupManager->GetEndTime())
    //            {
    //                break;
    //            }
    //        }
    //        if (m_pLayerManager /*&& m_positionDragMouse != pGroupManager->GetEndTime()*/)
    //        {
    //            m_pLayerManager->IUILayerManager_SetModifiedState(true);
    //        }
    //        int Myoffset = point.x() - (m_positionDragMouse - GetCellStartIndex()*100);
    //        bool IsAdjustBlock = false;

    //        int nGroupTimeSum = pGroupManager->GetActionAllTimeSum();
    //        int *pnOffsetArray = new int[pGroupManager->GetActionCount()];
    //        for (int i1 = 0;i1<pGroupManager->GetActionCount();i1++)
    //        {
    //            pnOffsetArray[i1] = 0;
    //        }

    //        for (int k =0; k < pGroupManager->GetActionCount(); k++)
    //        {
    //            CActionBlockData* pBlockData = pGroupManager->GetAt(k);
    //            int nRunTime = pBlockData->GetRunTime();
    //            int nRunTime = pBlockData->GetAllTime();//runtime or alltime???
    //            int nRatio = nRunTime / nGroupTimeSum;
    //            //TRACE(L"Action%d's fRatio is %f / %f = %f********",k,RunTime,fGroupTimeSum,fRatio);
    //            pnOffsetArray[k] = (int)Myoffset * nRatio;
    //            //if ((Myoffset < 0) && (abs(Myoffset)/pGroupManager->GetCount()+1) > RunTime)
    //            // 不能小于最小时间GRID_MIN_TIMEVALUE
    //            int nScale = m_pLayerManager->GetTimeScaleValues();
    //            //TRACE(L"Action%d's NewValue is %f \n",k,(RunTime + pfOffsetArray[k])*nScale);
    //            if (ceil(( nRunTime + pfOffsetArray[k] ) * nScale) < GRID_MIN_TIMEVALUE)
    //            {
    //                //				AfxMessageBox(language_loadstring(IDS_STRING_MOTIONLAYER_ADJUSTRANGETOOBROAD));
    //                m_IsDragMouse = false;
    //                m_positionDragMouse = 0;
    //                if (pnOffsetArray)
    //                {
    //                    delete []pnOffsetArray;
    //                    pnOffsetArray = NULL;
    //                }
    //                return;
    //            }
    //            else if (ceil(( nRunTime + pnOffsetArray[k] ) * nScale) > GRID_MAX_TIMEVALUE)
    //            {
    //                //				AfxMessageBox(language_loadstring(IDS_STRING_MOTIONLAYER_ADJUSTRANGETOOBROAD));
    //                m_IsDragMouse = false;
    //                m_positionDragMouse = 0;
    //                if (pnOffsetArray)
    //                {
    //                    delete []pnOffsetArray;
    //                    pnOffsetArray = NULL;
    //                }
    //                return;
    //            }
    //        }
    // 		for (int k =0; k < pGroupManager->GetCount(); k++)
    // 		{
    // 			CActionBlockData* BlockData = pGroupManager->GetAt(k);
    // 			float RunTime = BlockData->GetEndTime();
    // 			//if ((Myoffset < 0) && (abs(Myoffset)/pGroupManager->GetCount()+1) > RunTime)
    // 			// 不能小于最小时间GRID_MIN_TIMEVALUE
    // 			int nScale = m_pLayerManager->GetTimeScaleValues();
    // 			if (( RunTime + (float)Myoffset / pGroupManager->GetCount() ) * nScale < GRID_MIN_TIMEVALUE)
    // 			{
    // 				AfxMessageBox(language_loadstring(IDS_STRING_MOTIONLAYER_ADJUSTRANGETOOBROAD));
    // 				m_IsDragMouse = false;
    // 				m_positionDragMouse = 0;
    // 				return;
    // 			}
    // 		}
    //        int nEndTime = pGroupManager->GetStartTime();
    //        int nAdjustEndTime = 0;
    //        if (Myoffset != 0)
    //        {
    //            int blockCount = pGroupManager->GetActionCount();
    //            //AdjustEndTime = (float)abs(Myoffset) / blockCount;
    //            // 			int nTempEndTime = AdjustEndTime * 10;
    //            // 			//只取一位小数
    //            // 			AdjustEndTime = (float) nTempEndTime/ 10;
    //            //四舍五入保留一位小数
    //            //AdjustEndTime = Round(AdjustEndTime , 1);
    //            for (int i = 0; i < pGroupManager->GetActionCount(); i++)
    //            {
    //                CActionBlockData* AdjustBlockData = pGroupManager->GetAt(i);
    //                nAdjustEndTime = abs(pfOffsetArray[i]);
    //                //AdjustEndTime = Round(AdjustEndTime,1);
    //                if (Myoffset > 0)
    //                {
    //                    AdjustBlockData->SetRunTime(Round(AdjustBlockData->GetRunTime() + nAdjustEndTime,1));
    //                    AdjustBlockData->SetAllTime(Round(AdjustBlockData->GetAllTime() + nAdjustEndTime,1));
    //                }
    //                else
    //                {
    //                    //AdjustBlockData->SetEndTime(AdjustBlockData->GetEndTime() - AdjustEndTime);
    //                    AdjustBlockData->SetRunTime(Round(AdjustBlockData->GetRunTime() - nAdjustEndTime,1));
    //                    AdjustBlockData->SetAllTime(Round(AdjustBlockData->GetAllTime() - nAdjustEndTime,1));
    //                }
    //                //TRACE(L"Action%d's NewValue is %f \n",i,AdjustBlockData->GetEndTime() );
    //                nEndTime += AdjustBlockData->GetAllTime();
    //            }
    //        }
    //        else
    //            nEndTime = pGroupManager->GetEndTime();
    //        int nPrvEndTime = pGroupManager->GetEndTime();
    //        pGroupManager->SetEndTime(fEndTime/*pGroupManager->GetEndTime() + Myoffset*/);
    //        // 		if (Myoffset>=0)
    //        // 		{
    //        // 			AdjustAllGroupTime(pGroupManager, AdjustEndTime * pGroupManager->GetCount());
    //        // 		}
    //        // 		else
    //        AdjustAllGroupTime(pGroupManager, nEndTime - nPrvEndTime);
    //        if (m_pLayerManager)
    //        {
    //            g_IsUpdateUIByData = true;
    //            m_pLayerManager->SetActionDataGroupManagerToOptions(m_pBlockDataManager, pGroupManager->GetID(),0);
    //            m_pLayerManager->UpdateLayerWidget(m_pBlockDataManager, pGroupManager->GetID(), 0, m_curLayer);
    //            g_IsUpdateUIByData = false;

    //            //            m_pLayerManager->GetWidget()->Invalidate(false);
    //        }

    //        m_IsDragMouse = false;
    //        m_positionDragMouse = 0;
    //        if (pfOffsetArray)
    //        {
    //            delete []pfOffsetArray;
    //            pfOffsetArray = NULL;
    //        }
    //    }
}

//void CLayerCommonImpl::OnMouseMove(QPoint point)
//{
//    if (m_pBlockDataManager == NULL || m_pLayerManager == NULL)
//    {
//        return;
//    }
//    int nLineThick = m_pLayerManager->GetLineThick();
//    int nCellWidth = m_pLayerManager->GetCellWidth();
//    CActionBlockGroupManager* pGroupManager;
//    for (int j = 0; j < m_pBlockDataManager->GetCount(); j++)
//    {

//        pGroupManager = m_pBlockDataManager->GetAt(j);
//        if (pGroupManager == NULL)
//        {
//            continue;
//        }
//        int endTime = pGroupManager->GetEndTime() - GetCellStartIndex()*(nLineThick + nCellWidth);
//        QRect testRect((endTime-2), point.y()-10, endTime, point.y()+10);

//        //		if (testRect.PtInRect(point))
//        //		{
//        //			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
//        //			break;
//        //		}
//    }

//    if (m_IsDragMouse)
//    {
//        //		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
//        //		if (m_pLayerManager)
//        //		{
//        //			m_pLayerManager->SetCurrentSelectCol(GetCellStartIndex()*(nLineThick + nCellWidth) + point.x);
//        //		}
//    }
//}

void CLayerCommonImpl::OnLButtonUpAndShift()
{
    //    m_bMouseDown = false;
    if (m_GroupSelectID.count() > 1)
    {
        int resultValue = 0/*AfxMessageBox(language_loadstring(IDS_STRING_MOTIONLAYER_MERGELAYER), MB_YESNO, MB_ICONINFORMATION)*/;
        //		if (resultValue == IDCANCEL)
        //		{
        //			ClearSelectGroupBlock();
        //			return;
        //		}
        if (resultValue == 0/*IDYES*/)
        {
            g_IsModifyGroup = true;
            CActionBlockGroupManager* pGroupData = NULL;
            // 第一个合并组
            pGroupData = m_pBlockDataManager->FindBlockData(GetSelectGroupBlock());
            int nIncTime = 0;
            for (int i = 1; i < m_GroupSelectID.count(); i++)
            {
                int nGroupID = m_GroupSelectID.at(i);
                CActionBlockGroupManager* waitInsertGroup = m_pBlockDataManager->FindBlockData(nGroupID);

                for (int j =0; j < waitInsertGroup->GetActionCount(); j++)
                {
                    CActionBlockData* pActionBlockData = waitInsertGroup->GetAt(j);

                    // 先NEW内存出来，然后复制过去
                    CActionBlockData* pNewBlockData = new CActionBlockData();
                    char* pData = NULL;
                    int nBlockLen = 0;
                    pActionBlockData->GetData(&pData, nBlockLen);
                    if (pData && nBlockLen)
                    {
                        pNewBlockData->SetData(pData, nBlockLen);
                        delete [] pData;
                        pNewBlockData->SetID(pGroupData->GetUnUserID());
                    }
                    pGroupData->AddLayerBlock(pNewBlockData);
                }

                if (i == (m_GroupSelectID.count()-1))
                {
                    nIncTime = waitInsertGroup->GetEndTime();
                }
                // Modified Flag
                if (m_pLayerManager)
                {
                    m_pLayerManager->IUILayerManager_SetModifiedState(true);
                }
                m_pBlockDataManager->DeleteLayerBlock(nGroupID);
            }

            pGroupData->SetEndTime(nIncTime);

            if (m_pLayerManager)
            {
                g_IsUpdateUIByData = true;
                m_pLayerManager->SetActionDataGroupManagerToOptions(m_pBlockDataManager, pGroupData->GetID(),0);
                m_pLayerManager->UpdateLayerWidget(m_pBlockDataManager, pGroupData->GetID(),0, GetType());
                g_IsUpdateUIByData = false;
                //                m_pLayerManager->GetWidget()->Invalidate(false);
            }
        }
    }
}

void CLayerCommonImpl::OnLButtonDownAndShift(int nCol)
{
    int selectGroupId = GetSelectGroupBlock();
    if (selectGroupId == -1)
    {
        SetSelectGroupBlock(nCol);
    } else {
        ClearSelectGroupBlock();
        CActionBlockGroupManager* pGroupData = NULL;
        pGroupData = m_pBlockDataManager->FindBlockData(selectGroupId);
        int startTime = pGroupData->GetStartTime();
        int endTime = pGroupData->GetEndTime();

        if (nCol > endTime)
        {
            for (int i=0; i<m_pBlockDataManager->GetCount(); i++)
            {
                CActionBlockGroupManager* pManager = m_pBlockDataManager->GetAt(i);
                if (pManager == NULL)
                    continue;

                int nStartTime = pManager->GetStartTime();
                //int nEndTime = pManager->GetEndTime();

                if (nStartTime >= startTime && nStartTime <= nCol)
                {
                    int nId = pManager->GetID();
                    m_GroupSelectID.append(nId);
                }
            }
        }

        else if (nCol < startTime)
        {
            for (int i=0; i<m_pBlockDataManager->GetCount(); i++)
            {
                CActionBlockGroupManager* pManager = m_pBlockDataManager->GetAt(i);
                if (pManager == NULL)
                    continue;

                //int nStartTime = pManager->GetStartTime();
                int nEndTime = pManager->GetEndTime();

                if (nEndTime >= nCol && nEndTime <= endTime)
                {
                    int nId = pManager->GetID();
                    m_GroupSelectID.append(nId);
                }
            }
        }
        else
        {
            SetSelectGroupBlock(nCol);
        }
    }
}

void CLayerCommonImpl::SaveToCommandBox()
{
    CActionBlockGroupManager* pGroupData = NULL;
    for (int i=0; i<m_GroupSelectID.count(); i++)
    {
        int nGroupID = m_GroupSelectID.at(i);
        pGroupData = m_pBlockDataManager->FindBlockData(nGroupID);
    }

    if (pGroupData)
    {
        DialogCommandBox dlg;
        QFile styleSheet(":/res/qss/motiondefault.qss");
        if(styleSheet.open(QIODevice::ReadOnly))
        {
            dlg.setStyleSheet(styleSheet.readAll());
        }
        if (dlg.exec() == QDialog::Accepted)
        {
            QString strCommandBoxName = dlg.m_strCommandBoxName;
            if (!strCommandBoxName.isEmpty() && strCommandBoxName != "motion")
            {
                //leo
                //bool IsRename = false;

                //QString strCusActionPath = getUserDirPath(USERDIR_TEMPLATE_CUSTOM);
                //CreateActionDir(strCusActionPath); //如果自定义动作模板库不存在则创建自定义动作模板库目录
                //QDir dir(strCusActionPath);
                ////设置过滤器(目录，文件或非上级目录)
                //dir.setFilter(QDir::Files);
                //dir.setSorting(QDir::Name);
                ////取得目录中文件列表(包含目录)
                //QFileInfoList list = dir.entryInfoList();
                //int i=0;
                //if(!list.empty())
                //{
                //    do{
                //        QFileInfo fileInfo = list.at(i);
                //        QString strFileName = fileInfo.fileName();
                //        QString tempName = strCommandBoxName + UBX_TEMPLATE_EXT;
                //        if (strFileName.compare(tempName, Qt::CaseInsensitive) == 0)
                //        {
                //            IsRename = true;
                //            break;
                //        }

                //        i++;
                //    }while(i<list.size());
                //}

                //if (!IsRename)
                //{
                //    char* pdata = NULL;
                //    int nLens = 0;
                //    pGroupData->GetData(&pdata, nLens);
                //    qDebug()<<"Tree Node: m_treeView = "<<m_treeView<<endl;
                //    if (m_treeView)
                //    {
                //        // leo add 20170228  //动作模板
                //        m_treeView->AddNewTreeNodeAction(pdata, nLens, strCommandBoxName, (int)GetType());
                //    }
                //}
                //else
                //{
                //    QMessageBox::warning(NULL, "", QObject::tr("Same name block existed!"), QObject::tr("OK"));

                //}
                //leo add 20170228
                char* pdata = NULL;
                int nLens = 0;
                pGroupData->GetData(&pdata, nLens);
                qDebug()<<"Tree Node: m_treeView = "<< m_treeView <<endl;
                if (m_treeView !=  NULL)
                {
                    // leo add 20170228  //动作模板 可以丰富报错内容
                    int nError = m_treeView->AddNewTreeNodeAction(pdata, nLens, strCommandBoxName, (int)GetType());
                    if (nError == SAVE_TEMPLATE_NAME_EXIST_ERROR)
                        QMessageBox::warning(NULL, "", QObject::tr("Same name block existed!"), QObject::tr("OK"));
                }
            }
            else if(strCommandBoxName.isEmpty())
            {
                QMessageBox::warning(NULL, "", QObject::tr("The name of the block cant not be empty!"), QObject::tr("OK"));
            }
            else
            {
                QMessageBox::warning(NULL, "", QObject::tr("Can't name the new block as motion.abc!"), QObject::tr("OK"));
            }
        }

    }
}

/***********************************************************
 * 函数名： CreateActionDir
 * 功能：  创建动作模板库目录
 * 参数：  QString& strPath（路径名）
 * 返回值： bool 当创建成功时，返回真，否则，返回假
 * 作者：  周志平
 * 时间：  2015/12/04 14:00:00
 ************************************************************/
bool CLayerCommonImpl::CreateActionDir(QString& strPath)
{
    bool ret = false;
    QDir dir;
    bool exist = dir.exists(strPath);
    if(!exist)
        ret = dir.mkdir(strPath);
    return ret;
}

void CLayerCommonImpl::InsertBlankTime(int nCol)
{
    if (GetSelectGroupBlock() == -1)
        return;

    int nGroupID = GetSelectGroupBlock();
    CActionBlockGroupManager* pGroupManager = m_pBlockDataManager->FindBlockData(nGroupID);
    if (pGroupManager == NULL)
        return;

    pGroupManager->SetStartTime(pGroupManager->GetStartTime()+1);
    pGroupManager->SetEndTime(pGroupManager->GetEndTime()+1);
    AdjustAllGroupTime(pGroupManager, 1);

    //刷新已发生变化了的层
    int nSelectGroupID = GetSelectGroupBlock();
    if(m_pLayerManager)
    {
        m_pLayerManager->UpdateLayerWidget(m_pBlockDataManager, nSelectGroupID, 0, GetType());
    }
}

/**************************************************************************
* 函数名: OnCopy
* 功能: 动作组复制
* 参数:
* 返回值:
* 时间:2015/01/26 17:00
* 作者: 周志平
* 改动日志  修改了剪贴板的实现方法
**************************************************************************/
void CLayerCommonImpl::OnCopy()
{
    if (GetSelectGroupBlock() == -1)
        return;

    int nGroupID = GetSelectGroupBlock();
    CActionBlockGroupManager* pGroupManager = m_pBlockDataManager->FindBlockData(nGroupID);
    if (pGroupManager == NULL)
        return;

    char* pData = NULL;
    int nLen = 0;
    pGroupManager->GetData(&pData, nLen);
    if (pData==NULL || nLen == 0)
        return;

    int nLayerID = GetType();

    int nSizeInt = sizeof(int);
    char* buffer = new char[nLen + sizeof(int)];
    memcpy(buffer, &nLayerID, sizeof(int));
    memcpy(buffer + sizeof(int), pData, nLen);

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->clear();
    QByteArray bArray(buffer,nLen + sizeof(int));

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("GroupCopyData", bArray);

    clipboard->setMimeData(mimeData);

#ifndef Q_OS_WIN
    //windows系统
    if(mimeData)
    {
        delete mimeData;
        mimeData = NULL;
    }
#endif

    if(buffer)
    {
        delete [] buffer;
        buffer = NULL;
    }
    if (pData && nLen)
    {
        delete [] pData;
    }
}
/**************************************************************************
* 函数名: OnPaste
* 功能: 动作组粘贴
* 参数: int nCol  时间轴
* 返回值:
* 时间:2015/01/26 17:00
* 作者: 周志平
**************************************************************************/
void CLayerCommonImpl::OnPaste(int nCol)
{
    const QClipboard *clipboard = QApplication::clipboard();
    const  QMimeData* mimeData = clipboard->mimeData();
    if(mimeData == NULL)
    {
        qDebug()<<QObject::tr("can't find mimeData point") << endl;
        return;
    }
    bool IsInGroup = false;
    int startTime = 0;
    int endTime = 0;
    int nStartTime = 0;
    CActionBlockGroupManager* pFindGroupManager = NULL;
    for (int j = 0; j < m_pBlockDataManager->GetCount(); j++)
    {
        pFindGroupManager = m_pBlockDataManager->GetAt(j);
        if (pFindGroupManager == NULL)
        {
            continue;
        }
        startTime = pFindGroupManager->GetStartTime();
        endTime = pFindGroupManager->GetEndTime();

        if (startTime <= nCol && (nCol < endTime))
        {
            IsInGroup = true;
            nStartTime = startTime;
            nCol = startTime;
            break;
        }
    }

    if (IsInGroup)
    {
        //从剪贴板读取Buffer
        QByteArray bArray = mimeData->data("GroupCopyData");
        char * buffer = bArray.data();
        int nLen = bArray.length();
        if(nLen > 0)
        {
            int nTypeID = 0;
            memcpy(&nTypeID, buffer, sizeof(int));
            if (nTypeID == GetType())
            {
                CActionBlockGroupManager* pGroupManager = new CActionBlockGroupManager();
                pGroupManager->SetData(buffer+sizeof(int), nLen-sizeof(int));
                int nGroupLen = pGroupManager->GetEndTime()-pGroupManager->GetStartTime();
                pGroupManager->SetStartTime(nStartTime);
                pGroupManager->SetEndTime(nStartTime + nGroupLen);
                pGroupManager->SetID(m_pBlockDataManager->GetUnUserID());

                m_pBlockDataManager->MoveGroup(pFindGroupManager, nGroupLen); //后移
                m_pBlockDataManager->AddLayerBlock(pGroupManager);

                AjustTimeForInsert(pFindGroupManager, nGroupLen);
                ClearSelectGroupBlock();
                SetSelectGroupBlock(pGroupManager->GetStartTime()+2);
                g_IsUpdateUIByData = true;
                m_pLayerManager->SetActionDataGroupManagerToOptions(m_pBlockDataManager, pGroupManager->GetID(),0);
                m_pLayerManager->UpdateLayerWidget(m_pBlockDataManager, pGroupManager->GetID(),0, GetType());
                g_IsUpdateUIByData = false;
            }
        }
        //        GlobalUnlock(hData);
    }
    else
    {
        //从剪贴板读取Buffer
        QByteArray bArray = mimeData->data("GroupCopyData");
        char * buffer = bArray.data();
        int nLen = bArray.length();

//        CActionBlockGroupManager* pTempGroupManager = NULL;
//        pTempGroupManager = m_pBlockDataManager->GetFinalBlockData();
//        if (pTempGroupManager)
//        {
//            nStartTime = pTempGroupManager->GetEndTime();
//            nCol = pTempGroupManager->GetEndTime();
//        }
//        else
//        {
//            nStartTime = 0;
//            nCol = 0;
//        }
        if (nLen > 0)
        {
            int nTypeID = 0;
            memcpy(&nTypeID, buffer, sizeof(int));
            if (nTypeID == GetType())
            {
                CActionBlockGroupManager* pGroupManager = new CActionBlockGroupManager();
                pGroupManager->SetData(buffer+sizeof(int), nLen-sizeof(int));

                //TODO:Joker 偏移的时间为当前列开始时间减去原来的开始时间
                int nBlockOffset = nCol-pGroupManager->GetStartTime() -1;
                //TODO:Joker 开始时间为当前列减1
                pGroupManager->SetStartTime(nCol - 1);

                pGroupManager->SetEndTime(pGroupManager->GetEndTime()+nBlockOffset);
                pGroupManager->SetID(m_pBlockDataManager->GetUnUserID());
                m_pBlockDataManager->AddLayerBlock(pGroupManager);

                SetSelectGroupBlock(pGroupManager->GetStartTime());
                AdjustAllGroupTime(pGroupManager, pGroupManager->GetEndTime()-pGroupManager->GetStartTime());

//                if (m_pLayerManager)
//                {
//                    m_pLayerManager->SetActionDataGroupManagerToOptions(GetBlockDataManager(), pGroupManager->GetID());
//                }


//                int nBlockOffset = pGroupManager->GetEndTime() - pGroupManager->GetStartTime();

                if (m_pLayerManager)
                {
                    m_pLayerManager->IUILayerManager_SetModifiedState(true);
                }

//                pGroupManager->SetStartTime(nStartTime);
//                pGroupManager->SetEndTime(nStartTime+nBlockOffset);
//                pGroupManager->SetID(m_pBlockDataManager->GetUnUserID());
//                m_pBlockDataManager->AddLayerBlock(pGroupManager);

//                SetSelectGroupBlock(pGroupManager->GetStartTime()+2);
//                AdjustAllGroupTime(pGroupManager, pGroupManager->GetEndTime()-pGroupManager->GetStartTime());
                g_IsUpdateUIByData = true;
                m_pLayerManager->SetActionDataGroupManagerToOptions(m_pBlockDataManager, pGroupManager->GetID(), 0);
                m_pLayerManager->UpdateLayerWidget(m_pBlockDataManager, pGroupManager->GetID(),0, GetType());
                g_IsUpdateUIByData = false;
            }

        }
    }
    //    if (m_nPasteType == 2)
    //    {
    //        m_nPasteType = 0;  //Reset paste type after cut;
    //    }
}

void CLayerCommonImpl::DeleteSelectGroup()
{
    int TempGroupId = -1;
    for (int i=0; i<m_GroupSelectID.count(); i++)
    {
        int nGroupID = m_GroupSelectID.at(i);
        CActionBlockGroupManager* pGroup = m_pBlockDataManager->FindBlockData(nGroupID);
        int nMoveBlock = 0;
        if (pGroup)
        {
            nMoveBlock = pGroup->GetEndTime()-pGroup->GetStartTime();
        }
        int GroupIndex = m_pBlockDataManager->GetIndexByID(nGroupID);
        CActionBlockGroupManager* pGroupToMove = m_pBlockDataManager->GetAt(GroupIndex+1);
        // Modified Flag
        if (m_pLayerManager)
        {
            m_pLayerManager->IUILayerManager_SetModifiedState(true);
        }

        m_pBlockDataManager->DeleteLayerBlock(nGroupID);
        if (pGroupToMove)
        {
            TempGroupId = pGroupToMove->GetID();
            MoveAllGourpTime(pGroupToMove, -nMoveBlock);
        }
        else
        {
            TempGroupId = -1;
            // 如果是最后一块数据，则移动总体的时间
            if (GroupIndex > 0){
                CActionBlockGroupManager* pGroupToFront = m_pBlockDataManager->GetAt(GroupIndex-1);
                if (pGroupToFront)
                {
                    AdjustAllGroupTime(pGroupToFront, -nMoveBlock);
                }
            }
            else if (GroupIndex == 0)
            {
                // 当前层已经没有数据了
                int nCellCount = m_pLayerManager->GetNewMaxCellCount(0)+ADD_OFFSET;
                if ((m_pLayerManager->GetNewMaxCellCount(0)) % 100)
                {
                    nCellCount++;
                }
                m_pLayerManager->SetCellCount(nCellCount);
            }

        }

    }
    // 清空所有已经选择的块
    m_GroupSelectID.clear();
    if (TempGroupId != -1)
        m_GroupSelectID.append(TempGroupId);

    if (m_pLayerManager)
    {
        g_IsUpdateUIByData = true;
        m_pLayerManager->SetActionDataGroupManagerToOptions(GetBlockDataManager(), TempGroupId, 0);
        m_pLayerManager->UpdateLayerWidget(GetBlockDataManager(), TempGroupId, 0, GetType());
        g_IsUpdateUIByData = false;
    }
}

void CLayerCommonImpl::DoMenuCommand(uint nCommand, int nCol)
{
    ClearSelectGroupBlock();
    SetSelectGroupBlock(nCol);
    switch(nCommand)
    {
    case COPY_ACTION_CHOSEN:
        OnCopy();
        break;
    case PASTE_ACTION_CHOSEN:
        OnPaste(nCol);
        break;
    case CUT_ACTION_CHOSEN:
        OnCopy();
        DeleteSelectGroup();
        break;
    case DELETE_ACTION_CHOSEN:
        DeleteSelectGroup();
        break;
    case SELECTEDBLOCKPLAY_ACTION_CHOSEN:
        SelectedBlockPlay();
        break;
    case CANCELBLOCKPLAY_ACTION_CHOSEN:
        CancelBlockPlay();
        break;
    case INSERTBLANKTIME_ACTION_CHOSEN:
        InsertBlankTime(nCol);
//        OnMenuInsert(nCol);
        break;
    case SAVETOCOMMANDBOX_ACTION_CHOSEN:
        SaveToCommandBox();
        break;
    case MODIFYGROUP_ACTION_CHOSEN:
        OnModifyGroupName();
        CLayerCommonImpl::OnLButtonDBClick(nCol);
        break;
    default:
        break;
    }
}

void CLayerCommonImpl::OnModifyGroupName()
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
        std::wstring wsDescription = strDescription.toStdWString();
        CActionBlockGroupManager* pCurGroupManager = m_pBlockDataManager->FindBlockData(GetSelectGroupBlock());

        if (pCurGroupManager) {
            pCurGroupManager->SetDescription((WCHAR *)wsDescription.c_str());
        }
        
        if (m_pEvent) {
            ILayerOptionHandler_OnGroupManagerChange(GetSelectGroupBlock(), 0, true);
        }

        g_IsModifyGroup = true;
        m_pLayerManager->SetActionDataGroupManagerToOptions(m_pBlockDataManager, GetSelectGroupBlock(), 0);
    }
}

void CLayerCommonImpl::OnMenuInsert(int nCol)
{
    if (m_pLayerManager == NULL || m_pBlockDataManager == NULL)
    {
        return;
    }
    //    int nCellWidth = m_pLayerManager->GetCellWidth() + m_pLayerManager->GetLineThick();
    //////////////////////////////////////////////////////////////////////////
    //首先记住插入之前的组Index（也是插入动作帧之后的组Index）
    ClearSelectGroupBlock();
    SetSelectGroupBlock(nCol);
    int nGroupIndex = 0;
    int nActionIndex = 0;
    int nOffset = 0;
    GetSelectGroupAction(nCol,nGroupIndex,nActionIndex,nOffset);
    if (nGroupIndex == -1 || nActionIndex == -1)
    {
        return;
    }
    //////////////////////////////////////////////////////////////////////////
    if (m_pOptionHandler)
    {
//        m_pOptionHandler->InsertGroupByMenu(nCol);
    }
    //插入空白帧后需要重新刷新界面显示（调用SetActionDataGroupManagerToOptions函数刷新）

    //插入之后高亮当前插入组，级nCol位置
    // 	int nGroupPrvIndex = nGroupIndex -1 ;
    // 	if (nGroupPrvIndex == -1)
    // 	{
    // 		return;
    // 	}
    int nPrvCenterPos = m_pBlockDataManager->GetAt(nGroupIndex)->GetStartTime() + 1;
    //- GetCellStartIndex()* nCellWidth ;
    ClearSelectGroupBlock();
    SetSelectGroupBlock(nPrvCenterPos);

    int nSelGroupID = GetSelectGroupBlock();
    if (m_pLayerManager)
    {
        g_IsUpdateUIByData = true;
        m_pLayerManager->SetActionDataGroupManagerToOptions(GetBlockDataManager(), nSelGroupID,0);
        m_pLayerManager->UpdateLayerWidget(GetBlockDataManager(), nSelGroupID, 0, GetType());
        g_IsUpdateUIByData = false;
    }
}

void CLayerCommonImpl::OnRButtonDown(int nCol)
{
    ClearSelectGroupBlock();
    SetSelectGroupBlock(nCol);
    int nGroupIndex = 0;
    int nActionIndex = 0;
    int nOffset = 0;
    GetSelectGroupAction(nCol,nGroupIndex,nActionIndex,nOffset);
    int nSelGroupID = GetSelectGroupBlock();
    if (m_pLayerManager)
    {
        g_IsUpdateUIByData = true;
        m_pLayerManager->SetActionDataGroupManagerToOptions(GetBlockDataManager(), nSelGroupID , nActionIndex);
        m_pLayerManager->UpdateLayerWidget(GetBlockDataManager(), nSelGroupID, nActionIndex, GetType());
        g_IsUpdateUIByData = false;
    }
}

void CLayerCommonImpl::OnLButtonDownDebug(int nCol)
{

}

void CLayerCommonImpl::OnLButtonDbClickDebug(int nCol)
{
    int nGroupIndex = 0;
    int nActionIndex = 0;
    int nOffsetTime = 0;
    //    IUBXRobot3DView *pView = NULL;
    //	if (m_pLayerManager)
    //	{
    //		pView = m_pLayerManager->Get3DRobotViewInterface();
    //	}
    GetSelectGroupAction(nCol,nGroupIndex,nActionIndex,nOffsetTime);
    CActionBlockGroupManager *pManger = NULL;
    CActionBlockData *pActionBlock = NULL;
    if (nGroupIndex == -1 || nActionIndex == -1)
    {
        return;
    }
    if (m_pBlockDataManager)
    {
        pManger = m_pBlockDataManager->GetAt(nGroupIndex);
    }
    if (pManger)
    {
        pActionBlock = pManger->GetAt(nActionIndex);
    }

    char* pData = NULL;
    int nLen = 0;

    if (pActionBlock)
    {
        pActionBlock->GetExtBlockData(&pData, nLen);
    }

    if (pData)
    {
        delete []pData;
        pData = NULL;
    }
}

/**************************************************************************
* 函数名:createLayoutMenuPopMenuActions
* 功能:动作表鼠标右键菜单创建
* 参数:
* 返回值:
* 时间:2016/01/07
* 作者: 周志平
* 改动日志
**************************************************************************/
void CLayerCommonImpl::createLayoutMenuPopMenuActions()
{

}

//void CLayerCommonImpl::OnDraw(/*CDC* pDC*/)
//{
//    //	// 先画基本部分
//    //	DrawBase(pDC);
//    //	/*WM_SIZE*/
//    //	DrawgBlockGroup(pDC);

//}

bool CLayerCommonImpl::GetData(char** ppData, int& nLen)
{
    if (m_pBlockDataManager)
    {
        return m_pBlockDataManager->GetData(ppData, nLen);
    }

    return true;
}
//void CLayerCommonImpl::AdjustData()
//{
//    //    if (m_pBlockDataManager == NULL)
//    //    {
//    //        return;
//    //    }
//    //    int nEndTime = 0;
//    //    for (int i = 0;i<m_pBlockDataManager->GetCount();i++)
//    //    {
//    //        CActionBlockGroupManager *pGroupManager = m_pBlockDataManager->GetAt(i);
//    //        int nTempTime = pGroupManager->GetActionAllTimeSum();
//    //        pGroupManager->SetStartTime(nEndTime);
//    //        nEndTime += nTempTime;
//    //        pGroupManager->SetEndTime(nEndTime);
//    //    }
//}
bool CLayerCommonImpl::SetData(char* pData, int nLen)
{
    if (m_pBlockDataManager)
    {
        m_pBlockDataManager->SetData(pData, nLen);

    }

    return true;
}

// 取得类型，具体见宏定义
emActionLayerType CLayerCommonImpl::GetType()
{
    return ACTION_LAYER_UNKNOWN;
}

//void CLayerCommonImpl::SetHScrollOffset(int offset)
//{
//    int m_nCellWidth = m_pLayerManager->GetCellWidth();
//    int m_lineThick = m_pLayerManager->GetLineThick();

//    m_HScrollOffset = offset*(m_nCellWidth + m_lineThick);
//}

//void CLayerCommonImpl::SetPosition(int PositionX, int PositionY)
//{
//    x = PositionX;
//    y = PositionY;
//}

//void CLayerCommonImpl::GetPosition(int& x, int& y)
//{
//    x = this->x;
//    y = this->y;
//}

void CLayerCommonImpl::SetScalevalue(int scaleValue)
{
    //m_scaleValue = scaleValue;
    if (m_pLayerManager)
    {
        m_pLayerManager->SetTimeScaleValues(scaleValue);
        //CUIMainLayerLayout *pMainLayout =
    }
}

int CLayerCommonImpl::GetScaleValue()
{
    return m_pLayerManager->GetTimeScaleValues();
}

//void CLayerCommonImpl::SetCellStartIndex(uint CellIndex)
//{
//    m_CellStartIndex = CellIndex;
//}

//uint CLayerCommonImpl::GetCellStartIndex()
//{
//    return m_CellStartIndex;
//}

//int  CLayerCommonImpl::GetScreenCellCount()
//{
//    int m_nCellWidth = m_pLayerManager->GetCellWidth();
//    //	int m_lineThick = m_pLayerManager->GetLineThick();
//    //	int m_CellCount = m_pLayerManager->GetCellCount();
//    //	CRect rc;
//    //	if (m_ManagerHandle &&m_ManagerHandle->GetSafeHwnd())
//    //	{
//    //		m_ManagerHandle->GetClientRect(rc);
//    //	}
//    //	int nCount = rc.Width()/(m_lineThick + m_nCellWidth) + GetCellStartIndex();
//    //	//int nCount = rc.Width()+ GetCellStartIndex();
//    int nCount = 0;
//    return nCount;
//}
int  CLayerCommonImpl::GetCellCount()
{
    //return m_CellCount;
    return m_pLayerManager->GetCellCount();
}

//void CLayerCommonImpl::SetCellWidth(int width)
//{
//    m_pLayerManager->SetCellWidth(width);
//}

//void CLayerCommonImpl::GetLayerWidth(int &LayerWidth)
//{
//    int m_nCellWidth = m_pLayerManager->GetCellWidth();
//    int m_lineThick = m_pLayerManager->GetLineThick();
//    int m_CellCount = m_pLayerManager->GetCellCount();

//    LayerWidth = (m_nCellWidth + m_lineThick) * m_CellCount;
//}

//int CLayerCommonImpl::GetCellWidth()
//{
//    return m_pLayerManager->GetCellWidth();
//}

//void CLayerCommonImpl::SetCellHeight(int height)
//{
//    h = height;
//}

//int CLayerCommonImpl::GetCellHeight()
//{
//    return h;
//}

//void CLayerCommonImpl::SetLineThick(int nThick)
//{
//    //m_lineThick = nThick;
//    m_pLayerManager->SetLineThick(nThick);
//}

//int CLayerCommonImpl::GetLineThick()
//{
//    //return m_lineThick;
//    return m_pLayerManager->GetLineThick();
//}

//void CLayerCommonImpl::DrawgBlockGroup(/*CDC* pDC*/)
//{
//	if (m_pBlockDataManager == NULL)
//		return;

//	int m_lineThick = m_pLayerManager->GetLineThick();
//	int m_nCellWidth = m_pLayerManager->GetCellWidth();

//	int xStart, yStart;
//	GetPosition(xStart, yStart);

//	CRect rc;
//	m_ManagerHandle->GetClientRect(rc);
//	int DisplayEnd = rc.Width()/(m_lineThick + m_nCellWidth) + GetCellStartIndex();

//	for (int i=0; i<m_pBlockDataManager->GetCount(); i++)
//	{
//		CActionBlockGroupManager* pGroupManager = m_pBlockDataManager->GetAt(i);
//		if (pGroupManager == NULL)
//			continue;

//		int nStartTime = pGroupManager->GetStartTime()/(m_nCellWidth+m_lineThick);
//		int nEndTime = pGroupManager->GetEndTime()/(m_nCellWidth+m_lineThick);

//		CRect rect;
// 		rect.top = yStart;
// 		rect.bottom = rect.top+GetCellHeight()-1;

// 		if ((nStartTime <= DisplayEnd) && (nEndTime >= GetCellStartIndex()))
// 		{
////			rect.left = (nStartTime-GetCellStartIndex())*(m_nCellWidth+m_lineThick);
//			rect.left = pGroupManager->GetStartTime() - GetCellStartIndex()*(m_nCellWidth+m_lineThick);
//			float s = pGroupManager->GetEndTime();
//  			if (nEndTime <= DisplayEnd)
//  			{
////  			rect.right = (nEndTime-GetCellStartIndex())*(m_nCellWidth+m_lineThick);
//				//float fEndTime = pGroupManager->GetEndTime();
//				rect.right = pGroupManager->GetEndTime() - GetCellStartIndex()*(m_nCellWidth+m_lineThick);
//  			}
//			else
//			{
////  			rect.right = (DisplayEnd+1)*(m_nCellWidth+m_lineThick);
//				rect.right = (DisplayEnd+1)*(m_nCellWidth+m_lineThick);
//  			}
//		}

//		if (IsSelectGroupBlock(pGroupManager->GetID()))
//		{
//			CBrush bgBrush(m_GroupBlockSelColor);
//			pDC->FillRect(rect, &bgBrush);
//		} else {
//			CBrush bgBrush(m_GroupBlockDefColor);
//			pDC->FillRect(rect, &bgBrush);
//		}

//		char* pdata = NULL;
//		int nLens = 0;
//		pGroupManager->GetExtBlockData(&pdata, nLens);
//		delete[] pdata;
//		if (nLens == 12 || nLens == 14)
//		{
//			CBrush bgBrush(RGB(120, 80, 100));
//			pDC->FillRect(rect, &bgBrush);
//		}

//		// 画层名称
//		CFont font;
//		VERIFY( font.CreatePointFont( /*this->m_fontSize*/100, this->m_fontFamily, pDC ) );
//		CFont* def_font = pDC->SelectObject( &font );

//		CSize m_size = pDC->GetTextExtent( pGroupManager->GetDescription() ); //计算buf中的字体总宽为多少个像素
//		pDC->SetBkMode( TRANSPARENT );						  //设置字体背景为透明
//        CRect fontRect( rect.TopLeft(), QPoint( rect.right, rect.top + rect.Height()) );

//		pDC->SetTextColor( this->m_fontColor );
//        QString DescriptionInfo = L"  " + (QString)pGroupManager->GetDescription();
//		pDC->DrawText(DescriptionInfo, &fontRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE );

//		// 画实底线
//        QPen pen( PS_SOLID, /*this->m_lineThick*/m_pLayerManager->GetLineThick(), RGB(0, 0, 0));
//		pDC->SelectObject( &pen );

//		pDC->MoveTo(rect.left, rect.top);
//		pDC->LineTo(rect.right, rect.top);

//		pDC->MoveTo(rect.left, rect.top);
//		pDC->LineTo(rect.left, rect.bottom);

//		pDC->MoveTo(rect.left, rect.bottom);
//		pDC->LineTo(rect.right, rect.bottom);

//		pDC->MoveTo(rect.right, rect.top);
//		pDC->LineTo(rect.right, rect.bottom);

//		pDC->SelectObject(def_font);
//		font.DeleteObject();
//	}
//}

//void CLayerCommonImpl::DrawBase(/* CDC* pDC*/ )
//{
//	int m_nCellWidth = m_pLayerManager->GetCellWidth();
//	int m_lineThick = m_pLayerManager->GetLineThick();
//	int m_CellCount = m_pLayerManager->GetCellCount();

//	//画背景
//	int wTmp = (m_nCellWidth + m_lineThick) * m_CellCount ;
//	CRect rt( x, y, x + wTmp, y + h );


//	int xStart, yStart;
//	GetPosition(xStart, yStart);
//	int xTmp = xStart;

//	CRect rc;
//	m_ManagerHandle->GetClientRect(rc);
//	int nCount = rc.Width()/(m_lineThick + m_nCellWidth) + GetCellStartIndex();
//	CBrush bgBrush(RGB(254, 254, 254));
//	rt.right = (m_nCellWidth + m_lineThick) * (m_CellCount - GetCellStartIndex());
//	pDC->FillRect( rt, &bgBrush );
//    for( uint i = GetCellStartIndex(); i <= nCount; i++ )
//	{
//		if (i >= m_CellCount)
//			break;

//		//画竖线
//        QPen pen( PS_SOLID, m_pLayerManager->GetLineThick(), this->m_lineColor);
//		pDC->SelectObject( &pen );
//		pDC->MoveTo( xTmp, y);
//		pDC->LineTo( xTmp, y + h);

//		pDC->MoveTo(xTmp, y + h-1);
//		pDC->LineTo(xTmp+m_nCellWidth+m_lineThick, y+h-1);

//		if( (i+1) % m_divisor == 0 )
//		{
//			RECT rt;
//			rt.left = xTmp + 1;
//			rt.top = y;
//			rt.right = xTmp + m_nCellWidth;
//			rt.bottom = y + h-1;

//			CBrush brush(RGB(240, 240, 240));
//			pDC->SelectObject(&brush);
//			pDC->FillRect( &rt, &brush );
//		}

//		xTmp += m_lineThick;
//		xTmp += m_nCellWidth;
//	}

//	pDC->MoveTo( xTmp, y);
//	pDC->LineTo( xTmp, y + h);
//}

void CLayerCommonImpl::CreateOptionHandler()
{
    m_pOptionHandler = NULL;
}

///**************************************************************************
//* CreateLayerComonWidgetContainer
//* 功能: 创建公共层窗体
//* 参数: 无
//* 返回值: 创建成功返回ture,否则返回false
//* 时间: 2017/1/16
//* 作者: Joker
//*/
//bool CLayerCommonImpl::CreateLayerComonWidgetContainer()
//{
//    //    m_pLayerCommonWidgetContainer = new UILayerCommonContainer(this);
//    //    if (m_pLayerCommonWidgetContainer)
//    //    {
//    //        return true;
//    //    }
//    //    else
//    //    {
//    //        return false;
//    //    }
//}

ILayerOptionHandler* CLayerCommonImpl::GetOptionHandler()
{
    return m_pOptionHandler;
}

int CLayerCommonImpl::GetMaxTimeCellCount()
{
    //m_pBlockDataManager->SortBlockDataManager();

    if (m_pBlockDataManager->GetCount() == 0)
        return 0;

    CActionBlockGroupManager* pManager = m_pBlockDataManager->GetAt(m_pBlockDataManager->GetCount()-1);
    if (pManager)
    {
        return pManager->GetEndTime();
    }

    return 0;
}

void CLayerCommonImpl::SetLayerManager(IUILayerManager* pManager)
{
    m_pLayerManager = pManager;
}

IUILayerManager* CLayerCommonImpl::GetLayerManager()
{
    return m_pLayerManager;

}

void CLayerCommonImpl::UpdateLayerAction(int nGroupID, int nStartTime, emActionLayerType nType)
{
    if(m_pLayerManager)
    {
        m_pLayerManager->UpdateLayerAction(nGroupID, nStartTime, nType);
    }
}

void CLayerCommonImpl::ILayerOptionHandler_OnSelectActionManageChange(int nGroupID, int nActionId)
{
    if (m_pLayerManager == NULL || nGroupID <= 0 || nActionId < 0)
    {
        return;
    }
    ClearSelectGroupBlock();
    m_GroupSelectID.append(nGroupID);
    CActionBlockGroupManager* pSelectGroup = m_pBlockDataManager->FindBlockData(nGroupID);
    if (pSelectGroup == NULL)
    {
        return;
    }
    int nActionStartTime = pSelectGroup->GetStartTime();

    for (int i = 0 ;i < nActionId;i++)
    {
        if (i>= pSelectGroup->GetActionCount())
        {
            break;
        }
        CActionBlockData * pData = pSelectGroup->GetAt(i);
        if (pData )
        {
            nActionStartTime += pData->GetRunTime() + pData->GetStopTime();
        }
        else
            break;
    }
    UpdateLayerAction(nGroupID, nActionStartTime, GetType());

    // Add by jianjie 2017/10/17 点击某一层动作数据时，同时显示其它层动作帧(后续有更好办法当修改此处)
    //if (m_pLayerManager)
    //{
    //    m_pLayerManager->OnLayerLButtonDown(GetType() , nActionStartTime + 1);
    //}
}

//void CLayerCommonImpl::Ilayeroptionhandler_MotionlayerActionCommand(uint nCommand, int nCol)
//{
//    switch(nCommand)
//    {
//    case COPY_ACTION_CHOSEN:
//        m_nPasteType = 1;
//        OnCopy();
//        break;
//    case PASTE_ACTION_CHOSEN:
//        OnPaste(nCol);
//        break;
//    case CUT_ACTION_CHOSEN:
//        m_nPasteType = 2;
//        OnCopy();
//        DeleteSelectGroup();
//        break;
//    case DELETE_ACTION_CHOSEN:
//        DeleteSelectGroup();
//        break;
//    case SELECTEDBLOCKPLAY_ACTION_CHOSEN:
//        SelectedBlockPlay();
//        break;
//    case CANCELBLOCKPLAY_ACTION_CHOSEN:
//        CancelBlockPlay();
//        break;
//    case INSERTBLANKTIME_ACTION_CHOSEN:
//        OnMenuInsert(nCol);
//        break;
//    case SAVETOCOMMANDBOX_ACTION_CHOSEN:
//        SaveToCommandBox();
//        break;
//    case MODIFYGROUP_ACTION_CHOSEN:
//        CLayerCommonImpl::OnLButtonDBClick(nCol);
//        break;
//    default:
//        break;
//    }
//}

void CLayerCommonImpl::ILayerOptionHandler_OnSelectGroupManageChange(int nGroupID, int nActionId)
{
    if (m_pLayerManager == NULL)
    {
        return;
    }
    ClearSelectGroupBlock();
    m_GroupSelectID.append(nGroupID);
    if (m_pLayerManager)
    {
        CActionBlockGroupManager* pSelectGroup = m_pBlockDataManager->FindBlockData(nGroupID);
        if (pSelectGroup)
        {
            int startTime = pSelectGroup->GetStartTime();
            UpdateLayerAction(nGroupID, startTime, GetType());
        }
    }
}

void CLayerCommonImpl::ILayerOptionHandler_OnDeleteGroupBlock()
{
    DeleteSelectGroup();
}

void CLayerCommonImpl::ILayerOptionHandler_OnActionBlockDataChange(int nGroupID, int nActionBlockID, int nTimeInc)
{
    //Modified Flag
    //ClearSelectGroupBlock();
    //m_GroupSelectID.append(nGroupID);
    CActionBlockGroupManager* pGroupManager = m_pBlockDataManager->FindBlockData(nGroupID);
    if (pGroupManager == NULL)
        return;

    CActionBlockData* pActionBlock = pGroupManager->FindBlockData(nActionBlockID);
    if (pActionBlock == NULL)
        return;

    // 索引号(计算时间，刷新游标位)
    int nIndex = pGroupManager->GetIndexByID(nActionBlockID);

    pGroupManager->SetEndTime(pGroupManager->GetEndTime() + nTimeInc);
    //     float fEndTime = pGroupManager->GetCurrentActionTimeSum();
    //     pGroupManager->SetEndTime(fEndTime + fTimeInc);
    AdjustAllGroupTime(pGroupManager, nTimeInc);
    if (m_pLayerManager)
    {
        m_pLayerManager->IUILayerManager_SetModifiedState(true);
        // 刷新UI
        m_pLayerManager->UpdateLayerWidget(m_pBlockDataManager , nGroupID , nActionBlockID , GetType());
    }

    if (m_pBlockDataManager)
    {
        int nStartTime = pGroupManager->GetStartTime();

        for (int i = 0 ; i < nIndex; ++i)
        {
            CActionBlockData * pData = pGroupManager->GetAt(i);
            if (pData )
            {
                nStartTime += pData->GetRunTime() + pData->GetStopTime();
            }
            else
                continue;
        }
        UpdateLayerAction(nGroupID, nStartTime, GetType());
    }
}
void CLayerCommonImpl::ILayerOptionHandler_SetModifiedState(bool bState)
{
    if (m_pLayerManager)
    {
        m_pLayerManager->IUILayerManager_SetModifiedState(bState);
    }
}
int CLayerCommonImpl::ILayerOptionHandler_GetScaleValue()
{
    return GetScaleValue();
}
void CLayerCommonImpl::ILayerOptionHandler_SetScaleValue(int nScale)
{
    if (nScale == 0)
    {
        return;
    }
    SetScalevalue(nScale);
    // 	if (m_pEvent)
    // 	{
    // 		m_pEvent->LayerUIEvent_SetScaleValue(nScale);
    // 	}
}
void CLayerCommonImpl::ILayerOptionHandler_OnGroupManagerChange(int nGroupID, int nTimeInc, bool bIsUpdateMp3Widget)
{
    CActionBlockGroupManager* pManager = m_pBlockDataManager->FindBlockData(nGroupID);
    if (pManager == NULL)
        return;
    pManager->RecalculateTime();
    //    char* pdatas = NULL;
    //    int nLenss = 0;
    //    pManager->GetExtBlockData(&pdatas, nLenss);
    //    delete [] pdatas;
    //    if (nLenss == 6 || nLenss == 12)
    //    {
    //        pManager->RecalculateTime();
    //    }

    if (m_pLayerManager)
    {
        AdjustAllGroupTime(pManager, nTimeInc);
        m_pLayerManager->SetActionDataGroupManagerToOptions(GetBlockDataManager(), nGroupID,-1);
        if(bIsUpdateMp3Widget)
        {
            m_pLayerManager->UpdateLayerWidget(GetBlockDataManager(), nGroupID, -1, GetType());
        }
    }
    // Modified Flag
    if (m_pLayerManager)
    {
        m_pLayerManager->IUILayerManager_SetModifiedState(true);
    }
}
/**************************************************************************
* 函数名:ILayerOptionHandler_OnAddActionBlock
* 功能:添加动作块函数
* 参数:int nGroupManagerID     当前组ID
* 参数:CActionBlockData* pActionBlockData    动作块数据
* 参数:QString strBlockName                  动作块名字
* 返回值:
* 时间:2015/12/25 14:38
* 作者: 周志平
* 改动日志：2015/12/25 14:38这里暂时取消了当前时间的限制，所有Group 的开始时间皆为0，结束时间皆为100
**************************************************************************/
void CLayerCommonImpl::ILayerOptionHandler_OnAddActionBlock(int nGroupManagerID,CActionBlockData* pActionBlockData,QString strBlockName,bool bIsUpdateMP3Widget)
{
    // 是否是新添加的GROUP MANAGE
    bool bNotify = false;
    if (pActionBlockData == NULL)
        return;
    if(m_pBlockDataManager == NULL)
        return;

    CActionBlockGroupManager* pGroupManager = NULL;
    //    int nStartTime = 0;
    // 如果是－1，则根据当前的时候点，创建一个GROUP DATA
    if (nGroupManagerID == -1)
    {
        //        CActionBlockGroupManager* pTempGroupManager = NULL;
        //        if (m_pBlockDataManager->GetCount()>0)
        //        {
        //            pTempGroupManager = m_pBlockDataManager->GetFinalBlockData();
        //            nStartTime = pTempGroupManager->GetEndTime();
        //            if (m_pLayerManager->GetCurrentSelTime() <= nStartTime)
        //            {
        //                delete pActionBlockData;
        //                pActionBlockData =NULL;
        //                return;
        //            }
        //        }
        //        int nEndTime = nStartTime;
        //        int nCol = m_pLayerManager->GetCurrentSelectCol();
        //        int nGridMaxValue = GRID_MAX_TIMEVALUE / (m_pLayerManager->GetTimeScaleValues());
        //        int nGridMinValue = GRID_MIN_TIMEVALUE / (m_pLayerManager->GetTimeScaleValues());
        //        if (nCol - nStartTime > nGridMaxValue)
        //        {
        //            nEndTime = nStartTime + nGridMaxValue;
        //        }
        //        else if (nCol - nStartTime < nGridMinValue)
        //        {
        //            nEndTime = nStartTime + nGridMinValue;
        //        }
        //        else
        //            nEndTime = nCol;
        pGroupManager = new CActionBlockGroupManager();
        pGroupManager->SetID(m_pBlockDataManager->GetUnUserID());

        //WCHAR wstrDescription[MAX_DESCRIPTION_SIZE];
        //memset(wstrDescription,0,MAX_DESCRIPTION_SIZE*sizeof(WCHAR));
        std::wstring wsDescription = L"";
        int nDescriptionLen = 0;
        if (strBlockName.isEmpty())
        {
            QString strDescription = m_pBlockDataManager->GetUnUseDescription();
            //nDescriptionLen = QStringToWCHAR(wstrDescription, strDescription);
            wsDescription = strDescription.toStdWString();
            //wstrDescription[nDescriptionLen-1] = '\0';
        }
        else
        {
            //nDescriptionLen = QStringToWCHAR(wstrDescription, strBlockName);
            wsDescription = strBlockName.toStdWString();
            //wstrDescription[nDescriptionLen-1] = '\0';
        }
        pGroupManager->SetDescription((WCHAR *)wsDescription.c_str());

        //组的时间是时间刻度，这里的开始时间要等于格数-1
        pGroupManager->SetStartTime(m_pLayerManager->GetCurrentSelTime() - 1);
        pGroupManager->SetEndTime(pGroupManager->GetStartTime());

        m_pBlockDataManager->AddLayerBlock(pGroupManager);

        SetSelectGroupBlock(m_pLayerManager->GetCurrentSelTime());

        bNotify = true;
    }
    else
    {
        pGroupManager = m_pBlockDataManager->FindBlockData(nGroupManagerID);
        if(pGroupManager == NULL)
        {
            qDebug()<<QObject::tr("can't find *pGroupManager point")<<endl;
            return;
        }
    }
    // 重新分配结束时间
    int nLastEndTime = pGroupManager->GetEndTime();//分组的结束时刻
    int nCurrentStartTime = pActionBlockData->GetRunTime();
    int nCurrentEndTime = pActionBlockData->GetStopTime();
    int nTotalTime = nLastEndTime + nCurrentStartTime + nCurrentEndTime;//当前组的总时间
    pGroupManager->SetEndTime(nTotalTime);

    //    if (nCurrentAllTime == 0)
    //    {
    //        pActionBlockData->SetRunTime(nLastEndTime - nStartTime);
    //        pActionBlockData->SetStopTime(/*nLastEndTime - nStartTime*/0);
    //    }

    // 判断ACTION BLOCK的ID，如果为－1，则重新分配ID
    if (pActionBlockData->GetID() == -1)
    {
        pActionBlockData->SetID(pGroupManager->GetUnUserID());
    }
    pGroupManager->AddLayerBlock(pActionBlockData);
    AdjustAllGroupTime(pGroupManager, nCurrentStartTime + nCurrentEndTime);

    if (bNotify)
    {

        if (m_pLayerManager)
        {
            g_IsUpdateUIByData = true;
            m_pLayerManager->SetActionDataGroupManagerToOptions(m_pBlockDataManager, pGroupManager->GetID(),0);
            g_IsUpdateUIByData = false;
        }

    }
    // Modified Flag
    if (m_pLayerManager)
    {
        m_pLayerManager->IUILayerManager_SetModifiedState(true);
    }
    if (m_pLayerManager)
    {
        if(bIsUpdateMP3Widget)
        {
            m_pLayerManager->UpdateLayerWidget(m_pBlockDataManager, pGroupManager->GetID(),0, GetType());
        }
    }
}

void CLayerCommonImpl::PlayEvent_OnDebugStop(int nLayerType)
{
    m_IsSinglePlaying = false;
    m_pOptionHandler->SetControlState(true);
    m_IsPlaying = false;
}

void CLayerCommonImpl::SetLayerMenuState(bool menuState)
{
    m_IsPlaying = menuState;
}

void CLayerCommonImpl::SelectedBlockPlay()
{
    CActionBlockGroupManager* pCurrentPlayGroupManager = m_pBlockDataManager->FindBlockData(GetSelectGroupBlock());
    int endTime = pCurrentPlayGroupManager->GetEndTime();

    if (m_pLayerManager)
    {
        m_pLayerManager->IUILayerManager_SingleGroupPlaying(GetType(), pCurrentPlayGroupManager);
    }
    //if (m_pOptionHandler)
    //{
    //    m_pOptionHandler->SetControlState(false);
    //}
    m_IsPlaying = true;
    m_IsSinglePlaying =true;
}

void CLayerCommonImpl::CancelBlockPlay()
{
    if (m_pLayerManager)
    {
        m_pLayerManager->IUILayerManager_SingleBlockPlayingStop(GetType());
        m_IsPlaying = false;
        m_IsSinglePlaying = false;
        //m_pOptionHandler->SetControlState(true);
    }
}

void CLayerCommonImpl::ILayerOptionHandler_OnDataBlockPlaying(CActionBlockData *pActionData)
{
    if (pActionData == NULL)
    {
        return;
    }
    if (m_pLayerManager)
    {
        m_pLayerManager->IUILayerManager_SingleBlockPlaying(GetType(),pActionData);
    }
}
void CLayerCommonImpl::AddBlankBlockInTail()
{
    //	UBXAddBlankBlockDlg dlg;
    //	if (dlg.DoModal() == IDOK)
    //	{
    //		int CellCount = m_pLayerManager->GetCellCount();
    //		m_pLayerManager->SetCellCount(CellCount + dlg.m_BlankBlockCount);

    //		if(m_pLayerManager)
    //		{
    //            m_pLayerManager->GetWidget()->Invalidate(false);
    //			m_pLayerManager->UpdateScrollBar();
    //		}
    //	}
}

void CLayerCommonImpl::GetSelectGroupAction(int nCol, int &nGroupIndex, int &nActionIndex, int &nTimeOffset)
{
    CActionBlockGroupManager * pManager = NULL;
    bool bCursorInGroup =false;
    if (m_pBlockDataManager)
    {
        for (int i = 0;i<m_pBlockDataManager->GetCount();i++)
        {
            pManager = m_pBlockDataManager->GetAt(i);
            if (pManager)
            {
                int nGroupStartTime = pManager->GetStartTime();
                int nGroupEndTime = pManager->GetEndTime();
                if (nCol > nGroupStartTime)
                {
                    nGroupIndex = i;
                    if (nCol <= nGroupEndTime)
                    {
                        bCursorInGroup = true;
                        break;
                    } 
                    else
                    {
                        bCursorInGroup = false;
                    }
                }
            }
        }
    }
    if(!bCursorInGroup)
    {
        nActionIndex = -1;
        nTimeOffset =  -1;
        return;
    }
    CActionBlockData *pActionData = NULL;
    int nTimeSum = 0;
    if (pManager)
    {
        nTimeSum = pManager->GetStartTime();
        for (int i = 0;i<pManager->GetActionCount();i++)
        {
            pActionData = pManager->GetAt(i);
            if (pActionData)
            {
                int nActionEndTime = pActionData->GetRunTime() + pActionData->GetStopTime();
                nTimeSum += nActionEndTime;
                if ( nTimeSum >= nCol )
                {
                    nActionIndex = i;
                    nTimeOffset = nCol - nTimeSum + nActionEndTime;
                    break;
                }
            }
        }
    }
}

void CLayerCommonImpl::GetSelectGroupAction(int nCol, int &nGroupIndex, int &nActionIndex, int &nTimeOffset, int nStartTime)
{
    CActionBlockGroupManager* pManager = NULL;
    bool bCursorInGroup = false;
    if(m_pBlockDataManager)
    {
        SetActionBlockDataType::const_iterator iterStart = m_pBlockDataManager->FirstGroupIndex(nStartTime);
        for(; iterStart != m_pBlockDataManager->EndIndex(); iterStart++)
        {
            pManager = *iterStart;
            if(pManager)
            {
                int nGroupStartTime = pManager->GetStartTime();
                int nGroupEndTime = pManager->GetEndTime();
                if(nGroupEndTime >= nCol && nCol > nGroupStartTime)
                {
                    nGroupIndex = m_pBlockDataManager->GetIndexByID(pManager->GetID());
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
                if(nTimeSum >= nCol)
                {
                    nActionIndex = i;
                    nTimeOffset = nCol - nTimeSum + nActionEndTime;
                    break;
                }
            }
        }
    }
}

void CLayerCommonImpl::SetSelectGroupByOthers(int nCol)
{
    ClearSelectGroupBlock();
    SetSelectGroupBlock(nCol);
    int nGroupIndex = 0;
    int nActionIndex = 0;
    int nOffset = 0;
    GetSelectGroupAction(nCol,nGroupIndex,nActionIndex,nOffset);

    int nSelGroupID = GetSelectGroupBlock();
    if (m_pLayerManager)
    {
        g_IsUpdateUIByData = true;
        m_pLayerManager->SetActionDataGroupManagerToOptions(GetBlockDataManager(),  nSelGroupID,nActionIndex);
        m_pLayerManager->UpdateLayerWidget(GetBlockDataManager(), nSelGroupID, nActionIndex, GetType());
        g_IsUpdateUIByData = false;
    }
}

//这个也要重做
void CLayerCommonImpl::ILayerOptionHandler_InsertGroupByMenu(CActionBlockData* pActionBlockData, int nCol)
{
    //Modified Flag
    int startTime = 0;
    int endTime = 0;
    CActionBlockGroupManager* pFindGroupManager = NULL;
    for (int j = 0; j < m_pBlockDataManager->GetCount(); j++)
    {
        pFindGroupManager = m_pBlockDataManager->GetAt(j);
        startTime = pFindGroupManager->GetStartTime();
        endTime = startTime + pFindGroupManager->GetEndTime() - pFindGroupManager->GetStartTime();
        pFindGroupManager->SetEndTime(endTime);
        if ((startTime) <= nCol && (nCol < endTime))
        {
            if ((j-1) >=0)
            {
                CActionBlockGroupManager* pNextGroupManager(m_pBlockDataManager->GetAt(j-1));
                startTime = pNextGroupManager->GetStartTime();
                endTime = startTime + pFindGroupManager->GetEndTime() - pFindGroupManager->GetStartTime();
            } else {
                startTime = 0;
                endTime = 0;
            }
            break;
        }
    }

    CActionBlockGroupManager* pGroupManager = NULL;
    pGroupManager = new CActionBlockGroupManager();
    pGroupManager->SetID(m_pBlockDataManager->GetUnUserID());

    //WCHAR wstrDescription[MAX_DESCRIPTION_SIZE];
    //memset(wstrDescription, 0, MAX_DESCRIPTION_SIZE*sizeof(WCHAR));
    QString strDescription = m_pBlockDataManager->GetUnUseDescription();
    //int nDescriptionLen = QStringToWCHAR(wstrDescription, strDescription);
    //wstrDescription[nDescriptionLen - 1] ='\0';
    std::wstring wsDescription = strDescription.toStdWString();
    pGroupManager->SetDescription((WCHAR *)wsDescription.c_str());
    pGroupManager->SetStartTime(endTime);
    pGroupManager->SetEndTime(endTime + pActionBlockData->GetRunTime() + pActionBlockData->GetStopTime());
    //    char *pdatas = "motion";
    //    int nLenss = strlen(pdatas);
    //    pGroupManager->SetExtBlockData(nLenss, pdatas);

    m_pBlockDataManager->MoveGroup(pFindGroupManager, pActionBlockData->GetRunTime() + pActionBlockData->GetStopTime());
    m_pBlockDataManager->AddLayerBlock(pGroupManager);
    pGroupManager->AddLayerBlock(pActionBlockData);
    // Modified Flag
    if (m_pLayerManager)
    {
        m_pLayerManager->IUILayerManager_SetModifiedState(true);
    }
    if (pActionBlockData->GetID() == -1)
    {
        pActionBlockData->SetID(pGroupManager->GetUnUserID());
    }
    SetSelectGroupBlock(pGroupManager->GetStartTime());
    AjustTimeForInsert(pFindGroupManager, pGroupManager->GetEndTime()-pGroupManager->GetStartTime());
}

void CLayerCommonImpl::ILayerOptionHanlder_UnitedSimulation()
{
    if(m_pLayerManager)
    {
        m_pLayerManager->TotalLayerPlaying();
    }
}

void CLayerCommonImpl::ILayerOptionHanlder_PauseUnitedSimulation()
{
    if(m_pLayerManager)
    {
        m_pLayerManager->PauseTotalLayerDebug();
    }
}

void CLayerCommonImpl::ILayerOptionHanlder_StopUnitedSimulation()
{
    if(m_pLayerManager)
    {
        m_pLayerManager->StopTotalLayerDebug();
    }
}

void CLayerCommonImpl::ILayerOptionHandler_HasMotionData(bool hasData, int nLayerType)
{
    if(m_pLayerManager)
    {
        m_pLayerManager->HasMotionData(hasData, nLayerType);
    }
}

bool CLayerCommonImpl::ILayerOptionHandler_IsPlaying()
{
    return m_IsPlaying;
}

//void CLayerCommonImpl::ILayerOptionHandler_SetMusicModifiedState(bool bState)
//{
//    if(m_pLayerManager)
//    {
//        m_pLayerManager->IUILayerManager_SetMusicModifiedState(bState);
//    }
//}

//bool CLayerCommonImpl::ILayerOptionHandler_GetMusicModifiedState()
//{
//    if(m_pLayerManager)
//    {
//        return m_pLayerManager->IUILayerManager_GetMusicModifiedState();
//    }
//    else
//    {
//        return false;
//    }
//}

/**************************************************************************
* 函数名：ILayerOptionHandler_LoadMusic
* 功能:加载音乐文件
* 参数:
*    @[in ] strFilePath: 音乐文件路径
* 返回值: bool 如果加载成功返回true，否则返回false
* 时间: 2017/1/16
* 作者: Joker
*/
bool CLayerCommonImpl::ILayerOptionHandler_LoadMusic(QString strFilePath)
{
    bool bRet = false;

    if (m_pLayerManager)
    {
        bRet = m_pLayerManager->LoadMusic(strFilePath);
    }

    return bRet;
}

/**************************************************************************
* 函数名：ILayerOptionHandler_ShowExpression
* 功能:加载音乐文件
* 参数: strFileName   [IN]    表情文件名
*       nCurFrame     [IN]    表情帧号
* 返回值: 
* 时间: 2018/01/05
* 作者: jianjie
*/
void CLayerCommonImpl::ILayerOptionHandler_ShowExpression(QString strFileName, int nCurFrame)
{
    if (m_pLayerManager)
    {
        m_pLayerManager->IUILayerManager_ShowExpression(strFileName, nCurFrame);
    }
}

void CLayerCommonImpl::SetTreeView(ILibTreeNavigation* pView)
{
    m_treeView = pView;
}

void CLayerCommonImpl::TreeToView(char *pData, int nLen, int nCol, QString FileName)
{
    if (m_IsPlaying || m_IsSinglePlaying)
    {
        return;
    }
    bool IsInGroup = false;
    int startTime = 0;
    int endTime = 0;
    int nStartTime = nCol - 1;
    CActionBlockGroupManager* pFindGroupManager;
    for (int j = 0; j < m_pBlockDataManager->GetCount(); j++)
    {
        pFindGroupManager = m_pBlockDataManager->GetAt(j);
        startTime = pFindGroupManager->GetStartTime();
        endTime = pFindGroupManager->GetEndTime();

        if ((startTime) <= nCol && (nCol < endTime))
        {
            IsInGroup = true;
            nStartTime = startTime;
            break;
        }
    }

    if (IsInGroup)
    {
        CActionBlockGroupManager* pGroupManager = NULL;
        pGroupManager = new CActionBlockGroupManager();
        pGroupManager->SetData(pData, nLen);

        //////////////////
        //旧版本保存为整型数据，新版本情况下保存为浮点数，新版本读旧版本的endtime（float读int内存）数据会全是0,所以需要先设置一下他的EndTime
        // 根据z该组下所有的CActionData进行累加得到（拖拽进来的块默认起始时间为0）
        pGroupManager->SetEndTime(pGroupManager->GetEndTime());
        pGroupManager->SetID(m_pBlockDataManager->GetUnUserID());
        //WCHAR wstrDescription[MAX_DESCRIPTION_SIZE];
        //memset(wstrDescription,0,MAX_DESCRIPTION_SIZE*sizeof(WCHAR));
        //int nDescriptionLen = QStringToWCHAR(wstrDescription, FileName);
        ////wstrDescription[nDescriptionLen - 1] ='\0';
        //pGroupManager->SetDescription(wstrDescription);
        std::wstring wsDescription = FileName.toStdWString();
        pGroupManager->SetDescription((WCHAR *)wsDescription.c_str());

        pGroupManager->SetStartTime(0);
        int nGroupLen = pGroupManager->GetEndTime()-pGroupManager->GetStartTime();
        pGroupManager->SetStartTime(nStartTime);
        pGroupManager->SetEndTime(nStartTime + nGroupLen);

        m_pBlockDataManager->MoveGroup(pFindGroupManager, nGroupLen);
        m_pBlockDataManager->AddLayerBlock(pGroupManager);

        AjustTimeForInsert(pFindGroupManager, pGroupManager->GetEndTime()-pGroupManager->GetStartTime());
        ClearSelectGroupBlock();
        SetSelectGroupBlock(pGroupManager->GetStartTime()+2);

        if(m_pLayerManager)
        {
            g_IsUpdateUIByData = true;
            m_pLayerManager->SetActionDataGroupManagerToOptions(m_pBlockDataManager, pGroupManager->GetID(),0);
            m_pLayerManager->UpdateLayerWidget(m_pBlockDataManager, pGroupManager->GetID(), 0, GetType());
            g_IsUpdateUIByData = false;
        }

    }
    else
    {
        CActionBlockGroupManager* pGroupManager = NULL;
        pGroupManager = new CActionBlockGroupManager();
        if(!pGroupManager->SetData(pData, nLen))
        {
            return;
        }

        //////////////////
        //旧版本保存为整型数据，新版本情况下保存为浮点数，新版本读旧版本的endtime（float读int内存）数据会全是0,所以需要先设置一下他的EndTime
        // 根据z该组下所有的CActionData进行累加得到（拖拽进来的块默认起始时间为0）
        /////////////////
        int nEndTime = pGroupManager->GetEndTime() - pGroupManager->GetStartTime();
        pGroupManager->SetID(m_pBlockDataManager->GetUnUserID());

        //WCHAR wstrDescription[MAX_DESCRIPTION_SIZE];
        //memset(wstrDescription,0,MAX_DESCRIPTION_SIZE*sizeof(WCHAR));
        //int nDescriptionLen = QStringToWCHAR(wstrDescription, FileName);
        ////wstrDescription[nDescriptionLen - 1] ='\0';

        //pGroupManager->SetDescription(wstrDescription);
        pGroupManager->SetDescription((WCHAR *)FileName.toStdWString().c_str());
        pGroupManager->SetStartTime(nStartTime);
        pGroupManager->SetEndTime(nStartTime + nEndTime);
        m_pBlockDataManager->AddLayerBlock(pGroupManager);

        AdjustAllGroupTime(pGroupManager, pGroupManager->GetEndTime()-pGroupManager->GetStartTime());
        ClearSelectGroupBlock();
        SetSelectGroupBlock(pGroupManager->GetStartTime()+2);
        if(m_pLayerManager)
        {
            g_IsUpdateUIByData = true;
            m_pLayerManager->SetActionDataGroupManagerToOptions(m_pBlockDataManager, pGroupManager->GetID(),0);
            m_pLayerManager->UpdateLayerWidget(m_pBlockDataManager, pGroupManager->GetID(), 0, GetType());
            g_IsUpdateUIByData = false;
        }


    }
    if (m_pLayerManager)
    {
        m_pLayerManager->IUILayerManager_SetModifiedState(true);
    }
}

/**************************************************************************
* 函数名：ILayer_onMusicHasLoaded
* 功能: 音乐加载完成事件通知
* 参数:
*    @[in ] strMp3Path: 音乐路径
*    @[in ] strTitle: 音乐标题
*    @[in ] nTime: 音乐总时长
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CLayerCommonImpl::ILayer_onMusicHasLoaded(const QString &strMp3Path, const QString &strTitle, int nTime)
{
    m_pOptionHandler->MusicLoaded(strTitle, nTime);
}

/**************************************************************************
* 函数名：SetLayerCommonWidget
* 功能:设置Layer窗体对象
* 参数:
*    @[in ] pILayerCommonWidget Layer窗体对象
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
//void CLayerCommonImpl::SetLayerCommonWidget(ILayerCommonWidget *pILayerCommonWidget)
//{
//    m_pILayerCommonWidget = pILayerCommonWidget;
//}

void CLayerCommonImpl::AjustTimeForInsert(CActionBlockGroupManager* pManager, int nAdd)
{
    if (pManager == NULL)
        return;

    bool bNeedUpdateWindow = false;

    // 判断跟下一块数据有没有重叠
    int nIndex = m_pBlockDataManager->GetIndexByID(pManager->GetID());
    if (nIndex != m_pBlockDataManager->GetCount()-1 || nIndex != -1)
    {
        // 调整总的时间
        CActionBlockGroupManager* pLast = m_pBlockDataManager->GetAt(m_pBlockDataManager->GetCount()-1);
        int nCellCount = m_pLayerManager->GetNewMaxCellCount(pLast->GetEndTime()) + Constants::ADD_OFFSETS;
        m_pLayerManager->SetCellCount(nCellCount);
        bNeedUpdateWindow = true;
    }

    if (bNeedUpdateWindow)
    {
        //		if(m_pLayerManager)
        //		{
        //            m_pLayerManager->GetWidget()->Invalidate(false);
        //			m_pLayerManager->UpdateScrollBar();
        //		}
    }
}

void CLayerCommonImpl::ILayerOptionHandler_ModifyGroupName(int nCol)
{
    CLayerCommonImpl::OnLButtonDBClick(nCol);
}

void CLayerCommonImpl::ILayerOptionHandler_CopyGroup()
{
    // 	if (m_pLayerManager)
    // 	{
    // 		m_pLayerManager->IUILayerManager_SetModifiedState(true);
    // 	}
    OnCopy();
    // 	if (m_pLayerManager)
    // 	{
    // 		m_pLayerManager->IUILayerManager_SetModifiedState(true);
    // 	}
}

void CLayerCommonImpl::ILayerOptionHandler_PasteGroup(int nCol)
{
    OnPaste(nCol);
    if (m_pLayerManager)
    {
        m_pLayerManager->IUILayerManager_SetModifiedState(true);
    }
}

