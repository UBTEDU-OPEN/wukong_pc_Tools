/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CConfigs.h
* 创建时间：2016/07/15
* 文件标识：
* 文件摘要：参数设置
*
* 当前版本：1.0.0.0
* 作    者：asu
* 完成时间：2016/07/15
* 版本摘要：
*/

#ifndef CCONFIGS_H
#define CCONFIGS_H
#include "ubxenv_global.h"
#include <QString>
#include <QMap>
#include <vector>
#include "robotdef.h"
#include "singleton.h"

using std::vector;
class QDomElement;
enum emTemplateDir
{
    UBX_DEFAULT_FLOW_TEMPLATE = 0, 
    UBX_DEFAULT_ACTION_TEMPLATE,
    UBX_CUSTOM_FLOW_TEMPLATE,
    UBX_CUSTOM_ACTION_TEMPLATE,
    UBX_MOTION_TEMPLATE_COUNT,
};
//!!! app data dir
const QString ACT_LIB_DIR_NAME = "actlib";
const QString CONFIG_DIR_NAME = "Config";
const QString CUS_TEMPLATE_DIR_NAME = "CusTemplate";//!!!it is data dir
const QString DEFAULT_TEMPLATE_DIR_NAME = "defaultTemplate";//!!!this is app package install dir
const QString LOG_DIR_NAME = "Log"; // Log目录名称
const QString MUSIC_DIR_NAME = "Music";//Music目录名称
const QString PREVIEW_IMAGE_DIR_NAME = "PreviewImage";
const QString RECOMMEND_IMAGE_DIR_NAME = "RecommendImage";
const QString TEMP_DIR_NAME = "Temp";// Temp目录名称
const QString WAVE_DIR_NAME = "Wave";// Wave目录名称
const QString DETAILED_IMAGE_DIR_NAME = "DetailedImage"; // 动作详情图片
//!!! app package install dir
const QString LANGUAGES_DIR_NAME = "languages";
const QString RES_DIR_NAME = "res";
const QString RESOURCES_DIR_NAME = "Resources";
const QString ROBOT_DIR_NAME = "Robot";
const QString DOWNLOAD_DIR = "downloads";
//const QString LAYER_DIR_NAME = "Layer";//???
//leo mm
const QString TEMPLATE_DIR = "template";
const QString UPGRADE_XML_NAME = "upgrade.xml";
const QString UPGRADE_EXE_NAME = "upgrader.exe";

const QString TEMPLATE_NODE_DIR_NAME[] = 
{
    "DefaultFlowTemplate",
    "DefaultActionTemplate",
    "CustomFlowTemplate",
    "CustomActionTemplate",
};

const QString TEMPLATE_ACTION_NODE_DIR_NAME[] = {
    "EarTemplate",
    "EyeTemplate",
    "MotionTemplate",
};
const QString DUMP_DIR_NAME = "Dump";
const QString DUMP_FILE_NAME = "core.dump";

class UBXENV_EXPORT CConfigs : public utils::SingletonT<CConfigs>
{
public:
    explicit CConfigs();
    ~CConfigs();
public:
    //!!! all path is end without dir split char "/"
    static QString getFilePath(const QString& fileDir, const QString& fileName);
    //!app basic data dir
    static QString getAppBasicDataDir();
    //!app package install basic dir
    static QString getAppBasicInstallDir();

    //!app data path
    static QString getLocalActLibDir();
    static QString getLocalActLibPath(const QString& fileBasicName);

    //this is for user configs
    static QString getFileNameFromAppLocalDataLocationPath(const QString & fileName);
    static QString getLocalUserConfigDir();
    static QString getLocalUserConfigPath(const QString& fileBasicName);

    static QString getLocalCusTemplateDir();
    static QString getLocalDefaultTemplateDir();

    static QString getLocalLogDir();
    static QString getLocalLogFilePath();

    static QString getLocalMusicDir();
    static QString getLocalMusicPath(const QString& fileBasicName);

    static QString getLocalPreviewImgDir();
    static QString getLocalPreviewImgPath(const QString& fileBasicName);

    // 动作详情页预览图片
    static QString getLocalDetailedImageDir();
    static QString getLocalDetailedImagePath(const QString& fileBasicName);

    static QString getLocalRecommendImgDir();
    static QString getLocalRecommendImgPath(const QString& fileBasicName);

    static QString getLocalTempDir();
    static QString getLocalTempPath(const QString& fileBasicName);

    static QString getLocalWaveDir();
    static QString getLocalWavePath(const QString& fileBasicName);

    static QString getLocalDownloadDir();
    static QString getLocalDownloadPath(const QString& fileName);


    //!app package install path
    static QString getLocalLanguageDir();

    static QString getLocalResDir();
    static QString getLocalResPath(const QString& fileBasicName);

    static QString getLocalResourcesDir();
    static QString getLocalResourcesPath(const QString& fileBasicName);

    static QString getLocalRobotDir();
    static QString getLocalRobotPath(const QString& fileBasicName);

    static QString getDownloadPath();
    static QString getUpgradeExeFullPath();
    //some files path
    //动作表配置文件路径
    static QString getIniConfigFile();
    static QString getUpgradeFilePath();
    static void createDirsForApp();
    //leo mmm
    //static QPair<QString, QString> getLocalMotionTemplate(int type);
    static QString getLocalTemplateDir();
    static QMap<int, QString> getLocalTemplateFullPath(const emTreeModelType& type);
    static QString getSystemAppDataDir();
    static QString getAppTempDataDir(const QString& qsName);
    static QString getAppName();

    static QString getUserDirPath(const QString &strDirName);
    static QString getUserLanguage();
    static QString getAppSubDirPath(const QString &strDirName);
    static void readModelXml(const QDomElement &element, std::vector<MOTOR_DATA *> &motorlist);
    static void readModelXml(const QString &sXmlFilePath, vector<MOTOR_DATA *> &motorlist);
    static int getCfgMotorCount();
    static const QString getFileNameFromOuputPath(QString const & strFileName);
    static void loadConfig();
    static const QString getDumpPath();
};

#endif // CCONFIGS_H





