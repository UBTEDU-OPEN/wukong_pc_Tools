#ifndef _UIMOTIONLAYER_H_
#define _UIMOTIONLAYER_H_

#include "motioneditor_global.h"
#include "LayerCommonImpl.h"
/************************************************************************/
/* 动作层UI                                                                     */
/************************************************************************/
class MOTIONEDITOR_EXPORT CMotionLayer:public CLayerCommonImpl
{
public:
    CMotionLayer(void);
    ~CMotionLayer(void);

//	CActionBlockDataManager* m_pBlockDataManager;

public:
//    virtual bool Connect(QWidget* managerHandle);
//    virtual bool ReleaseConnection();

//    virtual bool GetData(char** ppData, int& nLen);
//    virtual bool SetData(char* pData, int nLen);
    // 取得类型，具体见宏定义
    virtual emActionLayerType GetType();
    virtual QString GetLayerName();
    virtual void CreateOptionHandler();
};

// 头部动作
class CHeadMotionLayer:public CLayerCommonImpl
{
public:
    CHeadMotionLayer(void);
    ~CHeadMotionLayer(void);

    //	CActionBlockDataManager* m_pBlockDataManager;

public:
    //    virtual bool Connect(QWidget* managerHandle);
    //    virtual bool ReleaseConnection();

    //    virtual bool GetData(char** ppData, int& nLen);
    //    virtual bool SetData(char* pData, int nLen);
    // 取得类型，具体见宏定义
    virtual emActionLayerType GetType();
    virtual QString GetLayerName();
    virtual void CreateOptionHandler();
};

// 手部动作
class CHandMotionLayer:public CLayerCommonImpl
{
public:
    CHandMotionLayer(void);
    ~CHandMotionLayer(void);

    //	CActionBlockDataManager* m_pBlockDataManager;

public:
    //    virtual bool Connect(QWidget* managerHandle);
    //    virtual bool ReleaseConnection();

    //    virtual bool GetData(char** ppData, int& nLen);
    //    virtual bool SetData(char* pData, int nLen);
    // 取得类型，具体见宏定义
    virtual emActionLayerType GetType();
    virtual QString GetLayerName();
    virtual void CreateOptionHandler();
};

// 腿部动作
class CLegMotionLayer:public CLayerCommonImpl
{
public:
    CLegMotionLayer(void);
    ~CLegMotionLayer(void);

    //	CActionBlockDataManager* m_pBlockDataManager;

public:
    //    virtual bool Connect(QWidget* managerHandle);
    //    virtual bool ReleaseConnection();

    //    virtual bool GetData(char** ppData, int& nLen);
    //    virtual bool SetData(char* pData, int nLen);
    // 取得类型，具体见宏定义
    virtual emActionLayerType GetType();
    virtual QString GetLayerName();
    virtual void CreateOptionHandler();
};
#endif  //_UIMOTIONLAYER_H_
