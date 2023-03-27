#include "cfgini.h"
#include <QSettings>
#include <QFile>
#include <QCoreApplication>

CCfgIni::CCfgIni ()
{
    m_strPath = QCoreApplication::applicationDirPath() + "/Config/config.ini";
}

bool CCfgIni::readItem (const QString& strKey, QVariant& var)
{
    if ( !QFile::exists(m_strPath) ){
        return false;
    }

    QSettings settings(m_strPath, QSettings::IniFormat);
    var = settings.value(strKey);
    return true;
}

bool CCfgIni::writeItem (const QString& strKey, const QVariant& var)
{
    if ( !QFile::exists(m_strPath) ){
        return false;
    }

    QSettings settings(m_strPath, QSettings::IniFormat);
    settings.setValue(strKey, var);
    return true;
}
