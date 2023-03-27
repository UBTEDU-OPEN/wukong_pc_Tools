#ifndef OPTIONSEARLEDWIDGETH
#define OPTIONSEARLEDWIDGETH

#include "motioneditor_global.h"
#include <QWidget>
#include "LayerUI/iActionLayerManager.h"
#include "UBXMotionEditor.h"
#include "LayerUI/ILayerOptionHandler.h"
#include "LayerUI/ActionBlockData.h"
#include <QListWidget>
#include <QTimer>
#include <QStyleOption>
#include <QPainter>
#include <qlayout.h>

namespace Ui {
class COptionsEarLedWidget;
}

class CActionBlockGroupManager;
class MOTIONEDITOR_EXPORT COptionsEarLedWidget : public QWidget, public ILayerOptionHandler
{
    Q_OBJECT

public:
    explicit COptionsEarLedWidget(QWidget *parent = 0);
    ~COptionsEarLedWidget();

public:
    // ILayerOptionHandler接口实现
    virtual bool Connect(QWidget* pParent, ILayerOptionHandler_Event* pEvent);
    virtual void ReleaseConnection();
    virtual QWidget* GetWidget();
    virtual void SetActionBlockDataManager(CActionBlockDataManager* pManager, int nCurrentGroupID,int nCurrentActionID, int nOffset = 0);
    virtual void SetControlState(bool controlState, int nLayerType = 1);
    virtual void InsertGroupByMenu(int nCol);

    void UpdateEarLedUI();
    void ImitationEarLed(int nRunTime, char leftLed, char rightLed, char bright, int nLedLight, int nLedOff);

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

    void onPbAddclicked();
    void onPbModifyclicked();
    void onPbInsertclicked();
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
//    virtual void run();

private:
    Ui::COptionsEarLedWidget *ui;
    ILayerOptionHandler_Event* m_pEvent;
    int m_nCol;
    QListWidget * m_pGroupListWidget;
    QListWidget * m_pBlockListWidget;
    void CreateGroupListPopMenuActions();
    void onGroupListDelete();

    void onDeleteBlock();

    void onGroupModifyName();
    void onGroupListCopy();
    void onGroupListPaste();

    void AdjustDialogInfo();
    void SetRawData(char* pData, int nLen);
    int GetRawData(char** ppData);   

    char m_Left;		// 左耳灯
    char m_Right;    // 右耳灯
    char m_Bright;
    int m_LedLight;
    int m_LedPutOut;
//    int m_nCurrentSelectItem;
    int m_nCurrentGroupListSelectItem;
//    int m_nCurBlockItemID;
    int m_nCurrentGroupID;
    vector<int> m_ShowId;
    CActionBlockDataManager* m_pGroupManager;
};

#endif // OPTIONSEARLEDWIDGETH
