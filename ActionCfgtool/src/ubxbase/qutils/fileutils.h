#ifndef FILEUTILS_H
#define FILEUTILS_H

//#include "JlCompress.h"
#include "qutils_global.h"
#include <QObject>
#include <QStringList>
#include <QFileInfo>
#include <QFile>
#include <QUrl>
#include <QTextStream>
//文件公共函数
class QUTILS_EXPORT FileUtils
{
public:
    Q_INVOKABLE static bool exists(const QString& fileName);

public:
    FileUtils();
    virtual ~FileUtils();
    static void clearDir(const QString &fullPath, bool delMainDir);
    static bool createDir(const QString& strDirPath);
    static QString getAppInstancePath() ;
    static QByteArray readLocalFile(const QString& path);
    static QString readAllText(const QString &fileName);
    static QString getFullPath(const QString& fileName);
    static QString getFileName(const QString& filePath);
    static QString getBaseName(const QString& filePath);
    static QString getParentBaseName(const QString& filePath);
    static QString getSuffix(const QString& filePath);
    static bool writeText(const QString& fileName, QString& content);
    static bool existsFullPath(const QString& fileName);
    static bool makeDir(const QString& strDir);
    static bool makePath(const QString& strPath);
    static void rmPath(const QString& strDir, bool bRemainFolder = false);
    static void delDirExceptMd5(const QString& strDir, const QString& qsMd5);
    static void delDirExceptFileName(const QString& strDir, const QString& qsExcFileName);
    static void delDirExceptZip(const QString& strDir);
    static bool copyFile(const QString& srcFile, const QString& destFile);
    static bool saveFile(const QByteArray* content, const QString& fileName, bool bCover = true);
    static bool renameFile(const QString& fileName, const QString& newName );
    static bool createFile(const QString& fileName, int initSize = 0, bool bForce = false);
    static bool rmFile(const QString& fileName);
    static bool isAbsolute(const QString& filePath);
    static qint64 getFileSize(const QString& filePath);
    static int fileCount(const QString& filePath);
    static QStringList getFileList(const QString& dir);
    static QString saveFileName(const QUrl &url);
    static bool saveToDisk(const QString &filename, QIODevice *reply);
    static QString cdUp(const QString& fileName);
    //static QStringList extractDir(const QString &fileName, const QString& filePath, bool isDelSrcFile);
    static QString FindAppointFile(const QString& srcFolder, const QString& suffix);
    static void copyFolder(const QString &srcFolder,const QString & dstFolder,const QFileInfoList &excludeFiles);
    static void copyFolder(const QString &srcFolder,const QString & dstFolder);
    static QFileInfoList getFileInfoList(const QString& qsPath);
    /*    static bool verifyMD5(const QString& filePath, const QString& qsMD5);
    static bool verifyAllPathMD5(const QString& filePath, const QString& qsMd5, QString& correctFilePath);*/
    static bool copyFileToPath(const QString& sourceDir ,const QString& toDir, bool coverFileIfExist);
    static bool deleteFile(const QString& fileName);
    static const QString getMd5(const QString & filePath);
private:
public:
    static void delDir(const QFileInfo& fileInfo);
    static void fileCounter(const QString& filePath, int* pCounter);
    static QString readAllTextFullPath(const QString& fileName);
};


#endif // FILEUTILS_H
