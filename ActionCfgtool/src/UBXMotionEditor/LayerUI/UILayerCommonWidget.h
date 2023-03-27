
/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CUILayerCommonWidget.h
* 创建时间：2017/01/17 10:15
* 文件标识：
* 文件摘要：公共层UI创建
*
* 当前版本：1.0.0.0
* 作    者：Joker
* 完成时间：2017/01/17
* 版本摘要：
*/
#ifndef MP3WIDGET_H
#define MP3WIDGET_H

#include "motioneditor_global.h"
#include <QPainter>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QThread>
#include <QDrag>
#include <QMimeData>

#include "stable.h"
#include "mp3player.h"
#include "mp3projconstants.h"
//#include "UIMotionLayer.h"
//#include "UIEarLedLayer.h"
//#include "UIEyeLedLayer.h"
//#include "UIMusicLayer.h"
#include "IActionLayerManager.h"
#include "ILayerCommonWidget.h"
#include "robotdef.h"

namespace Ui
{
class LayerCommonWidget;
}

enum PASTE_TYPE
{
    NO_PASTE = 0,
    PASTE_AFTER_COPY,
    PASTE_AFTER_CUT
};

class IUILayerManager;
class CUILayerImpl;

class MOTIONEDITOR_EXPORT CUILayerCommonWidget : public QWidget, public ILayerCommonWidget
{
    Q_OBJECT
public:
    explicit CUILayerCommonWidget(QWidget *parent = 0);
    ~CUILayerCommonWidget();


public:
    void loadData(emActionLayerType nLayerType);
    //实现ILayerCommonWidget接口
    //hels 获取显示内容的宽度
    virtual int GetContentWidth();

//    /**************************************************************************
//    * 函数名：GetDrawInterval
//    * 功能:根据TimeLevel计算波形取点间隔数,提供给WaveLayer使用
//    * 参数:
//    *    @[in ] nSampleRate: 采样率
//    * 返回值: 无
//    * 时间: 2017/1/16
//    * 作者: Joker
//    */
//    virtual int GetDrawInterval(int nSampleRate);
    /**************************************************************************
    * 函数名：SetTimeScale
    * 功能:设置时间粒度
    * 参数:
    *    @[in ] nScale: 时间粒度，由外部传进来
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual void SetTimeScale(int nScale);
    /**************************************************************************
    * 函数名：GetTimeScale
    * 功能: 获取单刻度时间粒度
    * 参数: 无
    * 返回值: 返回时间粒度
    * 时间: 2016/12/09
    * 作者: Joker
    */
    virtual int GetTimeScale();
    /**************************************************************************
    * 函数名：GetLineThick
    * 功能: 获取线的厚度
    * 参数: 无
    * 返回值: 返回线的厚度
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual int GetLineThick();

    /**************************************************************************
    * 函数名：GetCurrentSelectCol
    * 功能: 根据当前位置获取当前时间(单位是格)
    * 参数: 无
    * 返回值: 返回当前时间刻度
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual int GetCurrentSelectCol();

    /**************************************************************************
    * 函数名：SetCurrentSelectCol
    * 功能: 设置当前位置的时间(单位是格)
    * 参数: 无
    * 返回值: 返回当前时间刻度
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual void SetCurrentSelectCol( int nCol);

    //IUILayerManager_Event对应的接口调用
    virtual bool Connect(IUILayerManager* pParent, IUILayerManager_Event* pEvent);
    virtual void ReleaseConnection();
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
    virtual void UpdateLayerAction(int nGroupID, int nStartTime, emActionLayerType nType);
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
    virtual void UpdateLayerUIByCol(int nCol);

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
    virtual void UpdateLayerWidget(CActionBlockDataManager* pActionBlockDataManger, int nCurrentGroupID , int nCurrentActionID, emActionLayerType nLayerType);
    //    /**************************************************************************
    //    * 函数名：SetOptionHandlerEvent
    //    * 功能:设置编辑选项事件对象指针
    //    * 参数:
    //    *    @[in ] pEvent:编辑选项事件对象指针
    //    * 返回值: 无
    //    * 时间: 2017/1/16
    //    * 作者: Joker
    //    */
    //    virtual void SetOptionHandlerEvent(ILayerOptionHandler_Event* pEvent);
    virtual bool LoadMusic(const QString& strFilePath);

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
    virtual bool MusicHasLoaded(int nTime, QString strWavePath, QString strTitle);

    /**************************************************************************
    * 函数名：MusicHasReleased
    * 功能:音乐资源释放后界面处理
    * 参数:
    *    @[in ] sucessed: 资源是否释放
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual void  MusicHasReleased();

    /**************************************************************************
    * 函数名：GetCellWidth
    * 功能:获取一格的宽度
    * 参数: 无
    * 返回值: int 返回一格的宽度
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual int GetCellWidth();

//    /**************************************************************************
//    * 函数名：GetCellHeight
//    * 功能:获取一格的高度
//    * 参数: 无
//    * 返回值: int 返回一格的高度
//    * 时间: 2017/1/16
//    * 作者: Joker
//    */
//    virtual int GetCellHeight();

//    /**************************************************************************
//    * 函数名：GetMusicCellHeight
//    * 功能:获取音乐层一格所占的高度
//    * 参数: 无
//    * 返回值: int 返回音乐层高度
//    * 时间: 2017/1/16
//    * 作者: Joker
//    */
//    virtual int GetMusicCellHeight();

    /************************************
    * 名称: GetCellStartIndex()
    * 功能: 获取widget开始的cell的索引
    * 返回:   int 返回索引值
    * 时间:   2016/01/08
    * 作者:   Joker
    ************************************/
    virtual int GetCellStartIndex();

    /************************************
    * 名称: GetCellEndIndex
    * 功能: 获取widget能显示到的cell的索引，会根据widget宽度和startindex计算
    * 返回:   int 返回索引值
    * 时间:   2016/01/08
    * 作者:   hels
    ************************************/
    virtual int GetCellEndIndex();

    //ms为单位
    virtual long GetDisplayStartTime();
    virtual long GetDisplayEndTime();

    /**************************************************************************
    * 函数名：GetWidget
    * 功能:获取窗体指针
    * 参数: 无
    * 返回值: QWidget 返回窗体的指针
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual QWidget* GetWidget();


    virtual void SetCellCount(int nCount);
    virtual int GetCellCount();

    //以下是针对UI层界面的更新
    /**************************************************************************
    * 函数名：AddUILayer
    * 功能: 根据层类型添加相应的UI层
    * 参数:
    *    @[in ] nType: 层类型
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual bool AddUILayer(emActionLayerType nType);

    /**************************************************************************
    * 函数名：DeleteAllUILayer
    * 功能:删除所有层的UI
    * 参数:
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual void DeleteAllUILayer();

    virtual CUILayerImpl *FindLayerByIndex(int nIndex);
    virtual void AdjustLayerPosition();
    virtual bool DeleteUILayerByID(int nTypeID);
    virtual CUILayerImpl *FindLayerByID(int nTypeID);
	virtual void SetPlaying(bool bState);
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
    virtual void SetActionBlockDataManager(CActionBlockDataManager* pActionBlockDataManger, emActionLayerType nLayerType);

public:
    //    bool IsMusicLoaded();
//    void SetPlaying(bool play);
    void SetSinglePlaying(bool play);
//    /**************************************************************************
//    * 函数名：initUILayer
//    * 功能:初始化各个层单独的UI
//    * 参数: 无
//    * 返回值: 无
//    * 时间: 2017/1/16
//    * 作者: Joker
//    */
//    void initUILayer();

//    /**************************************************************************
//    * 函数名：AddUILayer
//    * 功能:根据层的类型创建层UI
//    * 参数:
//    *    @[in ] nLayerID: 层类型
//    * 返回值: 创建成功返回true，否则返回false
//    * 时间: 2017/1/16
//    * 作者: Joker
//    */
//    bool AddUILayer(int nLayerID);

    /**************************************************************************
    * 函数名：FindUILayerByLayerType
    * 功能:根据层的类型获取相应的UILayer
    * 参数:
    *    @[in ] nType: 层类型
    * 返回值: CUILayerImpl* 返回相应的UILayer对象
    * 时间: 2017/1/16
    * 作者: Joker
    */
    CUILayerImpl* FindUILayerByLayerType(emActionLayerType nType);

    /**************************************************************************
    * 函数名：AddUILayer
    * 功能: 添加UILayer,同时修改UILayer所在层的位置，由于Music Layer是画波形的，需要特殊处理
    * 参数:
    *    @[in ] pUILayer: 需要添加的Layer指针
    * 返回值: bool
    * 时间: 2017/1/16
    * 作者: Joker
    */
    bool AddUILayer(CUILayerImpl *pUILayer);

signals:
    /************************************
    * 名称: SigRangeChanged
    * 功能: 窗体范围改变时发送此信号，由滚动条监听，并做相应改变
    * 参数: [in]int min滚动条最小范围
    * 参数: [in]int max滚动条最大范围
    * 参数: [in]int nStep滚动条步进大小
    * 返回:   void
    * 时间:   2016/01/12
    * 作者:   hels
    ************************************/
    void SigRangeChanged(int min, int max, int nStep);

    //    /************************************
    //    * 名称: SigLoadMusic
    //    * 功能: 添加mp3时发送此信号，由解析模块监听
    //    * 参数: [in]const QString& strFilePath mp3文件路径
    //    * 返回:   void
    //    * 时间:   2016/01/12
    //    * 作者:   hels
    //    ************************************/
    //    void SigLoadMusic(const QString& strFilePath);

    //    /************************************
    //     * 名称: SigReleaseMusic
    //     * 功能: 删除mp3时发送此信号，由解析模块监听
    //     * 参数:
    //     * 返回:   void
    //     * 时间:   2016/01/28
    //     * 作者:   周志平
    //     ************************************/
    //    void SigReleaseMusic();

    //加载成功时发送，通知主窗体设置按钮状态
    void SigNotifyLoadedMusic();
    void SigNotifyReleasedMusic();

    //hels 音乐同步时，cell索引改变时发送，通知滚动条改变位置
    void SigCellIndexChanged(int index);

    void SigLButtonDoubleClick(int nCol);

    void SigReDrawAll();
    void SigRedDrawTimeAndAction();
    //    void SigOnPlayMP3(int nStartTime, int nEndTime, int nPlayState);
    void SigFinishMusicPlaying();

    //更新时间粒度和实际时间到UI
    void SigUpdateScaleAndRealTime(int nScaleValue, int nCol);

public slots:
    //hels 滚动条位置改变时调用，重绘所有波形相关绘图（时间、波形、底部cell）
    void OnScrollBarValueChanged(int value);

    //hels 音乐播放位置更新
    void OnPositionChanged(quint64 position);

    //hels 重绘所有图形，一般发生在大小改变，或者仿真时
    //改为信号和槽方式排队触发，Motion 线程模块更新时也会调用到此函数，之前的直接调用方式会导致程序崩溃
    void ReDrawAll();
    void OnReDrawTimeAndAction();
    void OnDeleteSelectedMusic();

public:
    //hels 音乐播放时，同步时间线和波形位置等
    void UpdateTimeLineWidthMusic(quint64 position);

    //提供给仿真结束时，恢复时间选择的位置
    //void StoreCurrentX();
    //void RestoreCurrentX();

    /**************************************************************************
    * 函数名：GetLastLayer
    * 功能:获取最后添加层
    * 参数: 无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
//    CUILayerImpl *GetLastLayer();

    /**************************************************************************
    * 函数名：GetPreLastLayer
    * 功能:获取最后添加层之前的那层，主要用来处理最后添加的层是Music层的情况
    * 参数: 无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
//    CUILayerImpl *GetPreLastLayer();
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent * event);
    virtual void mouseMoveEvent(QMouseEvent* event);

    //leo add 20170228
    emActionLayerType getDropDataType(const char* pData, const int nLen);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent * event);

protected:
    //hels 绘制背景部分
    void DrawBackGround(QPixmap* pixmap);

//    /************************************
//    * 名称: DrawBottomCell
//    * 功能: 绘制底部cell，参考原来windows下的界面
//    * 参数: [in]QPixmap* pixmap绘图对象指针
//    * 返回:   void
//    * 时间:   2016/01/07
//    * 作者:   hels
//    ************************************/
//    void DrawBottomCell(QPixmap* pixmap);

//    void DrawEarLedCell(QPixmap* pixmap);
//    void DrawEyeLedCell(QPixmap* pixmap);

    /************************************
    * 名称: DrawTimeLayer
    * 功能: 绘制显示时间
    * 参数: [in]QPixmap* pixmap绘图对象指针
    * 返回:   void
    * 时间:   2016/01/08
    * 作者:   hels
    ************************************/
    void DrawTimeLayer(QPixmap* pixmap);

    //绘制时间选择线,由于鼠标点击切换时间点时，不需要重绘全部内容，为提高效率，将时间线专门绘制
    void DrawTimeSelLine(QPixmap* pixmap);
    //将除时间线外的其他pixmap绘制到m_pPixmapMem中,贴图的操作，很快,主要给鼠标点击时间线时使用，其他时候用不到
    void DrawPixmapExcludeSelLine(QPixmap* pixmap);
    //创建动作组右键动作组菜单
    void CreateMotionLayerGroupListPopMenuActions();
    //创建音乐右键动作组菜单
    void CreateMusicLayerGroupListPopMenuActions();

//    void SetActStartTime(int nActStartTime);
//    void SetActEndTime(int nActEndTime);

    /**************************************************************************
    * 函数名：GetLayerTypeByPos
    * 功能:根据当前位置获取所在层
    * 参数:
    *    @[in ] point  位置
    * 返回值: int 返回所在层
    * 时间: 2017/1/16
    * 作者: Joker
    */
    emActionLayerType GetLayerTypeByPos(QPoint point);

    /**************************************************************************
    * 函数名：GetCurCellByPos
    * 功能:根据当前位置获取所在列
    * 参数:
    *    @[in ] point  位置
    * 返回值: int 返回所在列
    * 时间: 2017/1/16
    * 作者: Joker
    */
    int GetCurCellByPos(QPoint point);
private:
    enum TimeLevel
    {
        //毫秒级别
        ddd_10,
        ddd_50,
        ddd_100,
        //秒级别
        ss_1,
        ss_5,
        ss_10,
        //分钟级别
        mm_1,
        mm_5,
        mm_10
    };


//    /************************************
//    * 名称: GetCellEndIndex
//    * 功能: 获取widget能显示到的cell的索引，会根据widget宽度和startindex计算
//    * 返回:   int 返回索引值
//    * 时间:   2016/01/08
//    * 作者:   hels
//    ************************************/
//    int GetCellEndIndex();

    /************************************
    * 名称: GetTimeString
    * 功能: 获取显示的时间字符串，根据hh:mm:ss和mm:ss.ddd的精度进行计算，参考Adobe Audtion软件
    * 参数: [in]int nCurIndex当前的cell索引
    * 返回:   QString 时间字符串
    * 时间:   2016/01/08
    * 作者:   hels
    ************************************/
    QString GetTimeString(int nCurIndex);

    //hels 获取鼠标点击时，能切换时间线的有效区域
    //    QRect GetContainRect();

    //hels 鼠标中键改变enum TimeLevel，此值会影响波形绘制的精度
//    void TimeLevelChanged(TimeLevel timeLevel);

    //    //hels 初始化MP3播放
    //    void PrepareMP3();

    //    //hels 重置MP3播放
    //    void ResetMP3();

    void emitRangeChanged();

    void ClearDir(const QString& fullPath);

    //新UI如果cellcount太小，背景纯黑不好看，因此调整cellcount保证大于等于widget width
    void AdjustCellCount();

    //画动作组
    void onReDrawAction();

private slots:

    void OnCopy();
    void OnPaste();
    void OnCut();
    void OnDeleteSelectGroup();
    void OnSelectedBlockPlay();
    void OnCancelBlockPlay();
    void OnInsertBlankTime();
    void OnSaveToCommandBox();
    void OnModifyGroup();

    void on_btnZoomOut_clicked();

    void on_btnZoomIn_clicked();

private:
    Ui::LayerCommonWidget*  ui;

//    CActionBlockDataManager* m_pActionDataManager;
    int m_nCurrentGroupID;
    int m_nCurrentActionID;

    //    UIWave m_waveLayer;//hels 波形绘制对象
    MP3Player* m_pMP3Player;//hels MP3播放类对象
    //QThread* m_pMP3Thread;//hels MP3线程对象
    QString m_strMp3Path;//hels mp3路径
    QString m_strWavPath;
//    CUIMotionLayer m_actionLayer;
//    CUIEarLedLayer m_earLedLayer;
//    CUIEyeLedLayer m_eyeLedLayer;
//    CUIMusicLayer m_musicLayer;
    QList<CUILayerImpl*> m_listUILayer; //所有层UI的列表

    QPixmap* m_pPixmapMem;//hels 用于最终显示的内存图，其他的绘制最终都会贴到这张图上
//    QPixmap* m_pPixmapGroup;//动作组
//    QPixmap* m_pEarPixmapGroup;//耳朵组
//    QPixmap* m_pEyePixmapGroup;//眼睛组
//    QPixmap* m_pPixmapWave;//波形
    QPixmap* m_pPixmapBackGround;//hels 背景部分位图，保存刻度线等

    //Time 层的宽高
    int m_nTimeCellWidth;
    int m_nTimeCellHeight;

    //Music
    //宽高
    int m_nMusicCellWidth;
    int m_nMusicCellHeight;
//    int m_nPixelPerSec;//每1个cell像素大小(默认100像素)

    //底部默认cell
    int m_nCellWidth;//cell宽度
//    int m_nCellHeight;//cell高度
    int m_nCellCount;//cell总数
    int m_nLineThick;//cell线宽

    quint64 m_nCellTotalSeconds;//记录原始Cell个数对应的时间s为单位,当进行波形缩放时，需要用到此时间复原cell的正确个数

    //波形控件显示的初始位置
    int m_nStartX;
    int m_nStartY;

    int m_nSelLineLength;//hels 时间选择线的长度
    int m_nCurCol;//Joker 之前hels以像素为单位，如今改为以格为单位，具体时间要乘以时间粒度
    //hels 当前选中的时间线位置，以像素为单位，总长度为（m_nCellCount * (m_nCellWidth + m_nLineThick)）

    //hels 一个m_cellWidth + m_nLineThick即为一个cell,与TimeLevel无关
    //hels TimeLevel影响的是波形时间精度，即会影响m_nCount的大小
    int m_nCellStartIndex; //当前显示的第一个cell索引（此值与滚动条的位置有关，用于计算开始绘制的点）

    TimeLevel m_timeLevel;//显示时间精度

    //MP3起止播放时间，ms为单位
    quint64 m_nStartMilliSecs;
    quint64 m_nEndMilliSecs;

    //
    int m_timeScale;//时间粒度，由Motion模块提供

    int m_nWheelCount;//累计中键滚动2次缩放一次波形比例
    bool m_bForwards;//记录是向前滚动

    int m_bIsPlaying;
    int m_bIsSinglePlaying;
    int m_nCurChosenAction;
    int m_nPasteType;
    IUILayerManager* m_pIUILayerManager;
    double m_nCurSelXStore;
    int m_dragMode;
    //    int m_nCurActionLayer;
    IUILayerManager_Event* m_pEvent;

    //时间曾专用变量
    int m_nDivisor; //分隔基准，如果是他的倍数则画数字，否则不需要画
    int m_nNewUILayerStartY;
};

#endif // MP3WIDGET_H
