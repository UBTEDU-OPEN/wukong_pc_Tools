#ifndef CFGINI_H
#define CFGINI_H
#include <QString>
#include <QVariant>

// vedio setting key
#define KEY_VEDIOSETTING_DYNAMICNAME        "VideoSetting/DynamicName"
#define KEY_VEDIOSETTING_VEDIOPATH          "VideoSetting/VideoPath"
#define KEY_VEDIOSETTING_VEDIOPLAYERPATH    "VideoSetting/VideoPlayerPath"
#define KEY_VEDIOSETTING_VEDIOTYPE          "VideoSetting/VideoType"

// serial setting key
#define KEY_SERIALSETTING_SERIAL            "SerialSetting/Serial"

// dynamic setting key
#define KEY_DYNAMICSETTING_SHOWDYNAMICFUN   "DynamicSetting/ShowDynamicFun"

// language setting key
#define KEY_LANGUAGESETTING_CURRENTLANG     "Language/CurrentLang"

// system setting key
#define KEY_SYSSETTING_CANSHOWSQUATTIPS     "SysSetting/CanShowSquatTips"

//单例模式宏
#define SINGLETON_INSTANCE(_CLASS_)         \
public:                                     \
    inline static _CLASS_& getInstance()    \
    {                                       \
    static _CLASS_ s_instance;              \
    return s_instance;                      \
    }                                       \


//单例模式宏
#define SINGLETON(_CLASS_)                  \
SINGLETON_INSTANCE(_CLASS_)                 \
private:                                    \
    _CLASS_();                              \
    _CLASS_(_CLASS_ const&){}               \
    ~_CLASS_(){};                           \

class CCfgIni
{
public:
    SINGLETON(CCfgIni)

public:
    bool readItem (const QString& strKey, QVariant& var);
    bool writeItem (const QString& strKey, const QVariant& var);

protected:
    QString         m_strPath;
};

#endif // CFGINI_H
