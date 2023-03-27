#ifndef UPDATEPARAM_H
#define UPDATEPARAM_H
#include "updatelib_global.h"

class UPDATELIBSHARED_EXPORT UpdateParam
{
public:
    UpdateParam();
    virtual ~UpdateParam ();

public:
    void        setModuleName (const QString& strModuleName)    { m_strModuleName = strModuleName; }
    QString     getModuleName () const                          { return m_strModuleName; }

    void        setVersionName (const QString& strVersionName)  { m_strVersionName = strVersionName; }
    QString     getVersionName () const                         { return m_strVersionName; }

    void        setIncremental (bool isIncremental)             { m_isIncremental = isIncremental; }
    bool        getIncremental () const                         { return m_isIncremental; }

    void        setPackageUrl (const QString& strPackageUrl)    { m_strPackageUrl = strPackageUrl; }
    QString     getPackageUrl () const                          { return m_strPackageUrl; }

    void        setPackageMd5 (const QString& strPackageMd5)    { m_strPackageMd5 = strPackageMd5; }
    QString     getPackageMd5 () const                          { return m_strPackageMd5; }

    void        setPackageSize (int nPackageSize)               { m_nPackageSize = nPackageSize; }
    int         getPackageSize () const                         { return m_nPackageSize; }

    void        setIncrementUrl (QString strIncrementUrl)       { m_strIncrementUrl = strIncrementUrl; }
    QString     getIncrementUrl () const                        { return m_strIncrementUrl; }

    void        setIncrementMd5 (QString strIncrementMd5)       { m_strIncrementMd5 = strIncrementMd5; }
    QString     getIncrementMd5 () const                        { return m_strIncrementMd5; }

    void        setIncrementSize (int nIncrementSize)           { m_nIncrementSize = nIncrementSize; }
    int         getIncrementSize () const                       { return m_nIncrementSize; }

    void        setForced (bool isForced)                       { m_isForced = isForced; }
    bool        getForced () const                              { return m_isForced; }

    void        setReleaseNote (QString strReleaseNote)         { m_strReleaseNote = strReleaseNote; }
    QString     getReleaseNote () const                         { return m_strReleaseNote; }

    void        setReleaseTime (int nReleaseTime)               { m_nReleaseTime = nReleaseTime; }
    int         getReleaseTime () const                         { return m_nReleaseTime; }

protected:
    QString     m_strModuleName;            // 模块名称           多模块升级时，根据模块名称获取相应的升级版本信息
    QString     m_strVersionName;           // 版本名称           当前模块下的最新版本
    bool        m_isIncremental;            // 是否具有增量包      用以判断请求版本与最新版本间是否存在增量包
    QString     m_strPackageUrl;            // 全量包下载地址      全量包在云存储服务器中的下载地址
    QString     m_strPackageMd5;            // 全量包 MD5         全量包的 MD5 值，用以校验全量包的合法性
    int         m_nPackageSize;             // 全量包大小         全量包的文件大小，单位：Byte
    QString     m_strIncrementUrl;          // 增量包下载地址      增量包在云存储服务器中的下载地址；仅当：isIncremental=true时，该字段存在
    QString     m_strIncrementMd5;          // 增量包 MD5         增量包的MD5值，用以校验增量包的合法性；仅当：isIncremental=true 时，该字段存在
    int         m_nIncrementSize;           // 增量包大小         增量包的文件大小，单位：Byte
    bool        m_isForced;                 // 是否强制安装        isForced=true，说明该版本下载后必须安装（紧急推送版本可以使用该属性）
    QString     m_strReleaseNote;           // 升级日志           纯文本（支持空行格式：在后台管理系统中编辑升级日志，输入空行，接口返回：\r\n）
    int         m_nReleaseTime;             // 发布时间           时间戳，调用方根据当前时区自定义显示时间。（发布时间为：版本上线时间)
};

#endif // UPDATEPARAM_H
