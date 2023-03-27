#ifndef _UILAYERCOMMONIMPL_H_
#define _UILAYERCOMMONIMPL_H_

#include "motioneditor_global.h"
#include "ILayerUI.h"
#include "ActionBlockDataManager.h"
#include "ILayerOptionHandler.h"
#include "IGroupDataPlay.h"
#include "robotdef.h"

//#include "LayerUI/ILayerCommonWidget.h"

#define ADD_OFFSET  5
typedef ulong COLORREF;

class MOTIONEDITOR_EXPORT CLayerCommonImpl: public ILayerUI, public ILayerOptionHandler_Event/*, public IGroupDataPlayEvent*///2016/04/10万向峰合并代码时注释掉
{
public:
    CLayerCommonImpl(void);
    virtual ~CLayerCommonImpl(void);
protected:
    //    uint m_nPasteType;   // 0-no paste  1-Paste after copy 2-paste after cut
    int GetNewActionBalockDataID();
    bool IsSelectGroupBlock(int nID);
    void ClearSelectGroupBlock();
    void SetSelectGroupBlock(int nCol);
//    int GetSelectGroupAction(int nCol);
    int GetSelectGroupBlock();
    // 动态调整时间
    void AdjustAllGroupTime(CActionBlockGroupManager* pManager, int nAdd);
    void MoveAllGourpTime(CActionBlockGroupManager* pManager, int nOffset);
    void OnMenuInsert(int nCol);
    void OnModifyGroupName();
    void AjustTimeForInsert(CActionBlockGroupManager* pManager, int nAdd);

    void DeleteSelectGroup();
    void OnCopy();
    void OnPaste(int nCol);
    void SelectedBlockPlay();
    void InsertBlankTime(int nCol);
    void SaveToCommandBox();
    QString GetModuleDir();

//    void DrawBase( /*CDC* pDC*/ );
//    void DrawgBlockGroup(/*CDC* pDC*/);
    //hels更新Layer选中组,参数为组ID和Action对应的时间
    void UpdateLayerAction(int nGroupID, int nStartTime, emActionLayerType nType);
public:
    // ILayerOptionHandler_Event接口实现
    virtual void ILayerOptionHandler_OnAddActionBlock(int nGroupManagerID, CActionBlockData* pActionBlockData,
                                                      QString strBlockName = QObject::tr(""), bool bIsUpdateMP3Widget = false);
    virtual void ILayerOptionHandler_OnSelectGroupManageChange(int nGroupID, int nActionIndex);
    virtual void ILayerOptionHandler_OnSelectActionManageChange(int nGroupID,int nActionIndex);
    //设置
    //virtual void ILayerOptionHandler_SetActionBlockState(bool bState){};
    virtual void ILayerOptionHandler_OnActionBlockDataChange(int nGroupID, int nActionBlockID, int nTimeInc);
    virtual void ILayerOptionHandler_OnGroupManagerChange(int nGroupID, int nTimeInc, bool bIsUpdateMp3Widget = false);
    virtual int  ILayerOptionHandler_GetScaleValue();
    // 设置时间粒度值
    virtual void ILayerOptionHandler_SetScaleValue(int nScale) ;
    virtual void ILayerOptionHandler_OnDeleteGroupBlock();
    //双击块仿真
    virtual void ILayerOptionHandler_OnDataBlockPlaying(CActionBlockData *pActionData) ;
    virtual void ILayerOptionHandler_ModifyGroupName(int nCol);
    virtual void ILayerOptionHandler_CopyGroup();
    virtual void ILayerOptionHandler_PasteGroup(int nCol);

    virtual void ILayerOptionHandler_SetSelectGroupBlock(int nCol);
    //设置当前文件的修改状态（通过opTions实现）
    virtual void ILayerOptionHandler_SetModifiedState(bool bState) ;
    virtual void ILayerOptionHandler_InsertGroupByMenu(CActionBlockData* pActionBlockData, int nCol);

    //hels 供Motion模块调用,并通知MP3模块响应操作
    virtual void ILayerOptionHanlder_UnitedSimulation();//开始联合仿真
    virtual void ILayerOptionHanlder_PauseUnitedSimulation();//暂停联合仿真
    virtual void ILayerOptionHanlder_StopUnitedSimulation();//停止联合仿真
    virtual void ILayerOptionHandler_HasMotionData(bool hasData, int nLayerType);//通知MP3是否有动作帧，以便刷新按钮状态
    virtual bool ILayerOptionHandler_IsPlaying();

//    virtual void ILayerOptionHandler_SetMusicModifiedState(bool bState);
//    virtual bool ILayerOptionHandler_GetMusicModifiedState();

    /**************************************************************************
    * 函数名：ILayerOptionHandler_LoadMusic
    * 功能:加载音乐文件
    * 参数:
    *    @[in ] strFilePath: 音乐文件路径
    * 返回值: bool 如果加载成功返回true，否则返回false
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual bool ILayerOptionHandler_LoadMusic(QString strFilePath);

    // 表情显示 Add by jianjie 2018/01/05
    virtual void ILayerOptionHandler_ShowExpression(QString strFileName, int nCurFrame);

public:
    //ILayerUI接口实现
    virtual bool Connect(QWidget* managerHandle);
    virtual bool ReleaseConnection();
    virtual void SetLayerManager(IUILayerManager* pManager);
    virtual IUILayerManager* GetLayerManager();
    virtual CActionBlockDataManager* GetBlockDataManager(){return m_pBlockDataManager;}
    virtual void ClearSelectState();
    virtual void OnLButtonDBClick(int nCol);
    virtual void OnLButtonDown(int nCol, QPoint point, int nStartTime);
    virtual void OnMouseMove(int nCol);
    virtual void OnLButtonDownAndShift(int nCol);
    virtual void OnLButtonUpAndShift();
    virtual void OnLButtonUp(QPoint point);
    virtual void OnRButtonDown(int nCol);
    virtual void OnLButtonDownDebug(int nCol);
    virtual void OnLButtonDbClickDebug(int nCol);
//    virtual void OnMouseMove(QPoint point);
    virtual void AddBlankBlockInTail();
    virtual void GetSelectGroupAction(int nCol,int &nGroupIndex , int &nActionIndex , int &nTimeOffset);
    virtual void GetSelectGroupAction(int nCol, int &nGroupIndex , int &nActionIndex , int &nTimeOffset, int nStartTime);
//    virtual void AddGroupFromMenu() {};
    virtual void SetSelectGroupByOthers(int nCol);

    virtual void DoMenuCommand(uint nCommand, int nCol);

    //绘自己
//    virtual void OnDraw(/*CDC* pDC*/);

    virtual bool GetData(char** ppData, int& nLen);
    virtual bool SetData(char* pData, int nLen);
//    virtual void AdjustData() ;
    // 取得类型，具体见宏定义
    virtual emActionLayerType GetType();
    // 设置起始X,Y的位置
//    virtual	void SetPosition(int PositionX, int PositionY);
//    virtual void GetPosition(int& x, int& y);
//    virtual void SetHScrollOffset(int offset);
    virtual void SetScalevalue(int scaleValue);
    virtual int  GetScaleValue();
//    virtual void SetCellStartIndex(uint CellIndex);
//    virtual uint GetCellStartIndex();
//    virtual int  GetScreenCellCount();
//    virtual void GetLayerWidth(int &LayerWidth);
    virtual int  GetCellCount();
//    virtual void SetCellWidth(int width);
//    virtual int  GetCellWidth();
//    virtual void SetCellHeight(int height);
//    virtual int  GetCellHeight();
//    virtual void SetLineThick(int nThick);
//    virtual int  GetLineThick();
    virtual void CreateOptionHandler();
    virtual ILayerOptionHandler* GetOptionHandler();
    virtual int  GetMaxTimeCellCount();
    virtual void PlayEvent_OnDebugStop(int nLayerType);
    virtual void SetLayerMenuState(bool menuState);
    virtual void CancelBlockPlay();
    virtual void SetTreeView(ILibTreeNavigation* pView);
    virtual void TreeToView(char *pData, int nLen, int nCol, QString FileName);

    virtual int GetCurCol() { return m_nCurCol; };
    virtual void ResetCurCol() { m_nCurCol = 1; };

    /**************************************************************************
    * 函数名：SetLayerCommonWidget
    * 功能:设置Layer窗体对象
    * 参数:
    *    @[in ] pILayerCommonWidget Layer窗体对象
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
//    virtual void SetLayerCommonWidget(ILayerCommonWidget* pILayerCommonWidget);

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
    virtual void ILayer_onMusicHasLoaded(const QString &strMp3Path, const QString &strTitle, int nTime);
private:
    void createLayoutMenuPopMenuActions();
    bool CreateActionDir(QString &strPath);

    /**************************************************************************
    * CreateLayerComonWidgetContainer
    * 功能: 创建公共层窗体
    * 参数: 无
    * 返回值: 创建成功返回ture,否则返回false
    * 时间: 2017/1/16
    * 作者: Joker
    */
//    bool CreateLayerComonWidgetContainer();

protected:
    //int m_lineThick;
//    COLORREF m_lineColor;
//    int x, y;						//起始坐标
//    int h;
//    int m_divisor;
//    int m_HScrollOffset;

//    //int m_scaleValue;
//    uint m_CellStartIndex;
//    //int m_CellCount;

//    QString m_fontFamily;	//字体名称
//    int m_fontSize;			//字体大小
//    COLORREF m_fontColor;	//字体颜色

//    // 块默认颜色
//    COLORREF m_GroupBlockDefColor;
//    // 块选中颜色
//    COLORREF m_GroupBlockSelColor;

    QList<int>	m_GroupSelectID;

    // 用于保存数据用
    CActionBlockDataManager* m_pBlockDataManager;
    ILayerOptionHandler*	m_pOptionHandler;
//    ILayerCommonWidget* m_pILayerCommonWidget;

    IUILayerManager* m_pLayerManager;
    IGroupDataPlay*	m_pGroupDataPlay;

    QWidget* m_ManagerHandle;
    bool m_IsPlaying;
    bool m_IsSinglePlaying;
//    bool m_bMouseDown;
    ILibTreeNavigation* m_treeView;
    bool m_IsDragMouse;
    int m_positionDragMouse;
    bool m_IsHaveCopyData;
//    emActionLayerType m_curLayer;
private:
    int m_nCurCol;


};
#endif  //_UILAYERCOMMONIMPL_H_
