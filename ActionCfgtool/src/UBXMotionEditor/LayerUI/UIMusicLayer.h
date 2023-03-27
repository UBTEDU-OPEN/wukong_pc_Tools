/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：ActionLayer.h
* 创建时间：2016/01/12
* 文件标识：
* 文件摘要：动作层类，负责动作层绘制，同步等
*
* 当前版本：1.0.0.0
* 作    者：hels
* 完成时间：2016/01/12
* 版本摘要：
*/

#ifndef UIMUSICLAYER_H
#define UIMUSICLAYER_H
//#include <QObject>
#include "motioneditor_global.h"
#include <QMap>
#include "ActionBlockDataManager.h"
#include "ILayerCommonWidget.h"
#include "wavelayer.h"
#include "UILayerImpl.h"

class MOTIONEDITOR_EXPORT CUIMusicLayer : public CUILayerImpl/*, public QObject*/
{
    Q_OBJECT

public:
    CUIMusicLayer(ILayerCommonWidget *mp3Widget);
    ~CUIMusicLayer();

public:
    //获取层类型
    virtual emActionLayerType GetType();
    virtual void DrawGroup(/*QPixmap* pixmap*/);
    virtual void DrawBottomCell(QPixmap *pixmap);
    /**************************************************************************
    * 函数名：DrawBase
    * 功能:画该层基本部分
    * 参数: 无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual void DrawBase();

    /**************************************************************************
    * 函数名：GetCellHeight
    * 功能:获取一格的高度
    * 参数: 无
    * 返回值: int 返回一格的高度
    * 时间: 2017/1/16
    * 作者: Joker
    */
    virtual int GetCellHeight();

    void loadData();;
public:
    //hels 绘制动作组，包含组名等
    void DrawBlockGroup(QPixmap* pixmap);
//    //hels 绘制选中组中Action
//    void DrawBlockAction(QPixmap* pixmap);

//    void SetActionBlockDataManager(CActionBlockDataManager* pActionBlockDataManger);
//    CActionBlockDataManager* GetActionBlockDataManager();
    //void SetSelectGroupBlockCol(int nCol);
    void SetSelectGroupBlockID(int nID);
    void ClearSelectGroupBlock();
    /************************************
    * 名称: LoadMusic
    * 功能: 加载mp3时调用，转发给解析模块监听，加载完成后会立即进行处理
    * 参数: [in]const QString& strFilePath mp3文件路径
    * 返回:   bool
    * 时间:   2017/01/12
    * 作者:   Joker
    ************************************/
    bool LoadMusic(const QString& strFilePath);

    /************************************
     * 名称: ReleaseMusic
     * 功能: 删除mp3时调用，转发给解析模块监听
     * 参数: [@in]: strFilePath 音乐全路径名
     * 返回:   void
     * 时间:   2017/01/12
     * 作者:   Joker
     ************************************/
    void ReleaseMusic(QString strFilePath);

    /**************************************************************************
    * 函数名：IsMusicLoaded
    * 功能:音乐是否已经加载
    * 参数: [@in]: strFileName 根据文件名判断是否已加载
    * 返回值: bool 已经加载返回true， 否则返回false
    * 时间: 2017/1/16
    * 作者: Joker
    */
    bool IsMusicLoaded(QString strFileName);
    //hels 根据TimeLevel计算1ms所占像素
    double GetXScalePerMilliSecond();

    /**************************************************************************
    * 函数名：DeleteMusicFromMap
    * 功能:从map 中删除音乐
    * 参数:
    *    @[in ] strFileName: 音乐文件名
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void DeleteMusicFromMap(QString strFileName);

    /**************************************************************************
    * 函数名：RemoveAllMusicFromMap
    * 功能:从Map中删除所有音乐
    * 参数:无
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void RemoveAllMusicFromMap();



signals:
        /************************************
        * 名称: SigLoadMusic
        * 功能: 添加mp3时发送此信号，由解析模块监听
        * 参数: [in]const QString& strFilePath mp3文件路径
        * 返回:   void
        * 时间:   2016/01/12
        * 作者:   hels
        ************************************/
        void SigLoadMusic(const QString& strFilePath);

        /************************************
         * 名称: SigReleaseMusic
         * 功能: 删除mp3时发送此信号，由解析模块监听
         * 参数:
         * 返回:   void
         * 时间:   2016/01/28
         * 作者:   周志平
         ************************************/
        void SigReleaseMusic();

public slots:
    void SetDrawBlockAction(bool bDraw);

    //hels 音乐卸载完成时调用，完成相关数据删除
    void OnMusicReleased(bool sucessed);

private:
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
    void DrawWaveAction(QPixmap *pixmap, CActionBlockData *pBlockData, int nDisplayStartTime, int nDisplayEndTime);
    //hels 获取选中的组中动作
    void GetSelectGroupAction(int nCol, int& nGroupIndex, int& nActionIndex, int& nTimeOffset);
    bool IsSelectGroupBlock(int nID);
    /**************************************************************************
    * 函数名：GetDrawInterval
    * 功能:根据TimeLevel计算波形取点间隔数,提供给WaveLayer使用
    * 参数:
    *    @[in ] nSampleRate: 采样率
    * 返回值: 无
    * 时间: 2017/3/16
    * 作者: Joker
    */
    int GetDrawInterval(int nSampleRate);

private:
    QMap<QString ,UIWave*> m_mapUIWave; //波形Map容器，key是音乐文件名，value是波形UI
};

#endif // UIMUSICLAYER_H
