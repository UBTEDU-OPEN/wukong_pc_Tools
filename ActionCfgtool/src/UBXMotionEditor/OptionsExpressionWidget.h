
#ifndef _EXPRESSIONWIDGET_H_
#define _EXPRESSIONWIDGET_H_


#pragma once
#include <QWidget>
#include <QListWidget>

#include "LayerUI/ILayerOptionHandler.h"


namespace Ui
{
    class COptionsExpressionWidget;
}

class COptionsExpressionWidget : public QWidget , public ILayerOptionHandler
{
    Q_OBJECT

public:
    COptionsExpressionWidget(QWidget* parent = 0);
    ~COptionsExpressionWidget();

    // ILayerOptionHandler 接口实现
    virtual bool Connect(QWidget* pParent, ILayerOptionHandler_Event* pEvent);
    virtual void ReleaseConnection();
    virtual QWidget* GetWidget();
    virtual void SetActionBlockDataManager(CActionBlockDataManager* pManager, int nCurrentGroupID, int nCurrentActionID, int nOffset = 0);
    virtual void SetControlState(bool controlState, int nLayerType = 1) {}
    virtual void InsertGroupByMenu(int nCol) {}
    virtual void AddGroupByMenu() {}

private slots:

    // 选择表情文件
    void slotChoosePath();

    // 添加表情
    void slotAddExpression();

    // 插入表情
    void slotInsertExpression();

    // 修改表情
    void slotModifyExpression();

    // 表情组点击
    void slotGroupListItemPressed(QListWidgetItem* pItem);

    // 表情帧点击
    void slotBlockListItemPressed(QListWidgetItem* pItem);

    void slotDeleteGroupItem();

    void slotDeleteBlockItem();

private:
    Ui::COptionsExpressionWidget* ui;

    ILayerOptionHandler_Event* m_pEvent;

    QListWidget*    m_pGroupListWidget;
    QListWidget*    m_pBlockListWidget;

    CActionBlockDataManager* m_pGroupManager;

    QString     m_strFilePath;

    int         m_nCurrentGroupID;
    int         m_nGroupSelectItem;

    int         m_nScaleTime;

    void    initWidget();

    bool    loadExpressionFile(QString strFilePath);
    void    CopyExpressionFile(QString strSrcPath);
    void    showExpression(CActionBlockData* pBlockData, int nTime);

    void    parseJsonFile(QString strFilePath, int& nFps, int& nTotalFrames);

    int     GetRawData(char** ppData);
    void    SetRawData(char* pData , int nLen);

    int     getCurrentGroupManagerID();

    void    updateUIByGroupManager(CActionBlockDataManager* pGroupManager);
    void    updateBlockListByGroup(CActionBlockGroupManager* pGroup);
    void    updateBlockOptionUI(CActionBlockData* pBlockData);

    void    setGroupSelectItem(QListWidgetItem* pItem);

};

#endif  