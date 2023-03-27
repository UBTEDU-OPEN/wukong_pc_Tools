/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：UBXMotionEditor.h
* 创建时间：2015/12/09 15:27
* 文件标识：
* 文件摘要：动作编辑模块接口类定义
*
* 当前版本：1.0.0.0
* 作    者：周志平
* 完成时间：2015/12/09 15:27
* 版本摘要：
*/
#ifndef _UBXMOTIONEDITOR_H_
#define _UBXMOTIONEDITOR_H_

#include "motioneditor_global.h"
#include <QWidget>
#include <QString>
#include <QPoint>
#include "stable.h"

class IUBXRobot3DView;
class CIUBXSocketSession;
class ILibTreeNavigation;
//// 动作编辑接口类
//class IUBXMotionEditor_Event
//{
//    //是否允许通过MotorItemDlg对3D模型进行编辑（模型仿真的时候禁止编辑）
//    virtual void UBXMotionEditorEvent_Enable3DViewOperation(bool bEnable) = 0;
//};
class MOTIONEDITOR_EXPORT IUBXMotionEditor
{
public:
    virtual ~IUBXMotionEditor(){}
    virtual bool Connect(QWidget* pParent) = 0;
    virtual void Disconnect() = 0;
    virtual QWidget* GetWidget() = 0;

    // 读取数据
    virtual bool GetData(char** ppData, int &nLen) = 0;
    // 设置数据
    virtual bool SetData(char* pData, int nLen) = 0;
    // 转换旧数据
//    virtual void AdjustData() = 0;
    // 保存3D模型的接口
    virtual void UBXMotionEditor_Set3DRobotViewInterface(IUBXRobot3DView* pRobotView) = 0;
    //virtual void UBXMotionEditor_Set3DRobotViewInterface(IUBXRobot3DView* pRobotView) = 0;
    virtual void UBXMotionEditor_SetSocketSession(CIUBXSocketSession* pSession) = 0;
//    virtual void UBXMotionEditor_UpdateScrollBar() = 0;
    virtual void UBXMotionEditor_SetMusicResource(QString strFilePath) = 0;
    virtual void UBXMotionEditor_StopDebug() = 0;
    virtual void UBXMotionEditor_ReleaseMusicSource() = 0;
    virtual void UBXMotionEditor_SetTreeView(ILibTreeNavigation* pView) = 0;
//    virtual void UBXMotionEditor_TreeToView(char *pData, int nLen, QPoint* nPoint, QString FileName) = 0;
    virtual bool UBXMotionEditor_IsProjectModity() = 0;
    virtual bool UBXMotionEditor_IsRobotDebuging() = 0;
    virtual void UBXMotionEditor_SetProjectModity(bool bState) = 0;

    virtual void switchActionLayer(emActionLayerType eActionType) = 0;

//    virtual bool UBXMotionEditor_IsMusicModity() = 0;
//    virtual void UBXMotionEditor_SetMusicModity(bool bState) = 0;
};

MOTIONEDITOR_EXPORT bool CreateUBXMotionEditor(IUBXMotionEditor** ppEditor);
#endif  //_UBXMOTIONEDITOR_H_
