#ifndef _BASEBOLCK_H_
#define _BASEBOLCK_H_

#include "globaldefs.h"
#include "processhandle.h"
#include "rawdata.h"
#include "portinfomanager.h"
#include "type.h"

//#ifndef __MACH__
//typedef wchar_t WCHAR;
//#else
//typedef unsigned short WCHAR;
//#endif

#define MAX_DESCRIPTION_SIZE    50
#define MAX_CAPTION_SIZE		50

//#define BLOCK_ACTION		0
//#define BLOCK_FLOWCHART		1
//#define BLOCK_LOGIC			2
//#define BLOCK_TOOL			3
//#define BLOCK_VOICE			4
//enum {
//    BLOCK_ACTION,
//    BLOCK_FLOWCHART,
//    BLOCK_LOGIC,
//    BLOCK_TOOL,
//    BLOCK_VOICE,
//};

//// 语音流程图类型
//enum {
//    emVoiceInput = 1,   //语音输入
//    emVoiceDiscern,     //语音识别
//};
//
//enum {
//    emLogicIf = 1,      //逻辑if
//    emLogicFor,         //逻辑For
//    emLogicSwitch,      //逻辑Switch
//};

typedef struct _Point {
    int x;
    int y;
    void setX(const int &val) { x = val; };
    void setY(const int &val) { y = val; };
}Point;

class UBXACTIONDATA_EXPORTS CBaseBlock : public CRawData
{
public:
    CBaseBlock(void);
    ~CBaseBlock(void);

protected:
    int m_nStartXpos;                       // 坐标起始位置，用于在绘图的时候用
    int m_nStartYpos;                       // 坐标起始位置，用于在绘图的时候用
    WCHAR	m_strCaption[MAX_CAPTION_SIZE];
    WCHAR	m_strDescription[MAX_DESCRIPTION_SIZE];	// BLOCK的描述，用于对该BLOCK的说明
    //int		m_nLayerID;		// 当前层的ID，从1开始算起，0为ROOT层
    int		m_nItemID;		// 属于当前层的元素序号，如第1层，第一个元素则LayerID:1, ItemID:0
    //int		m_nParentItemID;	// 父层的元素序号，如果顶层，则ParentItemID为-1
    int		m_nBlockType;		// 功能块的类型：0，ActionBlok; 1, FlowBlock; 2, LogicBlock; 3, ToolBlock;
    int		m_nLinkToID;	// 当前块连接到下一块的ID

    int		m_nBaseBlockDataLen;		// 当前块的实际内容
    char*	m_pBaseBlockData;

    CPortInfoManager	m_PortManager;
    CProcessHandle*		m_pProcessHandle;

public:
    virtual bool GetRawData(char** ppData, int &nLen);
    virtual bool SetRawData(char* pData, int nLen);

    int GetBaseBlockData(char** ppData);
    void SetBaseBlockExtData(char* pData, int nLen);

    // 创建BaseBlock时，默认的配置
    void SetDefaultInfo(void);
    // 创建ROOT层时，默认的配置
    void SetRootPageInfo(void);

    // 设置ITEMID
    void SetItemID(int nItemID);
    // 获取ItemID
    int GetItemID();
    // 设置LINK TO ID
    void SetLinkToID(int nLinkId);
    // 获取LINK TO ID
    int GetLinkToID();
    // 设置BLOCK类型
    void SetBlockType(int nType);
    // 获取BLOCK的类型
    int GetBlockType();
    // 设置标题
    void SetCaption( WCHAR* strCaption);
    // 获取标题
     WCHAR* GetCaption();

    // 设置描述
    void SetDescription(WCHAR* pstrDescription);
    WCHAR* GetDescription();
    // 设置坐标
    void SetPosition(const Point &p);
    void SetPosition(const int x, const int y);
    // 获取坐标
    Point GetPosition();
    // 获取Portinfo manager
    CPortInfoManager* GetPortInfoManager();
};

#endif  //_BASEBOLCK_H_
