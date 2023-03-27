#include "filedirhandle.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>

const qint64 nUnitSizeForFile = 1024L;

CFileDirHandle::CFileDirHandle()
{

}

CFileDirHandle::~CFileDirHandle()
{

}

/************************************
 * 名称: getRootDirPaths
 * 功能: get the all dir paths of root path
 * 参数: [in]rootPath
 * 返回:   QStringList
 * 时间:   2016/06/01
 * 作者:   suhaiwu
 ************************************/
QStringList CFileDirHandle::getRootDirPaths(const QString &rootPath)
{
    QStringList dirList;
    dirList.clear();

    if(rootPath.isEmpty())
    {
        return dirList;
    }

    QDir dir(rootPath);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fileInfo, fileList)
    {
        if(fileInfo.isDir())
        {
            dirList.append(fileInfo.absoluteFilePath());
        }
    }

    return dirList;
}

QStringList CFileDirHandle::getRootFiles(const QString &rootPath)
{
    QStringList list;
    list.clear();

    if(rootPath.isEmpty())
    {
        return list;
    }

    QDir dir(rootPath);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fileInfo, fileList)
    {
        if(fileInfo.isFile())
        {
            list.append(fileInfo.absoluteFilePath());
        }
    }

    return list;
}

//get the files with filescondition in top dir of rootpath
QStringList CFileDirHandle::getRootFiles(const QString &rootPath, const QString &filesCondition)
{
    QStringList filesList = CFileDirHandle::getRootFiles(rootPath);

    QStringList filesRetList;
    filesRetList.clear();

    if(filesCondition.isEmpty())
    {
        return filesList;
    }

    const int nCountFiles = filesList.count();
    for(int index = 0; index < nCountFiles; ++index)
    {
        if(filesList.at(index).contains(filesCondition))
        {
            filesRetList.append(filesList.at(index));
        }
    }

    return filesRetList;
}

QStringList CFileDirHandle::getRootDirAndFiles(const QString &rootPath)
{
    QStringList dirs = getRootDirPaths(rootPath);
    QStringList files = getRootFiles(rootPath);

    return dirs + files;
}

QString CFileDirHandle::getFileDirPath(const QString &filePath)
{
    //返回父亲文件夹路径
    QFileInfo fileInfo(filePath);
    return fileInfo.absoluteDir().absolutePath();
}

/************************************
 * 名称: ClearDir
 * 功能: 清空目录内容，内部会递归清空子目录
 * 参数: [in]fullPath 要清空的主目录路径
 * 参数: [in]delMainDir 指示是否删除主目录本身
 * 返回:   void
 * 时间:   2016/04/08
 * 作者:   hels
 ************************************/
void CFileDirHandle::clearDir(const QString &fullPath, bool delMainDir)
{
    if(fullPath.isEmpty())
    {
        return;
    }

    QDir dir(fullPath);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fi, fileList)
    {
        if(fi.isFile())
        {
            fi.dir().remove(fi.fileName());
        }
        else if(fi.isDir())
        {
            clearDir(fi.absoluteFilePath(), delMainDir);
        }
    }

    if(delMainDir)
    {
        dir.rmpath(dir.absolutePath());
    }
}

bool CFileDirHandle::createDir(const QString &strDirPath)
{
    bool ret = false;

    QDir dir;
    bool exist = dir.exists(strDirPath);
    if(!exist)
    {
        ret = dir.mkdir(strDirPath);
    }

    return ret;
}

bool CFileDirHandle::mkpath(const QString &strPath)
{
    bool ret = false;

    QDir dir;
    bool exist = dir.exists(strPath);
    if(!exist)
    {
        ret = dir.mkpath(strPath);
    }

    return ret;
}

bool CFileDirHandle::isDirExist(const QString& strDir)
{
    QDir dir(strDir);
    return dir.exists();
}

bool CFileDirHandle::saveFile(const QString &filePath, const QByteArray &dataSaved)
{
    if(filePath.isEmpty())
    {
        return false;
    }

    const QString strDir = CFileDirHandle::getFileDirPath(filePath);
    CFileDirHandle::createDir(strDir);

    QByteArray data(dataSaved);

    QFile localFile(filePath);
    localFile.open(QIODevice::WriteOnly | QIODevice::Unbuffered);
    if (!localFile.isOpen())
    {
        return false;
    }

    quint64 dataSize = data.size();
    char *writeBuff = data.data();
    while (dataSize > 0)
    {
        quint64 writedSize = localFile.write(writeBuff, dataSize);
        dataSize -= writedSize;
        writeBuff += writedSize;
    }

    localFile.flush();
    localFile.close();

    return true;
}

//save data to file of filePath
bool CFileDirHandle::saveFile(const QString &filePath, const char * const pData, const int nDataLen)
{
    if(filePath.isEmpty() || !pData || (nDataLen <= 0))
    {
        return false;
    }

    const QString strDir = CFileDirHandle::getFileDirPath(filePath);
    CFileDirHandle::createDir(strDir);

    QFile localFile(filePath);
    localFile.open(QIODevice::WriteOnly | QIODevice::Unbuffered);
    if (!localFile.isOpen())
    {
        return false;
    }

    localFile.write(pData,nDataLen);
    localFile.flush();
    localFile.close();

    return true;
}

qint64 CFileDirHandle::read(const QString &filePath, char **pFileData, qint64 maxlen)
{
    qint64 readLen = 0;
    QFile fileobj(filePath);
    if(fileobj.open(QIODevice::ReadOnly))
    {
        if(0 >= maxlen)
        {
           maxlen = fileobj.size();
        }

        if(!(*pFileData))
        {
            *pFileData = new char[maxlen];

        }

        if(*pFileData)
        {
            readLen = fileobj.read((char*)(*pFileData), maxlen);
        }

        fileobj.close();

        return readLen;
    }

    return readLen;
}

QString CFileDirHandle::getFileName(const QString &filePath)
{
    QFileInfo info(filePath);
    return info.fileName();
}

QString CFileDirHandle::getFileBaseName(const QString &filePath)
{
    QFileInfo info(filePath);
    return info.baseName();
}

//获取文件后缀
QString CFileDirHandle::getFileSuffix(const QString &filePath, bool bPostfixToLower)
{
    QString suffix = CFileDirHandle::getFileSuffix(filePath);
    if(bPostfixToLower)
    {
        suffix = suffix.toLower();
    }
    else
    {
        suffix = suffix.toUpper();
    }

    return suffix;
}

//获取文件后缀
QString CFileDirHandle::getFileSuffix(const QString &filePath)
{
    if(filePath.isEmpty())
    {
        return "";
    }

    QFileInfo fileInfo(filePath);
    return fileInfo.suffix();
}

//generat the new file name by new file base name(change the file base name)
QString CFileDirHandle::generateFileNameByBaseName(const QString &oldFileName, const QString &newFileBaseName)
{
    if(oldFileName.isEmpty() || newFileBaseName.isEmpty())
    {
        return oldFileName;
    }

    QString strOldFileDirPath = CFileDirHandle::getFileDirPath(oldFileName);
    if(!strOldFileDirPath.isEmpty())
    {
        strOldFileDirPath += DIR_SPLIT_CHAR;
    }

    const QString strFileSuffix = CFileDirHandle::getFileSuffix(oldFileName);

    return strOldFileDirPath + newFileBaseName + strFileSuffix;
}

//change the file suffix
QString CFileDirHandle::generateFileNameBySuffix(const QString &oldFileName, const QString &newFileSuffix)
{
    if(oldFileName.isEmpty() || newFileSuffix.isEmpty())
    {
        return oldFileName;
    }

    const QString strFileSuffix = CFileDirHandle::getFileSuffix(oldFileName);

    QString newFileName = oldFileName.left(oldFileName.length() - strFileSuffix.length());
    if(!newFileName.endsWith(".") && newFileSuffix.startsWith("."))
    {
        newFileName.append(".");
    }

    return newFileName + newFileSuffix;
}


bool CFileDirHandle::copyFile(const QString &sourceFile, const QString &desFile, bool bCovered)
{
    if (sourceFile.isEmpty() || desFile.isEmpty() || (sourceFile == desFile))
    {
        return false;
    }

    if (!QFile::exists(sourceFile))
    {
        return false;
    }

    if(bCovered && QFile::exists(desFile))
    {
        QFile::remove(desFile);
    }

    const QString strDir = CFileDirHandle::getFileDirPath(desFile);
    CFileDirHandle::createDir(strDir);

    return QFile::copy(sourceFile, desFile);
}

//the file select format to QFileDialog
QString CFileDirHandle::getFileSelectFormat(const QString &strFmtTitle, const QString &strFmtFilter)
{
    QString strFmt;

#ifdef Q_OS_MAC
    strFmt.sprintf("%s(*%s);;%s(*%s)", strFmtTitle.toStdString().c_str(), strFmtFilter.toStdString().c_str(),
                 strFmtTitle.toStdString().c_str(), strFmtFilter.toStdString().c_str());
#else
    strFmt.sprintf("%s(*%s)", strFmtTitle.toStdString().c_str(), strFmtFilter.toStdString().c_str());
#endif

    return strFmt;
}

QString CFileDirHandle::getFilePath(const QString &fileDir, const QString &fileName)
{
    if(fileDir.endsWith(DIR_SPLIT_CHAR) || fileName.startsWith(DIR_SPLIT_CHAR))
    {
        return QDir::toNativeSeparators(fileDir + fileName);
    }

    return QDir::toNativeSeparators(fileDir + DIR_SPLIT_CHAR + fileName);
}

bool CFileDirHandle::getFileByteArrayForUpload(const QString& filePath, QByteArray& bytesArr)
{
    bytesArr.clear();

    bool bOk = false;

    QFile fileobj(filePath);
    if(fileobj.open(QIODevice::ReadOnly))
    {
        QByteArray bytesFileData = fileobj.readAll();
        fileobj.close();

        QString sFileName = CFileDirHandle::getFileName(filePath);
        const int nFileNameLenTypeLen = sizeof(int); //文件名长度的数据类型长度
        char szFileNameLen[nFileNameLenTypeLen] = {0};//保存文件名长度
        int nFileNameLen = sFileName.toUtf8().length();
        memcpy(szFileNameLen, &nFileNameLen, sizeof(szFileNameLen));

        bytesArr.append(szFileNameLen, sizeof(szFileNameLen)); //文件名长度
        bytesArr.append(sFileName.toUtf8()); //文件名

        const int nFileDataLenTypeLen = sizeof(int); //文件数据长度的数据类型长度
        char szFileDataLen[nFileDataLenTypeLen] = {0};//保存文件数据长度
        int nFileDataLen = bytesFileData.length();
        memcpy(szFileDataLen, &nFileDataLen, sizeof(szFileDataLen));

        bytesArr.append(szFileDataLen, sizeof(szFileDataLen)); //文件数据长度
        bytesArr.append(bytesFileData);//文件数据

        bOk = true;
    }

    return bOk;
}

//get file size
qint64 CFileDirHandle::getFileSize(const QString strFilePath)
{
    QFileInfo info(strFilePath);
    return info.size();
}

//file size dB To KB
qint64 CFileDirHandle::dBToKB(const qint64 nSize)
{
    return nSize*1.0/nUnitSizeForFile;
}

//file size dB To MB
qint64 CFileDirHandle::dBToMB(const qint64 nSize)
{
    return CFileDirHandle::dBToKB(nSize)/nUnitSizeForFile;
}

//file size dB To GB
qint64 CFileDirHandle::dBToGB(const qint64 nSize)
{
    return CFileDirHandle::dBToMB(nSize)/nUnitSizeForFile;
}
