#include "portconnectmap.h"
#include <vector>
#include "ubxdatabase.h"

CPortConnetMap::CPortConnetMap(void)
{
}


CPortConnetMap::~CPortConnetMap(void)
{
}


bool CPortConnetMap::GetRawData(char** ppData, int &nLen)
{
    std::vector<char> DataSave;

    // 数据长度 + 各项的数据

    //先保存数据长度，实际长度后面再改回来
    AddVectorData(&DataSave, (char*)&nLen, sizeof(nLen));

    //int m_nOutputItemID;			// 该层的资源ID，如果为OnStart点，则为-1
    AddVectorData(&DataSave, (char*)&m_nOutputItemID, sizeof(m_nOutputItemID));
    //int	m_nOutputIndex;				// 输出脚的序号
    AddVectorData(&DataSave, (char*)&m_nOutputIndex, sizeof(m_nOutputIndex));
    //int m_nInputItemID;				// 输入脚块的资源ID，如果是为本块的输入脚，则为-1
    AddVectorData(&DataSave, (char*)&m_nInputItemID, sizeof(m_nInputItemID));
    //int m_nInputIndex;				// 输入脚序号
    AddVectorData(&DataSave, (char*)&m_nInputIndex, sizeof(m_nInputIndex));

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

bool CPortConnetMap::SetRawData(char* pData, int nLen)
{
    // 数据长度 + 各项的数据
    int nTotalLen = 0;
    UBX_MEMCPY_INC(nTotalLen, pData);
    if (nTotalLen != nLen)
        return false;

    //int m_nOutputItemID;			// 该层的资源ID，如果为OnStart点，则为-1
    UBX_MEMCPY_INC(m_nOutputItemID, pData);
    //int	m_nOutputIndex;				// 输出脚的序号
    UBX_MEMCPY_INC(m_nOutputIndex, pData);
    //int m_nInputItemID;				// 输入脚块的资源ID，如果是为本块的输入脚，则为-1
    UBX_MEMCPY_INC(m_nInputItemID, pData);
    //int m_nInputIndex;				// 输入脚序号
    UBX_MEMCPY_INC(m_nInputIndex, pData);

    return true;
}
