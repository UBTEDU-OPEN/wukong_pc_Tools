#pragma once

#include "ubxenv_global.h"
#include "updaterdef.h"
#include <functional>
#include <list>

class QClient;
using std::list;
class UBXENV_EXPORT UpdaterInterface
{
public:
    UpdaterInterface();
    virtual ~UpdaterInterface();
public:
    QString formatVersionStr(const QString & in);
    bool isUpgrading(const QString & qsNativeVersion, const QString & qsRemoteVersion);
    bool upgradingVersionXmlFile(const QString &qsXmlPath, VERSIONINFO *pInfo);
    bool parseVersionXmlFile(const QString &qsXmlPath, VERSIONINFO *pInfo);
    bool checkUpgradeAvailable();
    void checkUpdate();
    void startUpgrading();
    bool calculatingPatch(const VERSIONINFO *native, const VERSIONINFO *remote, list<ProgramFileInfo>& patch);
    map<QString, ProgramFileInfo> parseRemoteVersionInfo(const QString &qsXmlPath);
    map<QString, ProgramFileInfo> localFileList();
    QString getRemoteVersion();
public:
    QString                     m_qsNativeVersion;
    QString                     m_qsBaseUrl;
    QString                     m_qsRemoteVersion;
    bool                        m_bNeedUpgrade;
    QClient                     *m_pClient;
};
