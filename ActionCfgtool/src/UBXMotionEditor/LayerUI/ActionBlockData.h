/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：
* 创建时间：2015/12/21 10:40
* 文件标识：动作块数据类定义
* 文件摘要：
*
* 当前版本：1.0.0.0
* 作    者：周志平
* 完成时间：2015/12/21 10:40
* 版本摘要：
*/
#ifndef _ACTIONBLOCKDATA_H_
#define _ACTIONBLOCKDATA_H_

#include "stable.h"
#include "type.h"
#include "motioneditor_global.h"

#define MAX_DESCRIPTION_SIZE 30

//该类所有时间对应的都是格数，具体时间要乘以时间粒度才是真实的时间
class MOTIONEDITOR_EXPORT CActionBlockData
{
public:
	CActionBlockData(void);
    virtual ~CActionBlockData(void);

public:
	/**************************************************************************
	* 函数名: GetData
	* 功能: 获取动作数据
	* 参数: 
	*    @[out] ppData: 动作数据缓冲区
	*    @[out] nLen: 动作数据长度
	* 返回值: 成功返回true，失败返回false
	* 时间: 2015/08/10 13:42
	* 作者: ZDJ
	*/
	virtual bool GetData(char** ppData, int& nLen);

	/**************************************************************************
	* 函数名: SetData
	* 功能: 设置动作数据
	* 参数: 
	*    @[in ] pData: 动作数据缓冲区
	*    @[in ] nLen: 动作数据长度
	* 返回值: void
	* 时间: 2015/08/10 13:44
	* 作者: ZDJ
	*/
	virtual void SetData(char* pData, int nLen);

	/**************************************************************************
	* 函数名: GetID
	* 功能: 获取动作ID
	* 时间: 2015/08/10 13:36
	* 作者: ZDJ
	*/
	int GetID();

	/**************************************************************************
	* 函数名: SetID
	* 功能: 设置动作ID
	* 参数: 
	*    @[in ] nID: 动作ID
	* 返回值: void
	* 时间: 2015/08/10 13:37
	* 作者: ZDJ
	*/
	void SetID(int nID);

	/**************************************************************************
	* 函数名: GetMotorAngle
	* 功能: 获取舵机角度
	* 参数: 
	*    @[in ] nMotroID: 舵机ID
	* 返回值: 舵机角度
	* 时间: 2015/08/10 13:38
	* 作者: ZDJ
	*/
	int  GetMotorAngle(int nMotroID);

	/**************************************************************************
	* 函数名: SetMotorAngle
	* 功能: 设置舵机角度
	* 参数: 
	*    @[in ] pData: 角度数据
	*    @[in ] nLen: 角度数据长度
	*    @[in ] nID: 舵机ID
    *    @[in ] nAngle: 设置角度
	* 返回值: 
	* 时间: 2015/08/10 13:40
	* 作者: ZDJ
	*/
    void SetMotorAngle(char* pData, int nLen, int nID, int nAngle);
	
	/**************************************************************************
	* 函数名: SetRunTime
	* 功能: 设置运行时间
	* 参数: 
    *    @[in ] nTime: 变换后的运行时间
	* 返回值: void
	* 时间: 2015/08/07 15:59
	* 作者: ZDJ
	*/
    void SetRunTime(int nTime);

	/**************************************************************************
	* 函数名: GetRunTime
	* 功能: 获取运行时间
	* 参数: void	
	* 返回值: 变换后的运行时间
	* 时间: 2015/08/07 16:00
	* 作者: ZDJ
	*/
    int GetRunTime();

	/**************************************************************************
    * 函数名: SetStopTime
	* 功能: 设置总时间
	* 参数: 
    *    @[in ] nTime: 变换后的总时间
	* 返回值: void
	* 时间: 2015/08/07 16:01
	* 作者: ZDJ
	*/
    void SetStopTime(int nTime);

	/**************************************************************************
    * 函数名: GetStopTime
	* 功能: 获取总时间
	* 参数: void	
	* 返回值: 变换后的总时间
	* 时间: 2015/08/07 16:00
	* 作者: ZDJ
	*/
    int GetStopTime();

	/**************************************************************************
	* 函数名: SetDescription
	* 功能: 设置描述信息
	* 参数: 
	*    @[in ] strDescription: 描述信息
	* 返回值: void
	* 时间: 2015/08/07 16:02
	* 作者: ZDJ
	*/
    void SetDescription(WCHAR* strDescription);

	/**************************************************************************
	* 函数名: GetDescription
	* 功能: 获取描述信息
	* 参数: void	
	* 返回值: 描述信息
	* 时间: 2015/08/07 16:02
	* 作者: ZDJ
	*/
    WCHAR *GetDescription();

	/**************************************************************************
	* 函数名: SetExtBlockData
	* 功能: 设置附加数据
	* 参数: 
	*    @[in ] nLen: 附加数据长度
	*    @[in ] pData: 附加数据缓冲区
	* 返回值: 
	* 时间: 2015/08/07 16:03
	* 作者: ZDJ
	*/
	void SetExtBlockData(int nLen, char* pData);

	/**************************************************************************
	* 函数名: GetExtBlockData
	* 功能: 获取附加数据
	* 参数: 
	*    @[out] ppData: 附加数据缓冲区
	*    @[out] nLen: 附加数据长度	
	* 返回值: void
	* 时间: 2015/08/07 16:03
	* 作者: ZDJ
	*/
	void GetExtBlockData(char** ppData, int& nLen);

protected:
	int m_ID;			// 模块ID
    int m_nRunTime;   // 运行时间，已经变换
	//float m_fFreezeTime;// 冻结时间，已经变换
    int m_nStopTime;   // 总时间，已经变换
    WCHAR  m_strDescription[MAX_DESCRIPTION_SIZE];	// 描述

	char* m_pBlockData;
	int m_nBlockDataLen;
};
#endif  //_ACTIONBLOCKDATA_H_
