/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
* 
* 文件名称：TabParser.h
* 创建时间：2015/08/15 9:29
* 文件标识：
* 文件摘要：tab文件解析类
* 
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/08/15 9:29
* 版本摘要：
*/
#ifndef _TABPARSER_H_
#define _TABPARSER_H_

#include <map>
#include <vector>
#include <QString>
#include <QFile>

using namespace std;

//动作基类
class Token
{
public:	
	virtual ~Token(){}	
};

//函数
class Function
{
public:
    Function(const QString& sName)
	{
		m_sName = sName;
	}	

    QString        m_sName;//函数名称
	vector<Token*> m_vecTokenList;//动作列表
};

//move动作
class DataToken : public Token
{
public:
	~DataToken()
	{
		vector<int>().swap(m_vecDataFrame);
	}
	vector<int> m_vecDataFrame;//数据帧
};

//函数动作
class FunToken : public Token
{
public:
	FunToken()
	{
		m_pCallFunction = NULL;
	}	

	Function* m_pCallFunction;//调用的函数
};

//tab文件解析类
class CTabParser
{
public:
	CTabParser(void);
	~CTabParser(void);

	/**************************************************************************
	* 函数名: GetCacheFunction
	* 功能: 获取缓存中的函数
	* 参数: 
	*    @[in ] sFuncName: 函数名称
	* 返回值: 存在返回函数指针，失败返回NULL
	* 时间: 2015/08/15 9:55
	* 作者: ZDJ
	*/
    Function* GetCacheFunction(const QString& sFuncName);

	/**************************************************************************
	* 函数名: GetCacheFunction
	* 功能: 获取缓存中的函数
	* 参数: 
	*    @[in ] pFunc: 函数指针
	* 返回值: 存在返回函数指针，失败返回NULL
	* 时间: 2015/08/15 9:55
	* 作者: ZDJ
	*/
	Function* GetCacheFunction(const Function* pFunc);

	/**************************************************************************
	* 函数名: ExistCacheFunction
	* 功能: 缓存中是否存在指定的函数
	* 参数: 
	*    @[in ] sFuncName: 函数名称
	* 返回值: 存在返回TRUE，失败返回FALSE
	* 时间: 2015/08/15 9:42
	* 作者: ZDJ
	*/
    BOOL ExistCacheFunction(const QString& sFuncName);

	/**************************************************************************
	* 函数名: ExistCacheFunction
	* 功能: 缓存中是否存在指定的函数
	* 参数: 
	*    @[in ] pFunc: 函数指针
	* 返回值: 存在返回TRUE，失败返回FALSE
	* 时间: 2015/08/15 9:42
	* 作者: ZDJ
	*/
	BOOL ExistCacheFunction(const Function* pFunc);

	/**************************************************************************
	* 函数名: AddCacheFunction
	* 功能: 添加函数到缓存中
	* 参数: 
	*    @[in ] sFuncName: 函数名称
	*    @[in ] pFunc: 函数
	* 返回值: 
	* 时间: 2015/08/15 9:54
	* 作者: ZDJ
	*/
    void AddCacheFunction(const QString& sFuncName, Function* pFunc);

	/**************************************************************************
	* 函数名: GetDataFunctionList
	* 功能: 获取含动作帧的函数列表
	* 参数: 
	*    @[in ] pRootFunc: 起始函数
	*    @[out] vecFuncList: 含动作帧的函数列表
	* 返回值: void
	* 时间: 2015/08/15 9:41
	* 作者: ZDJ
	*/
	void GetDataFunctionList(Function* pRootFunc, vector<Function*>& vecFuncList);

	/**************************************************************************
	* 函数名: ReadTab
	* 功能: 读取tab文件
	* 参数: 
	*    @[in ] sTabPath: tab文件路径
	*    @[out] vecFuncList: 返回函数列表
	* 返回值: 成功返回TRUE，失败返回FALSE
	* 时间: 2015/08/15 9:40
	* 作者: ZDJ
	*/
    BOOL ReadTab(const QString& sTabPath, vector<Function*>& vecFuncList);

	/**************************************************************************
	* 函数名: WriteTab
	* 功能: 保存tab文件
	* 参数: 
	*    @[in ] pMainFunc: 主函数
	*    @[in ] sSavePath: tab保存路径
	* 返回值: 成功返回TRUE，失败返回FALSE
	* 时间: 2015/08/15 9:39
	* 作者: ZDJ
	*/
    BOOL WriteTab(const Function* pMainFunc, const QString& sSavePath);

	/**************************************************************************
	* 函数名: ReleaseCache
	* 功能: 释放缓存
	* 时间: 2015/08/15 9:38
	* 作者: ZDJ
	*/
	void ReleaseCache();

	/**************************************************************************
	* 函数名: RemoveCacheFunction
	* 功能: 删除并释放缓存中的函数
	* 参数: 
	*    @[in ] sFuncName: 函数名称
	* 返回值: void
	* 时间: 2015/08/17 9:36
	* 作者: ZDJ
	*/
    void RemoveCacheFunction(const QString& sFuncName);

	/**************************************************************************
	* 函数名: RemoveCacheFunction
	* 功能: 删除并释放缓存中的函数
	* 参数: 
	*    @[in ] pFunc: 函数指针
	* 返回值: void
	* 时间: 2015/08/17 9:36
	* 作者: ZDJ
	*/
	void RemoveCacheFunction(const Function* pFunc);

private:
	/**************************************************************************
	* 函数名: ReleaseCache
	* 功能: 释放缓存
	* 参数: 
	*    @[in ] pRootFunc: 当前函数
	* 时间: 2015/08/15 9:38
	* 作者: ZDJ
	*/
	void ReleaseCache(Function* pRootFunc);

	/**************************************************************************
	* 函数名: WriteTab
	* 功能: 保存tab文件
	* 参数: 
	*    @[in ] pRootFunc: 当前函数
	*    @[out] vecLineStrList: tab字符串列表
	* 返回值: void
	* 时间: 2015/08/15 9:39
	* 作者: ZDJ
	*/
    void WriteTab(const Function* pRootFunc, vector<QString>& vecLineStrList);

	/**************************************************************************
	* 函数名: FindFunctionInsert
	* 功能: 查找当前函数的插入位置
	* 参数: 
	*    @[in ] vecLineStrList: tab字符串列表
	*    @[in ] pFunc: 当前函数
	* 返回值: tab字符串列表迭代器位置
	* 时间: 2015/08/18 15:18
	* 作者: ZDJ
	*/
    vector<QString>::iterator FindFunctionInsert(vector<QString>& vecLineStrList, const Function* pFunc);

	/**************************************************************************
	* 函数名: FindFunctionAppend
	* 功能: 查找当前函数的追加位置
	* 参数: 
	*    @[in ] vecLineStrList: tab字符串列表
	* 返回值: tab字符串列表迭代器位置
	* 时间: 2015/08/18 15:18
	* 作者: ZDJ
	*/
    vector<QString>::iterator FindFunctionAppend(vector<QString>& vecLineStrList);

	/**************************************************************************
	* 函数名: ExistFunction
	* 功能: 查找当前函数是否存在
	* 参数: 
	*    @[in ] vecLineStrList: tab字符串列表
	*    @[in ] sFuncName: 函数名称
	* 返回值: 存在返回TRUE，否则返回FALSE
	* 时间: 2015/08/18 15:18
	* 作者: ZDJ
	*/
    BOOL ExistFunction(vector<QString>& vecLineStrList, const QString& sFuncName);

private:
    typedef map<QString, Function*> MapFunctionType;//函数名到函数的映射
	MapFunctionType m_mapFunctionCache; 
};
#endif  //_TABPARSER_H_
