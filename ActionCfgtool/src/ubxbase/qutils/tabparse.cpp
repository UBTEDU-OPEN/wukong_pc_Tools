/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
* 
* 文件名称：tabparser.cpp
* 创建时间：2015/08/15 9:29
* 文件标识：
* 文件摘要：tab文件解析类
* 
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/08/15 9:29
* 版本摘要：
*/
#include "tabparse.h"

CTabParser::CTabParser(void)
{
}

CTabParser::~CTabParser(void)
{	

}
/**************************************************************************
* 函数名: GetCacheFunction
* 功能: 获取缓存中的函数
* 参数: 
*    @[in ] sFuncName: 函数名称
* 返回值: 存在返回函数指针，失败返回NULL
* 时间: 2015/08/15 9:55
* 作者: ZDJ
*/
Function* CTabParser::GetCacheFunction(const QString& sFuncName)
{
	MapFunctionType::iterator itor = m_mapFunctionCache.find(sFuncName);
	if(itor != m_mapFunctionCache.end())		
		return itor->second;
	return NULL;
}

/**************************************************************************
* 函数名: GetCacheFunction
* 功能: 获取缓存中的函数
* 参数: 
*    @[in ] pFunc: 函数指针
* 返回值: 存在返回函数指针，失败返回NULL
* 时间: 2015/08/15 9:55
* 作者: ZDJ
*/
Function* CTabParser::GetCacheFunction(const Function* pFunc)
{
	MapFunctionType::iterator itor = m_mapFunctionCache.begin();
	while(itor != m_mapFunctionCache.end())		
	{
		if(itor->second == pFunc)
			return itor->second;
		itor++;
	}
	return NULL;
}

/**************************************************************************
* 函数名: ExistCacheFunction
* 功能: 缓存中是否存在指定的函数
* 参数: 
*    @[in ] sFuncName: 函数名称
* 返回值: 存在返回TRUE，失败返回FALSE
* 时间: 2015/08/15 9:42
* 作者: ZDJ
*/
BOOL CTabParser::ExistCacheFunction(const QString &sFuncName)
{
	return GetCacheFunction(sFuncName) != NULL;
}

/**************************************************************************
* 函数名: ExistCacheFunction
* 功能: 缓存中是否存在指定的函数
* 参数: 
*    @[in ] pFunc: 函数指针
* 返回值: 存在返回TRUE，失败返回FALSE
* 时间: 2015/08/15 9:42
* 作者: ZDJ
*/
BOOL CTabParser::ExistCacheFunction(const Function* pFunc)
{
	return GetCacheFunction(pFunc) != NULL;
}

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
void CTabParser::AddCacheFunction(const QString &sFuncName, Function* pFunc)
{
	m_mapFunctionCache[sFuncName] = pFunc;
}

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
void CTabParser::GetDataFunctionList(Function* pRootFunc, vector<Function*>& vecFuncList)
{
	if(pRootFunc)
	{
        for(size_t i=0; i < pRootFunc->m_vecTokenList.size(); i++)
		{
			Token* pToken = pRootFunc->m_vecTokenList[i];
			DataToken* pDataToken = dynamic_cast<DataToken*>(pToken);
			if(pDataToken)
			{
				vecFuncList.push_back(pRootFunc);
				break;
			}
			else
			{
				FunToken* pFunToken = dynamic_cast<FunToken*>(pToken);
				if(pFunToken)
				{
					GetDataFunctionList(pFunToken->m_pCallFunction, vecFuncList);
				}				
			}
		}		
	}	
}

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
BOOL CTabParser::ReadTab(const QString &sTabPath, vector<Function*>& vecFuncList)
{
	ReleaseCache();

    QFile fileobj(sTabPath);
    if(fileobj.open(QIODevice::ReadOnly | QIODevice::Text))
	{
        QString sLineStr;
        QString sCurDefineFuncName;//当前定义的函数名
		Function* pMainFunc = NULL;//主函数

        char szBuf[STRING_LEN_1024] = {0};
        while(!fileobj.atEnd())
		{
            memset(szBuf, 0, STRING_LEN_1024);
            fileobj.readLine(szBuf, STRING_LEN_1024);

            sLineStr = QString::fromStdString(szBuf);
            sLineStr = sLineStr.trimmed();

            QString sFindStr = sLineStr;
            sFindStr = sFindStr.toUpper();

            if(sFindStr.indexOf("FUNCTION") != -1)
			{
                QString sFuncName = sLineStr.mid(8, sLineStr.indexOf('(') - 8);
                sFuncName = sFuncName.trimmed();

				sCurDefineFuncName = sFuncName;//记录当前定义的函数名

				if(!ExistCacheFunction(sFuncName))
				{
					//不存在该函数则添加
					Function* pNewFunc = new Function(sFuncName);
					AddCacheFunction(sFuncName, pNewFunc);

                    if(sFuncName == "Main")
						pMainFunc = pNewFunc;
				}
			}
            else if(sFindStr.indexOf("MOVE") != -1)
			{
				Function* pCurFunc = GetCacheFunction(sCurDefineFuncName);
				if(pCurFunc)
				{
					DataToken* pNewDataToken = new DataToken;
					pCurFunc->m_vecTokenList.push_back(pNewDataToken);

                    QString sDataStr = sLineStr.mid(4);
                    sDataStr = sDataStr.trimmed();
                    sDataStr = sDataStr.right(sDataStr.length() - 1);//去掉左括号
                    sDataStr = sDataStr.left(sDataStr.length() - 1);//去掉右括号
                    sDataStr = sDataStr.trimmed();

					int nLastIndex = 0;
                    for(int i=0;i<sDataStr.length();i++)
					{
						if(sDataStr[i] == ',')
						{
                            QString sNum = sDataStr.mid(nLastIndex, i - nLastIndex);
                            sNum = sNum.trimmed();
                            pNewDataToken->m_vecDataFrame.push_back(sNum.toInt());
							nLastIndex = i + 1;
						}
					}

                    QString sNum = sDataStr.mid(nLastIndex);
                    sNum = sNum.trimmed();
                    pNewDataToken->m_vecDataFrame.push_back(sNum.toInt());
				}					
			}
            else if(sFindStr.indexOf("END") != -1)
			{
			}
			else
			{
                if(sLineStr.indexOf('(') != -1 && sLineStr.indexOf(')') != -1)
				{
					//函数调用
					Function* pCurFunc = GetCacheFunction(sCurDefineFuncName);
					if(pCurFunc)
					{
						FunToken* pNewFunToken = new FunToken;

                        QString sCallFuncName = sLineStr.left(sLineStr.indexOf('('));
                        sCallFuncName = sCallFuncName.trimmed();

						Function* pCallFunc = GetCacheFunction(sCallFuncName);
						if(!pCallFunc)
						{
							pCallFunc = new Function(sCallFuncName);	
							AddCacheFunction(sCallFuncName, pCallFunc);//加入缓存
						}

						pNewFunToken->m_pCallFunction = pCallFunc;
						pCurFunc->m_vecTokenList.push_back(pNewFunToken);
					}
				}
			}
		}

        fileobj.close();

		GetDataFunctionList(pMainFunc, vecFuncList);	
		return TRUE;
	}

	return FALSE;
}

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
BOOL CTabParser::WriteTab(const Function* pMainFunc, const QString &sSavePath)
{
    QFile fileobj(sSavePath);
    if(fileobj.open(QIODevice::WriteOnly | QIODevice::Text))
	{
        vector<QString> vecLineStrList;

		WriteTab(pMainFunc, vecLineStrList);
        for(size_t i = 0; i < vecLineStrList.size(); i++)
		{
            QString sTemp = vecLineStrList[i];
            string szStr = sTemp.toStdString();
            fileobj.write(szStr.c_str(), szStr.length());
			if(i < vecLineStrList.size() - 1)
                fileobj.write("\r\n", 2);
		}

        fileobj.close();
	}
	return TRUE;
}

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
void CTabParser::WriteTab(const Function* pRootFunc, vector<QString>& vecLineStrList)
{
	if(pRootFunc)
	{
		Function* pCurFunc = const_cast<Function*>(pRootFunc);
        QString sLineStr = "function " + pCurFunc->m_sName + "()";//函数定义开始
		vecLineStrList.push_back(sLineStr);

        sLineStr = "end";//函数定义结束
		vecLineStrList.push_back(sLineStr);

		if(!ExistCacheFunction(pCurFunc))
			AddCacheFunction(pCurFunc->m_sName, pCurFunc);

        for(size_t j = 0; j < pCurFunc->m_vecTokenList.size(); j++)
		{
			Token* pToken = pCurFunc->m_vecTokenList[j];

			DataToken* pDataToken = dynamic_cast<DataToken*>(pToken);
			if(pDataToken)
			{
				//move调用
                sLineStr = "    Move(";
                for(size_t k = 0; k < pDataToken->m_vecDataFrame.size(); k++)
				{
                    QString sNum;
                    sNum.sprintf("%5d", pDataToken->m_vecDataFrame[k]);
					if(k > 0)
                        sNum.insert(0, ", ");//插入逗号分隔符
					sLineStr += sNum;
				}

                sLineStr += ")";//move指令构造结束
                vector<QString>::iterator itorInsert = FindFunctionInsert(vecLineStrList, pCurFunc);
				vecLineStrList.insert(itorInsert, sLineStr);
			}
			else
			{
				FunToken* pFuncToken = dynamic_cast<FunToken*>(pToken);
				if(pFuncToken)
				{
					//子函数调用
                    sLineStr = "    " + pFuncToken->m_pCallFunction->m_sName +  "()";
                    vector<QString>::iterator itorAppend = FindFunctionAppend(vecLineStrList);
					vecLineStrList.insert(itorAppend, sLineStr);

					if(!ExistFunction(vecLineStrList, pFuncToken->m_pCallFunction->m_sName))
					{
						//创建函数定义
						WriteTab(pFuncToken->m_pCallFunction, vecLineStrList);
					}
				}
			}
		}		
	}
}

/**************************************************************************
* 函数名: ReleaseCache
* 功能: 释放缓存
* 时间: 2015/08/15 9:38
* 作者: ZDJ
*/
void CTabParser::ReleaseCache()
{
    Function* pMainFunc = GetCacheFunction("Main");
	ReleaseCache(pMainFunc);
}

/**************************************************************************
* 函数名: ReleaseCache
* 功能: 释放缓存
* 参数: 
*    @[in ] pRootFunc: 当前函数
* 时间: 2015/08/15 9:38
* 作者: ZDJ
*/
void CTabParser::ReleaseCache(Function* pRootFunc)
{
	if(pRootFunc && ExistCacheFunction(pRootFunc))
	{
		vector<Token*>::iterator itor = pRootFunc->m_vecTokenList.begin();
		while(itor != pRootFunc->m_vecTokenList.end())
		{
			FunToken* pFunToken = dynamic_cast<FunToken*>(*itor);
			if(pFunToken)
			{	
				ReleaseCache(pFunToken->m_pCallFunction);
			}

			delete *itor;
			itor = pRootFunc->m_vecTokenList.erase(itor);						
		}	

		RemoveCacheFunction(pRootFunc);
	}
}

/**************************************************************************
* 函数名: RemoveCacheFunction
* 功能: 删除并释放缓存中的函数
* 参数: 
*    @[in ] sFuncName: 函数名称
* 返回值: void
* 时间: 2015/08/17 9:36
* 作者: ZDJ
*/
void CTabParser::RemoveCacheFunction(const QString &sFuncName)
{
	MapFunctionType::iterator itor = m_mapFunctionCache.find(sFuncName);
	if(itor != m_mapFunctionCache.end())
	{
		delete itor->second;
		m_mapFunctionCache.erase(itor);
	}
}

/**************************************************************************
* 函数名: RemoveCacheFunction
* 功能: 删除并释放缓存中的函数
* 参数: 
*    @[in ] pFunc: 函数指针
* 返回值: void
* 时间: 2015/08/17 9:36
* 作者: ZDJ
*/
void CTabParser::RemoveCacheFunction(const Function* pFunc)
{
	MapFunctionType::iterator itor = m_mapFunctionCache.begin();
	while(itor != m_mapFunctionCache.end())
	{
		if(itor->second == pFunc)
		{
			delete itor->second;
            m_mapFunctionCache.erase(itor++);
		}
		else
			itor++;
	}
}

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
vector<QString>::iterator CTabParser::FindFunctionInsert(vector<QString> &vecLineStrList, const Function* pFunc)
{
    vector<QString>::iterator itorRet = vecLineStrList.begin();
    vector<QString>::iterator itor = itorRet;
	while(itor != vecLineStrList.end())
	{
        QString sLineStr = *itor;
        if(sLineStr.indexOf("function") != -1)
		{
            sLineStr = sLineStr.mid(8, sLineStr.indexOf('(') - 8);
            sLineStr = sLineStr.trimmed();
			if(sLineStr == pFunc->m_sName)
			{
				itorRet = itor + 1;//function行的下一行字符串位置
			}
		}
        else if(sLineStr.indexOf("end") != -1)
		{
			if(itor >= itorRet)
			{
				itorRet = itor;//找到了插入位置
				break;
			}
		}

		itor++;
	}

	return itorRet;
}

/**************************************************************************
* 函数名: FindFunctionAppend
* 功能: 查找当前函数的追加位置
* 参数: 
*    @[in ] vecLineStrList: tab字符串列表
* 返回值: tab字符串列表迭代器位置
* 时间: 2015/08/18 15:18
* 作者: ZDJ
*/
vector<QString>::iterator CTabParser::FindFunctionAppend(vector<QString>& vecLineStrList)
{
    vector<QString>::iterator itorRet = vecLineStrList.begin();
    vector<QString>::iterator itor = itorRet;
	while(itor != vecLineStrList.end())
	{
		itorRet = itor;
		itor++;
	}

	return itorRet;
}

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
BOOL CTabParser::ExistFunction(vector<QString> &vecLineStrList, const QString &sFuncName)
{
    vector<QString>::iterator itor = vecLineStrList.begin();
	while(itor != vecLineStrList.end())
	{
        QString sLineStr = *itor;
        if(sLineStr.indexOf("function") != -1)
		{
            sLineStr = sLineStr.mid(8, sLineStr.indexOf('(') - 8);
            sLineStr = sLineStr.trimmed();
			if(sLineStr == sFuncName)
			{
				return TRUE;
			}
		}

		itor++;
	}

	return FALSE;
}
