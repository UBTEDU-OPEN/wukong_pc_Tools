#include "PortInfo.h"
#include "ubxdatabase.h"
#include <vector>
using std::vector;

CPortInfo::CPortInfo(void)
{
    m_nIndex = 0;
    m_bDirection = false;
    m_nDataType = 0;
    m_nPortType = 0;
    m_pDataStore = NULL;
    m_nDataStoreLen = 0;
    m_bMuiltiOutput = 0;

    memset(m_strCaption,0, MAX_PORTINFO_SIZE * sizeof(WCHAR));
}


CPortInfo::~CPortInfo(void)
{
    if (m_pDataStore)
    {
        delete [] m_pDataStore;
        m_pDataStore = NULL;
    }
}

bool CPortInfo::GetRawData(char** ppData, int &nLen)
{
    // 总的长度(int) + 数据
    vector<char> DataSave;

    // 保存数据长度, 先保存占位，后面再写回实际的长度
    AddVectorData(&DataSave, (char*)&nLen, sizeof(nLen));

    // m_nIndex
    AddVectorData(&DataSave, (char*)&m_nIndex, sizeof(m_nIndex));
    // m_bDirection
    AddVectorData(&DataSave, (char*)&m_bDirection, sizeof(m_bDirection));
    // m_nDataType
    AddVectorData(&DataSave, (char*)&m_nDataType, sizeof(m_nDataType));
    // m_nPortType
    AddVectorData(&DataSave, (char*)&m_nPortType, sizeof(m_nPortType));
    // m_nDataStoreLen
    AddVectorData(&DataSave, (char*)&m_nDataStoreLen, sizeof(m_nDataStoreLen));
    if (m_nDataStoreLen)
    {
        AddVectorData(&DataSave, m_pDataStore, m_nDataStoreLen);
    }
    // m_bMuiltiOutput
    AddVectorData(&DataSave, (char*)&m_bMuiltiOutput, sizeof(m_bMuiltiOutput));
    //WCHAR	m_strCaption[MAX_PORTINFO_SIZE];	// 端口名字
    AddVectorData(&DataSave, (char*)m_strCaption, sizeof(m_strCaption));

    nLen = DataSave.size();
    ModifyVertorData(DataSave, 0, (char*)&nLen, sizeof(nLen));
    *ppData = new char[nLen];
    char* p = *ppData;
    if (p)
    {
        CopyVertorToPointer(DataSave, p);
    }
    DataSave.clear();

    return true;
}

bool CPortInfo::SetRawData(char* pData, int nLen)
{
    int nTotalLen = 0;
    //memcpy(&nTotalLen, pData, sizeof(nTotalLen));
    UBX_MEMCPY_INC(nTotalLen, pData);
    if (nTotalLen != nLen)
        return false;

    // m_nIndex
    UBX_MEMCPY_INC(m_nIndex, pData);
    // m_bDirection
    UBX_MEMCPY_INC(m_bDirection, pData);
    // m_nDataType
    UBX_MEMCPY_INC(m_nDataType, pData);
    // m_nPortType
    UBX_MEMCPY_INC(m_nPortType, pData);
    // m_nDataStoreLen
    UBX_MEMCPY_INC(m_nDataStoreLen, pData);

    if (m_nDataStoreLen)
    {
        m_pDataStore = new char[m_nDataStoreLen + 1];
        memset(m_pDataStore, 0, sizeof(m_nDataStoreLen + 1));
        //UBX_MEMCPY_INC(m_pDataStore[0], pData);
        memcpy(m_pDataStore, pData, m_nDataStoreLen);
        pData += m_nDataStoreLen;
    }
    // m_bMuiltiOutput
    UBX_MEMCPY_INC(m_bMuiltiOutput, pData);
    //WCHAR	m_strCaption[MAX_PORTINFO_SIZE];	// 端口名字
    memcpy(m_strCaption, pData, sizeof(m_strCaption));

    return true;
}

void CPortInfo::SetIndex(int nIndex)
{
    m_nIndex = nIndex;
}

int CPortInfo::GetIndex()
{
    return m_nIndex;
}

void CPortInfo::SetDirection(bool bDirection)
{
    m_bDirection = bDirection;
}

bool CPortInfo::GetDirection()
{
    return m_bDirection;
}

void CPortInfo::SetPortType(int nPortType)
{
    m_nPortType = nPortType;
}

int CPortInfo::GetPortType()
{
    return m_nPortType;
}

void CPortInfo::SetCaption(WCHAR* pstrCaption)
{
    if (pstrCaption)
    {
        //memset(m_strCaption, 0,MAX_PORTINFO_SIZE * sizeof(WCHAR));
        //memcpy(m_strCaption, pstrCaption, MAX_PORTINFO_SIZE * sizeof(WCHAR));
        wcsncpy(m_strCaption, pstrCaption, MAX_PORTINFO_SIZE);
    }
}

WCHAR* CPortInfo::GetCaption()
{
    return m_strCaption;
}

//暂时使用这个
std::string CPortInfo::GetCaptionByType( const int type ) const
{
    std::string ret = "";
    switch (type)
    {
    case PORT_TYPE_START :
        ret = ("OnStart");
          break;
    case PORT_TYPE_CANCEL :
        ret = ("OnCancel");
          break;
     case PORT_TYPE_STOP :
         ret = ("OnStop");
          break;
     case PORT_TYPE_NEXT :
         ret = ("Next");
          break;
     case PORT_TYPE_LOOP :
         ret = ("Loop");
          break;
     case PORT_TYPE_DEFAULT :
         ret = ("Default");
          break;
     case PORT_TYPE_TRUE :
         ret = ("True");
          break;
     case PORT_TYPE_FALSE :
         ret = ("False");
          break;
     default :
         ret = ("");
    }
    //
    //WCHAR wstrDescription[MAX_DESCRIPTION_SIZE] = {0};
    //QStringToWCHAR(wstrDescription, qsRet);
    return ret;
}