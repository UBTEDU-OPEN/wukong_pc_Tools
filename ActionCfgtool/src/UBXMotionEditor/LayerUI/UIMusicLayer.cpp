
#include "stable.h"
#include "UIMusicLayer.h"
#include "ActionBlockGroupManager.h"
#include "ActionBlockData.h"
#include "mp3projconstants.h"
#include "Util.h"
//#include "mp3widget.h"
#include <QPixmap>
#include <QMessageBox>
#include <QDir>
#include "configs.h"
#include "fileutils.h"

CUIMusicLayer::CUIMusicLayer(ILayerCommonWidget *mp3Widget)
    :CUILayerImpl(mp3Widget)
{
    m_nCellHeight = Constants::DEFAULT_CELL_MUSIC_HEIGHT;
}

CUIMusicLayer::~CUIMusicLayer()
{
    RemoveAllMusicFromMap();
}

//获取层类型
emActionLayerType CUIMusicLayer::GetType()
{
    return ACTION_LAYER_MUSIC;
}

void CUIMusicLayer::SetSelectGroupBlockID(int nID)
{
    if(!m_listGroupSelectID.contains(nID))
    {
        m_listGroupSelectID.append(nID);
    }
}

void CUIMusicLayer::ClearSelectGroupBlock()
{
    m_listGroupSelectID.clear();
}

void CUIMusicLayer::DrawGroup()
{
    //组是画底色
    DrawBlockGroup(m_pPixmap);
    //    if(m_bDrawBlockAction)
    //    {
    //        DrawBlockAction(pixmap);
    //    }
}

void CUIMusicLayer::SetDrawBlockAction(bool bDraw)
{
    m_bDrawBlockAction = bDraw;
}

void CUIMusicLayer::DrawBlockGroup(QPixmap *pixmap)
{
    //音乐层存的是实际的时间，不需要时间粒度
    if(NULL == pixmap ||
            NULL == m_pCUILayerCommonWidget ||
            NULL == m_pActionBlockDataManager)
    {
        return;
    }
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
        //判断音乐组是否在显示区
        if( (nStartTime <= m_nCurEndTime) && (nEndTime >= m_nCurStartTime) )
        {
//            qDebug()<<"ActionMusicHasInGroup"<<endl;
            int nActionEndTime = nStartTime;
            CActionBlockData* pBlockData = NULL;
            for (int i = 0; i < pGroupManager->GetActionCount(); i++)
            {
                int nActionStartTime = nActionEndTime;
                pBlockData = pGroupManager->GetAt(i);
                nActionEndTime += pBlockData->GetRunTime() + pBlockData->GetStopTime();
                //判断该音乐帧是否在显示区
                if ( (nActionStartTime <= m_nCurEndTime) && (nActionEndTime >= m_nCurStartTime) )
                {
                    //qDebug()<<"ActionMusicHasInAction"<<endl;
                    DrawWaveAction(pixmap, pBlockData, nActionStartTime, nActionEndTime);
                }
                else if (nActionStartTime > m_nCurEndTime)
                {
                    return;
                }
            }
        }
        else if(nStartTime > m_nCurEndTime)
        {
            return;
        }
    }
}

/**************************************************************************
* 函数名：DrawWaveAction
* 功能:画每一个动作帧的波形
* 参数:
*    @[in ] pixmap: 画图对象指针
*    @[in ] pBlockData: 动作组
*    @[in ] nActionStartTime: 音乐帧开始时间，单位为(格)
*    @[in ] nActionEndTime:   音乐帧结束时间，单位为(格)
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUIMusicLayer::DrawWaveAction(QPixmap *pixmap, CActionBlockData* pBlockData, int nActionStartTime, int nActionEndTime)
{
#if 1
    if( (pixmap == NULL) || (m_pCUILayerCommonWidget == NULL) )
    {
        return;
    }
    int nScale = m_pCUILayerCommonWidget->GetTimeScale();
    int nCellWidth = m_pCUILayerCommonWidget->GetCellWidth();
    int nLineThick = m_pCUILayerCommonWidget->GetLineThick();

    WCHAR* pwstrName = pBlockData->GetDescription();
    //QString strName = QString::fromUtf16(pwstrName);
    QString strName = QString::fromStdWString(pwstrName);
    UIWave* pUIWave = m_mapUIWave.value(strName);

    if (pUIWave == NULL)
    {
        return;
    }

    WaveItemData data;

    data.nActionStartTime = nActionStartTime;
    data.nActionEndTime = nActionEndTime;
    data.nDisplayStartTime = m_nCurStartTime;
    data.nDisplayEndTime = m_nCurEndTime;
    data.nDrawInterval = GetDrawInterval(pUIWave->GetSimpleRate());
    data.dScale = nScale;
    data.nPixelPerCell = nCellWidth + nLineThick;

    //只画当前显示能看得见的部分波形图
    pUIWave->DrawWavePixmap(pixmap, data);
#endif
}

void CUIMusicLayer::GetSelectGroupAction(int nCol, int &nGroupIndex, int &nActionIndex, int &nTimeOffset)
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

bool CUIMusicLayer::IsSelectGroupBlock(int nID)
{
    return m_listGroupSelectID.contains(nID);
}

/************************************
* 名称: LoadMusic
* 功能: 加载mp3时调用，转发给解析模块监听，加载完成后会立即进行处理
* 参数: [in]const QString& strFilePath mp3文件路径
* 返回:   bool
* 时间:   2017/01/12
* 作者:   Joker
************************************/
bool CUIMusicLayer::LoadMusic(const QString &strFilePath)
{
    UIWave* pUIWave = new UIWave();
    bool bRet =  pUIWave->OnLoadMusic(strFilePath);
    if(bRet)
    {
        if (pUIWave->GetBitRate() > 128)
        {
            QMessageBox::warning(NULL, QObject::tr("AlphaMini"), QObject::tr("The bit rate of the current music is %1kbps, the robot can not play a "
                "music whose bit rate more than 128kbps, please select another music!").arg(pUIWave->GetBitRate()), QObject::tr("OK"));
            SAFE_DELETE(pUIWave);
            return false;
        }

        if (m_pCUILayerCommonWidget)
        {
            m_pCUILayerCommonWidget->MusicHasLoaded(pUIWave->GetTime(), pUIWave->GetWavPath(),
                                                    pUIWave->GetMusicTitle());
        }
        QString strFileName = FileUtils::getFileName(strFilePath);
        m_mapUIWave.insert(strFileName, pUIWave);
    }
    else
    {
        QMessageBox::warning(NULL, QObject::tr("AlphaMini"), QObject::tr("Please check the format of the music!"), QObject::tr("OK"));
    }

    return bRet;
    //    emit SigLoadMusic(strFilePath);

}

/************************************
 * 名称: ReleaseMusic
 * 功能: 删除mp3时调用，转发给解析模块监听
 * 参数: @in]: strFilePath 音乐全路径名
 * 返回:   void
 * 时间:   2017/01/12
 * 作者:   Joker
 ************************************/
void CUIMusicLayer::ReleaseMusic(QString strFilePath)
{
    emit SigReleaseMusic();
    if (m_mapUIWave.contains(strFilePath))
    {
        m_mapUIWave.remove(strFilePath);
    }

}

/**************************************************************************
* 函数名：IsMusicLoaded
* 功能:音乐是否已经加载
* 参数: [@in]: strFileName 根据文件名判断是否已加载
* 返回值: bool 已经加载返回true， 否则返回false
* 时间: 2017/1/16
* 作者: Joker
*/
bool CUIMusicLayer::IsMusicLoaded(QString strFileName)
{
    if (m_mapUIWave.contains(strFileName))
    {
        return true;
    }
    return false;
}

void CUIMusicLayer::OnMusicReleased(bool sucessed)
{
    //    if(sucessed)
    //    {
    //        ClearDir(getUserDirPath(USERDIR_MUSIC));
    //        m_nCellCount = 0;
    //        m_nCellTotalSeconds = 0;
    //        m_timeLevel = ss_1;
    //        emitRangeChanged();
    //        m_nEndMilliSecs = 0;//转为毫秒
    //        //        if(m_pEvent)
    //        //        {
    //        //            m_pEvent->ILayerOptionHandler_DeleteMusic();
    //        //            m_pEvent->ILayerOptionHandler_MusicLoaded("", "", 0);
    //        //        }
    //        updateCellCount();
    //        emit SigNotifyReleasedMusic();
    //    }
    if (sucessed)
    {
        if (m_pCUILayerCommonWidget)
        {
            m_pCUILayerCommonWidget->MusicHasReleased();
        }
    }
}

//根据TimeLevel计算1ms所占像素值
double CUIMusicLayer::GetXScalePerMilliSecond()
{
    double dScale = 0;
    if (m_pCUILayerCommonWidget)
    {

        dScale = (double)m_pCUILayerCommonWidget->GetTimeScale();

    }
    return dScale/ 1000.0;
}

/**************************************************************************
* 函数名：DeleteMusicFromMap
* 功能:从map 中删除音乐
* 参数:
*    @[in ] strFileName: 音乐文件名
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUIMusicLayer::DeleteMusicFromMap(QString strFileName)
{
    if (m_mapUIWave.contains(strFileName))
    {
        UIWave* pUIWave = m_mapUIWave.value(strFileName);
        SAFE_DELETE(pUIWave);
        m_mapUIWave.remove(strFileName);
    }
}

/**************************************************************************
* 函数名：RemoveAllMusicFromMap
* 功能:从Map中删除所有音乐
* 参数:无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUIMusicLayer::RemoveAllMusicFromMap()
{
    for (QMap<QString, UIWave*>::iterator iter =   m_mapUIWave.begin(); iter != m_mapUIWave.end(); iter++)
    {
        UIWave* pUIWave = iter.value();
        SAFE_DELETE(pUIWave);
    }
    m_mapUIWave.empty();
}

/************************************
* 名称: DrawBottomCell
* 功能: 绘制底部cell，参考原来windows下的界面
* 参数: [in]QPixmap* pixmap绘图对象指针
* 返回:   void
* 时间:   2016/01/07
* 作者:   hels
************************************/
void CUIMusicLayer::DrawBottomCell(QPixmap* pixmap)
{
    //暂时什么都不画
    return;
}

/**************************************************************************
* 函数名：DrawBase
* 功能:画该层基本部分
* 参数: 无
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CUIMusicLayer::DrawBase()
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
    //    int nCellHeigth = m_pCUILayerCommonWidget->GetMusicCellHeight();
    m_pPixmap = new QPixmap(nWidth, m_nCellHeight);
    m_pPixmap->fill(Qt::transparent);
}

int CUIMusicLayer::GetCellHeight()
{
    return m_nCellHeight;
}

/**************************************************************************
* 函数名:   loadData
* 功能:     
* 参数:   
*....@[out] void
* 返回值:   
* 时间:     2017/03/21 16:27
* 作者:   Leo
*/
void CUIMusicLayer::loadData()
{
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

        if((nStartTime <= m_nCurEndTime) && (nEndTime >= m_nCurStartTime))
        {
            CActionBlockData* pBlockData = NULL;
            for(int i = 0; i <= pGroupManager->GetActionCount(); ++i)
            {
                pBlockData = pGroupManager->GetAt(i);
                if(pBlockData != NULL)
                {
                    WCHAR* pwstrName = pBlockData->GetDescription();
                    QString strName = QString::fromStdWString(pwstrName);

                    //leo add 20170320
                    QString qsMusicPath = QDir::toNativeSeparators(CConfigs::getUserDirPath(USERDIR_MUSIC) + QDir::separator() + strName);
                    if (QFile::exists(qsMusicPath))
                    {
                        LoadMusic(qsMusicPath);
                    }
                    //if (!m_setMusicName.contains(strName) && LoadMusic(qsMusicPath))
                    //{
                    //    m_setMusicName.insert(strName);
                    //}
                }
            }
        }
        else if(nStartTime > m_nCurEndTime)
        {
            return;
        }
    }
}

/**************************************************************************
* 函数名：GetDrawInterval
* 功能:根据TimeLevel计算波形取点间隔数,提供给WaveLayer使用
* 参数:
*    @[in ] nSampleRate: 采样率
* 返回值: 无
* 时间: 2017/3/16
* 作者: Joker
*/
int CUIMusicLayer::GetDrawInterval(int nSampleRate)
{
    if (m_pCUILayerCommonWidget == NULL)
    {
        return 0;
    }

    int nCellWidth = m_pCUILayerCommonWidget->GetCellWidth();
    int nLineThick = m_pCUILayerCommonWidget->GetLineThick();
    int n_timeScale = m_pCUILayerCommonWidget->GetTimeScale();
    //计算每秒取的采样点数
    int nDrawInterval = (int)nSampleRate / (nCellWidth + nLineThick);//由于一格中只有m_nPixelPerSec个像素
    return nDrawInterval * n_timeScale / 1000;
    //因此在采样点中每隔nDrawInterval取一个样本绘制
}
