/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CFileDirHandle
* 创建时间：20160701
* 文件标识：
* 文件摘要：handle the file and dir util
*
* 当前版本：1.0.0.0
* 作    者：asu
*/


#ifndef CFILEDIRHANDLE_H
#define CFILEDIRHANDLE_H

#include "qutils_global.h"
#include <QByteArray>
#include <QStringList>
#include <QDir>

const QString DIR_SPLIT_CHAR = QDir::separator(); //QDir::separator() "/"


class QUTILS_EXPORT CFileDirHandle
{
public:
    CFileDirHandle();
    ~CFileDirHandle();

public:

    /************************************
     * 名称: getRootDirPaths
     * 功能: get the all dir paths of root path
     * 参数: [in]rootPath
     * 返回:   QStringList
     * 时间:   2016/06/01
     * 作者:   suhaiwu
     ************************************/
    static QStringList getRootDirPaths(const QString& rootPath);
    static QStringList getRootFiles(const QString& rootPath);
    //get the files with filescondition in top dir of rootpath
    static QStringList getRootFiles(const QString& rootPath, const QString& filesCondition);
    static QStringList getRootDirAndFiles(const QString& rootPath);

    //get the file parent dir Path（//返回父亲文件夹路径）
    static QString getFileDirPath(const QString& filePath);


    /************************************
     * 名称: ClearDir
     * 功能: 清空目录内容，内部会递归清空子目录
     * 参数: [in]fullPath 要清空的主目录路径
     * 参数: [in]delMainDir 指示是否删除主目录本身
     * 返回:   void
     * 时间:   2016/04/08
     * 作者:   hels
     ************************************/
    static void clearDir(const QString &fullPath, bool delMainDir);

    static bool createDir(const QString& strDirPath);

    static bool mkpath(const QString& strPath);

    static bool isDirExist(const QString& strDir);
    /************************************
     * 名称: saveFile
     * 功能: save file
     * 参数: [in]fullPath 路径
     * 参数: [in]data  data to be saved
     * 返回:   true is ok
     * 时间:   2016/07/19
     * 作者:   asu
     ************************************/
    static bool saveFile(const QString& filePath, const QByteArray& data);
    //save data to file of filePath
    static bool saveFile(const QString& filePath, const char *const pData, const int nDataLen);

    static qint64 read(const QString& filePath, char** pFileData, qint64 maxlen = 0);

    //file name with suffix
    static QString getFileName(const QString& filePath);
    //file base without suffix
    static QString getFileBaseName(const QString& filePath);
    //获取文件后缀, with out "."
    static QString getFileSuffix(const QString &filePath, bool bPostfixToLower);
    static QString getFileSuffix(const QString &filePath);


    //generat the new file name by new file base name(change the file base name)
    static QString generateFileNameByBaseName(const QString& oldFileName, const QString& newFileBaseName);
    //change the file suffix
    static QString generateFileNameBySuffix(const QString& oldFileName, const QString& newFileSuffix);

    //
    static bool copyFile(const QString& sourceFile, const QString& desFile, bool bCovered);

    //the file select format to QFileDialog
    static QString getFileSelectFormat(const QString& strFmtTitle, const QString& strFmtFilter);
    //generate the file path
    static QString getFilePath(const QString& fileDir, const QString& fileName);


    //for get the upload file byteArr to upload format
    static bool getFileByteArrayForUpload(const QString& filePath, QByteArray& bytesArr);


    //get file size
    static qint64 getFileSize(const QString strFilePath);
    //file size dB To KB
    static qint64 dBToKB(const qint64 nSize);
    //file size dB To MB
    static qint64 dBToMB(const qint64 nSize);
    //file size dB To GB
    static qint64 dBToGB(const qint64 nSize);

};

#endif // CFILEDIRHANDLE_H
