#pragma
//#include <QWidget>
//#include <QString>
//#include <QPoint>
#include "stable.h"
#include "motioneditor_global.h"
class IUBXRobot3DView;
class CIUBXSocketSession;
class ILibTreeNavigation;


class MOTIONEDITOR_EXPORT IActionLayerManager
{
public:
    // 初始化
    virtual bool Connect(QWidget* pWnd) = 0;
    // 释放资源
    virtual void ReleaseConnection() = 0;

    virtual QWidget* GetWidget() = 0;
    virtual bool GetData(char** ppData, int &nLen) = 0;
    virtual bool SetData(char* pData, int nLen) = 0;
//    virtual void AdjustData() =0;
    virtual void ActionLayerManager_Set3DRobotViewInterface(IUBXRobot3DView* pRobotView) = 0;
    virtual void ActionLayerManager_SetSocketSession(CIUBXSocketSession* pSession) = 0;
//    virtual void ActionLayerManager_UpdateScrollBar() = 0;
    virtual void ActionLayerManager_SetMusicResource(QString strFilePath) = 0;
    virtual void ActionLayerManager_StopDebug() = 0;
    virtual void ActionLayerManager_ReleaseMusicSource() = 0;
    virtual void ActionLayerManager_SetTreeView(ILibTreeNavigation* pView) = 0;
//    virtual void ActionLayerManager_TreeToView(char *pData, int nLen, QPoint* nPoint, QString FileName) = 0;
    virtual bool ActionLayerManager_IsFileModified() = 0;
    virtual bool ActionLayerManager_IsRobotDebuging() = 0;
    virtual void ActionLayerManager_SetFileModified(bool bState) = 0;

    virtual void ActionLayerManager_SwitchActionLayer(emActionLayerType eActionType) = 0;
//    virtual bool ActionLayerManager_IsMusicModified() = 0;
//    virtual void ActionLayerManager_SetMusicModified(bool bState) = 0;
};

bool CreateIActionLayerManager(IActionLayerManager** ppManager);
