#ifndef PUBDEF_H
#define PUBDEF_H

#include <QString>
#include <QList>

#if _MSC_VER >= 1600
    #pragma execution_character_set("utf-8")
#endif

enum EM_ROBOT_FILE_PATH_TYPE
{
    EN_ROBOT_NONE_TYPE              = 0,    /**< 未知 未知路径类型 */
    EN_ROBOT_ACTIONS_TYPE           = 1,    /**< actions 动作资源-- .ubx、.lua或zip（如果包含文件夹压缩成zip） */
    EN_ROBOT_EXPRESS_TYPE           = 2,    /**< expresss 表情资源--.gif、.png、.lua、json或zip(如果包含文件夹压缩成zip) */
    EN_ROBOT_BEHAVIORS_TYPE         = 3,    /**< behaviors 表现力资源-- .xml */
    EN_ROBOT_SCENES_TYPE            = 4,    /**< scenes 场景资源-- .xml */
    EN_ROBOT_SCRIPTS_TYPE           = 5,    /**< scripts 脚本--.lua */
    EN_ROBOT_LOCALTTS_TYPE          = 6,    /**< localTTs 本地tts资源--.mp3 */
    EN_ROBOT_MUSIC_TYPE             = 7,    /**< music 音乐资源--.mp3 */
    EN_ROBOT_APK_TYPE               = 8,    /**< apk 打包文件 --.apk */
    EN_ROBOT_LOACATION_TYPE         = 9,    /**< location */
    EN_ROBOT_FILE_PATH_TYPE_COUNT
};

#define ASSGIN_ACTIVE_DEVICE_NUM_MAX  100   /**< 最大指定激活设备数量 */
#define trs(Var)                      QString(Var).toStdString()

enum EM_MUSIC_TYPE
{
    EN_SYSTEM_MUSIC_TYPE = 1,   /**< 系统音乐 */
    EN_CUSTOM_MUSIC_TYPE = 2,   /**< 自定义音乐 */
    EN_UPDATE_MUSIC_TYPE = 3    /**< 后台更新音乐 */
};

enum EM_PLAY_LOOP_TYPE
{
    EN_PLAY_SINGLE_TYPE = 0,            /**< 单曲播放 */
    EN_PLAY_SINGLE_LOOP_TYPE = 1,       /**< 单曲循环 */
    EN_PLAY_LIST_LOOP_TYPE = 2,         /**< 列表循环 */
    EN_PLAY_LOOP_TYPE_COUNT
};

/**
 * @brief 语言类型
 */
enum EN_LANGUAGE_TYPE
{
    EN_LANGUAGE_EN_US = 0,              /**< 英文    */
    EN_LANGUAGE_ZH_CN = 1,              /**< 中文简体 */
    EN_LANGUAGE_KO    = 2               /**< 韩语 */
};

typedef struct{
    QString m_strDisplayName;
    int     m_nMusicTime;
    QString m_strDownLoadUrl;
}tagMusicInfo;
typedef QMap<QString, tagMusicInfo*> QMusicInfoMap;
typedef QMap<int, QMusicInfoMap*> QTypeMusicInfoMap;

#endif // PUBDEF_H
