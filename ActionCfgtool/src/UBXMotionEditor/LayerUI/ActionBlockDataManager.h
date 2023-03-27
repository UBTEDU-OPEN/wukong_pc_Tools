#ifndef _ACTIONBLOCKDATAMANAGER_H_
#define _ACTIONBLOCKDATAMANAGER_H_

#include "motioneditor_global.h"
#include "ActionBlockGroupManager.h"
#include "stable.h"
#include <set>
using std::set;

class CompareClass
{
public:
    //分组按开始时刻升序排列
    bool operator () (CActionBlockGroupManager* pBlock1, CActionBlockGroupManager* pBlock2) const
    {
        int nTimeSpan = pBlock1->GetStartTime() - pBlock2->GetStartTime();
        if(nTimeSpan == 0)
            return pBlock1->GetID() < pBlock2->GetID();
        else
            return nTimeSpan < 0 ? true : false;
    }
};
typedef set<CActionBlockGroupManager*, CompareClass> SetActionBlockDataType;


class MOTIONEDITOR_EXPORT CActionBlockDataManager
{
public:
    CActionBlockDataManager(void);
     ~CActionBlockDataManager(void);

public:
    virtual bool Connect(void);
    virtual void ReleaseConnection();
    virtual bool GetData(char** ppData, int &nLen);
    virtual bool SetData(char* pData, int nLen);

    // 删除指定 块 信息
    virtual bool DeleteLayerBlock(int nID);

    // 扩展指定块
    //virtual bool ExtrudeLayerBlock(int nID, int startTime, int endTime);
    // 增加块
    virtual bool AddLayerBlock(CActionBlockGroupManager* pLayer);
    // 修改块
    //virtual bool ModifyLayerBlockDescription(int nID, QString DescriptionInfo);

    virtual void ClearBlockDataList();

    //virtual bool OnSaveData(int nID);

    // 是否存在
    //bool IsExistBlockData(int nID);
    CActionBlockGroupManager* FindBlockData(int nID);
    CActionBlockGroupManager* GetFinalBlockData();

    bool GetAllBlockData(vector<CActionBlockGroupManager*> &blockData);

    int GetCount();
    CActionBlockGroupManager* GetAt(int nIndex);

    // 获取没有用过的ID
    int GetUnUserID();

    /**************************************************************************
    * 函数名: SetUnUseBaseID
    * 功能:设置动作帧未使用ID基数
    * 时间: 2016/03/16 20:52
    * 作者: 周志平
    */
    void SetUnUseBaseID();

    // 获取没有用过的名
    QString GetUnUseDescription();

    void ClearListNoRleaseMemory();

    //bool AddLayerBlockNoSort(CActionBlockGroupManager* pLayer);
    int GetIndexByID(int nID);

    /**************************************************************************
    * 函数名: FirstGroupIndex
    * 功能:查找第一个包含给定nStartTime的索引
    * 时间: 2016/03/18
    * 作者: hels
    */
    SetActionBlockDataType::const_iterator FirstGroupIndex(int nStartTime);
    SetActionBlockDataType::const_iterator EndIndex();

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
    bool MoveGroup(CActionBlockGroupManager* pBaseGroup, int nTimeSpan);

    //leo add 20170318
    CActionBlockGroupManager* const getFirstBlockData();
private:
    // 保存所有创建的层的容器
    //QList<CActionBlockGroupManager*> m_ListActionBlockData;



    SetActionBlockDataType   m_setActionBlockData;  //分组集合

    //leo add 暂时增加一个音乐文件标志
    //QMap< ,CActionBlockGroupManager>

    int m_nUnUseBaseID;     //分组未使用ID基数；
};
#endif  //_ACTIONBLOCKDATAMANAGER_H_
