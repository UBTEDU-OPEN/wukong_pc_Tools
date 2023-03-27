/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：ubxrobot3dwidget.h
* 创建时间：2015/12/28
* 文件标识：
* 文件摘要：实现用于展示3D模型的Qt OpenGL窗体
*
* 当前版本：1.0.0.0
* 作    者：hels
* 完成时间：2015/12/28
* 版本摘要：
*/

#ifndef UBXROBOT3DWIDGET_H
#define UBXROBOT3DWIDGET_H

#include <QMap>
#include "IUBXRobot3DView.h"
#include "ubxrobotcontrolinterface.h"
#include "ubxrobot3ddisplay.h"


//多线程方式使用QGLWidget http://stackoverflow.com/questions/18345356/qglwidget-in-another-thread-what-is-the-documentation-referring-to
class UBXRobot3DWidget : public QGLWidget, public IUBXRobotControl

{
    Q_OBJECT

public:
    explicit UBXRobot3DWidget(QWidget *parent = 0, IUBXRobot3DViewEvent *pEvent = 0, IUBXRobotViewControlNotify* pNotify = 0);
    ~UBXRobot3DWidget();

    //IUBXRobotControl接口函数实现
    virtual bool Control_GetMotorDatas(char** pData, int& nLen, emActionLayerType eActionType);
    virtual void Control_SetMotorDatas(char* pData, int nLen, int runTime);
    virtual int Control_GetMotorAngles(char* pData, int nDataLen, int nMotorID);
    virtual void Control_SetViewMotorDatasManual(char* pData, int nLen, int runTime);
    virtual void Control_SetMotorAngle(int nID, int nAngle);
    virtual void Control_SetMotorPowerState(int nMotorID, int nState);
    virtual void Control_SetAllMotorPowerState(int nState);
    //virtual void Control_EnableControl(bool bEnable);
public:
    int GetMotorCount();
    void initializeGLContext();

    /************************************
    * 名称: UpdateNodeDate
    * 功能: 更新控制界面舵机节点数据
    * 访问权限:    public
    * 参数: [in]const char* szNodeName 舵机节点名
    * 参数: [in]float angle 舵机当前角度
    * 参数: [in]float offset 舵机偏移（根据angle和offset将3D坐标的角度转为0-180°）
    * 参数: [in]TypeAngleChanged nChangeType 引起角度改变的原因
    * 返回:   void
    * 时间:   2015/12/28
    * 作者:   hels
    ************************************/
    void UpdateNodeDate(const char* szNodeName, float angle, float offset, TypeAngleChanged nChangeType);
protected:

    //以下为QGLWidget 虚函数实现
    /*QGLWidget provides three convenient virtual functions that you can reimplement in your subclass to perform the typical OpenGL tasks:
    paintGL() - Renders the OpenGL scene. Gets called whenever the widget needs to be updated.
    resizeGL() - Sets up the OpenGL viewport, projection, etc. Gets called whenever the widget has been resized (and also when it is shown for the first time because all newly created widgets get a resize event automatically).
    initializeGL() - Sets up the OpenGL rendering context, defines display lists, etc. Gets called once before the first time resizeGL() or paintGL() is called.
    */
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    //QWidget窗体处理事件
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QHideEvent *);

signals:
    //以下为窗体鼠标事件，分开发送为了对应FbxPlayer中的void MouseCallback(int button, int state, int x, int y);和void MotionCallback(int x, int y);
    void mouseEvent(QMouseEvent *event);//鼠标抬起和按下
    void mouseMotion(QMouseEvent *event);//鼠标移动
    void mouseWheel(QWheelEvent *event);//鼠标中键事件

    void SigUpdateNodeDate(const char* szNodeName, float angle, float offset, TypeAngleChanged nChangeType);
    //
    void clickNode(int nID);
    void angleChanged(int nID, int value, int nTime, TypeAngleChanged nType);
    void reset();
    void resized(int w, int h);
    void nextFrame();
    void setPlayState(bool bstate);
    void RotationSmooth(int nID, float fRot, int nTime);
    void updateData();//请求fbx更新UI角度信息，用于平滑动画后，请求更新

    void SigFirstFrameFinished();

    void sigSoftSimulate(MOTOR_DATA* pMotor , int nMotorCount , int nTime , int nCurveType);

protected slots:
    void OnFirstFrameFinished();

private:
    IUBXRobot3DViewEvent* m_pIUBXRobot3DViewEvent;
    IUBXRobotViewControlNotify* m_pNotify;
    UBXRobot3DDisplay* m_pDisplay;
    QThread m_thread;
    QMap<int, int> m_mapPowerState;

    bool m_bEnableShow;
};

#endif // UBXROBOT3DWIDGET_H
