/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
* 
* 文件名称：Global.h
* 创建时间：2015/05/18 8:52
* 文件标识：
* 文件摘要：检查更新导出函数定义
* 
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/05/18 8:52
* 版本摘要：
*/
#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QStringList>
#include "robotdef.h"

#define CHECKRESULT_NOUPDATE   0 //无需更新
#define CHECKRESULT_NEEDUPDATE 1 //需要更新
#define CHECKRESULT_ERROR      2 //更新出错

#define STR_LEN 64
#define URL_LEN 1024
#define DESCRIPTION_LEN 1024 * 3


const QString UPDATE_MODULE_DEFAULT_QSS = ":/res/qss/UBXUpdateModuleDefault.qss";


typedef struct _FILEINFO
{
//	char name[STR_LEN];   //名称，如test.exe
//	char version[STR_LEN];//版本，如1.2.0.0
//	int  size;            //大小，单位字节
//	char date[STR_LEN];   //日期
//	char url[URL_LEN];    //下载地址，如http://www.abc.com/test.exe
//    char description[DESCRIPTION_LEN]; // 版本描述信息

    // Modify by huangwuxian 2016.7.27
    QString m_sName;  // 名称，如test.exe
    QString m_sVersion; //版本，如1.2.0.0
    int m_nSize;  //大小，单位字节
    QString m_sUpdateDateTime;   //日期
    QString m_sUrl;    //下载地址，如http://www.abc.com/test.exe
    QStringList m_slstDescriptions;  // 版本描述信息

	_FILEINFO()
	{
        m_sName = "";
        m_sVersion = "";
        m_nSize = 0;
        m_sUpdateDateTime = "";
        m_sUrl = "";
        m_slstDescriptions.clear();

//		memset(this, 0, sizeof(_FILEINFO));
	}
	_FILEINFO& operator = (const _FILEINFO& fi)
	{
        this->m_sName = fi.m_sName;
        this->m_sVersion = fi.m_sVersion;
        this->m_nSize = fi.m_nSize;
        this->m_sUpdateDateTime = fi.m_sUpdateDateTime;
        for (int num = 0; num < fi.m_slstDescriptions.size(); num++)
        {
            this->m_slstDescriptions.append(fi.m_slstDescriptions[num]);
        }
//		memcpy(this, &fi, sizeof(_FILEINFO));
		return *this;
	}

    void reset()
    {
        m_sName = "";
        m_sVersion = "";
        m_nSize = 0;
        m_sUpdateDateTime = "";
        m_sUrl = "";
        m_slstDescriptions.clear();
    }
}FILEINFO;

/**************************************************************************
* 函数名: CompareVersion
* 功能: 比较版本
* 参数: 
*    @[in ] sLocalVer: 本地版本，版本号如1.0.0.1
*    @[in ] sUpdateVer: 更新版本，版本号如1.0.0.1
*    @[in ] chktype: 更新类型  
* 返回值: 更新版本较新返回1，更新版本较旧返回-1，相等返回0
* 时间: 2015/09/18 9:49
* 作者: ZDJ
*/
int CompareVersion(const QString& sLocalVer, const QString& sUpdateVer, emCheckUpdateType chktype);

/**************************************************************************
* 函数名: CheckUpdate
* 功能: 检查更新
* 参数: 
* 	@[in ] sUpdateURL: 版本检测文件网络地址，如http://www.test.com/version.xml
* 	@[out] fileinfo: 需要更新的文件信息
* 	@[in ] sLocalVer: 本地机器人版本
* 	@[in ] chktype: 机器人版本类型
* 返回值: 检测到新版本返回1， 未检测到新版本返回0，出错返回2
* 时间: 2015/05/15 9:32
* 作者: ZDJ
*/
int CheckUpdate(const QString& sUpdateURL, FILEINFO& fileinfo, const QString& sLocalVer, emCheckUpdateType chktype);

/**************************************************************************
* 函数名: DownloadFile
* 功能: 下载文件
* 参数: 
* 	@[in ] sFileURL: 文件的网络地址，地址如http://www.test.com/test.exe
* 	@[in ] sSavePath: 文件的保存路径
* 	@[in ] procnotify: 通知回调
* 	@[in ] hUser: 通知目标
* 返回值: 成功返回TRUE，失败返回FALSE
* 时间: 2015/05/15 9:32
* 作者: ZDJ
*/
//BOOL DownloadFile(
//    const QString& sFileURL,
//    const QString& sSavePath,
//	FILEDOWNPROCESSNOTIFY procnotify = NULL, 
//    LPVOID hUser = NULL);

/**************************************************************************
* 函数名: CancelDownload
* 功能: 取消下载
* 时间: 2015/06/12 10:22
* 作者: ZDJ
*/
void CancelDownload();

/**************************************************************************
* 函数名: GetReqBasicParam
* 功能: 获取请求基本参数
* 时间: 2016/07/26 10:22
* 作者: hwx
*/
QString GetReqBasicParam();

/**************************************************************************
* 函数名:parserUpdateVersionInfo
* 功能: 解析json返回的版本信息
* 	@[in ] pJsonInfo: json格式内容
* 	@[in ] nSize: 缓冲区大小
* 	@[out ] fileinfo: 更新文件信息
* 时间: 2016/07/26 10:22
* 作者: hwx
*/
bool parserUpdateVersionInfo(const char *pJsonInfo, int nSize, FILEINFO& fileinfo);

#endif
