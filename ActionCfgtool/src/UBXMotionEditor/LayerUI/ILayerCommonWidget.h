#ifndef ILAYERCOMMONWIDGET_H
#define ILAYERCOMMONWIDGET_H

#include "motioneditor_global.h"
class QWidget;
class IUILayerManager_Event;
class IUILayerManager;
class CActionBlockDataManager;
class CUILayerImpl;

class MOTIONEDITOR_EXPORT ILayerCommonWidget
{
public:
    virtual bool Connect(IUILayerManager* pParent, IUILayerManager_Event* pEvent) = 0;
    virtual void ReleaseConnection() = 0;
    //获取窗体
    virtual QWidget* GetWidget() = 0;
     //hels 获取显示内容的宽度
    virtual int GetContentWidth() = 0;
    //ms为单位
    virtual long GetDisplayStartTime() = 0;
    virtual long GetDisplayEndTime() = 0;

//    //hels 根据TimeLevel计算波形取点间隔数,提供给WaveLayer使用
//    virtual int GetDrawInterval(int nSampleRate) = 0;

    //Joker 设置一格所占时间
    virtual void SetTimeScale(int nScale) = 0;
    //获取单刻度时间粒度
    virtual int GetTimeScale() = 0;
    //获取线的厚度
    virtual int GetLineThick() = 0;
    //获取当前时间刻度
    virtual int GetCurrentSelectCol() = 0;
    //功能: 设置当前位置的时间(单位是格)
    virtual void SetCurrentSelectCol( int nCol) = 0;

    //根据动作组更新LayerUI
    virtual void UpdateLayerAction(int nGroupID, int nStartTime, emActionLayerType nType) = 0;
    //根据当前时间位置更新LayerUI
   virtual void UpdateLayerUIByCol(int nCol) = 0;
    //通知LayerUI更新界面
    virtual void UpdateLayerWidget(CActionBlockDataManager* pGroupManager, int nCurrentGroupID , int nCurrentActionID, emActionLayerType nLayerType) = 0;
    //加载Music
    virtual bool LoadMusic(const QString& strFilePath) = 0;
    //音乐加载完成后界面处理
    virtual bool MusicHasLoaded(int nTime, QString strWavePath, QString strTitle) = 0;
    //音乐资源释放后界面处理
    virtual void  MusicHasReleased() = 0;
    //    //设置编辑选项事件对象指针
//    virtual void SetOptionHandlerEvent(ILayerOptionHandler_Event* pEvent) = 0;

    //获取一格所占的宽度
    virtual int GetCellWidth() = 0;
//    //获取一格所占的高度
//    virtual int GetCellHeight() = 0;

//    //获取音乐层一格所占的高度
//    virtual int GetMusicCellHeight() = 0;

    virtual void SetCellCount(int nCount) = 0;
    virtual int GetCellCount() = 0;

    //功能: 获取widget开始的cell的索引
    virtual int GetCellStartIndex() = 0;
    //功能: 获取widget能显示到的cell的索引，会根据widget宽度和startindex计算
    virtual int GetCellEndIndex() = 0;
    //根据层类型添加相应的UI层
    virtual bool AddUILayer(emActionLayerType nType) = 0;
    //删除所有层的UI
    virtual void DeleteAllUILayer() = 0;

    virtual CUILayerImpl *FindLayerByIndex(int nIndex) = 0;
    virtual void AdjustLayerPosition() = 0;
    virtual bool DeleteUILayerByID(int nTypeID) = 0;
    virtual CUILayerImpl *FindLayerByID(int nTypeID) = 0;
    virtual void SetPlaying(bool bState) = 0;
	 //功能:设置UI层组的数据，这里只添加数据，不做画图
    virtual void SetActionBlockDataManager(CActionBlockDataManager* pActionBlockDataManger, emActionLayerType nLayerType) = 0;
    virtual void loadData(emActionLayerType nLayerType) = 0;
};

#endif // ILAYERCOMMONWIDGET_H

