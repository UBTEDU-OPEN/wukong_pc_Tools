#include "baseblock.h"
#include "ubxdatabase.h"
#include "formatfunc.h"

CBaseBlock::CBaseBlock(void)
{
    m_nStartXpos = 0;
    m_nStartYpos = 0;
    //m_nLayerID = 0;
    m_nItemID = 0;
    //m_nParentItemID = 0;
    m_nBlockType = 0;
    m_nLinkToID = 0;
    //m_pProcessHandle = NULL;

    m_nBaseBlockDataLen = 0;
    m_pBaseBlockData = NULL;
    memset(m_strDescription,0,MAX_DESCRIPTION_SIZE * sizeof(WCHAR));
    memset(m_strCaption, 0,MAX_CAPTION_SIZE * sizeof(WCHAR));
//    ZeroMemory(m_strDescription, sizeof(m_strDescription));
//	ZeroMemory(m_strCaption, sizeof(m_strCaption));

    SetDefaultInfo();
}

CBaseBlock::~CBaseBlock(void)
{
    if (m_pBaseBlockData)
    {
        delete [] m_pBaseBlockData;
        m_pBaseBlockData = NULL;
    }
}

bool CBaseBlock::GetRawData(char** ppData, int &nLen)
{
    // 总的数据长度(int)+引脚数据长度(int)+引脚数据+各项数据
    vector<char> DataSave;

    // 先保存数据总长度，后面再填实际的长度
    AddVectorData(&DataSave, (char*)&nLen, sizeof(nLen));

    // 保存引脚数据
    char* pPortInfo = NULL;
    int nPortInfoLen = 0;
    m_PortManager.GetRawData(&pPortInfo, nPortInfoLen);
    if (pPortInfo && nPortInfoLen)
    {
        // 引脚数据长度
        AddVectorData(&DataSave, (char*)&nPortInfoLen, sizeof(nPortInfoLen));
        // 引脚数据
        AddVectorData(&DataSave, pPortInfo, nPortInfoLen);

        delete [] pPortInfo;
    }

    //int m_nStartXpos;	// 坐标起始位置，用于在绘图的时候用
    AddVectorData(&DataSave, (char*)&m_nStartXpos, sizeof(m_nStartXpos));
    //int m_nStartYpos;	// 坐标起始位置，用于在绘图的时候用
    AddVectorData(&DataSave, (char*)&m_nStartYpos, sizeof(m_nStartYpos));
    //WCHAR	m_strCaption[MAX_CAPTION_SIZE];
    AddVectorData(&DataSave, (char*)m_strCaption, sizeof(m_strCaption));
    //WCHAR	m_strDescription[MAX_DESCRIPTION_SZIE];	// BLOCK的描述，用于对该BLOCK的说明
    AddVectorData(&DataSave, (char*)m_strDescription, sizeof(m_strDescription));
    //int		m_nItemID;		// 属于当前层的元素序号，如第1层，第一个元素则LayerID:1, ItemID:0
    AddVectorData(&DataSave, (char*)&m_nItemID, sizeof(m_nItemID));
    //int		m_nBlockType;		// 功能块的类型：0，ActionBlok; 1, FunctionBlock; 2, LogicBlock; 3, ToolBlock;
    AddVectorData(&DataSave, (char*)&m_nBlockType, sizeof(m_nBlockType));
    //int		m_nLinkToID;	// 当前块连接到下一块的ID
    AddVectorData(&DataSave, (char*)&m_nLinkToID, sizeof(m_nLinkToID));

    AddVectorData(&DataSave, (char*)&m_nBaseBlockDataLen, sizeof(m_nBaseBlockDataLen));
    if (m_nBaseBlockDataLen)
    {
        AddVectorData(&DataSave, m_pBaseBlockData, m_nBaseBlockDataLen);
    }

    nLen = DataSave.size();
    ModifyVertorData(DataSave, 0, (char*)&nLen, sizeof(nLen));
    *ppData = new char[nLen];
    if (*ppData)
    {
        CopyVertorToPointer(DataSave, *ppData);
    }
    DataSave.clear();

    return true;
}

bool CBaseBlock::SetRawData(char* pData, int nLen)
{
    m_PortManager.ClearPortInotList();

    // 总的数据长度(int)+引脚数据长度(int)+引脚数据+各项数据
    int nTotalLen = 0;
    UBX_MEMCPY_INC(nTotalLen, pData);
    if (nTotalLen != nLen)
        return false;

    // 引脚数据长度
    int nPortDataLen = 0;
    UBX_MEMCPY_INC(nPortDataLen, pData);
    if (nPortDataLen)
    {
        // 引脚数据
        char* pPortData = new char[nPortDataLen];
        memcpy(pPortData, pData, nPortDataLen);
        pData += nPortDataLen;

        m_PortManager.SetRawData(pPortData, nPortDataLen);

        delete [] pPortData;
    }

    //int m_nStartXpos;	// 坐标起始位置，用于在绘图的时候用
    UBX_MEMCPY_INC(m_nStartXpos, pData);
    //int m_nStartYpos;	// 坐标起始位置，用于在绘图的时候用
    UBX_MEMCPY_INC(m_nStartYpos, pData);
    //WCHAR	m_strCaption[MAX_CAPTION_SIZE];
    memcpy(m_strCaption, pData, sizeof(m_strCaption));
    pData += sizeof(m_strCaption);
    //WCHAR	m_strDescription[MAX_DESCRIPTION_SZIE];	// BLOCK的描述，用于对该BLOCK的说明
    memcpy(m_strDescription, pData, sizeof(m_strDescription));
    pData += sizeof(m_strDescription);
    //int		m_nItemID;		// 属于当前层的元素序号，如第1层，第一个元素则LayerID:1, ItemID:0
    UBX_MEMCPY_INC(m_nItemID, pData);
    //int		m_nBlockType;		// 功能块的类型：0，ActionBlok; 1, FunctionBlock; 2, LogicBlock; 3, ToolBlock;
    UBX_MEMCPY_INC(m_nBlockType, pData);
    //int		m_nLinkToID;	// 当前块连接到下一块的ID
    UBX_MEMCPY_INC(m_nLinkToID, pData);

    UBX_MEMCPY_INC(m_nBaseBlockDataLen, pData);
    //AddVectorData(&DataSave, (char*)&m_nBaseBlockDataLen, sizeof(m_nBaseBlockDataLen));
    if (m_nBaseBlockDataLen)
    {
        //AddVectorData(&DataSave, m_pBaseBlockData, m_nBaseBlockDataLen);
        if(m_pBaseBlockData)
        {
            delete [] m_pBaseBlockData;
            m_pBaseBlockData = NULL;
        }
        m_pBaseBlockData = new char[m_nBaseBlockDataLen];
        memcpy(m_pBaseBlockData, pData, m_nBaseBlockDataLen);
        pData += m_nBaseBlockDataLen;
    }

    return true;
}

void CBaseBlock::SetRootPageInfo()
{
    m_PortManager.ClearPortInotList();

    CPortInfo *pInfo = new CPortInfo();

    // OnStart点
    pInfo->SetDirection(DIR_INPUT);
    pInfo->SetPortType(PORT_TYPE_START);
    m_PortManager.AddPortInfoToList(pInfo, true);

    // OnStop点
    pInfo = new CPortInfo();
    pInfo->SetDirection(DIR_OUTPUT);
    pInfo->SetPortType(PORT_TYPE_STOP);
    m_PortManager.AddPortInfoToList(pInfo, true);
}

void CBaseBlock::SetDefaultInfo(void)
{
    m_PortManager.ClearPortInotList();

    CPortInfo *pInfo = new CPortInfo();

    // OnStart点
    pInfo->SetDirection(DIR_INPUT);
    pInfo->SetPortType(PORT_TYPE_START);

    std::string strName = "OnStart";
    int len = strName.length() + 1;
    WCHAR* pwszCaption = new WCHAR[len*2];
    memset(pwszCaption, 0, len*2);
    utils::ansi2wc(strName.c_str(), len*2, pwszCaption);
    pInfo->SetCaption(pwszCaption);
    delete[] pwszCaption;

    m_PortManager.AddPortInfoToList(pInfo, true);

    // OnCancel点
    pInfo = new CPortInfo();
    pInfo->SetDirection(DIR_INPUT);
    pInfo->SetPortType(PORT_TYPE_CANCEL);
    strName = "OnCancel";
    len = strName.length() + 1;
    pwszCaption = new WCHAR[len*2];
    memset(pwszCaption, 0, len*2);
    utils::ansi2wc(strName.c_str(), len * 2, pwszCaption);
    pInfo->SetCaption(pwszCaption);
    delete[] pwszCaption;
    m_PortManager.AddPortInfoToList(pInfo, true);

    // OnStop点
    pInfo = new CPortInfo();
    pInfo->SetDirection(DIR_OUTPUT);
    pInfo->SetPortType(PORT_TYPE_STOP);
    strName = "OnStop";
    len = strName.length() + 1;
    pwszCaption = new WCHAR[len*2];
    memset(pwszCaption, 0, len*2);
    utils::ansi2wc(strName.c_str(), len * 2, pwszCaption);
    pInfo->SetCaption(pwszCaption);
    delete[] pwszCaption;
    m_PortManager.AddPortInfoToList(pInfo, true);
}

/************************************************************************
* [功	能]	设置BLOCK的类型
* [输	入]	nType : 类型
* [输	出]	无
************************************************************************/
void CBaseBlock::SetBlockType(int nType)
{
    m_nBlockType = nType;
}

/************************************************************************
* [功	能]	获取BLOCK的类型
* [输	入]	无
* [输	出]	无
************************************************************************/
int CBaseBlock::GetBlockType()
{
    return m_nBlockType;
}

/************************************************************************
* [功	能]	设置BLOCK的标题
* [输	入]	strCaption : 标题
* [输	出]	无
************************************************************************/
void CBaseBlock::SetCaption( WCHAR* strCaption)
{
    if (strCaption == NULL)
        return;
    memset(m_strCaption,0, MAX_CAPTION_SIZE*sizeof(WCHAR));
    memcpy(m_strCaption, strCaption ,MAX_CAPTION_SIZE*sizeof(WCHAR));
}

/************************************************************************
* [功	能]	获取BLOCK的标题
* [输	入]	无
* [输	出]	无
************************************************************************/
WCHAR* CBaseBlock::GetCaption()
{
    return m_strCaption;
}

/************************************************************************
* [功	能]	设置BLOCK的描述
* [输	入]	strDescription : 描述
* [输	出]	无
************************************************************************/
void CBaseBlock::SetDescription( WCHAR* pstrDescription)
{
    if (pstrDescription == NULL)
        return;

    memcpy(m_strDescription, pstrDescription ,MAX_DESCRIPTION_SIZE*sizeof(WCHAR));
//   wcsncpy(m_strDescription, pstrDescription,MAX_DESCRIPTION_SZIE);
}

/************************************************************************
* [功	能]	获取BLOCK的描述
* [输	入]	无
* [输	出]	无
************************************************************************/
 WCHAR* CBaseBlock::GetDescription()
{
    return m_strDescription;
}

/************************************************************************
* [功	能]	设置坐标
* [输	入]	p
* [输	出]	无
************************************************************************/
void CBaseBlock::SetPosition(const Point &p)
{
    m_nStartXpos = p.x;
    m_nStartYpos = p.y;
}

/**************************************************************************
* 函数名:   SetPosition
* 功能:
* 参数:
*....@[in]  const int x
*....@[in]  const int y
*....@[out] void
* 返回值:
* 时间:     2017/10/10 16:29
* 作者:   Leo
*/
void CBaseBlock::SetPosition(const int x, const int y)
{
    m_nStartXpos = x;
    m_nStartYpos = y;
}

/************************************************************************
* [功	能]	获取坐标
* [输	入]	无
* [输	出]	无
************************************************************************/
Point CBaseBlock::GetPosition()
{
    Point p;
    p.setX(m_nStartXpos);
    p.setY(m_nStartYpos);
    return p;
}

/************************************************************************
* [功	能]	获取Portinfo manager
* [输	入]	无
* [输	出]	无
************************************************************************/
CPortInfoManager* CBaseBlock::GetPortInfoManager()
{
    return &m_PortManager;
}

/************************************************************************
* [功	能]	设置ITEMID
* [输	入]	无
* [输	出]	无
************************************************************************/
void CBaseBlock::SetItemID(int nItemID)
{
    m_nItemID = nItemID;
}

/************************************************************************
* [功	能]	获取ItemID
* [输	入]	无
* [输	出]	无
************************************************************************/
int CBaseBlock::GetItemID()
{
    return m_nItemID;
}

/************************************************************************
* [功	能]	设置LINK TO ID
* [输	入]	无
* [输	出]	无
************************************************************************/
void CBaseBlock::SetLinkToID(int nLinkId)
{
    m_nLinkToID = nLinkId;
}

/************************************************************************
* [功	能]	获取LINK TO ID
* [输	入]	无
* [输	出]	无
************************************************************************/
int CBaseBlock::GetLinkToID()
{
    return m_nLinkToID;
}

int CBaseBlock::GetBaseBlockData(char** ppData)
{
    *ppData = NULL;
    if (m_nBaseBlockDataLen)
    {
        *ppData = new char[m_nBaseBlockDataLen];
        memcpy(*ppData, m_pBaseBlockData, m_nBaseBlockDataLen);
    }

    return m_nBaseBlockDataLen;
}

void CBaseBlock::SetBaseBlockExtData(char* pData, int nLen)
{
    if (m_pBaseBlockData)
    {
        delete [] m_pBaseBlockData;
        m_pBaseBlockData = NULL;
    }

    m_nBaseBlockDataLen = nLen;
    if (m_nBaseBlockDataLen)
    {
        m_pBaseBlockData = new char[m_nBaseBlockDataLen];
        if (m_pBaseBlockData)
        {
            memcpy(m_pBaseBlockData, pData, m_nBaseBlockDataLen);
        }
    }
}
