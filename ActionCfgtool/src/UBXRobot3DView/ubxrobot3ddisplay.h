/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：ubxrobot3ddisplay.h
* 创建时间：2016/01/04
* 文件标识：
* 文件摘要：用于提供3D Robot绘制类，负责与libFBX通信
*
* 当前版本：1.0.0.0
* 作    者：hels
* 完成时间：2016/01/04
* 版本摘要：
*/

#ifndef UBXROBOT3DDISPLAY_H
#define UBXROBOT3DDISPLAY_H
#include <QGLContext>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QPointer>
#include <QThread>
#include <QWindow>


class UBXRobot3DDisplay : public QObject
{
    Q_OBJECT
public:
    explicit UBXRobot3DDisplay(QGLContext *ctx);
    ~UBXRobot3DDisplay();
public:
    int getMotorCount();
    QMap<int, int> getMotorInfo();

    //获取是否可见
    bool isVisible();

    //设置是否可见
    void setVisible(bool bVisible);


public slots:
    //响应3D窗体大小改变的槽函数
    void onSize(int w, int h);
    //响应定时器槽函数，定时刷新3D显示
    void onNextFrame();
    //响应平滑动画槽函数，进行平滑设置
    void onTriggerModelFreshed(int eActionType);
    //响应控制界面中选中节点时的槽函数
    void onClickNode(int nID);
    //响应控制界面reset事件的槽函数
    void onReset();

    /************************************
    * 名称: onAngleChanged
    * 功能: 响应控制界面角度改变的槽函数
    * 参数: [in]nID 舵机ID
    * 参数: [in]value 角度值
    * 参数: [in]nTime 时间
    * 参数: [in]nType 引起角度改变的原因
    * 返回:   void
    * 时间:   2016/01/12
    * 作者:   hels
    ************************************/
    void onAngleChanged(int nID, int value, int nTime, TypeAngleChanged nType);

    void onSetModelPlayState(bool bstate);
    void onUpdateData();

    void slotSoftSimulate(MOTOR_DATA* pMotor , int nMotorCount , int nTime , int nCurveType);

    //鼠标事件槽函数
    void onMouseEvent(QMouseEvent *event);//鼠标抬起和按下
    void onMouseMotion(QMouseEvent *event);//鼠标移动
    void onMouseWheel(QWheelEvent *event);//鼠标中键事件
protected:
    void timerEvent(QTimerEvent *event);

signals:
    void SigFirstFrameFinished();//第一帧显示完成

private:
    QGLContext* m_pGLContext;//封装了OpenGL 渲染上下文，具体的用法参考Qt帮助文档
    QBasicTimer m_timer;//基础定时器，用于控制刷新的帧率
    bool m_bIsFirstFrame;
    bool m_bVisible;   //是否可见
};

#endif // UBXROBOT3DDISPLAY_H
