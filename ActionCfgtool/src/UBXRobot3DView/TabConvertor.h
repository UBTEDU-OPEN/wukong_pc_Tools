/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
* 
* 文件名称：TabConvertor.h
* 创建时间：2015/08/17 9:56
* 文件标识：
* 文件摘要：tab转换类
* 
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/08/17 9:56
* 版本摘要：
*/
#ifndef _TABCONVERTOR_H_
#define _TABCONVERTOR_H_

#include "TabParser.h"
#include "util.h"
#include "robotdef.h"

//#ifndef MOTOR_NUM
//#define MOTOR_NUM 16  //舵机数量
//#endif

#ifndef MOTOR_TAB_NUM
#define MOTOR_TAB_NUM 20  //tab文件中的舵机数量
#endif

#ifndef MOTOR_TAB_RUNTIME_COL
#define MOTOR_TAB_RUNTIME_COL MOTOR_TAB_NUM  //tab文件中的运行时间列索引
#endif

#ifndef MOTOR_TAB_ALLTIME_COL
#define MOTOR_TAB_ALLTIME_COL MOTOR_TAB_RUNTIME_COL + 1  //tab文件中的总时间列索引
#endif

#ifndef ACTION_LAYER_TIME
#define ACTION_LAYER_TIME		0
#endif
#ifndef ACTION_LAYER_MOTION
#define ACTION_LAYER_MOTION		1
#endif
#ifndef ACTION_LAYER_EYE
#define ACTION_LAYER_EYE		2
#endif
#ifndef ACTION_LAYER_EAR
#define ACTION_LAYER_EAR		3
#endif
#ifndef ACTION_LAYER_MUSIC
#define ACTION_LAYER_MUSIC		4
#endif

//动作数据
typedef struct _MYACTIONDATA
{
    ~_MYACTIONDATA()
    {
        CUtil::ReleaseVector(vecMotorList);
    }

    //获取舵机数
    int getMotorCount()
    {
        return vecMotorList.size();
    }

	vector<MOTOR_DATA*> vecMotorList;//舵机数据列表
	float fRunTime;//运行时间，单位ms
    float fAllTime;//总时间，单位ms
}MYACTIONDATA;

//分组数据
typedef struct _MYGROUPDATA
{	
    _MYGROUPDATA(const QString& sName)
	{
		sGroupName = sName;
	}
	~_MYGROUPDATA()
	{
		CUtil::ReleaseVector(vecActionList);
	}

    //获取舵机数
    int getMotorCount()
    {
        if(vecActionList.size() > 0)
            return vecActionList[0]->getMotorCount();
        return 0;
    }

    QString               sGroupName;   //分组名称
	vector<MYACTIONDATA*> vecActionList;//动作列表
}MYGROUPDATA;

//tab文件数据
typedef struct _MYTABDATA
{
	~_MYTABDATA()
	{
		CUtil::ReleaseVector(vecGroupList);
	}

    //获取舵机数
    int getMotorCount()
    {
        if(vecGroupList.size() > 0)
            return vecGroupList[0]->getMotorCount();
        return 0;
    }

	vector<MYGROUPDATA*> vecGroupList;//分组列表
}MYTABDATA;

//tab转换类
class CTabConvertor
{
public:
	CTabConvertor(void);
	~CTabConvertor(void);

	/**************************************************************************
	* 函数名: GetTabData
	* 功能: 获取tab文件数据
	* 参数: 
	*    @[in ] sTabPath: tab文件路径
	*    @[out] pMyTabData: 动作数据
	* 返回值: 成功返回TRUE，失败返回FALSE
	* 时间: 2015/08/14 10:54
	* 作者: ZDJ
	*/
    BOOL GetTabData(const QString &sTabPath, MYTABDATA*& pMyTabData);

	/**************************************************************************
	* 函数名: CreateAesData
	* 功能: 生成aes格式数据
	* 参数: 
	*    @[in ] pMyTabData: tab数据
	*    @[out] pData: aes数据缓冲区
	*    @[out] nLen: aes数据缓冲区长度
	* 返回值: 成功返回TRUE，失败返回FALSE
	* 时间: 2015/08/14 13:20
	* 作者: ZDJ
	*/
    BOOL CreateAesData(MYTABDATA* pMyTabData, char*& pData, int& nLen);

	/**************************************************************************
	* 函数名: CreateHtsData
	* 功能: 生成hts格式数据
	* 参数: 
	*    @[in ] pMyTabData: tab数据
	*    @[out] pData: hts数据缓冲区
	*    @[out] nLen: hts数据缓冲区长度
	* 返回值: 成功返回TRUE，失败返回FALSE
	* 时间: 2015/08/14 13:20
	* 作者: ZDJ
	*/
	BOOL CreateHtsData(const MYTABDATA* pMyTabData, char*& pData, int& nLen);

	/**************************************************************************
	* 函数名: CreateAesData
	* 功能: 生成aes格式数据
	* 参数: 
	*    @[in ] sTabPath: tab文件路径
	*    @[out] pData: aes数据缓冲区
	*    @[out] nLen: aes数据缓冲区长度
	* 返回值: 成功返回TRUE，失败返回FALSE
	* 时间: 2015/08/14 13:20
	* 作者: ZDJ
	*/
    BOOL CreateAesData(const QString &sTabPath, char*& pData, int& nLen);

	/**************************************************************************
	* 函数名: CreateHtsData
	* 功能: 生成hts格式数据
	* 参数: 
	*    @[in ] sTabPath: tab文件路径
	*    @[out] pData: hts数据缓冲区
	*    @[out] nLen: hts数据缓冲区长度
	* 返回值: 成功返回TRUE，失败返回FALSE
	* 时间: 2015/08/14 13:20
	* 作者: ZDJ
	*/
    BOOL CreateHtsData(const QString &sTabPath, char*& pData, int& nLen);

	/**************************************************************************
	* 函数名: GetHtsFrameData
	* 功能: 获取hts帧数据
	* 参数: 
	*    @[in ] nFrameType: 帧类型, 1,2,3
	*    @[in ] nTotalFrame: 总帧数
	*    @[in ] nCurFrame: 当前帧
	*    @[in ] pMoveData: 当前move数据
	*    @[in ] nMotorDataLen: 舵机数据长度，舵机数量加运行时间和总时间
	*    @[in ] pRemain: 当前move数据补偿
	* 返回值: hts帧数据
	* 时间: 2015/08/19 14:15
	* 作者: ZDJ
	*/
	byte* GetHtsFrameData(int nFrameType, int nTotalFrame, int nCurFrame, int* pMoveData, int nMotorDataLen, int* pRemain);

	/**************************************************************************
	* 函数名: CreateHtsData
	* 功能: 生成hts格式数据
	* 参数: 
	*    @[in ] sAesPath: aes文件路径
	*    @[out] pOutData: hts数据缓冲区
	*    @[out] nOutLen: hts数据缓冲区长度
	* 返回值: 成功返回TRUE，失败返回FALSE
	* 时间: 2015/08/14 13:20
	* 作者: ZDJ
	*/
    BOOL CreateHtsData2(const QString &sAesPath, char*& pOutData, int& nOutLen);

    /**************************************************************************
    * 函数名: CreateHtsData
    * 功能: 生成hts格式数据
    * 参数:
    *    @[in ] pInAesFileData: aes数据缓冲区
    *    @[in ] lInFileLen: aes数据缓冲区长度
    *    @[out] pOutData: hts数据缓冲区
    *    @[out] nOutLen: hts数据缓冲区长度
    * 返回值: 成功返回TRUE，失败返回FALSE
    * 时间: 2016/05/26
    * 作者: Suhaiwu
    */
    BOOL CreateHtsData(byte* pInAesFileData, const ULONGLONG lInLen, char*& pOutData, int& nOutLen);

private:
	CTabParser m_tabparser;//tab解析器	
};

#endif  //_TABCONVERTOR_H_
