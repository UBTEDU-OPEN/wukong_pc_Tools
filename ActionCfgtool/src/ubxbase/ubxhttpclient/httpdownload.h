#pragma once
#include "ubxhttpclientdef.h"
#include <curl/curl.h>
#include <string>
#include <vector>
#include <mutex>

#include <functional>
#include <map>

using namespace std;
namespace Ubxdownload {
#ifdef __MACH__
typedef __int64_t LONGLONG;
#endif
    typedef std::function<void(const std::string, LONGLONG, LONGLONG)> _DOWNLOAD_CALL_BACK;
#define MAXWORK 200
    enum DownloadState {
        initial = -1,
        unstart = 0,        //未开始
        excuting,       //正在下载
        complete,       //已完成
    };

    typedef struct DownloadInfo
    {
        std::string  url;
        std::string  filePath;
        //std::string  fileName;
        //std::function<void(const std::string fileName, LONGLONG done, LONGLONG total)> _func;
    } DLIO;

    typedef struct CurlDownloadInfo
    {
        std::string url;
        std::string fileName;
        long preLocalLen;
        double totalFileLen;
        double curDownloadLen;
        CurlDownloadInfo() : preLocalLen(0), totalFileLen (0.0f), curDownloadLen(0.0f)
        {
        }
    } CURDI;

    class UBXHTTPCLIENT_EXPORTS HttpDownload
    {
    public:
        HttpDownload(void);
        ~HttpDownload(void);
        int startDownloadThread();
        double getTotalFileLenth(const char* url);
        long getLocalFileLenth(const char* fileName);
        void getFileNameFormUrl(char* fileName, const char* url);
        void addDownloadWork(const DLIO &downloadWork, _DOWNLOAD_CALL_BACK func);
        int setConnectTimeOut(long nConnectTimeOut);
        int getCurrentDownloadInfo(CURDI* lpDownloadInfo);
        bool createMultiDir(const char* pathName);
        bool isDownloadBegin();
        bool isDownloadEnd();
         size_t __stdcall progressFuncT(double t, double d, double ultotal, double ulnow);
    protected:
        void singleDownloadProc();
        static size_t writeFunc(char *str, size_t size, size_t nmemb, void *stream);
        static size_t progressFunc(double* fileLen, double t, double d, double ultotal, double ulnow);
    private:
        std::string         m_filePath;
        std::string         m_downloadPath;
        DownloadState       m_state; 
        long                m_curLocalFileLenth;
        long 				m_nConnectTimeOut;
        //DLIO                m_dowloadWork[MAXWORK];
        vector<DLIO>        m_works;
        int                 m_curIndex;
        CURL                *m_pCurl;
        std::mutex          m_mutex;
        static CURDI               m_curDownloadInfo;
        static std::map<string, _DOWNLOAD_CALL_BACK>   m_callBackFuncs;
    };
}

