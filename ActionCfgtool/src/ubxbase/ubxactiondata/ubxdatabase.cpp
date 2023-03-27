#include "ubxdatabase.h"

/**************************************************************************
* 函数名:   AddVectorData
* 功能:
* 参数:
*....@[in]  vector<char> * pVerData
*....@[in]  char * pData
*....@[in]  int nLen
*....@[out] UBXACTIONDATA_EXPORTS int
* 返回值:
* 时间:     2017/09/21 19:37
* 作者:   Leo
*/
int AddVectorData(std::vector<char>* pVerData, char* pData, int nLen)
{
    if (!(pVerData && pData))
        return 0;
    for (int i = 0; i < nLen; i++)
    {
        pVerData->push_back(pData[i]);
    }
    return pVerData->size();
}
/**************************************************************************
* 函数名:   ModifyVertorData
* 功能:
* 参数:
*....@[in]  vector<char> & pVerData
*....@[in]  int nPos
*....@[in]  char * pData
*....@[in]  int nLen
*....@[out] UBXACTIONDATA_EXPORTS void
* 返回值:
* 时间:     2017/09/21 19:37
* 作者:   Leo
*/
void ModifyVertorData(std::vector<char>& pVerData, int nPos, char* pData, int nLen)
{
    if (!pData)
        return;
    for (int i = nPos, k = 0; k < nLen; i++, k++)
    {
        pVerData[i] = pData[k];
    }
}
/**************************************************************************
* 函数名:   CopyVertorToPointer
* 功能:
* 参数:
*....@[in]  vector<char> & pVerData
*....@[in]  char * pData
*....@[out] UBXACTIONDATA_EXPORTS int
* 返回值:
* 时间:     2017/09/21 19:38
* 作者:   Leo
*/
int CopyVertorToPointer(std::vector<char>& pVerData, char* pData)
{
    if (pData == NULL)
        return 0;
    for (int i = 0; i < pVerData.size(); i++)
    {
        pData[i] = pVerData[i];
    }
    return pVerData.size();
}