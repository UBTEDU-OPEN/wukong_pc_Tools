#ifndef CUILAYERIMPL_H
#define CUILAYERIMPL_H

#include "motioneditor_global.h"
#include <QObject>
#include "ActionBlockDataManager.h"
#include "ILayerCommonWidget.h"

class MOTIONEDITOR_EXPORT CUILayerImpl: public QObject
{
    Q_OBJECT
public:
    CUILayerImpl(ILayerCommonWidget *mp3Widget);
    virtual ~CUILayerImpl();
public:
    virtual void loadData();
    virtual void SetActionBlockDataManager(CActionBlockDataManager* pActionBlockDataManger);
    virtual CActionBlockDataManager* GetActionBlockDataManager();
    virtual void SetSelectGroupBlockID(int nID);
    virtual void ClearSelectGroupBlock();

    virtual void DrawGroup();
    virtual void SetDrawBlockAction(bool bDraw);

    /**************************************************************************
    * 函数名：DrawBase
    * 功能:画该层基本部分
    * 参数: 无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual void DrawBase();
    //hels 绘制动作组，包含组名等
    virtual void DrawBlockGroup(QPixmap* pixmap);
    //hels 绘制选中组中Action
    virtual void DrawBlockAction(QPixmap* pixmap);
    //hels 获取选中的组中动作
    virtual void GetSelectGroupAction(int nCol, int& nGroupIndex, int& nActionIndex, int& nTimeOffset);
    virtual bool IsSelectGroupBlock(int nID);
    //获取层类型
    virtual emActionLayerType GetType(){return ACTION_LAYER_UNKNOWN;};
    /**************************************************************************
    * 函数名：GetStartPosition
    * 功能:获取该层起始坐标
    * 参数: 无
    * 返回值: 返回获得的坐标，用于绘制该层UI
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual QPoint GetStartPosition();
    /**************************************************************************
    * 函数名：SetStartPosition
    * 功能:设置该层起始坐标
    * 参数:
    *    @[in ] point: 当层的位置发现变化时，通过接口将起始位置保存起来
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual void SetStartPosition(QPoint point);

    virtual void DrawBottomCell(QPixmap *pixmap);
    /**************************************************************************
    * 函数名：GetPixmap
    * 功能:获取该层像素图
    * 参数: 无
    * 返回值: QPixmap* 返回像素图，主要作为贴图供公共层直接贴，减少画图开销
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual QPixmap* GetPixmap();

    /**************************************************************************
    * 函数名：GetCellHeight
    * 功能:获取一格的高度
    * 参数: 无
    * 返回值: int 返回一格的高度
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual int GetCellHeight();
protected:
    CActionBlockDataManager* m_pActionBlockDataManager;
    ILayerCommonWidget* m_pCUILayerCommonWidget;
    QPixmap* m_pPixmap;// 该层像素图样，供画图设备最后贴图使用
    QList<int> m_listGroupSelectID;
    bool m_bDrawBlockAction;
    int m_nCurStartTime;
    int m_nCurEndTime;
    QPoint m_startPos;
    int m_nCellHeight;//cell高度

};

#endif // CUILAYERIMPL_H
