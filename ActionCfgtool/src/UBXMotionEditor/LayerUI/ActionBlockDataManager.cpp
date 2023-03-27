#include "stable.h"
#include "ActionBlockDataManager.h"

bool CActionBlockDataManager::GetData(char** ppData, int &nLen)
{
    vector<char> DataSave;
    // DataLen（int） + BlockNUM(INT) + [BLOCKDATALEN + BALOCKDATA]
    AddVectorData(&DataSave, (char*)&nLen, sizeof(nLen));
    int nBlockCount = m_setActionBlockData.size();
    AddVectorData(&DataSave, (char*)&nBlockCount, sizeof(nBlockCount));

    SetActionBlockDataType::iterator itor = m_setActionBlockData.begin();
    //for(int i=0; i<nBlockCount; i++)
    for(; itor != m_setActionBlockData.end(); itor++)
    {
        CActionBlockGroupManager* pBlockData = *itor;
        if (pBlockData == NULL)
            continue;

        char* pBlock = NULL;
        int nBlockDataLen = 0;
        pBlockData->GetData(&pBlock, nBlockDataLen);
        // data lens
        AddVectorData(&DataSave, (char*)&nBlockDataLen, sizeof(nBlockDataLen));
        if (pBlock && nBlockDataLen)
        {
            // data
            AddVectorData(&DataSave, pBlock, nBlockDataLen);
            delete [] pBlock;
        }
    }

    nLen = DataSave.size();
    ModifyVertorData(DataSave, 0, (char*)&nLen, sizeof(nLen));
    if (nLen)
    {
        *ppData = new char[nLen];
        Q_ASSERT(*ppData);
        CopyVertorToPointer(DataSave, *ppData);
    }

    return true;
}
bool CActionBlockDataManager::SetData(char* pData, int nLen)
{
    // 数据长度（INT） + DATA
    int nTotalLen = 0;
    UBX_MEMCPY_INC(nTotalLen, pData);
    if (nTotalLen != nLen)
        return false;

    // Block num
    int nBlockNum = 0;
    UBX_MEMCPY_INC(nBlockNum, pData);

    for (int i=0; i<nBlockNum; i++)
    {
        char* pBlockData = NULL;
        int nBlockDataLen = 0;
        // block data len
        UBX_MEMCPY_INC(nBlockDataLen, pData);
        if (nBlockDataLen)
        {
            pBlockData = new char[nBlockDataLen];
            memcpy(pBlockData, pData, nBlockDataLen);
            pData += nBlockDataLen;

            CActionBlockGroupManager* pActionData =  new CActionBlockGroupManager();
            if (pActionData)
            {
                pActionData->SetData(pBlockData, nBlockDataLen);
                AddLayerBlock(pActionData);
            }
            delete [] pBlockData;
        }
    }

    SetUnUseBaseID();

    return true;
}

//******************************************************************************//
//添加块
bool CActionBlockDataManager::AddLayerBlock(CActionBlockGroupManager* pBlock)
{
    //Modified Flag
    CActionBlockGroupManager* itemFound = NULL;

    if (pBlock == NULL)
        return false;

    //if (IsExistBlockData(pBlock->GetID()))
    //    return false;

    m_setActionBlockData.insert(pBlock);

    // 排序
    //SortBlockDataManager();

    return true;
}

//bool CActionBlockDataManager::AddLayerBlockNoSort(CActionBlockGroupManager* pLayer)
//{
//    CActionBlockGroupManager* itemFound = NULL;

//    if (pLayer == NULL)
//        return false;

//    //if (IsExistBlockData(pLayer->GetID()))
//    //    return false;

//    m_setActionBlockData.insert(pLayer);

//    return true;
//}

int CActionBlockDataManager::GetIndexByID(int nID)
{
    SetActionBlockDataType::iterator itor = m_setActionBlockData.begin();
    for(int i = 0; itor != m_setActionBlockData.end(); itor++, i++)
    {
        CActionBlockGroupManager* pBlock = *itor;
        if(pBlock->GetID() == nID)
            return i;
    }

    return -1;
}

SetActionBlockDataType::const_iterator CActionBlockDataManager::FirstGroupIndex(int nStartTime)
{
    CActionBlockGroupManager temp;
    temp.SetStartTime(nStartTime);
    SetActionBlockDataType::const_iterator iter = m_setActionBlockData.lower_bound(&temp);
    if(iter != m_setActionBlockData.begin())
    {
        iter--;
    }
    return iter;
}

SetActionBlockDataType::const_iterator CActionBlockDataManager::EndIndex()
{
    return m_setActionBlockData.end();
}

//删除块
bool CActionBlockDataManager::DeleteLayerBlock(int nID)
{
    //Modified Flag
    CActionBlockGroupManager* itemFound = NULL;

    SetActionBlockDataType::iterator itor = m_setActionBlockData.begin();
    for(; itor != m_setActionBlockData.end(); itor++)
    {
        itemFound = *itor;
        if (itemFound && itemFound->GetID() == nID)
        {
            itemFound->ReleaseConnection();
            delete itemFound;
            m_setActionBlockData.erase(itor);
            return true;
        }
    }

    return false;
}


//-------------------------------------------------------------------------------------------------
CActionBlockDataManager::CActionBlockDataManager(void) : m_nUnUseBaseID (0)
{
}


CActionBlockDataManager::~CActionBlockDataManager(void)
{
}

void CActionBlockDataManager::ClearBlockDataList()
{
    CActionBlockGroupManager* itemFound = NULL;
    SetActionBlockDataType::iterator itor = m_setActionBlockData.begin();
    for(; itor != m_setActionBlockData.end(); itor++)
    {
        itemFound = *itor;
        if (itemFound )
        {
            itemFound->ReleaseConnection();
            delete itemFound;
        }
    }
    m_setActionBlockData.clear();
}

//bool CActionBlockDataManager::IsExistBlockData(int nID)
//{
//    CActionBlockGroupManager* itemFound = NULL;

//    for (int i=0; i<m_ListActionBlockData.count(); i++)
//    {
//        itemFound = m_ListActionBlockData.at(i);
//        if (itemFound && itemFound->GetID() == nID)
//        {
//            return true;
//        }
//    }

//    return false;
//}

CActionBlockGroupManager* CActionBlockDataManager::FindBlockData(int nID)
{
    CActionBlockGroupManager* itemFound = NULL;

    SetActionBlockDataType::iterator itor = m_setActionBlockData.begin();
    for(; itor != m_setActionBlockData.end(); itor++)
    {
        itemFound = *itor;
        if (itemFound && itemFound->GetID() == nID)
        {
            return itemFound;
        }
    }

    return NULL;
}

CActionBlockGroupManager* CActionBlockDataManager::GetFinalBlockData()
{
    CActionBlockGroupManager* itemFound = NULL;
    if (m_setActionBlockData.size() <= 0)
    {
        return NULL;
    }
    SetActionBlockDataType::iterator itor = m_setActionBlockData.end();
    itor--;

    itemFound = *itor;
    if (itemFound != NULL)
    {
        return itemFound;
    }

    return NULL;
}


bool CActionBlockDataManager::GetAllBlockData(vector<CActionBlockGroupManager*> &blockData)
{
    CActionBlockGroupManager* itemFound = NULL;
    SetActionBlockDataType::iterator itor = m_setActionBlockData.begin();
    for(; itor != m_setActionBlockData.end(); itor++)
    {
        itemFound = *itor;
        blockData.push_back(itemFound);
    }

    return true;
}

int CActionBlockDataManager::GetCount()
{
    return m_setActionBlockData.size();
}

CActionBlockGroupManager* CActionBlockDataManager::GetAt(int nIndex)
{
    SetActionBlockDataType::iterator itor = m_setActionBlockData.begin();
    for(int i = 0; itor != m_setActionBlockData.end(); itor++, i++)
    {
        if(i == nIndex)
            return *itor;
    }
    return NULL;
}

int CActionBlockDataManager::GetUnUserID()
{
    return ++m_nUnUseBaseID;
}

/**************************************************************************
* 函数名: SetUnUseBaseID
* 功能:设置动作帧未使用ID基数
* 时间: 2016/03/16 20:52
* 作者: 周志平
*/
void CActionBlockDataManager::SetUnUseBaseID()
{
    int nBaseID = 0;
    CActionBlockGroupManager* itemFound = NULL;
    SetActionBlockDataType::iterator itor = m_setActionBlockData.begin();
    for (; itor != m_setActionBlockData.end(); itor++)
    {
        itemFound = *itor;
        if(itemFound->GetID() > nBaseID)
        {
            nBaseID = itemFound->GetID();
        }
    }
    m_nUnUseBaseID = nBaseID;
}

void CActionBlockDataManager::ClearListNoRleaseMemory()
{    
    m_setActionBlockData.clear();
}

/**************************************************************************
* 函数名:GetUnUseDescription
* 功能:获取没有使用的描述符
* 参数:
* 返回值:QString
* 时间: 2015/12/22 17:22
* 作者: 周志平
**************************************************************************/
QString CActionBlockDataManager::GetUnUseDescription()
{
    QString strDescription;
    int i = 0;
    while (true)
    {
        bool bFound = true;
        strDescription = QString("name %1").arg(i++);
        if (m_setActionBlockData.size() == 0)
            break;

        CActionBlockGroupManager* itemFound = NULL;
        SetActionBlockDataType::iterator itor = m_setActionBlockData.begin();
        for (; itor != m_setActionBlockData.end(); itor++)
        {
            itemFound = *itor;
            if (itemFound == NULL)
                continue;

            WCHAR* pwstrName = itemFound->GetDescription();
            //QString strName = WCHARToQString(pwstrName,MAX_DESCRIPTION_SIZE*sizeof(WCHAR));
            std::wstring wsName;
            wsName.copy((wchar_t*)(pwstrName), MAX_DESCRIPTION_SIZE, 0);
            QString strName = QString::fromStdWString(wsName);
            if (strDescription.compare(strName, Qt::CaseInsensitive) == 0)
            {
                bFound = false;
                break;
            }

        }

        if (bFound)
            break;
    }

    return strDescription;
}

bool CActionBlockDataManager::Connect(void)
{
    return true;
}

void CActionBlockDataManager::ReleaseConnection()
{
    ClearBlockDataList();
}

/**************************************************************************
* 函数名: MoveGroup
* 功能: 移动分组
* 参数:
*    @[in ] pBaseGroup: 起始分组
*    @[in ] nTimeSpan: 移动的时间间隔，正值向右，负值向左
* 返回值: 成功返回true，失败返回false
* 时间: 2016/03/19 12:13
* 作者: ZDJ
*/
bool CActionBlockDataManager::MoveGroup(CActionBlockGroupManager* pBaseGroup, int nTimeSpan)
{
    SetActionBlockDataType::iterator itor = m_setActionBlockData.find(pBaseGroup);
    if(itor != m_setActionBlockData.end())
    {
        for(; itor != m_setActionBlockData.end(); itor++)
        {
            CActionBlockGroupManager* pGroup = *itor;
            int nTimeFrame = pGroup->GetEndTime() - pGroup->GetStartTime(); //动作帧时间
            pGroup->SetStartTime(pGroup->GetStartTime() + nTimeSpan);
            pGroup->SetEndTime(pGroup->GetStartTime() + nTimeFrame);
        }

        return true;
    }

    return false;
}
/**************************************************************************
* 函数名:   getFirstBlockData
* 功能:     
* 参数:   
*....@[out] CActionBlockData*
* 返回值:   
* 时间:     2017/03/18 16:55
* 作者:   Leo
*/
CActionBlockGroupManager* const CActionBlockDataManager::getFirstBlockData()
{
    CActionBlockGroupManager* p = NULL;
    if (m_setActionBlockData.size() > 0){
        p = *(m_setActionBlockData.begin());
    }
    return p;
}
