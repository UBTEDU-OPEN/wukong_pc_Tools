/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：ubxrobot3dcontrolwidget.h
* 创建时间：2016/01/04
* 文件标识：
* 文件摘要：3D Robot控制界面类
*
* 当前版本：1.0.0.0
* 作    者：hels
* 完成时间：2016/01/04
* 版本摘要：
*/


#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include "ubxrobot3dview_global.h"
#include <QWidget>
#include <QButtonGroup>
#include <QDomDocument>
#include <QDomNode>
#include <QDomElement>
#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include <QMap>
#include "IUBXRobot3DView.h"

namespace Ui {
class UBXRobot3DControlWidget;
}

//读角度类型
enum emReadAngleType
{
    eReadAngleAll,    //全部回读
    eReadAngleHand,  //回读手部
    eReadAngleLeg,    //回读腿部
    eReadAngleSingle  //回读单个舵机
};

class UBXROBOT3DVIEWSHARED_EXPORT UBXRobot3DControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UBXRobot3DControlWidget(QWidget *parent = 0);
    ~UBXRobot3DControlWidget();

    //更新控制界面显示数据
    void UpdateNodeDate(const char* szNodeName, float angle, float offset, TypeAngleChanged nChangeType);

    /**************************************************************************
    * 函数名: EnableCtrls
    * 功能: 设置控件启用状态
    * 参数:
    *    @[in ] bEnable: 启用标识
    * 返回值: void
    * 时间: 2016/01/13 14:40
    * 作者: ZDJ
    */
    void EnableCtrls(bool bEnable);

    /**************************************************************************
    * 函数名: ResetRobot
    * 功能: 复位机器人
    * 时间: 2016/01/25 17:49
    * 作者: ZDJ
    */
    void ResetRobot();

    /**************************************************************************
    * 函数名: ReadDefaultAngle
    * 功能: 读取舵机默认角度
    * 参数:
    *    @[out] motorlist: 舵机数据列表
    * 返回值: void
    * 时间: 2016/01/26 10:15
    * 作者: ZDJ
    */
    void ReadDefaultAngle(vector<MOTOR_DATA*>& motorlist);

    /**************************************************************************
    * 函数名: readModelXml
    * 功能: 读取3D模型的xml配置文件
    * 参数:
    *    @[in ] element: dom元素
    *    @[out] motorlist: 舵机数据列表
    * 返回值: void
    * 时间: 2016/01/26 10:15
    * 作者: ZDJ
    */
    void readModelXml(const QDomElement& element, vector<MOTOR_DATA*>& motorlist);

    void ConnectToMainWidgets(IUBXRobot3DViewEvent* pEvent);

signals:
    //操作控件时发射以下信号，用于通知外部数据有改变
    void clickNode(int nID);
    void angleChanged(int nID, int value, int nTime, TypeAngleChanged nType);
    void reset();

    void sigShowActionLayer(emActionLayerType eActionType);

private slots:
    //各个控制控件槽函数
    void onButtonToggled(int id, bool checked);
    //void onSliderAnglevalueChanged(int value);
    void on_pushButtonMinus_clicked();
    void on_pushButtonPlus_clicked();
    void on_pushButtonReset_clicked();
    void on_pushButtonLegReadback_clicked();
    void on_pushButtonHandReadBack_clicked();
    void on_pushButtonReadAll_clicked();
    //区分判断什么时候设置机器人角度，因为直接拖动3D模型也会改变，不能直接通过QSider的valueChanged来做，否则重复设置了
    void onSliderMoved(int nPosition);
    //void onSliderPressed();

    // 单个舵机回读
    void slotSingleReadBack();

    // 头部回读
    void slotHeadReadBack();

    void slotServoIdClicked(int nId);

    void slotSingleClicked();

    //void on_pushButtonOpenPort_clicked();

public slots:
    /**************************************************************************
    * 函数名: OnPortOpenChanged
    * 功能: 端口改变消息响应
    * 时间: 2016/01/28 17:39
    * 作者: ZDJ
    */
    void OnPortOpenChanged(BOOL bOpen);

    //回读计时器超时函数
    void OnTimeOutRead();

signals:
    /**************************************************************************
    * 函数名: PortOpenChanged
    * 功能: 端口改变事件
    * 时间: 2016/01/28 17:39
    * 作者: ZDJ
    */
    void PortOpenChanged(BOOL bOpen);

protected:
    //If you subclass from QWidget, you need to provide a paintEvent for your custom QWidget as below:
    void paintEvent(QPaintEvent *)
    {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
    virtual void resizeEvent(QResizeEvent * event);
    virtual void timerEvent(QTimerEvent* event);
    //virtual void customEvent(QEvent* event);

    bool eventFilter(QObject * obj, QEvent * event);

private:
    void changeRobotAngle(bool bEmitSignal = true, int nTime = 100);

private:
    static QMap<QString, int> groupNameID;
    QMap<int, QPushButton*> m_mapNodeRadios;

private:
    Ui::UBXRobot3DControlWidget *ui;
    QButtonGroup* m_btnGroup;//管理radiobutton
    QLabel* m_pLabelSliderNum;

    vector<MOTOR_DATA*> m_vecDefaultMotorDataList; //默认舵机数据
    vector<int> m_vecMinAngelList;
    vector<int> m_vecMaxAngelList;

    emReadAngleType  m_eReadAngleType;

    bool m_bIsReading;//记录是否是回读导致的slider数据改变，这样在valuechanged中就不能再次去set真实机器人的angle，
                      //否则会导致机器人舵机上电（拖动单个舵机改变角度就会上电）

    QTimer* m_pTimerRead;//回读计数器，每次回读禁用一段时间，硬件不支持连续发送回读命令
    bool m_bCanRead;//记录是否可以继续发送回读命令

    // 舵机ID双击
    QTimer* m_pTimerSingleClick;
    bool m_bDoubleClicked;

    IUBXRobot3DViewEvent*  m_pEvent;
};

#endif // CONTROLWIDGET_H
