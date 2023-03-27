/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
* 
* 文件名称：Util.h
* 创建时间：2015/05/14 17:25
* 文件标识：
* 文件摘要：辅助函数类。
* 
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/05/14 17:25
* 版本摘要：
*/
#ifndef UTIL_H
#define UTIL_H

#include "qutils_global.h"
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <string>
using namespace std;
#ifdef USEVERSION
#pragma comment(lib,"version.lib")
#endif
#ifdef _WIN32
#include <windows.h>
#include <TlHelp32.h>
#include <shlwapi.h>
#endif

#include <QFile>

#define SAFE_FREE(p) {if(p){ free(p); p = NULL;}}
#define SAFE_DELETE(p) {if(p){ delete p; p = NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p){ delete [] p; p = NULL;}}
#define SAFE_FILE_CLOSE(p) {if(p){ fclose(p); p = NULL;}}
#define SAFE_CLOSE(p) {if(p){ p->Close(); p = NULL;}}
#define NULLSTR ""


#define STRUCT_CONSTRUCTOR_DEFAULT(T)         \
T()                                           \
{                                             \
	memset(this, 0, sizeof(T));               \
}

#define STRUCT_CONSTRUCTOR_COPY(T)            \
T(const T& obj)                               \
{                                             \
	if (this != &obj)                         \
	{                                         \
		memcpy(this, &obj, sizeof(T));        \
	}                                         \
}


#define STRUCT_OPERATOR_ASSIGN(T)             \
T& operator = (const T& obj)                  \
{                                             \
	if (this != &obj)                         \
	{                                         \
		memcpy(this, &obj, sizeof(T));        \
	}                                         \
	return *this;                             \
}

#define STRUCT_CONSTRUCTOR(T)                 \
T()                                           \
{                                             \
	memset(this, 0, sizeof(T));               \
}                                             \
T(const T& obj)                               \
{                                             \
	if (this != &obj)                         \
	{                                         \
		memcpy(this, &obj, sizeof(T));        \
	}                                         \
}                                             \
T& operator = (const T& obj)                  \
{                                             \
	if (this != &obj)                         \
	{                                         \
		memcpy(this, &obj, sizeof(T));        \
	}                                         \
	return *this;                             \
}


#define UBX_MEMCPY_INC(t, s) memcpy(&t, s, sizeof(t));s+=sizeof(t);

#define TRUE  1
#define FALSE 0
#define NULL  0

#define IN
#define OUT

#ifdef Q_OS_WIN
#define SELECTANY __declspec(selectany)
#else
#define SELECTANY
#endif

#define SAFE_DELETE(p)       if(p){ delete p; p = NULL;}
#define SAFE_DELETE_ARRAY(p) if(p){ delete [] p; p = NULL;}
#define SAFE_FREE(p)         if(p){ free(p); p = NULL;}
#define SAFE_CLOSEHANDLE(p)  if(p){ CloseHandle(p); p = NULL;}

//拷贝内存并移动源数据指针
#define MEMCPY_OFFSETSRC(dest,src,size) {memcpy(dest, src, size); src += size;}

//拷贝内存并移动目标数据指针
#define MEMCPY_OFFSETDEST(dest,src,size) {memcpy(dest, src, size); dest += size;}

//拷贝内存并移动源数据指针和目标数据指针
#define MEMCPY_OFFSETBOTH(dest,src,size) {memcpy(dest, src, size); src += size; dest += size;}

//将符号转为字符串
#define GETSTR(s) #s

#define STRING_LEN_32    32
#define STRING_LEN_64    64
#define STRING_LEN_128   128
#define STRING_LEN_256   256
#define STRING_LEN_512   512
#define STRING_LEN_1024  1024

#ifdef Q_OS_WIN
#define STDCALL __stdcall
#else
#define STDCALL
#define CALLBACK
#endif

#define STRUCT_INIT(T)\
T()\
{\
    memset(this, 0, sizeof(T));\
}\
T(const T& obj)\
{\
    if(this != &obj)\
        memcpy(this, &obj, sizeof(T));\
}\
T& operator = (const T& obj)\
{\
    if(this != &obj)\
        memcpy(this, &obj, sizeof(T));\
    return *this;\
}

#define STRUCT_CONSTRUCT_DEFAULT(T)\
T()\
{\
    memset(this, 0, sizeof(T));\
}

#define STRUCT_CONSTRUCT_COPY(T)\
T(const T& obj)\
{\
    if(this != &obj)\
        memcpy(this, &obj, sizeof(T));\
}

#define STRUCT_OPERATOR_ASSIGN(T)\
T& operator = (const T& obj)\
{\
    if(this != &obj)\
        memcpy(this, &obj, sizeof(T));\
    return *this;\
}

#define ZXPROPERTY(NAME, TYPE, VARIATE)\
    public:                   \
        TYPE Get##NAME()      \
        {					  \
            return VARIATE;   \
        }					  \
        void Set##NAME(TYPE var)\
        {					  \
            VARIATE = var;    \
        }

#define ZXPROPERTY_GET(NAME, TYPE, VARIATE)\
    public:                   \
        TYPE Get##NAME()      \
        {					  \
            return VARIATE;   \
        }

#define ZXPROPERTY_SET(NAME, TYPE, VARIATE)\
    public:                     \
        void Set##NAME(TYPE var)\
        {					    \
            VARIATE = var;      \
        }


#define ZXPROPERTY_REF(NAME, TYPE, VARIATE)\
    public:                   \
        TYPE& Get##NAME()     \
        {					  \
            return VARIATE;   \
        }					  \
        void Set##NAME(const TYPE& var)\
        {					  \
            VARIATE = var;    \
        }

#define ZXPROPERTY_REF_GET(NAME, TYPE, VARIATE)\
    public:                   \
        TYPE& Get##NAME()     \
        {					  \
            return VARIATE;   \
        }

#define ZXPROPERTY_REF_SET(NAME, TYPE, VARIATE)\
    public:                   \
        void Set##NAME(const TYPE& var)\
        {					  \
            VARIATE = var;    \
        }

typedef struct MYDATA
{
        unsigned char* databuf;
	int datalen;
        MYDATA(unsigned char* pdata, int len)
	{
		databuf = pdata;
		datalen = len;
	}
	~MYDATA()
	{
		Release();
	}

	void Release()
	{
		SAFE_DELETE_ARRAY(databuf);
		datalen = 0;
	}

	STRUCT_CONSTRUCTOR(MYDATA)
}MYDATA;

typedef struct FILEVERINFO
{
    QString sFileDesc;      //文件描述
    QString sInternalName;  //内部名称
    QString sLegalCopyright;//合法版权
    QString sOriginName;    //原始文件名
    QString sProductName;   //产品名称，一般是重大升级或改进版本
    QString sProductVer;    //产品版本
    QString sCompanyName;   //公司名称
    QString sFileVer;       //文件版本，每次发布的版本
}FILEVERINFO;

class QUTILS_EXPORT CUtil
{
public:
	CUtil(void);
	virtual ~CUtil(void);

	/**************************************************************************
	* FunctionName:
	*     ReleaseVector
	* Description:
	*     销毁vector指针列表
	* Parameter:
	*     @[in out] vec: vector列表
	* ReturnValue:
	*     void
	*/
    template <typename T>
	static void ReleaseVector(vector<T>& vec);

	/**************************************************************************
	* FunctionName:
	*     ReleaseMap
	* Description:
	*     销毁map指针映射
	* Parameter:
	*     @[in out] mapobj: map映射
	* ReturnValue:
	*     void
	*/
    template <class K, class V>
	static void ReleaseMap(map<K, V>& mapobj);	
	
	/**************************************************************************
	* FunctionName:
	*     StrSplit
	* Description:
	*     字符串分割
	* Parameter:
	*     @[in ] source: 源字符串
	*     @[in ] ch: 分割字符
	* ReturnValue:
	*     返回分割后的字符串列表
	*/
    static vector<QString> StrSplit(const QString& source, char ch);

	/**************************************************************************
	* FunctionName:
	*     StrSplit
	* Description:
	*     字符串分割
	* Parameter:
	*     @[in ] source: 源字符串
	*     @[in ] frontch: 前分割字符
	*     @[in ] rearch: 后分割字符
	* ReturnValue:
	*     返回分割后的字符串列表
	*/
    static vector<QString> StrSplit(const QString& source, char frontch, char rearch);

	/**************************************************************************
	* FunctionName:
	*     StrSplit
	* Description:
	*     字符串分割
	* Parameter:
	*     @[in ] source: 源字符串
	*     @[in ] splitstr: 分割字符串
	* ReturnValue:
	*     返回分割后的字符串列表
	*/
    static vector<QString> StrSplit(const QString& source, const QString& splitstr);

	/**************************************************************************
	* FunctionName:
	*     StrSplit
	* Description:
	*     字符串分割
	* Parameter:
	*     @[in ] source: 源字符串
	*     @[in ] frontstr: 前分割字符串
	*     @[in ] rearstr: 后分割字符串
	* ReturnValue:
	*     返回分割后的字符串列表
	*/
    static vector<QString> StrSplit(const QString& source, const QString& frontstr, const QString& rearstr);

	/**************************************************************************
	* FunctionName:
	*     ReleaseSet
	* Description:
	*     销毁set指针列表
	* Parameter:
	*     @[in out] setobj: set列表
	* ReturnValue:
	*     void
	*/
    template <class T>
	static void ReleaseSet(set<T>& setobj);

	/**************************************************************************
	* FunctionName:
	*     ReleaseStack
	* Description:
	*     销毁stack指针列表
	* Parameter:
	*     @[in out] stackobj: stack列表
	* ReturnValue:
	*     void
	*/
    template <class T>
	static void ReleaseStack(stack<T>& stackobj);	

	/**************************************************************************
	* 函数名: VectorFind
	* 功能: 查找指定值
	* 时间: 2015/10/16 9:21
	* 作者: ZDJ
	*/
    template <class T>
    static bool VectorFind(const vector<T>& vecobj, const T& var);

    /**************************************************************************
    * 函数名: GetFileSize
    * 功能: 获取文件大小
    * 参数:
    *    @[in ] sFilePath: 文件路径
    * 返回值: 文件大小
    * 时间: 2016/01/04 09:47
    * 作者: ZDJ
    */
    static qint64 GetFileSize(const QString& sFilePath);

    /**************************************************************************
    * 函数名: GetFileExt
    * 功能: 获取文件后缀名
    * 参数:
    *    @[in ] sFilePath: 文件路径
    * 返回值: 文件后缀名
    * 时间: 2016/01/13 16:17
    * 作者: ZDJ
    */
    static QString GetFileExt(const QString& sFilePath);

    /**************************************************************************
    * 函数名: GetShortFileName
    * 功能: 获取短文件名，如a.exe
    * 参数:
    *    @[in ] sFilePath: 文件路径
    * 返回值: 短文件名
    * 时间: 2016/01/13 16:17
    * 作者: ZDJ
    */
    static QString GetShortFileName(const QString& sFilePath);

    /**************************************************************************
    * 函数名: GetQString
    * 功能: 获取字符串
    * 参数:
    *    @[in ] pStr: WCHAR字符串指针
    *    @[in ] nLen: 字符串字节数
    * 返回值: QString字符串
    * 时间: 2016/01/13 19:40
    * 作者: ZDJ
    */
    static QString GetQString(wchar_t* pStr, int nLen);

    /**************************************************************************
    * 函数名: GetWString
    * 功能: 获取宽字符串
    * 参数:
    *    @[in ] str: 字符串
    *    @[out] pWStr: WCHAR字符串指针
    * 返回值: 字符串字节数
    * 时间: 2016/01/13 19:40
    * 作者: ZDJ
    */
    static int GetWString(const QString& str, wchar_t* pWStr);

    /**************************************************************************
    * 函数名: ReplaceFileExt
    * 功能: 替换文件后缀名
    * 参数:
    *    @[in ] sSrcPath: 文件路径
    *    @[in ] sNewExt: 文件新后缀
    * 返回值: 新文件路径
    * 时间: 2015/09/18 20:29
    * 作者: ZDJ
    */
    static QString ReplaceFileExt(const QString& sSrcPath, const QString& sNewExt);

#ifdef USEVERSION
    /**************************************************************************
    * 函数名: GetFileVersion
    * 功能: 获取程序版本信息
    * 参数:
    *    @[in ] sAppPath：程序路径
    *    @[out] fileverinfo：程序版本信息
    * 返回值: 成功返回true，失败返回false
    * 时间: 2016/03/07 16:07
    * 作者: ZDJ
    */
    static bool GetFileVersion(const QString& sAppPath, FILEVERINFO& fileinfo);
#endif
    /**************************************************************************
    * 函数名: GetAppID
    * 功能: 获取程序ID号
    * 参数:
    *    @[in ] sAppPath：程序路径
    *    @[out] sAppID：程序ID
    * 返回值: 成功返回true，失败返回false
    * 时间: 2016/04/05 16:07
    * 作者: 周志平
    */
    static bool GetAppID(const QString& sAppPath, QString& sAppID);

#ifdef _WIN32
    /**************************************************************************
    * FunctionName:
    *     KillExe
    * Description:
    *     结束进程
    * Parameter:
    *     @[in ] sProcessName: 进程名称 如a.exe
    * ReturnValue:
    *     成功返回TRUE，失败返回FALSE
    */
    static BOOL KillExe(const QString &sProcessName);

    /**************************************************************************
    * FunctionName:
    *     FindProcessName
    * Description:
    *     查找进程名称
    * Parameter:
    *     @[in ] sProcessName: 进程路径 如a.exe
    *     @[out] pe: 进程信息
    * ReturnValue:
    *     成功返回TRUE，失败返回FALSE
    */
    static BOOL FindProcessName(const QString &sProcessName, PROCESSENTRY32& pe);

    /**************************************************************************
    * FunctionName:
    *     FindProcessPath
    * Description:
    *     查找进程路径
    * Parameter:
    *     @[in ] sProcessName: 进程路径 如c:\a.exe
    *     @[out] pe: 进程信息
    * ReturnValue:
    *     成功返回TRUE，失败返回FALSE
    */
    static BOOL FindProcessPath(const QString &sProcessPath, PROCESSENTRY32& pe);
#endif

    /**************************************************************************
    * FunctionName: listFilter
    * Description : get the filted list
    * Parameter:
    *     @[in] inStringList: the input list will be filted
    *     @[in] filterList: the filter list will be removed
    * ReturnValue: QStringList
    * date  : 20160601
    * author: suhaiwu
    */
    static QStringList listFilter(const QStringList& inStringList, const QStringList& filterList);

    /**************************************************************************
    * 函数名: GetFormatSizeString
    * 功能: 获取格式化后的大小字符串
    * 参数:
    *    @[in ] nSize: 大小
    * 返回值: 格式化后的大小字符串
    * 时间: 2015/12/25 17:38
    * 作者: ZDJ
    */
    static QString GetFormatSizeString(qint64 nSize);
};

/**************************************************************************
* FunctionName:
*     ReleaseVector
* Description:
*     销毁vector指针列表
* Parameter:
*     @[in out] vec: vector列表
* ReturnValue:
*     void
*/
template <typename T>
void CUtil::ReleaseVector(vector<T>& vec)
{
    typename vector<T>::iterator iter;
	for(iter=vec.begin();iter!=vec.end();)
	{
		delete *iter;
		iter = vec.erase(iter);
	}
	vector<T>().swap(vec);
}

/**************************************************************************
* FunctionName:
*     ReleaseMap
* Description:
*     销毁map指针映射
* Parameter:
*     @[in out] mapobj: map映射
* ReturnValue:
*     void
*/
template <class K, class V>
void CUtil::ReleaseMap(map<K, V>& mapobj)
{
    class map<K, V>::iterator iter;
	for(iter=mapobj.begin();iter!=mapobj.end();)
	{
		delete iter->second;
		iter = mapobj.erase(iter);
	}
	map<K, V>().swap(mapobj);
}

/**************************************************************************
* FunctionName:
*     ReleaseSet
* Description:
*     销毁set指针列表
* Parameter:
*     @[in out] setobj: set列表
* ReturnValue:
*     void
*/
template <class T>
void CUtil::ReleaseSet(set<T>& setobj)
{
    class set<T>::iterator iter;
	for(iter=setobj.begin();iter!=setobj.end();)
	{
		delete *iter;
		iter = setobj.erase(iter);
	}
	set<T>().swap(setobj);
}

/**************************************************************************
* FunctionName:
*     ReleaseStack
* Description:
*     销毁stack指针列表
* Parameter:
*     @[in out] stackobj: stack列表
* ReturnValue:
*     void
*/
template <class T>
void CUtil::ReleaseStack(stack<T>& stackobj)
{
	while(stackobj.size() > 0)
	{
		delete stackobj.top();
		stackobj.pop();
	}
	stack<T>().swap(stackobj);
}

/**************************************************************************
* 函数名: VectorFind
* 功能: 查找指定值
* 时间: 2015/10/16 9:21
* 作者: ZDJ
*/
template <class T>
bool CUtil::VectorFind(const vector<T>& vecobj, const T& var)
{
    class vector<T>::iterator itor = vecobj.begin();
    for(; itor != vecobj.end(); itor++)
	{
		if((*itor) == var)
			return true;
	}

	return false;
}

#endif
