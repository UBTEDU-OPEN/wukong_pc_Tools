#include "httpdownload.h"
#ifdef _WIN32
#include <io.h>
#else
#include <stdio.h>
#include <sys/stat.h>
#endif
#include <thread>
#include "formatfunc.h"

using namespace std;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using std::placeholders::_4;
using std::placeholders::_5;

namespace Ubxdownload {

    CURDI HttpDownload::m_curDownloadInfo;
    std::map<string, _DOWNLOAD_CALL_BACK> HttpDownload::m_callBackFuncs;

    HttpDownload::HttpDownload(void)
    {
        m_state = initial;
        m_nConnectTimeOut = 0;
        curl_global_init(CURL_GLOBAL_ALL);
        m_curIndex = 0;
    }

    HttpDownload::~HttpDownload(void)
    {
        curl_global_cleanup();
    }

    bool HttpDownload::isDownloadBegin()
    {
        if (m_state == unstart)
            return true;
        return false;
    }

    bool HttpDownload::isDownloadEnd()
    {
        if (m_state == excuting)
            return true;
        return false;
    }

    bool HttpDownload::createMultiDir(const char* pathName)
    {
#ifdef _WIN32
        if (pathName == NULL) return false;
        char filePath[256] = { 0 };
        strcpy(filePath, pathName);
        int i = 0, pathLen = strlen(pathName);
        string curPath;
        char curFilePath[256] = { 0 };
        WIN32_FIND_DATA swf;
        if (filePath[pathLen - 1] != '\\') {
            filePath[pathLen] = '\\';
        }
        while (filePath[i] != '\0') {
            if (filePath[i] == ':') {
                i += 2;
                continue;
            }
            if (filePath[i] == '\\') {
                memcpy(curFilePath, filePath, i);
                curFilePath[i] = '\0';
                curPath = curFilePath;
#ifdef UNICODE
                std::wstring wcurPath = utils::ansi2unicode(curPath);
#endif
#ifdef UNICODE
                if (FindFirstFile(wcurPath.c_str(), &swf) == INVALID_HANDLE_VALUE) //目录不存在就创建
#else
                if (FindFirstFile(curPath.c_str(), &swf) == INVALID_HANDLE_VALUE) //目录不存在就创建
#endif
                {
#ifdef UNICODE
                    if (!CreateDirectory(wcurPath.c_str(), NULL))
#else
                    if (!CreateDirectory(curPath.c_str(), NULL))
#endif
                    {
                        return false;
                    }
                }
            }
            i++;
        }
#endif
        return true;
    }

    void HttpDownload::addDownloadWork(const DLIO &downloadWork, _DOWNLOAD_CALL_BACK func)
    {
        std::string filePath = downloadWork.filePath;
        const std::string url = downloadWork.url;
        //路径交给上层处理
        int i = filePath.length() - 1;
        bool isPath = true;
        while (filePath[i] != '\\' && filePath[i] != '/')
        {
            char c = filePath[i];
            if (filePath[i] == '.' && filePath[i + 1] != '\0') {
                isPath = false;
            }
            i--;
        }
        if (isPath) {
            if (!createMultiDir(filePath.c_str()))
                return;
            char fileName[256] = { 0 };
            getFileNameFormUrl(fileName, url.c_str());
            if (filePath[filePath.length() - 1] != '\\') {
                //strcat(filePath.c_str(), "\\");
                filePath += ("\\");
            }
            //strcat(filePath, fileName);
            filePath += fileName;
        }
        else {
            char realPath[256] = { 0 };
            for ( int k = 0; k < i; ++ k ) {
                realPath[k] = filePath[k];
            }
            realPath[i] = '\\';
            if (!createMultiDir(realPath)) {
                return;
            }
        }
        DownloadInfo work;
        work.url = url;
        work.filePath = filePath;
        m_works.push_back(work);
        m_callBackFuncs.insert(std::make_pair(filePath, func));
        ++m_curIndex;
    }

    void HttpDownload::getFileNameFormUrl(char* fileName, const char* url)
    {
        int urlLen = strlen(url);
        char mUrl[512] = { 0 };
        char fName[256] = { 0 };
        strcpy(mUrl, url);
        int cutIndex = 0;
        int i = urlLen - 1, j = 0;
        while (mUrl[--i] != '/');
        i++;
        while (mUrl[i] != '\0' && mUrl[i] != '?' &&mUrl[i] != '&')
        {
            fName[j++] = mUrl[i++];
        }
        fName[j] = '\0';
        strcpy(fileName, fName);
        return;
    }

    long HttpDownload::getLocalFileLenth(const char* fileName)
    {
        if (m_state == unstart)
            return m_curLocalFileLenth;
        FILE *fp = NULL;
        fp = fopen (fileName, "rb");
        if (fp != nullptr) {
            fseek (fp, 0, SEEK_END);
            m_curLocalFileLenth = ftell(fp);
            fclose(fp);
        }
//        char strTemp[256] = { 0 };
//        strcpy(strTemp, fileName);
//        FILE* fp = fopen(strTemp, "rb");
//        if (fp != NULL) {
//            m_curLocalFileLenth = filelength(fileno(fp));
//            fclose(fp);
//            return m_curLocalFileLenth;
//        }
//        return 0;
        return m_curLocalFileLenth;
    }

    double HttpDownload::getTotalFileLenth(const char* url)
    {
        char szUrl[512] = { 0 };
        strcpy(szUrl, url);
        double downloadFileLenth = 0;
        CURL* pCurl = curl_easy_init();
        curl_easy_setopt(pCurl, CURLOPT_URL, szUrl);
        curl_easy_setopt(pCurl, CURLOPT_HEADER, 1L);
        curl_easy_setopt(pCurl, CURLOPT_NOBODY, 1L);
        if (curl_easy_perform(pCurl) == CURLE_OK)  {
            curl_easy_getinfo(pCurl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadFileLenth);
        }
        else {
            downloadFileLenth = -1;
        }
        curl_easy_cleanup(pCurl);
        return downloadFileLenth;
    }

    size_t HttpDownload::writeFunc(char *str, size_t size, size_t nmemb, void *stream)
    {
        return fwrite(str, size, nmemb, (FILE*)stream);
    }

    size_t HttpDownload::progressFunc(
        double* pFileLen,
        double t,// 下载时总大小  
        double d, // 已经下载大小  
        double ultotal, // 上传是总大小  
        double ulnow)   // 已经上传大小  
    {
        if (t == 0) {
            return 0;
        }

        *pFileLen = d;
        const std::string _name = m_curDownloadInfo.fileName;
        auto it = m_callBackFuncs.find(_name);
        if (it != m_callBackFuncs.end() && it->second != nullptr) {
            (it->second)(_name, d, t);
        }
        //if (t - d <= 1e-6 && d > 1e-6) {
        //    m_callBackFuncs.erase(it);
        //}
        return 0;
    }

    size_t HttpDownload::progressFuncT( double t, double d, double ultotal, double ulnow)
    {
        return 0;
    }

    int HttpDownload::startDownloadThread()
    {
        if (m_state == initial || m_state == excuting) {
            //HANDLE downloadThread = CreateThread(NULL, 0, singleDownloadProc, this, 0, NULL);
            //CloseHandle(downloadThread);
            std::thread  workThread(std::bind(&HttpDownload::singleDownloadProc, this));
            //std::thread t1(&HttpClient::downloadRequest, this, request, fullPath, _func);
            workThread.detach();
            return 0;
        }
        return -1;
    }

    void HttpDownload::singleDownloadProc()
    {
        int curDLIndex = 0;
        CURL* pCurl = curl_easy_init();
        while (curDLIndex < m_curIndex/* && m_state == excuting*/)
        {
            std::string fileName = m_works[curDLIndex].filePath;
            std::string url = m_works[curDLIndex].url;

            m_curDownloadInfo.url = url;
            m_curDownloadInfo.fileName = fileName;
            long localFileLen = getLocalFileLenth(fileName.c_str());
            m_curLocalFileLenth = localFileLen;
            m_curDownloadInfo.preLocalLen = m_curLocalFileLenth;
            double totalFileLen = m_curDownloadInfo.totalFileLen = getTotalFileLenth(url.c_str());
            //如果需要下载文件的大小大于等于本地文件的大小，直接下载下一个文件
            if ( localFileLen >= (long)totalFileLen ) {
                curDLIndex++;
                m_state = initial;
                continue;
            }
            FILE* fp = fopen(fileName.c_str(), "ab+");
            //文件打开错误，进行下一个文件的下载
            if (fp == nullptr) {
                m_state = initial;
                continue;
            }
            curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, m_nConnectTimeOut);//
            curl_easy_setopt(pCurl, CURLOPT_HEADER, 0L);
            curl_easy_setopt(pCurl, CURLOPT_NOBODY, 0L);
            curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(pCurl, CURLOPT_RESUME_FROM, localFileLen);

            curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, writeFunc);
            curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, fp);

            curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 0L);
            //curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, std::bind(&HttpDownload::progressFuncT, this, _1, _2, _3, _4));
            curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, progressFunc);
            curl_easy_setopt(pCurl, CURLOPT_PROGRESSDATA, &(m_curDownloadInfo.curDownloadLen));

            m_state = unstart;
            if (!curl_easy_perform(pCurl)) {
                curDLIndex++;
                m_state = initial;
            }
            fclose(fp);
        }
        curl_easy_cleanup(pCurl);
        m_state = excuting;
    }

    int HttpDownload::getCurrentDownloadInfo(CURDI* lpDownloadInfo)
    {
        *lpDownloadInfo = m_curDownloadInfo;
        return 0;
    }

    int HttpDownload::setConnectTimeOut(long nConnectTimeOut)
    {
        if (m_state == unstart)
            return -1;
        else
            m_nConnectTimeOut = nConnectTimeOut;
        return 0;
    }
};
