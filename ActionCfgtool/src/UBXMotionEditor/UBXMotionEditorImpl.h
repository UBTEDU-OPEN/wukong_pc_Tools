#ifndef _UBXMOTIONEDITORIMPL_H_
#define _UBXMOTIONEDITORIMPL_H_

#include "motioneditor_global.h"
#include "UBXMotionEditor.h"
#include "LayerUI/iActionLayerManager.h"


class MOTIONEDITOR_EXPORT CUBXMotionEditorImpl: public IUBXMotionEditor
{
public:
    CUBXMotionEditorImpl(void);
    ~CUBXMotionEditorImpl(void);

public:
    //IUBXMotionEditor 接口实现
    virtual bool Connect(QWidget* pParent);
    virtual void Disconnect();
    virtual QWidget* GetWidget();

    virtual bool GetData(char** ppData, int &nLen);
    virtual bool SetData(char* pData, int nLen);
//    virtual void AdjustData() ;
    virtual void UBXMotionEditor_Set3DRobotViewInterface(IUBXRobot3DView* pRobotView);
    virtual void UBXMotionEditor_SetSocketSession(CIUBXSocketSession* pSession);
//    virtual void UBXMotionEditor_UpdateScrollBar();
    virtual void UBXMotionEditor_SetMusicResource(QString strFilePath) ;
    virtual void UBXMotionEditor_SetTreeView(ILibTreeNavigation* pView);
//    virtual void UBXMotionEditor_TreeToView(char *pData, int nLen, QPoint* nPoint, QString FileName);
    virtual void UBXMotionEditor_StopDebug();
    virtual void UBXMotionEditor_ReleaseMusicSource() ;
    virtual bool UBXMotionEditor_IsProjectModity();
    virtual bool UBXMotionEditor_IsRobotDebuging() ;
    virtual void UBXMotionEditor_SetProjectModity(bool bState) ;

    virtual void switchActionLayer(emActionLayerType eActionType);

//    virtual bool UBXMotionEditor_IsMusicModity();
//    virtual void UBXMotionEditor_SetMusicModity(bool bState);
    //////////////////////////////////////////////////////////////////////////

private:
    //CActionLayerManager*		m_pActionLayerManager;
//    ILayerOptionHandler*			m_pUILayerManager;
    IActionLayerManager*        m_pUILayerManager;

};
#endif  //_UBXMOTIONEDITORIMPL_H_
