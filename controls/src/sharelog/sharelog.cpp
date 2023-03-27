#include "sharelog.h"
#include <QString>
#include <QCoreApplication>
#include <QDir>
#include <QTextCodec>
INITIALIZE_EASYLOGGINGPP

el::base::type::StoragePointer getStorage()
{
    return el::base::elStorage;
}

std::string convertUtf8toString (const QString& strSrc)
{
    QTextCodec* code_gbk = QTextCodec::codecForName("system");
    std::string str = code_gbk->fromUnicode(strSrc).data();
    return str;
}

void initShareLog()
{
    QString strPath = QCoreApplication::applicationDirPath();
    QString strLogDir = strPath + "/log/";
    QDir dir (strLogDir);
    if ( !dir.exists() ){
        dir.mkdir(strLogDir);
    }
    QString strLogPath = strLogDir + "log_%datetime{%Y%M%d}.log";

    el::Configurations conf;
    // 重新设置GLOBAL级别的配置项FORMAT的值
    conf.setGlobally(el::ConfigurationType::Format, "%datetime{%Y-%M-%d %H:%m:%s.%z} %level : %msg");
    conf.setGlobally(el::ConfigurationType::Enabled, "true");
    conf.setGlobally(el::ConfigurationType::ToFile,  "true");
    conf.setGlobally(el::ConfigurationType::Filename, convertUtf8toString(strLogPath));
    conf.setGlobally(el::ConfigurationType::MillisecondsWidth, "3");
    conf.setGlobally(el::ConfigurationType::MaxLogFileSize, "104857600"); //单位:B / 1048576 1MB / 104857600 100MB / 1073741824 1GB
    // 必须设置标记 LoggingFlag::StrictLogFileSizeCheck
    // 否则,配置文件中MAX_LOG_FILE_SIZE = 104857600不生效
    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
    el::Loggers::reconfigureLogger("default", conf);
}
