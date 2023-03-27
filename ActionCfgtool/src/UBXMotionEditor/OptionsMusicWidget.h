#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "motioneditor_global.h"
#include <QWidget>
#include <QStyleOption>
#include <QListWidget>
#include "LayerUI/ILayerManager.h"
#include "LayerUI/IUILayerManager.h"
#include "UIMainLayerLayout.h"
#include "LayerUI/ILayerOptionHandler.h"

#define MAX_PATH_SIZE 500

namespace Ui {
class COptionsMusicWidget;
}

class MOTIONEDITOR_EXPORT COptionsMusicWidget : public QWidget, public ILayerOptionHandler
{
    Q_OBJECT

public:
    explicit COptionsMusicWidget(QWidget *parent = 0);
    ~COptionsMusicWidget();
    /**************************************************************************
    * 函数名：initWidget
    * 功能:初始化窗体
    * 参数: 无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void initWidget();

private:
    /**************************************************************************
    * 函数名：CopyMusicFileTo
    * 功能: 拷贝加载的音乐文件到特定目录
    * 参数:
    *    @[in ] strSourcePath: 待拷贝音乐的源路径
    *    @[in ] strMusicName: 音乐文件名称
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: wanxf
    */
    void CopyMusicFileTo(QString strSourcePath, QString strMusicName);

    void deleteMusicFile();

public:
    //ILayerOptionHandler接口实现
    virtual bool Connect(QWidget* pParent, ILayerOptionHandler_Event* pEvent);
    virtual void ReleaseConnection();
    virtual QWidget *GetWidget();
    /**************************************************************************
    * 函数名：MusicLoaded
    * 功能:更新音乐名称和时长
    * 参数:
    *    @[in ] strTitle: 音乐标题
    *    @[in ] nTime: 音乐总时间，单位为s
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
     virtual void MusicLoaded(const QString& strTitle, int nTime);

    /**************************************************************************
    * 函数名：SetActionBlockDataManager
    * 功能:设置动作数据，当动作组发生变化时，通过该接口更新数据
    * 参数:
    *    @[in ] pManager: 需要更新的动作组
    *    @[in ] nCurrentGroupID: 动作组ID
    *    @[in ] pManager: 动作帧ID
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual void SetActionBlockDataManager(CActionBlockDataManager* pManager, int nCurrentGroupID,int nCurrentActionID, int nOffset = 0);

    virtual bool IsMusicLoaded();//判断音乐是否加载，供Motion模块调用

    virtual void SetControlState(bool controlState, int nLayerType = 1);//ACTION_LAYER_MOTION		1
    virtual void InsertGroupByMenu(int nCol);
    virtual void AddGroupByMenu() {}

    //signals:
    //    //定义成信号,停止函数调用在其他线程中，因此通过信号和槽来完成
    //    void SigMotionPlayStop();
    //    void SigSetControlState(bool controlState, int nLayerType);
    //private slots:
    //    void OnMotionPlayStop();
    //    void OnSetControlState(bool controlState, int nLayerType);
    //    void OnReleaseMusic();
    void OnLoadedMusic();

    //    void OnFinishMusicPlaying();

    //    void OnLButtonDoubleClick(int nCol);
    //    void on_btnAddMusic_clicked();

    //    void on_playMP3_clicked();

    //    void on_stopMP3_clicked();

    //    void on_playMotion_clicked();

    //    void on_stopMotion_clicked();

    //    void on_btnDeleteMusic_clicked();

    //    void on_playEarLed_clicked();

    //    void on_playEyeLed_clicked();

    //    void on_stopEarLed_clicked();

    //    void on_stopEyeLed_clicked();

    int GetRawData(char **ppData);
    void SetRawData(char *pData, int nLen);
    int GetCurrenGroupManagerID();
    /**************************************************************************
    * 函数名：UpdateBlockList
    * 功能:更新动作列表
    * 参数:
    *    @[in ] pGroup: 动作组数据指针
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void UpdateBlockList(CActionBlockGroupManager *pGroup);
    void SetCurSelectBlockItem(int nBlockItem);
    

    /**************************************************************************
    * 函数名：UpdateUIByData
    * 功能:根据Data更新动作组列表
    * 参数: 无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void UpdateUIByData();

    /**************************************************************************
    * 函数名：SetCurrentSelectItem
    * 功能:设置当前动作组动作帧列表
    * 参数:
    *    @[in ] item: 动作组item
    *    @[in ] nIndex: 动作组item索引
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void SetCurrentSelectItem(QListWidgetItem *item, int nIndex);

    /**************************************************************************
    * 函数名：UpdataActionByGroupManager
    * 功能:动作组发生变化时，需要更新动作帧数据
    * 参数:
    *    @[in ] pGroup: 需要更新的动作组
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void UpdataActionByGroupManager(CActionBlockGroupManager *pGroup);

    // 显示选中的ID
    void ShowSelectRow(int row);

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
private slots:
    /**************************************************************************
    * 函数名：onChoosePathClick
    * 功能:选择音乐路径按钮
    * 参数: 无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void onPbChoosePathClicked();

    /**************************************************************************
    * 函数名：onPbAddClicked
    * 功能:添加音乐动作
    * 参数: 无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void onPbAddClicked();

    /**************************************************************************
    * 函数名：onPbInsertClicked
    * 功能: 插入音乐动作帧
    * 参数: 无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void onPbInsertClicked();

    /**************************************************************************
    * 函数名：slotDeleteGroup
    * 功能: 删除音乐组
    * 参数:无
    * 返回值: 无
    * 时间: 2017/11/08
    * 作者: jianjie
    */
    void slotDeleteGroup();

    /**************************************************************************
    * 函数名：DeleteLayerBlock
    * 功能: 删除动作块
    * 参数:无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void slotDeleteLayerBlock();

    /**************************************************************************
    * 函数名：onPbModifyClicked
    * 功能: 插入音乐动作帧
    * 参数: 无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void onPbModifyClicked();

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
    Ui::COptionsMusicWidget *ui;
    //    CUILayerCommonContainer*  m_pMp3Container;
    ILayerOptionHandler_Event *m_pEvent;
    //    CUILayerCommonWidget* m_pCUILayerCommonWidget;
    bool m_bMotionPlaying;
    bool m_bEyeLedPlaying;
    bool m_bEarLedPlaying;
    bool m_bMusicPlaying;
    bool m_bMusicPause;
    bool m_bHasMotionData;
    QString m_strEditFilePath;
    QString m_strMusicTitle;

    int m_nCurrentGroupListSelectItem;
    int m_nCurrentGroupID;
    CActionBlockDataManager* m_pGroupManager;
    QListWidget * m_pGroupListWidget;
    QListWidget * m_pBlockListWidget;
};

#endif // MAINWIDGET_H
