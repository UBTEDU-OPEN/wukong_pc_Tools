#pragma once

#include "ubxenv_global.h"
#include "singleton.h"
#include <QString>
#include <map>
#include <vector>
using std::map;
using std::vector;

class QClient;

typedef struct ProgramFileInfo_t {
    QString     _name;
    //QString     _number;
    QString     _path;
    QString     _md5;
    QString     _url;
} ProgramFileInfo;

typedef struct versionInfo_t {
    QString                         _mainVersion;
    QString                         _url;
    std::map<QString, ProgramFileInfo>   _files_map;
} VERSIONINFO;

class UBXENV_EXPORT UBXEnv : public utils::SingletonT<UBXEnv>
{
public:
    UBXEnv();
    virtual ~UBXEnv();
public:
    QString formatVersionStr(const QString & in);
    bool isUpgrading(const QString & qsNativeVersion, const QString & qsRemoteVersion);
    bool upgradingVersionXmlFile(const QString &qsXmlPath, VERSIONINFO *pInfo);
    bool parseServerVersionXmlFile(const QString &qsXmlPath, VERSIONINFO *pInfo);
    bool checkUpgradeAvailable();
    void checkUpdate();
    void startUpgrading();
    bool calculatingPatch(const VERSIONINFO *native, const VERSIONINFO *remote, vector<ProgramFileInfo>& patch);
    map<QString, ProgramFileInfo> parseRemoteVersionInfo(const QString &qsXmlPath);
    map<QString, ProgramFileInfo> localFileList();
    QString getRemoteVersion();

public:
    QString                     m_qsNativeVersion;
    QString                     m_qsBaseUrl;
    QString                     m_qsRemoteVersion;
    bool                        m_bNeedUpgrade;
    VERSIONINFO                 m_nativeInfo;
    VERSIONINFO                 m_remoteInfo;
    vector<ProgramFileInfo>     m_vecPatch;
    QClient                     *m_pClient;
};
