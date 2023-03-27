
#include <QFile>
#include <QDir>

#include "cthreadcopyfile.h"
#include "ubxupdatemoduleconst.h"
#include "configs.h"
#include "widget/messagebox.h"

CThreadCopyFile::CThreadCopyFile(QObject * parent) : QThread(parent),
    m_bIs1P(true),
    m_bIsChinese(true),
    m_strRobotRootPath("")
{

}

CThreadCopyFile::~CThreadCopyFile()
{

}

void CThreadCopyFile::run()
{
    AlphaRobotLog::getInstance()->printDebug(QString("Start copy file"));
    if  (!copyBinFile())
    {
        //CMessageBox::critical(NULL, tr("Error"), tr("Failed to copy bin file. Please retry again."));
        AlphaRobotLog::getInstance()->printError("Copy bin file failed");
    }
    if (!copyDefaultActionToRobot())
    {
        AlphaRobotLog::getInstance()->printError("Copy default action file failed");
        return;
    }
}

bool CThreadCopyFile::copyDefaultActionToRobot()
{
    // 创建action/music文件夹
    QDir dir(m_strRobotRootPath);
    if (!dir.exists())
    {
         AlphaRobotLog::getInstance()->printError(QString("Directory %1 not exist!").arg(m_strRobotRootPath));
         return false;
    }
    if (!dir.mkdir(UpdateModuleConstant::ROBOT_ACTION_FILE_PATH))
    {
        AlphaRobotLog::getInstance()->printDebug("Failed to create dir " + UpdateModuleConstant::ROBOT_ACTION_FILE_PATH);
    }
    if (!dir.mkdir(UpdateModuleConstant::ROBOT_MUSIC_FILE_PATH))
    {
        AlphaRobotLog::getInstance()->printDebug("Failed to create dir " + UpdateModuleConstant::ROBOT_MUSIC_FILE_PATH);
    }

    // 应用程序路径
    QString strAppPath = CConfigs::getAppBasicInstallDir();

    QString strDefaultTemplatePath = strAppPath + "/" + DEFAULT_TEMPLATE_DIR_NAME;

    // 动作文件路径
    QString strSrcActionDir = "";

    if (m_bIs1P)
    {
        if (m_bIsChinese)
        {
            strSrcActionDir = CConfigs::getFilePath(strDefaultTemplatePath + "/1P20161109/CN", UpdateModuleConstant::ROBOT_ACTION_FILE_PATH);
        }
        else
        {
            strSrcActionDir = CConfigs::getFilePath(strDefaultTemplatePath + "/1P20161109/EN", UpdateModuleConstant::ROBOT_ACTION_FILE_PATH);
        }
    }
    else
    {
        if (m_bIsChinese)
        {
            strSrcActionDir = CConfigs::getFilePath(strDefaultTemplatePath + "/1S20161109/CN", UpdateModuleConstant::ROBOT_ACTION_FILE_PATH);
        }
        else
        {
            strSrcActionDir = CConfigs::getFilePath(strDefaultTemplatePath + "/1S20161109/EN", UpdateModuleConstant::ROBOT_ACTION_FILE_PATH);
        }
    }

    QString strDestActionDir = CConfigs::getFilePath(m_strRobotRootPath, UpdateModuleConstant::ROBOT_ACTION_FILE_PATH);
    copyDir(strSrcActionDir, strDestActionDir);

    // 音乐文件路径
    QString strSrcMusicDir = "";
    if (m_bIs1P)
    {
        if (m_bIsChinese)
        {
            strSrcMusicDir = CConfigs::getFilePath(strDefaultTemplatePath + "/1P20161109/CN", UpdateModuleConstant::ROBOT_MUSIC_FILE_PATH);
        }
        else
        {
            strSrcMusicDir = CConfigs::getFilePath(strDefaultTemplatePath + "/1P20161109/EN", UpdateModuleConstant::ROBOT_MUSIC_FILE_PATH);
        }
    }
    else
    {
        if (m_bIsChinese)
        {
            strSrcMusicDir = CConfigs::getFilePath(strDefaultTemplatePath + "/1S20161109/CN", UpdateModuleConstant::ROBOT_MUSIC_FILE_PATH);
        }
        else
        {
            strSrcMusicDir = CConfigs::getFilePath(strDefaultTemplatePath + "/1S20161109/EN", UpdateModuleConstant::ROBOT_MUSIC_FILE_PATH);
        }
    }
    QString strDestMusicDir = CConfigs::getFilePath(m_strRobotRootPath, UpdateModuleConstant::ROBOT_MUSIC_FILE_PATH);
    copyDir(strSrcMusicDir, strDestMusicDir);

    return true;
}

bool CThreadCopyFile::copyDir(const QString &strSrcDir, const QString &strDestDir)
{
    if (strSrcDir.isEmpty() || strDestDir.isEmpty())
    {
        return false;
    }

    QDir dirSrc(strSrcDir);
    if (!dirSrc.exists())
    {
        CMessageBox::critical(NULL, tr("Error"), tr("Dir not exist: ") + strSrcDir);
        return false;
    }

    QDir dirDest(strDestDir);
    if (!dirDest.exists())
    {
        CMessageBox::critical(NULL, tr("Error"), tr("Dir not exist: ") + strDestDir);
        return false;
    }

    AlphaRobotLog::getInstance()->printDebug(QString("Copy dir %1 to %2").arg(strSrcDir).arg(strDestDir));

    QStringList lstFilter;
    QFileInfoList lstAllSrcFiles = dirSrc.entryInfoList(lstFilter, QDir::NoDotAndDotDot | QDir::Files);
    bool bHasCopyFailed = false;
    for (int i = 0; i < lstAllSrcFiles.size(); i++)
    {
        QFileInfo fileInfo = lstAllSrcFiles[i];
        QString strFileName = fileInfo.fileName();
        QString strDesFilePath = CConfigs::getFilePath(strDestDir, strFileName);
        if (!QFile::copy(fileInfo.filePath(), strDesFilePath))
        {
            AlphaRobotLog::getInstance()->printError(QString("Copy file %1 to %2 failed").arg(fileInfo.filePath()).arg(strDesFilePath));
            bHasCopyFailed = true;
            continue;
        }
    }

    return bHasCopyFailed;
}

bool CThreadCopyFile::copyBinFile()
{
    QString strSrcFile = m_strBinFileLocalPath;
    if (!QFile::exists(strSrcFile))
    {
        AlphaRobotLog::getInstance()->printDebug(QString("File %1 not exist").arg(strSrcFile));
        return false;
    }

    QFileInfo fileInfo(strSrcFile);
    QString strDestFile =  CConfigs::getFilePath(m_strRobotRootPath, fileInfo.fileName());
    QString strLog = QString("Copy file from %1 to %2").arg(strSrcFile).arg(strDestFile);
    AlphaRobotLog::getInstance()->printDebug(strLog);
    int nCount = 60;

    // 由于即使移动盘已经出现，但是此时磁盘可能仍然不可写，因此
    // 需要反复尝试
    while (nCount--)
    {
        if (QFile::exists(strDestFile))
        {
            QFile::remove(strDestFile);
        }
        if (QFile::copy(strSrcFile, strDestFile))
        {
            break;
        }
        Sleep(1000);
    }
    return true;
}
