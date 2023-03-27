#ifndef UBXUPDATEMODULECONST_H
#define UBXUPDATEMODULECONST_H

namespace UpdateModuleConstant {

    // 1S
    const QString STRING_VERSION_ALPHA1S_V = "alpha1s_v";
    const QString STRING_VERSION_ALPHA1S_V2 = "alpha1_v2.0";
    const QString STRING_VERSION_ALPHA1SP_V = "alpha1_v2.06";

    // 1P
    const QString STRING_VERSION_ALPHA1P_V = "alpha1p_v";
    const QString STRING_VERSION_ALPHA1P_V2 = "alpha1_v2.05";
    const QString STRING_VERSION_ALPHA1P_PLUS_V2 = "alpha1_v2.07";

    const QString NET_KEYWORD_VERSION_NAME = "versionName";
    const QString NET_KEYWORD_VERSION_PATH = "versionPath";
    const QString NET_KEYWORD_VERSION_UPDATE_TIME = "versionUpdateTime";
    const QString NET_KEYWORD_VERSION_RESUME = "versionResume";
    const QString NET_KEYWORD_VERSION_DES_NAME = "name";
    const QString NET_KEYWORD_VERSION_SIZE = "versionSize";
    const QString NET_KEYWORD_VERSION_INFO = "info";

    const QString UDISK_FORMAT_FAT32 = "fat32";

    const QString SYSTEM_EMBEDED_VERSION_082214 = "2016082214";

    const QString CN_BIN_FILE_2016103001 = "alpha1_v2016103001_cn.bin";
    const QString EN_BIN_FILE_2016103001 = CN_BIN_FILE_2016103001;

    const QString ROBOT_ACTION_FILE_PATH = "action";
    const QString ROBOT_MUSIC_FILE_PATH = "music";

    const QString CHINESE_KEYWORD = "Chinese";
    const QString ENGLISH_KEYWORD = "English";

    const QString ROBOT_1P_KEYWORD = "1P";
    const QString ROBOT_1S_KEYWORD = "1S";

    const quint64 STORAGE_VOLUME_130M = 1000 * 1024 * 130;

    const int TIMEOUT_1000MS = 1000; // 1s时间

    const int WAIT_FOR_UDISK_SHOWUP_TIMEOUT = 3 * 60 * 1000;  // 等待移动盘出现的时间
    const int WAIT_FOR_FOTMAT_UDISK_START_TIMEOUT = 1000 * 60;  // 等待移动盘开始格式化的时间
}

#endif // UBXUPDATEMODULECONST_H

