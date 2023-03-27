/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
* 
* 文件名称：Util.cpp
* 创建时间：2015/05/14 17:25
* 文件标识：
* 文件摘要：辅助函数类。
* 
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/05/14 17:25
* 版本摘要：
*/
#include "util.h"

#include <QTextStream>
#include <QTextCodec>
#include <QDomDocument>
#include <QDir>
#include "type.h"
#include "fileutils.h"

CUtil::CUtil(void)
{
}

CUtil::~CUtil(void)
{
}

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
vector<QString> CUtil::StrSplit(const QString &source, char ch)
{
    vector<QString> strlist;
	int m=0,n=0;
    for(int i=0;i<source.length();)
	{
		if(source[i]==ch)
		{
			n=i-1;
            QString str = source.mid(m,n-m+1);
			strlist.push_back(str);

			m=n+2;
			n=m;
			i=m;
		}
		else
			i++;
	}

	//以分割符结尾的时候  m的值最终等于source的长度
	//反之  m的值小于source的长度
    if(m>0&&m<source.length())
	{
        strlist.push_back(source.mid(m));
	}
	return strlist;
}

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
vector<QString> CUtil::StrSplit(const QString &source, char frontch, char rearch)
{
    vector<QString> strlist;
	int m=0,n=0;

    for(int i=0;i<source.length();)
	{
		BOOL bfindrear = FALSE;
		if(source[i]==frontch)
		{
			m=i+1;

            for(int j=m;j<source.length();j++)
			{
				if(source[j]==rearch)
				{
					bfindrear = TRUE;

					n=j-1;

                    QString str = source.mid(m,n-m+1);
					strlist.push_back(str);

					i=j+1;//跳过rear
					m=n=i;
					break;
				}
			}			
		}
		if(!bfindrear)
			i++;
	}

	//以rear结尾的时候  m的值最终等于source的长度
	//反之  m的值小于source的长度
    if(m>0&&m<source.length())
	{
        strlist.push_back(source.mid(m));
	}
	return strlist;
}

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
vector<QString> CUtil::StrSplit(const QString &source, const QString &splitstr)
{
    vector<QString> strlist;
	int index1 = 0;	
    while(index1 < source.length())
	{
        int index2 = source.indexOf(splitstr, index1);
		if(index2 != -1)
		{
            strlist.push_back(source.mid(index1, index2-index1));
            index1 = index2 + splitstr.length();
		}
		else
		{
            strlist.push_back(source.mid(index1));
            index1 = source.length();
		}
	}
	return strlist;
}

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
vector<QString> CUtil::StrSplit(const QString &source, const QString &frontstr, const QString &rearstr)
{
    vector<QString> strlist;
	int index1 = 0;
    while(index1 < source.length())
	{
        int index2 = source.indexOf(frontstr, index1);
        int index3 = source.indexOf(rearstr, index1);
		if(index2 != -1)
		{
			if(index3 != -1)
			{
                strlist.push_back(source.mid(index2+frontstr.length(), index3-index2-frontstr.length()));
                index1 = index3 + rearstr.length();
			}
			else
			{
                index1 = source.length();
			}			
		}
		else
		{
            index1 = source.length();
		}
	}
	return strlist;
}

/**************************************************************************
* 函数名: GetFileSize
* 功能: 获取文件大小
* 参数:
*    @[in ] sFilePath: 文件路径
* 返回值: 文件大小
* 时间: 2016/01/04 09:47
* 作者: ZDJ
*/
qint64 CUtil::GetFileSize(const QString& sFilePath)
{    
    qint64 nSize = 0;
    QFile file(sFilePath);
    if(file.open(QIODevice::ReadOnly))
    {
        nSize = file.size();
        file.close();
    }
    return nSize;
}

/**************************************************************************
* 函数名: GetFileExt
* 功能: 获取文件后缀名
* 参数:
*    @[in ] sFilePath: 文件路径
* 返回值: 文件后缀名
* 时间: 2016/01/13 16:17
* 作者: ZDJ
*/
QString CUtil::GetFileExt(const QString& sFilePath)
{
    return sFilePath.mid(sFilePath.lastIndexOf('.'));
}

/**************************************************************************
* 函数名: GetShortFileName
* 功能: 获取短文件名，如a.exe
* 参数:
*    @[in ] sFilePath: 文件路径
* 返回值: 短文件名
* 时间: 2016/01/13 16:17
* 作者: ZDJ
*/
QString CUtil::GetShortFileName(const QString& sFilePath)
{
    QString qsFilePath = QDir::toNativeSeparators(sFilePath);
    return sFilePath.mid(qsFilePath.lastIndexOf(QDir::separator()) + 1);
}

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
QString CUtil::GetQString(wchar_t* pStr, int nLen)
{
    int nAnsiLen = nLen / sizeof(WCHAR);
    char* pAnsiStr = new char[nAnsiLen + 1];
    memset(pAnsiStr, 0, nAnsiLen + 1);
    for(int i = 0; i < nAnsiLen; i++)
    {
        pAnsiStr[i] = *((char*)pStr + i * sizeof(WCHAR));
    }
    QString sTemp = QString::fromStdString(pAnsiStr);
    SAFE_DELETE_ARRAY(pAnsiStr);
    return sTemp;
}

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
int CUtil::GetWString(const QString& str, wchar_t* pWStr)
{
    string sAnsi = str.toStdString();
    const char* pAnsiStr = sAnsi.c_str();
    int nAnsiLen = sAnsi.length();
    int nOutLen = (nAnsiLen + 1) * sizeof(WCHAR);

    memset(pWStr, 0, nOutLen);
    for(int i = 0; i < nAnsiLen; i++)
    {
        memcpy(pWStr + i, pAnsiStr + i, 1);
    }
    return nOutLen;
}

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
QString CUtil::ReplaceFileExt(const QString& sSrcPath, const QString& sNewExt)
{
    QString sRet;
    int nDotIndex = sSrcPath.lastIndexOf('.');
    if(sNewExt.length() > 0 )
    {
        if(sNewExt[0] == '.')
        {
            sRet = sSrcPath.left(nDotIndex) + sNewExt;
        }
        else
        {
            sRet = sSrcPath.left(nDotIndex + 1) + sNewExt;
        }
    }
    else
    {
        sRet = sSrcPath;
    }

    return sRet;
}

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
bool CUtil::GetFileVersion(const QString& sAppPath, FILEVERINFO& fileverinfo)
{
    bool bRet = false;

#ifdef _WIN32
    LPVOID pBlock = NULL;

    enum VersionKeyEnum
    {
        eVersionKeyEnumComments = 0,
        eVersionKeyEnumInternalName,     //内部名称
        eVersionKeyEnumProductName,      //产品名称
        eVersionKeyEnumCompanyName,      //公司名称
        eVersionKeyEnumLegalCopyright,   //合法版权
        eVersionKeyEnumProductVersion,   //产品版本
        eVersionKeyEnumFileDescription,  //文件描述
        eVersionKeyEnumLegalTrademarks,  //合法商标
        eVersionKeyEnumPrivateBuild,
        eVersionKeyEnumFileVersion,      //文件版本
        eVersionKeyEnumOriginalFilename, //原始文件名
        eVersionKeyEnumSpecialBuild,
    };

    QStringList sVersionKeyArr;
    sVersionKeyArr.append("Comments");
    sVersionKeyArr.append("InternalName");
    sVersionKeyArr.append("ProductName");
    sVersionKeyArr.append("CompanyName");
    sVersionKeyArr.append("LegalCopyright");
    sVersionKeyArr.append("ProductVersion");
    sVersionKeyArr.append("FileDescription");
    sVersionKeyArr.append("LegalTrademarks");
    sVersionKeyArr.append("PrivateBuild");
    sVersionKeyArr.append("FileVersion");
    sVersionKeyArr.append("OriginalFilename");
    sVersionKeyArr.append("SpecialBuild");

    do
    {
        std::wstring wsFilePath = sAppPath.toStdWString();
        DWORD dwSize = GetFileVersionInfoSize(wsFilePath.c_str(), NULL);
        if(dwSize == 0)
            break;

        pBlock = malloc(dwSize);
        if(!GetFileVersionInfo(wsFilePath.c_str(), 0, dwSize, pBlock))
            break;

        char* pVerValue = NULL;
        UINT nSize = 0;
        if(!VerQueryValue(pBlock, QString("\\VarFileInfo\\Translation").toStdWString().c_str(), (LPVOID*)&pVerValue, &nSize))
            break;

        QString strTranslation;
        strTranslation.sprintf("%04x%04x", *((unsigned short int*)pVerValue), *((unsigned short int*)&pVerValue[2]));//080404b0为中文，040904E4为英文

        for(int i=0; i<sVersionKeyArr.size(); i++)
        {
            QString strSubBlock;
            strSubBlock.sprintf("\\StringFileInfo\\%s\\%s", strTranslation.toStdString().c_str(), sVersionKeyArr.at(i).toStdString().c_str());
            if(!VerQueryValue(pBlock, strSubBlock.toStdWString().c_str(), (LPVOID*)&pVerValue, &nSize))
                continue;

            QString sTemp = QString::fromWCharArray((WCHAR*)pVerValue);
            //lstrcpy(sTemp.GetBufferSetLength(nSize), (WCHAR*)pVerValue);
            //sTemp.ReleaseBuffer();

            switch(i)
            {
            case eVersionKeyEnumFileDescription:
                fileverinfo.sFileDesc = sTemp;
                break;
            case eVersionKeyEnumInternalName:
                fileverinfo.sInternalName = sTemp;
                break;
            case eVersionKeyEnumLegalCopyright:
                fileverinfo.sLegalCopyright = sTemp;
                break;
            case eVersionKeyEnumOriginalFilename:
                fileverinfo.sOriginName = sTemp;
                break;
            case eVersionKeyEnumProductName:
                fileverinfo.sProductName = sTemp;
                break;
            case eVersionKeyEnumProductVersion:
                fileverinfo.sProductVer = sTemp;
                break;
            case eVersionKeyEnumCompanyName:
                fileverinfo.sCompanyName = sTemp;
                break;
            case eVersionKeyEnumFileVersion:
                fileverinfo.sFileVer = sTemp;
                break;
            //...添加其他属性信息

            }
        }

        bRet = TRUE;
    } while (0);

    if(pBlock)
        free(pBlock);
#else
    do
    {
        QString sInfoPath = sAppPath;
        sInfoPath = sInfoPath.left(sInfoPath.lastIndexOf('/')) + "/../Info.plist";
//        if (!PathFileExists(sInfoPath))
//        {
//            break;
//        }
        if (!FileUtils::exists(sInfoPath)) {
            break;
        }
        QFile xmlFile(sInfoPath);
        if (!xmlFile.open(QIODevice::ReadOnly))
        {
            break;
        }

        QTextStream readStream(&xmlFile);
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        readStream.setCodec(codec);
        QString sXmlDataStr = readStream.readAll();
        xmlFile.close();

        QDomDocument xmlDoc;
        if (!xmlDoc.setContent(sXmlDataStr))
        {
            break;
        }
        QDomElement xmlRoot = xmlDoc.documentElement();
        if (QString::compare(xmlRoot.tagName(), "plist", Qt::CaseInsensitive) != 0)
        {
            break;
        }

        QDomNode dictNode = xmlRoot.namedItem("dict");
        if (dictNode.isNull())
        {
            break;
        }
        QDomElement dictElement = dictNode.toElement();

        QDomNode childNode = dictElement.firstChild();
        while(!childNode.isNull())
        {
            //QDomElement childElement = childNode.toElement();
            QString sChildTagName = childNode.toElement().tagName();
            QString sChildText = childNode.toElement().text();

            if(QString::compare(sChildTagName, "key", Qt::CaseInsensitive) == 0)
            {
                if(QString::compare(sChildText, "CFBundleVersion", Qt::CaseInsensitive) == 0)
                {
                    childNode = childNode.nextSibling();
                    fileverinfo.sFileVer = childNode.toElement().text();
                }
                else if(QString::compare(sChildText, "CFBundleGetInfoString", Qt::CaseInsensitive) == 0)
                {
                    childNode = childNode.nextSibling();
                    fileverinfo.sLegalCopyright = childNode.toElement().text();
                }
            }

            childNode = childNode.nextSibling();
        }

        bRet = true;

    }while(0);
#endif
    return bRet;
}

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
bool CUtil::GetAppID(const QString& sAppPath,QString& sAppID)
{
    bool bRet = false;
    do
    {
        QString sInfoPath = sAppPath;
        sInfoPath = sInfoPath.left(sInfoPath.lastIndexOf('/')) + "/../Info.plist";
//        if (!PathFileExists((LPCWSTR)sInfoPath.utf16()))
//        {
//            break;
//        }
        if (!FileUtils::exists(sInfoPath))
        {
            break;
        }

        QFile xmlFile(sInfoPath);
        if (!xmlFile.open(QIODevice::ReadOnly))
        {
            break;
        }

        QTextStream readStream(&xmlFile);
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        readStream.setCodec(codec);
        QString sXmlDataStr = readStream.readAll();
        xmlFile.close();

        QDomDocument xmlDoc;
        if (!xmlDoc.setContent(sXmlDataStr))
        {
            break;
        }
        QDomElement xmlRoot = xmlDoc.documentElement();
        if (QString::compare(xmlRoot.tagName(), "plist", Qt::CaseInsensitive) != 0)
        {
            break;
        }

        QDomNode dictNode = xmlRoot.namedItem("dict");
        if (dictNode.isNull())
        {
            break;
        }
        QDomElement dictElement = dictNode.toElement();

        QDomNode childNode = dictElement.firstChild();
        while(!childNode.isNull())
        {
            QString sChildTagName = childNode.toElement().tagName();
            QString sChildText = childNode.toElement().text();

            if(QString::compare(sChildTagName, "key", Qt::CaseInsensitive) == 0)
            {
                if(QString::compare(sChildText, "CFBundleIdentifier", Qt::CaseInsensitive) == 0)
                {
                    childNode = childNode.nextSibling();
                    sAppID = childNode.toElement().text();
                    break;
                }
            }

            childNode = childNode.nextSibling();
        }

        bRet = true;

    }while(0);
    return bRet;
}

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
QStringList CUtil::listFilter(const QStringList &inStringList, const QStringList &filterList)
{
    if(inStringList.isEmpty() || filterList.isEmpty())
    {
        return inStringList;
    }

    QStringList retList;
    retList = inStringList;

    const int nCountFilter = filterList.count();
    for(int index = 0; index < nCountFilter; ++index)
    {
        if(retList.contains(filterList.at(index)))
        {
            retList.removeAll(filterList.at(index));
        }
    }

    return retList;
}

/**************************************************************************
* 函数名: GetFormatSizeString
* 功能: 获取格式化后的大小字符串
* 参数:
*    @[in ] nSize: 大小
* 返回值: 格式化后的大小字符串
* 时间: 2015/12/25 17:38
* 作者: ZDJ
*/
QString CUtil::GetFormatSizeString(qint64 nSize)
{
    QString sRet;
    const qint64 nUnitSize = 1024L;
    if (nSize < nUnitSize)
        sRet.sprintf("%lldB", nSize);
    else if (nSize >= nUnitSize && nSize < nUnitSize * nUnitSize)
        sRet.sprintf("%.1fKB", nSize * 1.0 / nUnitSize);
    else if (nSize >= nUnitSize * nUnitSize && nSize < nUnitSize * nUnitSize * nUnitSize)
        sRet.sprintf("%.1fMB", nSize * 1.0 / (nUnitSize * nUnitSize));
    else if (nSize >= nUnitSize * nUnitSize * nUnitSize && nSize < nUnitSize * nUnitSize * nUnitSize * nUnitSize)
        sRet.sprintf("%.1fGB", nSize * 1.0 / (nUnitSize * nUnitSize * nUnitSize));
    else
        sRet.sprintf("%lldB", nSize);

    return sRet;
}

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
BOOL CUtil::KillExe(const QString& sProcessName)
{
    static PROCESSENTRY32  pe;
    BOOL bRet = FALSE;
    HANDLE hSnapshot;
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    pe.dwSize = sizeof(PROCESSENTRY32);
    Process32First(hSnapshot, &pe);
    do
    {
        if(0 == wcsicmp(pe.szExeFile, sProcessName.toStdWString().c_str()))
        {
            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
            if(hProcess != NULL)
            {
                UINT nExit = 1;
                if(TerminateProcess(hProcess, nExit))
                {
                    bRet = TRUE;
                    break;
                }
            }
        }
    }while(Process32Next(hSnapshot, &pe));

    CloseHandle(hSnapshot);
    return  bRet;
}

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
BOOL CUtil::FindProcessName(const QString& sProcessName, PROCESSENTRY32& pe)
{
    BOOL bRet = FALSE;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    ZeroMemory(&pe, sizeof(pe));
    pe.dwSize = sizeof(PROCESSENTRY32);
    Process32First(hSnapshot, &pe);
    do
    {
        MODULEENTRY32 mes;
        ZeroMemory(&mes, sizeof(mes));
        mes.dwSize = sizeof(MODULEENTRY32);
        HANDLE hTempSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE , pe.th32ProcessID);
        if(!hTempSnapshot)
            continue;
        Module32First(hTempSnapshot , &mes);
        CloseHandle(hTempSnapshot);

        if(wcsicmp(mes.szModule, sProcessName.toStdWString().c_str()) == 0)
        {
            bRet = TRUE;
            break;
        }
    }while(Process32Next(hSnapshot, &pe));

    CloseHandle(hSnapshot);
    return  bRet;
}

/**************************************************************************
* FunctionName:
*     FindProcessPath
* Description:
*     查找进程路径
* Parameter:
*     @[in ] sProcessPath: 进程路径 如c:\a.exe
*     @[out] pe: 进程信息
* ReturnValue:
*     成功返回TRUE，失败返回FALSE
*/
BOOL CUtil::FindProcessPath(const QString &sProcessPath, PROCESSENTRY32& pe)
{
    BOOL bRet = FALSE;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    ZeroMemory(&pe, sizeof(pe));
    pe.dwSize = sizeof(PROCESSENTRY32);
    Process32First(hSnapshot, &pe);
    do
    {
        MODULEENTRY32 mes;
        ZeroMemory(&mes, sizeof(mes));
        mes.dwSize = sizeof(MODULEENTRY32);
        HANDLE hTempSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE , pe.th32ProcessID);
        if(!hTempSnapshot)
            continue;
        Module32First(hTempSnapshot , &mes);
        CloseHandle(hTempSnapshot);

        if(wcsicmp(mes.szExePath, sProcessPath.toStdWString().c_str()) == 0)
        {
            bRet = TRUE;
            break;
        }
    }while(Process32Next(hSnapshot, &pe));

    CloseHandle(hSnapshot);
    return  bRet;
}
#endif
