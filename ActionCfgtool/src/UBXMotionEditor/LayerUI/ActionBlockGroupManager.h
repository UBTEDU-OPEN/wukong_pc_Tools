#ifndef _ACTIONBLOCKGROUPMANAGER_H_
#define _ACTIONBLOCKGROUPMANAGER_H_

#include "motioneditor_global.h"
#include <vector>
#include "ActionBlockData.h"

using std::vector;

//这里所有的时间都是时间格，具体的时间要乘以时间粒度
class MOTIONEDITOR_EXPORT CActionBlockGroupManager
{
public:
    CActionBlockGroupManager(void);
    virtual ~CActionBlockGroupManager(void);

protected:
    int m_nID;			// 模块ID
    int m_nStartTime;	// 起始时间
    int m_nEndTime;		// 终止时间
    WCHAR  m_strDescription[MAX_DESCRIPTION_SIZE];	// 描述

    char* m_pBlockData;
//    int m_nBlockDataLen;
    int m_nUnUseBaseID;     //动作帧未使用ID基数；

public:
    virtual bool Connect(void);

    /**************************************************************************
    * 函数名: ReleaseConnection
    * 功能: 清理块
    * 时间: 2015/08/10 15:08
    * 作者: ZDJ
    */
    virtual void ReleaseConnection();

    /**************************************************************************
    * 函数名: GetData
    * 功能: 获取动作组数据
    * 参数:
    *    @[out] ppData: 动作组数据
    *    @[out] nLen: 动作组数据长度
    * 返回值: 成功返回true,失败返回false
    * 时间: 2015/08/10 14:01
    * 作者: ZDJ
    */
    virtual bool GetData(char** ppData, int &nLen);

    /**************************************************************************
    * 函数名: SetData
    * 功能: 设置动作组数据
    * 参数:
    *    @[in ] pData: 动作组数据
    *    @[in ] nLen: 动作组数据长度
    * 返回值: 成功返回true,失败返回false
    * 时间: 2015/08/10 14:01
    * 作者: ZDJ
    */
    virtual bool SetData(char* pData, int nLen);

    /**************************************************************************
    * 函数名: DeleteLayerBlock
    * 功能: 删除指定块
    * 参数:
    *    @[in ] nID: 块ID
    * 返回值: 成功返回true，失败返回false
    * 时间: 2015/08/10 14:11
    * 作者: ZDJ
    */
    virtual bool DeleteLayerBlock(int nID);

    /**************************************************************************
    * 函数名: AddLayerBlock
    * 功能: 增加块
    * 参数:
    *    @[in ] pBlockData: 块数据
    * 返回值: 成功返回true，失败返回false
    * 时间: 2015/08/10 14:14
    * 作者: ZDJ
    */
    virtual bool AddLayerBlock(CActionBlockData* pBlockData);

    /**************************************************************************
    * 函数名: ClearBlockDataList
    * 功能: 清除动作列表
    * 时间: 2015/08/10 14:17
    * 作者: ZDJ
    */
    virtual void ClearBlockDataList();

//    /**************************************************************************
//    * 函数名: IsExistBlockData
//    * 功能: 是否存在指定块
//    * 参数:
//    *    @[in ] nID: 块ID
//    * 返回值: 存在返回true,不存在返回false
//    * 时间: 2015/08/10 14:19
//    * 作者: ZDJ
//    */
//    bool IsExistBlockData(int nID);

    /**************************************************************************
    * 函数名: FindBlockData
    * 功能: 查找指定块
    * 参数:
    *    @[in ] nID: 块ID
    * 返回值: 存在返回块指针,不存在返回NULL
    * 时间: 2015/08/10 14:20
    * 作者: ZDJ
    */
    CActionBlockData* FindBlockData(int nID);

    /**************************************************************************
    * 函数名: GetActionCount
    * 功能: 获取动作数量
    * 时间: 2015/08/06 18:17
    * 作者: ZDJ
    */
    int GetActionCount();

    /**************************************************************************
    * 函数名: GetAt
    * 功能: 获取指定索引的块
    * 参数:
    *    @[in ] nIndex: 块索引
    * 返回值: 成功返回块数据，失败返回NULL
    * 时间: 2015/08/10 14:22
    * 作者: ZDJ
    */
    CActionBlockData* GetAt(int nIndex);

    /**************************************************************************
    * 函数名: GetIndexByID
    * 功能: 获取指定ID的索引
    * 参数:
    *    @[in ] nID: 块ID
    * 返回值: 成功索引
    * 时间: 2017/7/20
    * 作者: jianjie
    */
    int GetIndexByID(int nID);

    /**************************************************************************
    * 函数名: GetUnUserID
    * 功能: 获取一个没有使用过的ID
    * 时间: 2015/08/10 14:24
    * 作者: ZDJ
    */
    int GetUnUserID();

    /**************************************************************************
    * 函数名: GetID
    * 功能: 获取分组ID
    * 返回值: 分组ID
    * 时间: 2015/08/10 14:25
    * 作者: ZDJ
    */
    int GetID();

    /**************************************************************************
    * 函数名: SetID
    * 功能: 设置分组ID
    * 参数:
    *    @[in ] nID: 分组ID
    * 时间: 2015/08/10 14:26
    * 作者: ZDJ
    */
    void SetID(int nID);

    /**************************************************************************
    * 函数名: SetStartTime
    * 功能: 设置开始时刻
    * 参数:
    *    @[in ] nStartTime: 开始时刻
    * 返回值: void
    * 时间: 2015/08/10 14:27
    * 作者: ZDJ
    */
    void SetStartTime(int nStartTime);

    /**************************************************************************
    * 函数名: GetStartTime
    * 功能: 获取开始时刻
    * 返回值: 开始时刻
    * 时间: 2015/08/10 14:27
    * 作者: ZDJ
    */
    int GetStartTime();

    /**************************************************************************
    * 函数名: GetActionTimeByIndex
    * 功能: 获取指定块的开始时刻
    * 参数:
    *    @[in ] nActionIndex: 指定块的索引
    * 返回值: 开始时刻
    * 时间: 2015/08/10 14:31
    * 作者: ZDJ
    */
    int GetActionTimeByIndex(int nActionIndex);

    /**************************************************************************
    * 函数名: SetEndTime
    * 功能: 设置结束时刻
    * 参数:
    *    @[in ] nEndTime: 结束时刻
    * 返回值: void
    * 时间: 2015/08/07 13:29
    * 作者: ZDJ
    */
    void SetEndTime(int nEndTime);

    /**************************************************************************
    * 函数名: GetEndTime
    * 功能: 获取结束时刻
    * 参数: void
    * 返回值: 结束时刻
    * 时间: 2015/08/07 13:30
    * 作者: ZDJ
    */
    int GetEndTime();

//    /**************************************************************************
//    * 函数名: SetExtBlockData
//    * 功能: 设置附加数据
//    * 参数:
//    *    @[in ] nLen: 附加数据长度
//    *    @[in ] pData: 附加数据缓冲区
//    * 返回值: void
//    * 时间: 2015/08/07 13:26
//    * 作者: ZDJ
//    */
//    void SetExtBlockData(int nLen, char* pData);

//    /**************************************************************************
//    * 函数名: GetExtBlockData
//    * 功能: 获取附加数据
//    * 参数:
//    *    @[out] ppData: 附加数据缓冲区
//    *    @[out] nLen: 附加数据长度
//    * 返回值:
//    * 时间: 2015/08/07 13:28
//    * 作者: ZDJ
//    */
//    void GetExtBlockData(char** ppData, int& nLen);

    /**************************************************************************
    * 函数名: SetDescription
    * 功能: 设置描述信息
    * 参数:
    *    @[in ] strDescription: 描述信息
    * 返回值: void
    * 时间: 2015/08/07 13:22
    * 作者: ZDJ
    */
    void SetDescription(WCHAR* strDescription);

    /**************************************************************************
    * 函数名: GetDescription
    * 功能: 获取描述信息
    * 参数: void
    * 返回值: 描述信息
    * 时间: 2015/08/07 13:23
    * 作者: ZDJ
    */
    WCHAR *GetDescription();

    /**************************************************************************
    * 函数名: GetAllBlockData
    * 功能: 获取所有动作
    * 参数:
    *    @[out] blockData: 返回的动作列表
    * 返回值: void
    * 时间: 2015/08/07 13:23
    * 作者: ZDJ
    */
    void GetAllBlockData(vector<CActionBlockData*>& blockDataList);

    /**************************************************************************
    * 函数名: RecalculateTime
    * 功能: 重新计算分组的结束时刻
    * 时间: 2015/08/06 16:22
    * 作者: ZDJ
    */
    void RecalculateTime();

    /**************************************************************************
    * 函数名: InsertBeforeId
    * 功能: 在当前动作前插入新动作
    * 参数:
    *    @[in ] nID: 当前动作id
    *    @[in ] pActionBlock: 新动作数据
    * 返回值: void
    * 时间: 2015/08/07 13:31
    * 作者: ZDJ
    */
    void InsertBeforeId(int nID, CActionBlockData* pActionBlock);

    /**************************************************************************
    * 函数名: InsertAfterId
    * 功能: 在当前动作后插入新动作
    * 参数:
    *    @[in ] nID: 当前动作id
    *    @[in ] pActionBlock: 新动作数据
    * 返回值: void
    * 时间: 2015/08/07 13:32
    * 作者: ZDJ
    */
    void InsertAfterId(int nID, CActionBlockData* pActionBlock);

//    /**************************************************************************
//    * 函数名: 获取所有的动作的运动时间之和
//    * 功能:
//    * 时间: 2015/08/13 10:52
//    * 作者: ZDJ
//    */
//    int GetActionRunTimeSum();

//    /**************************************************************************
//    * 函数名: 获取所有的动作的总时间之和
//    * 功能:
//    * 时间: 2015/08/13 10:52
//    * 作者: ZDJ
//    */
//    int GetActionAllTimeSum();

    /**************************************************************************
    * 函数名: 设置动作帧未使用ID基数
    * 功能:
    * 时间: 2016/03/16 20:52
    * 作者: 周志平
    */
    void SetUnUseBaseID();

private:
    // 保存所有创建的层的容器
    QList<CActionBlockData*> m_ListActionBlockData;
};
#endif  //_ACTIONBLOCKGROUPMANAGER_H_
