/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
* 
* 文件名称：Global.cpp
* 创建时间：2015/05/18 8:51
* 文件标识：
* 文件摘要：检查更新导出函数定义
* 
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/05/18 8:51
* 版本摘要：
*/
#include "StdAfx.h"
#include "Global.h"
#include "MD5.h"
#include "alpharobotlog.h"

#include "ubxupdatemoduleconst.h"

#include <QFileInfo>


QtHttpOperSync g_httpoper;  //http文件操作对象

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
int CompareVersion(const QString &sLocalVer, const QString &sUpdateVer, emCheckUpdateType chktype)
{
    if(chktype == eCheckUpdateTypeWindows || chktype == eCheckUpdateTypeMac)
	{
        vector<QString> vecLocalVerNumList = CUtil::StrSplit(sLocalVer, '.');
        vector<QString> vecUpgradeVerNumList = CUtil::StrSplit(sUpdateVer, '.');

		for(int i = 0; i < vecLocalVerNumList.size(); i++)
		{
            int nLocalVerNum = vecLocalVerNumList[i].toInt();
            int nUpgradeVerNum = vecUpgradeVerNumList[i].toInt();
			if(nUpgradeVerNum > nLocalVerNum)
				return 1;
			else if(nUpgradeVerNum < nLocalVerNum)
			{
				return -1;
			}		
		}

		return 0;
	}
	else
	{
        int nLocalVerNum = sLocalVer.toInt();
        int nUpgradeVerNum = sUpdateVer.toInt();

        if(nUpgradeVerNum > nLocalVerNum)
        {
            return 1;
        }
		else if(nUpgradeVerNum < nLocalVerNum)
		{
			return -1;
		}

		return 0;
	}	
}

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
int CheckUpdate(const QString &sUpdateURL, FILEINFO& fileinfo, const QString &sLocalVer, emCheckUpdateType chktype)
{	
    //TODO:周志平 二代的更新模块要重新做
//	byte* pOutData = NULL;
//    qint64 nOutLen = 0;
//    char sErr[255] = {0};
//	char* pszBuf = NULL;

//	int nRet = CHECKRESULT_NOUPDATE;
//	do
//	{
//		char szParam[512] = {0};
//        int nProd = eUbtProduct1S;
//		int nProdType  = -1;
//        int nVerType = -1;

//        if(chktype == eCheckUpdateTypeWindows)
//		{
//            nProdType = eProduct1SVersionOld;
//            nVerType = eProductOnSystemWindows;
//		}

//        else if(chktype == eCheckUpdateTypeOldRobot1S)
//        {
//            nProdType = eProduct1SVersionOld;
//            nVerType = eProductOnSystemEmbedded;
//        }

//        else if(chktype == eCheckUpdateTypeNewRobot1S)
//		{
//            nProdType = eProduct1SVersionNew;
//            nVerType = eProductOnSystemEmbedded;
//		}
//        else if(chktype == eCheckUpdateTypeRobot1P)
//		{
//            nProdType = eProduct1SVersion1P;
//            nVerType = eProductOnSystemEmbedded;
//		}

//        // 1SV2/1PV2/1SP/1PPlus都使用eProduct1SVersion1SV2作为请求
//        else if(chktype == eCheckUpdateTypeNewRobot1SV2 ||chktype == eCheckUpdateTypeRobot1PV2 ||
//                chktype == eCheckUpdateTypeRobot1SP || chktype == eCheckUpdateTypeRobot1PPlus)
//		{
//            nProdType = eProduct1SVersion1S1P;
//            nVerType = eProductOnSystemEmbedded;
//		}
//        else if(chktype == eCheckUpdateTypeMac)
//        {
//            nProdType = eProduct1SVersionOld;
//            nVerType = eProductOnSystemMac;
//        }
//		else
//		{
//			nRet = CHECKRESULT_ERROR;
//			break;
//		}

//        // 基本参数
//        QString sBasicParam = GetReqBasicParam();
//        sprintf(szParam, "{%s,\"versionProd\":\"%d\",\"versionProdType\":\"%d\",\"versionSystemType\":\"%d\",\"versionName\":\"V%s\"}", sBasicParam.toStdString().c_str(),
//                nProd,
//                nProdType,
//                nVerType,
//                sLocalVer.toStdString().c_str());

//        ///////////////////////////Log////////////////////////////////////////////////////////////
//        QString strLog = QString("[Req Version] URL->%1,  parameter->%2").arg(sUpdateURL).arg(szParam);
//        AlphaRobotLog::getInstance()->printDebug(strLog);
//        //////////////////////////////////////////////////////////////////////////////////////////////

//        if(!g_httpoper.ExecuteRequest(eRequestPost, sUpdateURL, szParam, NULL, NULL, pOutData, nOutLen, sErr))
//		{
//			nRet = CHECKRESULT_ERROR;
//            AlphaRobotLog::getInstance()->printError(QString(sErr));
//			break;
//		}

//		pszBuf = new char[nOutLen + 1];
//		memset(pszBuf, 0, nOutLen + 1);
//		memcpy(pszBuf, pOutData, nOutLen);

//        ///////////////////////////Log////////////////////////////////////////////////////////////
//        strLog = QString("received version answer: %1").arg(QString(pszBuf));
//        AlphaRobotLog::getInstance()->printDebug(strLog);
//        //////////////////////////////////////////////////////////////////////////////////////////////
//        if (parserUpdateVersionInfo(pszBuf, nOutLen, fileinfo))
//        {
//            QString sUpdateVer = fileinfo.m_sVersion;
//            sUpdateVer = sUpdateVer.toUpper();
//            if(sUpdateVer.indexOf('V', 0, Qt::CaseInsensitive) == 0)
//            {
//                sUpdateVer = sUpdateVer.remove(0, 1);
//            }

//            int nResult = CompareVersion(sLocalVer, sUpdateVer, chktype);
//            if(nResult > 0)
//            {
//                nRet = CHECKRESULT_NEEDUPDATE;
//            }
//            else
//            {
//                nRet = CHECKRESULT_NOUPDATE;
//            }
//            break;
//        }
//        else
//        {
//            nRet = CHECKRESULT_ERROR;
//            break;
//        }

////        Json::Reader jsonreader;
////		Json::Value  jvalue_root;
////		if(jsonreader.parse(pszBuf, jvalue_root))
////		{
////			if(jvalue_root.size() > 0)
////			{
////                if(strcmp(jvalue_root["info"].asCString(), "9999") == 0)
////                {
////					int nIndex = 0;
////					strcpy(fileinfo.version, jvalue_root["models"][nIndex]["versionName"].asCString());
////					//strcpy(fileinfo.date, jvalue_root["models"][nIndex]["versionUpdateTime"].asCString());
////					strcpy(fileinfo.url, jvalue_root["models"][nIndex]["versionPath"].asCString());
////                   // strcpy(fileinfo.description, jvalue_root["models"][nIndex]["versionResume"].asCString());

////					string stemp = fileinfo.url;
////					strcpy(fileinfo.name, stemp.substr(stemp.rfind('/')+1).c_str());

////                    QString sUpdateVer = QString::fromStdString(fileinfo.version);
////                    sUpdateVer = sUpdateVer.toUpper();
////                    if(sUpdateVer.indexOf('V', 0, Qt::CaseInsensitive) == 0)
////                    {
////                        sUpdateVer = sUpdateVer.remove(0, 1);
////                    }

////					int nResult = CompareVersion(sLocalVer, sUpdateVer, chktype);

////					//test
////					//nResult = 1;
////					//////

////					if(nResult > 0)
////						nRet = CHECKRESULT_NEEDUPDATE;
////					break;
////				}
////			}
////			else
////			{
////				nRet = CHECKRESULT_ERROR;
////				break;
////			}
////		}
////		else
////		{
////			nRet = CHECKRESULT_ERROR;
////			break;
////		}
//	} while (0);

//	SAFE_FREE(pOutData);
//	SAFE_FREE(pszBuf);
//	return nRet;
    return 0;
}

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
BOOL DownloadFile(
    const QString& sFileURL,
    const QString& sSavePath,
	FILEDOWNPROCESSNOTIFY procnotify/* = NULL*/, 
    LPVOID hUser /*= NULL*/)
{	
	byte* pFileData = NULL;
    qint64 nFileLen = 0;
    char sErr[255] = {0};

	BOOL bRet = FALSE;
	do 
	{
		//下载新版本										
        if(!g_httpoper.ExecuteRequest(eRequestGet, sFileURL, NULL, procnotify, hUser, pFileData, nFileLen, sErr))
		{								
			break;
		}

#ifdef Q_OS_WIN
		//终止已经打开的安装进程
		PROCESSENTRY32 pe;		
        if(CUtil::FindProcessPath(sSavePath.toStdString().c_str(), pe))
		{
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
			if(hProcess)
				TerminateProcess(hProcess, 0);
		}
        QString sTmpFilePath = sSavePath.left(sSavePath.length()-4) + ".tmp";
        sTmpFilePath = sTmpFilePath.mid(sTmpFilePath.lastIndexOf('\\') + 1);

        if(CUtil::FindProcessName(sTmpFilePath.toStdString().c_str(), pe))
		{
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
			if(hProcess)
				TerminateProcess(hProcess, 0);
		}

#else
        QString sProcName = "killall";
        QString sAppName = sSavePath.mid(sSavePath.lastIndexOf(QDir::separator()) + 1);
        sAppName = sSavePath.left(sSavePath.lastIndexOf("."));

        QStringList lstParams;
        lstParams << sAppName;
        QProcess process;
        process.start(sProcName, lstParams);
        process.waitForFinished();
#endif

        QFile file(sSavePath);
        if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		{
            file.write((char*)pFileData, nFileLen);
            file.close();
			bRet = TRUE;
        }

	} while (0);

	SAFE_FREE(pFileData);

	return bRet;
}

/**************************************************************************
* 函数名: CancelDownload
* 功能: 取消下载
* 时间: 2015/06/12 10:22
* 作者: ZDJ
*/
void CancelDownload()
{
	g_httpoper.Cancel();
}

/**************************************************************************
* 函数名: GetReqBasicParam
* 功能: 获取请求基本参数
* 时间: 2016/07/26 10:22
* 作者: hwx
*/
QString GetReqBasicParam()
{
    QString sParam;
    QString sAppPath = QCoreApplication::applicationFilePath();

    FILEVERINFO fileinfo;
    CUtil::GetFileVersion(sAppPath, fileinfo);

    int nAppType = ALPHA_APP_TYPE;   //产品种类，1为1s，2为2代
    QString sServiceVersion = fileinfo.sFileVer;  //版本号
    const string szEncKey = "UBTech832%1293*6";

    QDateTime curtime = QDateTime::currentDateTime();
    QString sRequestTime = curtime.toString("yyyy-MM-dd hh:mm:ss");//请求时间

    string szTempKey = sRequestTime.toStdString() + szEncKey; //时间+密钥

    CMD5 md5obj;
    md5obj.GenerateMD5((byte*)szTempKey.c_str(), szTempKey.length());

    char* pszRequestKey = md5obj.ToString();

    sParam.sprintf("\"appType\":\"%d\",\"serviceVersion\":\"%s\",\"requestKey\":\"%s\",\"requestTime\":\"%s\"", nAppType, sServiceVersion.toStdString().c_str(), pszRequestKey, sRequestTime.toStdString().c_str());
    return sParam;
}

/**************************************************************************
* 函数名:parserUpdateVersionInfo
* 功能: 解析json返回的版本信息
* 	@[in ] pJsonInfo: json格式内容
* 	@[in ] nSize: 缓冲区大小
* 	@[out ] fileinfo: 更新文件信息
* 时间: 2016/07/26 10:22
* 作者: hwx
*/
bool parserUpdateVersionInfo(const char *pJsonInfo, int nSize, FILEINFO& fileinfo)
{
    QJsonParseError jsonError;
    QJsonDocument parseDocument = QJsonDocument::fromJson(pJsonInfo, & jsonError);
    bool bRet = false;
    if(QJsonParseError::NoError != jsonError.error)
    {
        AlphaRobotLog::getInstance()->printError("parserUpdateVersionInfo: Jason parser error");
        return bRet;
    }

    if(!parseDocument.isObject())
    {
        return bRet;
    }

    QJsonObject objRoot = parseDocument.object();
    if (!objRoot.contains(UpdateModuleConstant::NET_KEYWORD_VERSION_INFO))
    {
        AlphaRobotLog::getInstance()->printError("Parse json format error, no 'info' keyword");
        return bRet;
    }

    // 返回是否正确
    QString sRstInfo = objRoot.value(UpdateModuleConstant::NET_KEYWORD_VERSION_INFO).toString();
    if (sRstInfo.compare("0000") != 0)
    {
        AlphaRobotLog::getInstance()->printError("parserUpdateVersionInfo: Error result");
        return bRet;
    }

    // 获取models字段
    if (!objRoot.contains(NET_MODELS))
    {
        AlphaRobotLog::getInstance()->printError("Parse json format error, no 'models keyword");
        return bRet;
    }

    // models下是数组格式
    QJsonValue valueModels = objRoot.value(NET_MODELS);
    if(!valueModels.isArray())
    {
        AlphaRobotLog::getInstance()->printError("Parse json format error");
        return bRet;
    }

    QString sUserLanguage = "";
    if (LANGUAGE_CHINESE_POSTFIX ==  getUserLanguage())
    {
        sUserLanguage  = "CN";
    }
    else
    {
        sUserLanguage = "EN";
    }

    QJsonArray arrayModels = valueModels.toArray();
    for (int modelsId = 0; modelsId < arrayModels.size(); modelsId++)
    {
        QJsonValue modelItem = arrayModels.at(modelsId);
        if (!modelItem.isObject())
        {
            AlphaRobotLog::getInstance()->printError("Parse json format error");
            return bRet;
        }

        QJsonObject objModel = modelItem.toObject();

        // 版本名称
        if (!objModel.contains(UpdateModuleConstant::NET_KEYWORD_VERSION_NAME))
        {
            AlphaRobotLog::getInstance()->printError(QString("The version request result doesn't contain version name"));
            return bRet;
        }
        QString sVersionName  = objModel.value(UpdateModuleConstant::NET_KEYWORD_VERSION_NAME).toString();

        // 版本URL
        if (!objModel.contains(UpdateModuleConstant::NET_KEYWORD_VERSION_PATH))
        {
            AlphaRobotLog::getInstance()->printError(QString("The version request result doesn't contain update version url"));
            return bRet;
        }
        QString sVersionPath = objModel.value(UpdateModuleConstant::NET_KEYWORD_VERSION_PATH).toString();

        // 版本更新时间
        if (!objModel.contains(UpdateModuleConstant::NET_KEYWORD_VERSION_UPDATE_TIME))
        {
            AlphaRobotLog::getInstance()->printError(QString("The version request result doesn't contain version update time"));
            return bRet;
        }
        QString sVersionUpdateTime = QString("%1").arg(objModel.value(UpdateModuleConstant::NET_KEYWORD_VERSION_UPDATE_TIME).toInt());

        // 版本大小
        if (!objModel.contains(UpdateModuleConstant::NET_KEYWORD_VERSION_SIZE))
        {
            AlphaRobotLog::getInstance()->printError(QString("The version request result doesn't contain version update size"));
            return bRet;
        }
        QJsonValue valueSize = objModel.value(UpdateModuleConstant::NET_KEYWORD_VERSION_SIZE);
        int nSize = valueSize.toInt();

        // 版本描述信息
        QStringList lstVersionDes;
        if (!objModel.contains(UpdateModuleConstant::NET_KEYWORD_VERSION_RESUME))
        {
            AlphaRobotLog::getInstance()->printError(QString("The version request result doesn't contain version resume"));
            return bRet;
        }

        // 返回的语言格式是字符串类型，需要再次按json格式解析
        QJsonValue valueAllLanguageDes = objModel.value(UpdateModuleConstant::NET_KEYWORD_VERSION_RESUME);
        if (valueAllLanguageDes.isString())
        {
            QString sAllLanguageDes = valueAllLanguageDes.toString();
            if (sAllLanguageDes.isEmpty())
            {
                continue;
            }

            QJsonDocument docLanguageDes = QJsonDocument::fromJson(sAllLanguageDes.toStdString().c_str(), &jsonError);
            if(QJsonParseError::NoError != jsonError.error)
            {
                AlphaRobotLog::getInstance()->printError("parserUpdateVersionInfo: Jason parser language error");
                return bRet;
            }


            if (!docLanguageDes.isObject())
            {
                AlphaRobotLog::getInstance()->printError(QString("The version request result lanugage is not object"));
                return bRet;
            }

            QJsonObject objCountryLanguage = docLanguageDes.object();
            if (!objCountryLanguage.contains(sUserLanguage))
            {
                AlphaRobotLog::getInstance()->printError(QString("The version request result doesn't containt language keyword: ") + sUserLanguage);
                return bRet;
            }

            // 获取指定语言的版本描述
            QJsonValue valueDesItems = objCountryLanguage.value(sUserLanguage);
            if (!valueDesItems.isArray())
            {
                AlphaRobotLog::getInstance()->printError(QString("The version request result description is not array format "));
                return bRet;
            }

            QJsonArray arrayDesItems = valueDesItems.toArray();
            for (int desId = 0; desId < arrayDesItems.size(); desId++)
            {
                QJsonValue valueDes = arrayDesItems.at(desId);
                if (!valueDes.isObject())
                {
                    continue;
                }

                QJsonObject objDesItem = valueDes.toObject();
                QString sDesItem = objDesItem.value(UpdateModuleConstant::NET_KEYWORD_VERSION_DES_NAME).toString();
                if (sDesItem.isEmpty())
                {
                    continue;
                }
                lstVersionDes.append(sDesItem);
            }
            //continue;
        }  // end if string
        else
        {
            AlphaRobotLog::getInstance()->printError(QString("The version request result update description is not in string format"));
        }

        // 从URL中获取文件名称，如xx.exe
        QFileInfo o_file_info(sVersionPath);
        QString sFileName = o_file_info.fileName();

        fileinfo.m_sVersion = sVersionName;
        fileinfo.m_sUpdateDateTime = sVersionUpdateTime;
        fileinfo.m_sUrl = sVersionPath;
        fileinfo.m_slstDescriptions = lstVersionDes;
        fileinfo.m_sName = sFileName;
        fileinfo.m_nSize = nSize;

        bRet = true;
     }

    return bRet;
}
