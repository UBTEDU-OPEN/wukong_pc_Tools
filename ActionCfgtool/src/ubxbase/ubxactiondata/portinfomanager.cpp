#include "portinfomanager.h"
#include <vector>
#include "ubxdatabase.h"
#include "formatfunc.h"
using std::vector;

//**************!!!!! ci mu kuai jing guo da gai ***********//
CPortInfoManager::CPortInfoManager(void)
{
    //for (int i = 0; i < m_InputList.count(); i++)
    //{
    //    CPortInfo* pInfo = m_InputList.at(i);
    //    if (pInfo == NULL)
    //        continue;
    //    m_InputList.removeAll(pInfo);
    //}
    //for (auto it = m_InputList.begin(); it != m_InputList.end(); ++it) {
    //    CPortInfo *pInfo = *it;
    //    if (pInfo == nullptr) {
    //        continue;
    //    }
    //    m_InputList.erase(it);
    //}
    m_vecInput.clear();
}

CPortInfoManager::~CPortInfoManager(void)
{
    ClearPortInotList();
}

/************************************************************************/
/* [功	能] 获取没有用过的ID                                                                     */
/************************************************************************/
int CPortInfoManager::GetUnUsePortID()
{
    int nUnUseID = 0;

    if (m_vecInput.size() == 0) {
        return nUnUseID;
    }

    while (1)
    {
        bool bExit = true;
        for (CPortInfo *pInfo : m_vecInput) {
            if (pInfo == nullptr) {
                continue;
            }
            if (pInfo->GetIndex() == nUnUseID) {
                ++nUnUseID;
                bExit = false;
                break;
            }
        }
        if (bExit)
            break;
    }
    return nUnUseID;
}

/************************************************************************
* [功	能]	添加PortInfo到列表
* [输	入]	pInfo : PortInfo
            bChangeIndex - true 重置PortInfo的Index
* [输	出]	无
************************************************************************/
bool CPortInfoManager::AddPortInfoToList(CPortInfo* pInfo, bool bChangeIndex /* = false */)
{
    if (pInfo == nullptr)
        return false;
    if (bChangeIndex) {
        int nIndex = GetUnUsePortID();
        pInfo->SetIndex(nIndex);
    }
    m_vecInput.push_back(pInfo);
    return true;
}

bool CPortInfoManager::DeletePortInfoFromList(std::string const & caption)
{
    for (auto it = m_vecInput.begin(); it != m_vecInput.end(); ++it) {
        CPortInfo *pInfo = *it;
        std::string strCaption = utils::wc2s(pInfo->GetCaption(), MAX_PORTINFO_SIZE * sizeof(WCHAR));
        if (strCaption.compare(caption) == 0){
            m_vecInput.erase(it);
            delete pInfo;
            return true;
        }
    }
    return false;
}

bool CPortInfoManager::ModifyPortInfoFromList(std::string const &  caption, std::string const &  waitModifyStr)
{
    for (CPortInfo *pInfo : m_vecInput) {
        std::string strCaption = utils::wc2s(pInfo->GetCaption(), MAX_PORTINFO_SIZE*sizeof(WCHAR));
        if (0 == strCaption.compare(waitModifyStr)) {
            std::wstring wstr = utils::s2ws(strCaption);
            pInfo->SetCaption((WCHAR *)wstr.c_str());
            return true;
        }
    }
    return false;
}

bool CPortInfoManager::DeletePortInfoByType(int nPortType)
{
    for (auto it = m_vecInput.begin(); it != m_vecInput.end(); ++it)
    {
        CPortInfo *pInfo = *it;
        if (pInfo->GetPortType() == nPortType)
        {
            m_vecInput.erase(it);
            //leo add 20170410 start;
            delete pInfo;
            return true;
        }
    }
    return false;
}

CPortInfo* CPortInfoManager::FindPortByCaption(std::string const & caption)
{
    for (CPortInfo *pInfo : m_vecInput) {
        std::string strCaption = utils::wc2s(pInfo->GetCaption(), MAX_PORTINFO_SIZE * sizeof(WCHAR));
        if (0 == strCaption.compare(caption))
        {
            return pInfo;
        }
    }
    return nullptr;
}

/************************************************************************
* [功	能]清空列表
* [输	入]	pInfo : PortInfo
* [输	出]	无
************************************************************************/
void CPortInfoManager::ClearPortInotList(void)
{
    for (CPortInfo* pInfo : m_vecInput) {
        if (pInfo != nullptr) {
            delete pInfo;
        }
    }
    m_vecInput.clear();
}

/************************************************************************
* [功	能]获取PORT的个数
* [输	入]	无
* [输	出]	无
************************************************************************/
int CPortInfoManager::GetCount()
{
    return m_vecInput.size();
}

/************************************************************************
* [功	能]获取CPortInfo
* [输	入]	无
* [输	出]	无
************************************************************************/
CPortInfo* CPortInfoManager::GetAt(int i)
{
    CPortInfo *&pInfo = m_vecInput[i];
    return pInfo;
}

/************************************************************************
* [功	能]	获取当前页的输入输出数据
* [输	入]	ppData - 保存的指针
            nLen - 保存的长度
* [输	出]	无
************************************************************************/
bool CPortInfoManager::GetRawData(char** ppData, int& nLen)
{
    vector<char> DataSave;
    // 数据总长度 + 管脚个数 + [第一个管脚的长度 + 管脚数据]
    int nDataLen = 0;
    // 保存数据长度
    int nPos = AddVectorData(&DataSave, (char*)&nDataLen, sizeof(nDataLen));
    // 管脚个数
    int nPortLen = GetCount();
    // 保存管脚个数
    nPos = AddVectorData(&DataSave, (char*)&nPortLen, sizeof(nPortLen));
    for (int i = 0; i < nPortLen; ++i)
    {
        char* pRawData = nullptr;
        int nPortDataLen = 0;
        CPortInfo* pPortInfo = GetAt(i);
        if (pPortInfo == nullptr)
            return false;
        try
        {
            pPortInfo->GetRawData(&pRawData, nPortDataLen);
            if(pRawData != nullptr) {
                // 管脚的数据长度
                AddVectorData(&DataSave, (char*)&nPortDataLen, sizeof(nPortDataLen));
                // 管脚的数据长度
                AddVectorData(&DataSave, pRawData, nPortDataLen);
                delete[] pRawData;
            }
        }
        catch(...)
        {
            return false;
        }
    }

    // 重写实际的总大小
    nLen = DataSave.size();
    ModifyVertorData(DataSave, 0, (char*)&nLen, sizeof(nLen));
    *ppData = new char[nLen];
    char* p = *ppData;
    if (p != nullptr) {
        CopyVertorToPointer(DataSave, p);
    }
    DataSave.clear();
    return true;
}

/************************************************************************
* [功	能]	设置当前页的输入输出数据
* [输	入]	ppData - 保存的指针
            nLen - 保存的长度
* [输	出]	无
************************************************************************/
bool CPortInfoManager::SetRawData(char* pData, int nLen)
{
    ClearPortInotList();
    // 数据总长度 + 管脚个数 + [第一个管脚的长度 + 管脚数据]
    int nTotalLen = 0;
    UBX_MEMCPY_INC(nTotalLen, pData);
    if (nTotalLen != nLen)
        return false;
    // 管脚个数
    int nPortNum = 0;
    UBX_MEMCPY_INC(nPortNum, pData);
    for (int i=0; i<nPortNum; i++)
    {
        // 管脚数据长度
        int PortDataLen = 0;
        UBX_MEMCPY_INC(PortDataLen, pData);
        // 管脚数据
        char* pPortData = new char[PortDataLen];
        memcpy(pPortData, pData, PortDataLen);
        pData += PortDataLen;
        CPortInfo* pPortInfo = new CPortInfo();
        pPortInfo->SetRawData(pPortData, PortDataLen);
        AddPortInfoToList(pPortInfo);
        delete [] pPortData;
    }
    return true;
}
