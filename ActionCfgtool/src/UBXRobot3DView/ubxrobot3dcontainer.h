/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：ubxrobot3dcontainer.h
* 创建时间：2016/01/04
* 文件标识：
* 文件摘要：3D View和3D Control的容器类，用于包含3D界面
*
* 当前版本：1.0.0.0
* 作    者：hels
* 完成时间：2016/01/04
* 版本摘要：
*/


#ifndef UBXROBOT3DCONTROL_H
#define UBXROBOT3DCONTROL_H

#include <QWidget>
#include <QThread>
#include <QStyleOption>

#include "ubxrobot3dwidget.h"
#include "IUBXRobot3DView.h"
#include "ubxrobot3ddisplay.h"
#include "ubxrobot3dcontrolwidget.h"
#include "ubxrobotcontrolinterface.h"

namespace Ui {
class UBXRobot3DControl;
}

class UBXROBOT3DVIEWSHARED_EXPORT UBXRobot3DContainer : public QWidget, public IUBXRobot3DView, public IUBXRobotViewControlNotify
{
    Q_OBJECT

public:
    explicit UBXRobot3DContainer(QWidget *parent = 0, IUBXRobot3DViewEvent *pUBXRobotEvent = 0);
    ~UBXRobot3DContainer();

public:
    void UpdateNodeDate(const char* szNodeName, float angle, float offset, TypeAngleChanged nChangeType);
public:
    //IUBXRobotViewControlNotify接口实现
    virtual void Set3DViewRotationManual(int nDir, float fRot, const char* pNode, int nTime = 200);
    virtual void SetRotation(float x, float y, float z, float w, char* pNode);
    virtual void OnSetRotation(int nID, int nAngle, int iTime);
    virtual bool IsComConnectted();
    virtual bool IsRobotDebuging();
    virtual void LostPowerForRobot(int nID);
    virtual void OnReadRotation(int nID);
    virtual void ResetAllAngle();

    void setDefaultStyle();

    UBXRobot3DControlWidget *GetCtrlWidget();
	UBXRobot3DWidget* get3DWidget();
public slots:
    void OnSetRobotAngle(int nID, int nAngle);

public:
    //IUBXRobot3DView接口实现
    virtual void Connect(QWidget* parent, IUBXRobot3DViewEvent* pEvent);
    virtual void Disconnect();
    //virtual bool SetSerialPortControl(CUBXSerialPortSesstion *pConnect);
    //获取各舵机角度
    virtual bool GetMotorDatas(char** pData, int& nLen, emActionLayerType eActionType);
    //设置各舵机角度
    virtual void SetMotorDatas(char* pData, int nLen, int runTime);
    //设置各舵机角度
    virtual void SetMotorDatasManual(char* pData, int nLen, int runTime);
    virtual void SetMotorAngle(int nID, int nAngle);
    //设置3D模型的仿真状态
    virtual void Set3DModelPlayState(bool bState);
    virtual void SetMotorPowerState(int nMotorID, int nState);
    //设置所有舵机角度
    virtual void SetAllMotorAngle(char* pData, int nLen, int nRunTime, int nAllTime);
    //获取串口连接状态
    virtual bool GetComConnectState();
    //启用/禁用控制界面
    virtual void EnableViewCtrl(bool bEnable);
    //获取舵机数目
    virtual int GetMotorCount();
    //获取窗体
    virtual QWidget *GetWidget();

    virtual void SetMotorDatasSmoothly(char* pData, int nLen, int iTime);

signals:
    void SigEnable(bool bEnable);
    void angleChanged(int nID, int value, int nTime, TypeAngleChanged nType);
protected:
    void showEvent(QShowEvent * event);
    //If you subclass from QWidget, you need to provide a paintEvent for your custom QWidget as below:
    void paintEvent(QPaintEvent *);

protected slots:
    void OnFirstFrameFinished();

private:
    Ui::UBXRobot3DControl *ui;
    UBXRobot3DWidget*           m_pUBXRobot3DWidget; //3DViewWidget用于显示3D Robot
    UBXRobot3DControlWidget*    m_pControlWidget; //3DControl提供控制界面
    IUBXRobot3DViewEvent*       m_pEvent;
    //CUBXSerialPortSesstion* m_pConnectPort; //串口接口指针
};

#endif // UBXROBOT3DCONTROL_H
