#ifndef OPTIONEYELEDWIDGET_H
#define OPTIONEYELEDWIDGET_H

#include "motioneditor_global.h"
#include <QWidget>
#include "LayerUI/iActionLayerManager.h"
#include "UBXMotionEditor.h"
#include "LayerUI/ILayerOptionHandler.h"
#include "LayerUI/ActionBlockData.h"
#include <QListWidget>
#include <QTimer>
#include <QStyleOption>
#include <QButtonGroup>
#include <QPainter>
#include <qlayout.h>

namespace Ui {
class COptionEyeLedWidget;
}

enum emLedLightColor
{
    eLedLightColor_Red = 1,
    eLedLightColor_Green,
    eLedLightColor_Blue,
    eLedLightColor_Yellow,
    eLedLightColor_Pink,
    eLedLightColor_Indigo,
    eLedLightColor_White,
};

class CActionBlockGroupManager;
class MOTIONEDITOR_EXPORT COptionEyeLedWidget : public QWidget, public ILayerOptionHandler
{
    Q_OBJECT

public:
    explicit COptionEyeLedWidget(QWidget *parent = 0);
    ~COptionEyeLedWidget();

public:
    // ILayerOptionHandler接口实现
    virtual bool Connect(QWidget* pParent, ILayerOptionHandler_Event* pEvent);
    virtual void ReleaseConnection();
    virtual QWidget* GetWidget();
    virtual void SetActionBlockDataManager(CActionBlockDataManager* pManager, int nCurrentGroupID,int nCurrentActionID, int nOffset = 0);
    virtual void SetControlState(bool controlState, int nLayerType = 1);
    virtual void InsertGroupByMenu(int nCol);

    //hels 动作播放停止或者结束
//    virtual void MotionPlayStop();

    /**************************************************************************
    * 函数名：UpdateEyeLedUI
    * 功能:根据数据更新眼睛灯的UI
    * 参数: 无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void UpdateEyeLedUI();
    /**************************************************************************
    * 函数名：ImitationEyeLed
    * 功能:仿真眼睛灯
    * 参数:
    *    @[in ] leftLed: 左眼
    *    @[in ] rightLed: 右眼
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void ImitationEyeLed(int nRunTime, char leftLed, char rightLed ,char bright, char color, int nLedLight, int nLedPutOut);

    void UpdateUI();

private:
    int GetCurrenGroupManagerID();
    void SetShowID(int *pID, int nCount);
    // 显示选中的ID
    void ShowSelectRow(int row);

    void UpdateUIByData();
    void SetCurrentSelectGroupItem(QListWidgetItem * item, int nIndex);
    void UpdataBlockListByGroupManager(CActionBlockGroupManager* pGroup);
    void initWidget();
    /**************************************************************************
    * 函数名：UpdateBlockOptionUI
    * 功能:更新描述具体的Block信息的UI
    * 参数:
    *    @[in ] pBlockData: Block 信息
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void UpdateBlockOptionUI(CActionBlockData* pBlockData);
    //    void SetCurSelectBlockItem(QListWidgetItem * item, int nIndex);

signals:
    void SigMotionPlayStop();
    void SigSetControlState(bool controlState, int nLayerType);

protected slots:
    void onLeftLedsBtnToggled(int id, bool bChecked);
    void onRightLedsBtnToggled(int id, bool bChecked);


private slots:
    //用于刷新界面的颜色
    void onButtonToggled(int id, bool checked);

    void onPbAddclicked();
    void onPbInsertclicked();
    void onPbModifyclicked();
    void on_m_ListActionGroup_itemPressed(QListWidgetItem *item);


    void on_m_ListActionBlock_itemPressed(QListWidgetItem *item);

protected:
    //If you subclass from QWidget, you need to provide a paintEvent for your custom QWidget as below:
    void paintEvent(QPaintEvent *)
    {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

private:
    Ui::COptionEyeLedWidget *ui;
    ILayerOptionHandler_Event* m_pEvent;
    int m_nCol;
    QListWidget * m_pGroupListWidget;
    QListWidget * m_pBlockListWidget;
    void CreateGroupListPopMenuActions();
    void onGroupListDelete();

    void onDeleteBlock();         //删除眼睛灯块操作

    void onGroupModifyName();
    void onGroupListCopy();
    void onGroupListPaste();
    void AdjustDialogInfo();
    void SetRawData(char* pData, int nLen);
    int GetRawData(char** ppData);

    char m_color;
    char m_Left;	 // 左眼睛灯
    char m_Right;    // 右眼睛灯
    char m_Bright;
    int m_LedLigth;
    int m_LedPutOut;
    QButtonGroup m_btnGroup;
    //    int m_nCurrentSelectItem;
    int m_nCurrentGroupListSelectItem;
    int m_nCurrentGroupID;
    vector<int> m_ShowId;
    CActionBlockDataManager* m_pGroupManager;
};

#endif // OPTIONEYELEDWIDGET_H
