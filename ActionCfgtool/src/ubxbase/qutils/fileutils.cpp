//#include "stdafx.h"
#include "fileutils.h"
#include <QDir>
#include <QFileInfo>
#include <QCoreApplication>
#include <QFileInfoList>
#include <QTextStream>
#include <QCryptographicHash>

//文件公共函数
FileUtils::FileUtils()
{

}


FileUtils::~FileUtils()
{

}


/**************************************************************************
* 函数名:   clearDir
* 功能:
* 参数:
*....@[in]  const QString & fullPath
*....@[in]  bool delMainDir
*....@[out] void
* 返回值:
* 时间:     2017/10/16 14:38
* 作者:   Leo
*/
void FileUtils::clearDir(const QString &fullPath, bool delMainDir)
{
    if (fullPath.isEmpty()) {
        return;
    }
    QDir dir(fullPath);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    QFileInfoList fileList = dir.entryInfoList();
    foreach(QFileInfo fi, fileList) {
        if (fi.isFile()) {
            fi.dir().remove(fi.fileName());
        } else if (fi.isDir()) {
            clearDir(fi.absoluteFilePath(), delMainDir);
        }
    }

    if (delMainDir) {
        dir.rmpath(dir.absolutePath());
    }
}

/**************************************************************************
* 函数名:   createDir
* 功能:
* 参数:
*....@[in]  const QString & strDirPath
*....@[out] bool
* 返回值:
* 时间:     2017/10/16 14:38
* 作者:   Leo
*/
bool FileUtils::createDir(const QString& strDirPath)
{
    bool ret = false;
    QDir dir;
    bool exist = dir.exists(strDirPath);
    if (!exist) {
        ret = dir.mkdir(strDirPath);
    }
    return ret;
}

QString FileUtils::getAppInstancePath()
{
	QString exeFileName = QCoreApplication::applicationFilePath();
	QFileInfo file(exeFileName);
	return file.canonicalPath();
}

QString FileUtils::getFullPath(const QString& fileName)
{
	if(QDir::isAbsolutePath(fileName))
	{
		if(fileName.endsWith('/') || fileName.endsWith('\\'))
			return fileName.left(fileName.length() - 1);
		return fileName;
	}

	return  getAppInstancePath() + "/" + fileName;
}

bool FileUtils::existsFullPath(const QString& fileName)
{
	QFileInfo file(getFullPath(fileName));
	return file.exists();
}

bool FileUtils::exists(const QString& fileName)
{
	QString newFileName = getFullPath(fileName);
	return existsFullPath(newFileName);
}

QString FileUtils::readAllText(const QString &fileName)
{
	QString fullPath = getFullPath(fileName);
	if(existsFullPath(fullPath)) 
        return readAllTextFullPath(fullPath);
	return "";
}

bool FileUtils::writeText(const QString& fileName, QString& content)
{
	QFile file(FileUtils::getFullPath(fileName));
	if(!file.open(QIODevice::ReadWrite))
	{
		return false;
	}
	QTextStream out(&file);
	out<<content;
	file.close();
	return true;
}

QByteArray FileUtils::readLocalFile(const QString& path)
{
	QString fileName = getFullPath(path);
	QFile file(fileName);

	if(!file.open(QIODevice::ReadOnly))
	{
		return 0;
	}

	return file.readAll();
}

QString FileUtils::readAllTextFullPath(const QString& fileName)
{
	QFile file(fileName);

	if(!file.open(QIODevice::ReadOnly))
	{
		return "";
	}
	QTextStream os(&file);
	QString content = os.readAll();
	file.close();
	return content;
}

bool FileUtils::makeDir(const QString& strDir)
{
	if(strDir.isEmpty())  return false; 
	QString fullPath = getFullPath(strDir);
	QDir newDir;
	if(newDir.exists(fullPath)) return true;

	return newDir.mkdir(fullPath);
}

bool FileUtils::makePath(const QString& strPath)
{
	QString fullPath = getFullPath(strPath);
	QFileInfo pathInfo(fullPath);
	//if (!pathInfo.isDir()) return false;
	if(pathInfo.exists()) return true;

	QDir newPath;
	return newPath.mkpath(fullPath);
}

bool FileUtils::saveFile(const QByteArray* content, const QString& fileName, bool bCover)
{
	if(!content || fileName.isEmpty()) return false;
	//if(makePath(dir)) return false;
	int pos = fileName.lastIndexOf('/');
	if(pos>0) makePath(fileName.left(pos));

	QString fullPath  = getFullPath(fileName);
	QFile file(fullPath);
	
	int nByte = 0;
	if(!bCover)
	{
		if(file.exists()) return false;
	}

	if(!file.open(QIODevice::WriteOnly))
	{
		return false;
	}
	nByte = file.write(content->data(), content->length());
	file.close();
	return nByte<0 ? false: true;
}


QStringList FileUtils::getFileList(const QString& dir)
{
    QDir dirInfo(dir);
    return dirInfo.entryList(QDir::Files);
}

bool FileUtils::copyFile(const QString& srcFile, const QString& destFile)
{
	QString srcFullPath = getFullPath(srcFile);
	QString dstFullPath = getFullPath(destFile);
	if(!existsFullPath(srcFullPath))
        return false;
	if(!srcFullPath.compare(dstFullPath, Qt::CaseInsensitive)) 
        return true;
	QFileInfo fileInfo(dstFullPath);
	if(!fileInfo.exists())	
	{
		makePath(fileInfo.path());
	}
	else if(existsFullPath(dstFullPath)) 
	{
		QFile::remove(dstFullPath);
	}
	return QFile::copy(srcFullPath, dstFullPath);
}

/*
	删除非空文件夹
*/
//
//void FileUtils::delDirExceptMd5(const QString& strDir, const QString& qsMd5)
//{
//    if (qsMd5.isEmpty())
//       return;
//    QString dstDir = getFullPath(strDir);
//    QFileInfo fileInfo(dstDir);
//    // 是否已存在该md5文件
//    bool bNeedCheck = true;
//    if(fileInfo.isDir())
//    {
//        int childCount =0;
//        QString dir = fileInfo.filePath();
//        QDir thisDir(dir);
//        childCount = thisDir.entryInfoList().count();
//        QFileInfoList newFileList = thisDir.entryInfoList();
//        if(childCount > 2)
//        {
//            for(int i=0;i<childCount;i++)
//            {
//                const QString& tmpFileName = newFileList.at(i).fileName();
//                if(tmpFileName == "." || tmpFileName == "..")
//                {
//                    continue;
//                }
//                else
//                {
//                     if (bNeedCheck)
//                     {
//                         // 如果前面已存在该md5文件则删除后面的
//                         bNeedCheck = (!verifyMD5(newFileList.at(i).filePath(), qsMd5));
//                         if (bNeedCheck)
//                         {
//                             delDir(newFileList.at(i));
//                         }
//                     }
//                     else
//                     {
//                         delDir(newFileList.at(i));
//                     }
//                }
//            }
//        }
//        // fileInfo.absoluteDir().rmpath(fileInfo.fileName());
//    }
//    else if(fileInfo.isFile())
//    {
//        fileInfo.absoluteDir().remove(fileInfo.fileName());
//    }
//}

void FileUtils::delDirExceptFileName(const QString& strDir, const QString& qsExcFileName)
{
    QString dstDir = getFullPath(strDir);
    QFileInfo fileInfo(dstDir);
    if(fileInfo.isDir())
    {
        int childCount =0;
        QString dir = fileInfo.filePath();
        QDir thisDir(dir);
        childCount = thisDir.entryInfoList().count();
        QFileInfoList newFileList = thisDir.entryInfoList();
        if(childCount > 2)
        {
            for(int i=0;i<childCount;i++)
            {
                const QString& tmpFileName = newFileList.at(i).fileName();
                if(tmpFileName == "." || tmpFileName == ".." || 
                    tmpFileName.compare(qsExcFileName, Qt::CaseInsensitive))
                {
                    continue;
                }
                else
                {
                    delDir(newFileList.at(i));
                }
            }
        }
        // fileInfo.absoluteDir().rmpath(fileInfo.fileName());
    }
    else if(fileInfo.isFile())
    {
        fileInfo.absoluteDir().remove(fileInfo.fileName());
    }
}

void FileUtils::delDirExceptZip(const QString& strDir)
{
    QString dstDir = getFullPath(strDir);
    QFileInfo fileInfo(dstDir);
    if(fileInfo.isDir())
    {
        int childCount =0;
        QString dir = fileInfo.filePath();
        QDir thisDir(dir);
        childCount = thisDir.entryInfoList().count();
        QFileInfoList newFileList = thisDir.entryInfoList();
        if(childCount > 2)
        {
            for(int i = 0;i < childCount; ++i)
            {
                const QString& tmpFileName = newFileList.at(i).fileName();
                const QString& suffix = newFileList.at(i).suffix();
                if(tmpFileName == "." || tmpFileName == ".." || 
                    suffix.compare("zip", Qt::CaseInsensitive) == 0)
                {
                    continue;
                }
                else
                {
                    delDir(newFileList.at(i));
                }
            }
        }
        fileInfo.absoluteDir().rmpath(fileInfo.fileName());
    }
    else if(fileInfo.isFile())
    {
        fileInfo.absoluteDir().remove(fileInfo.fileName());
    } 
}

void FileUtils::rmPath(const QString& strDir, bool bRemainFolder)
{
	QString dstDir = getFullPath(strDir);
    QDir dir(strDir);
	QFileInfo file(dstDir);
	delDir(file);
    if (bRemainFolder)
    {
        dir.mkpath(strDir);
    }
}

void FileUtils::delDir(const QFileInfo& fileInfo)
{
    QString name = fileInfo.filePath();
	if(fileInfo.isDir())
	{
		int childCount =0;
		QString dir = fileInfo.filePath();
		QDir thisDir(dir);
		childCount = thisDir.entryInfoList().count();
		QFileInfoList newFileList = thisDir.entryInfoList();
		if(childCount > 2)
        {
			for(int i=0;i<childCount;i++)
            {
				if(newFileList.at(i).fileName() == "." || newFileList.at(i).fileName() == "..")
                {
					continue;
				}

				delDir(newFileList.at(i));
			}
		}

		fileInfo.absoluteDir().rmpath(fileInfo.fileName());
	}
	else if(fileInfo.isFile())
	{
		fileInfo.absoluteDir().remove(fileInfo.fileName());
	}
}


int FileUtils::fileCount(const QString& filePath)
{
	int fCount = 0;
	fileCounter(filePath, &fCount);
	return fCount;
}

void FileUtils::fileCounter(const QString& filePath, int* pCounter)
{
	QFileInfo curFile(filePath);
	if(!curFile.exists()) return;

	if (curFile.isDir())
	{
		QString dir = curFile.filePath();
		QDir curDir(dir);
		int childCount = curDir.entryInfoList().count();
		QFileInfoList newFileList = curDir.entryInfoList();
		if(childCount > 2)
		{
			for(int i = 0;i< childCount; i++)
			{
				if(newFileList.at(i).fileName() == "." || newFileList.at(i).fileName() == "..")
					continue;
				fileCounter(newFileList.at(i).filePath(), pCounter);
			}
		}
	}
	else
	{
		if(pCounter) *pCounter += 1;
	}
}

bool FileUtils::isAbsolute(const QString& filePath) 
{
	QFileInfo fInfo(filePath);
	return fInfo.isAbsolute();
}

bool FileUtils::rmFile(const QString& fileName)
{
	if(!FileUtils::exists(fileName)) return true;
	return QFile::remove(FileUtils::getFullPath(fileName));
}

//************************************************************************
// Method:    renameFile	重命名文件
// Access:    public static 
// Returns:   bool
// Parameter: fileName
// Parameter: newName
//************************************************************************
bool FileUtils::renameFile( const QString& fileName, const QString& newName )
{
	QString srcName = getFullPath(fileName);
	QString destName = getFullPath(newName);
	if(!FileUtils::exists(fileName)) return false;
	return QFile::rename(srcName, destName);
}

//***********************************************************************
// Method:    createFile	创建文件
// Access:    public static 
// Returns:   bool
// Parameter: fileName		创建的文件路径
// Parameter: initSize		文件的初始化大小
// Parameter: bForce		是否强制创建不存在的路径,该值为true时,
//							如果路径不存在,创建路径;
//***********************************************************************
bool FileUtils::createFile( const QString& fileName, int initSize /*= 0*/, bool bForce /*= false*/ )
{
	QString fileFullPath = getFullPath(fileName);
	QFileInfo fileInfo(fileFullPath);
	if(fileFullPath.isEmpty() || fileInfo.isDir())
		return false;

	QFile file(fileFullPath);
	if(bForce && !QFileInfo(fileInfo.path()).exists())
		makePath(fileInfo.path());
	if(!file.open(QIODevice::ReadWrite)) return false;
	if(initSize > 0 && !file.resize(initSize)) 
	{
		file.close();
		rmFile(fileFullPath);
		return false;
	}
	file.close();
	return true;
}

qint64 FileUtils::getFileSize( const QString& filePath )
{
	QString fileFullPath = getFullPath(filePath);
	QFileInfo info(fileFullPath);
	return info.size();
}

QString FileUtils::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (QFile::exists(basename)) {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i)))
            ++i;
        basename += QString::number(i);
    }

    if (basename.isEmpty())
        basename = "download";
    return basename;
}

bool FileUtils::saveToDisk(const QString &filename, QIODevice *reply)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(reply->readAll());
    file.close();

    return true;
}

QString FileUtils::cdUp(const QString& fileName)
{
    QFileInfo fileInfo(fileName);
    if (!fileInfo.exists())
        return "";
    QDir dir(fileName);
    dir.cdUp();
    return (QDir::toNativeSeparators(dir.absolutePath()));
}

//QStringList FileUtils::extractDir(const QString &fileName, const QString& filePath, bool isDelSrcFile)
//{
//    QStringList sl = JlCompress::extractDir(fileName, filePath);
//    if (isDelSrcFile)
//    {
//        QFile::remove(fileName);
//    }
//    return sl;
//}

QFileInfoList FileUtils::getFileInfoList(const QString& qsPath)
{
    QStringList list;
    QDir dir(qsPath);
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i != folderList.size(); i++)
    {
        QString name = folderList.at(i).absoluteFilePath();
        QFileInfoList childFileList = getFileInfoList(name);
        fileList.append(childFileList);
    }

    return fileList;
}

void FileUtils::copyFolder(const QString &srcFolder,const QString & dstFolder, const QFileInfoList &excludeFiles)
{
    if (excludeFiles.size() < 1 || srcFolder.isEmpty() || dstFolder.isEmpty())
        return;
    QDir dir( srcFolder );
    QStringList sl;
    foreach (QFileInfo info, excludeFiles)
    {
        sl.append(info.filePath());
    }

    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();
    int count = list.count();
    for (int index = 0; index < count; index++)
    {
        QFileInfo fileInfo = list.at(index);
        QString fileName = fileInfo.fileName();
        if( excludeFiles.indexOf( fileName , Qt::CaseInsensitive) != -1 )
            continue;
        if( !dir.exists(dstFolder) )
            dir.mkpath(dstFolder);
        QString newSrcFileName = srcFolder + "/" + fileInfo.fileName();
        QString newDstFileName = dstFolder + "/" + fileInfo.fileName();
        QFile::copy(newSrcFileName, newDstFileName);
    }

    dir.setFilter(QDir::Dirs);
    list = dir.entryInfoList();
    count = list.count();
    for (int index = 0; index < count; index++)
    {
        QFileInfo fileInfo = list.at(index);
        QString fileName = fileInfo.fileName();
        if (fileName == "." || fileName == "..")
            continue;
        if( fileInfo.isDir())
        {
            if( excludeFiles.indexOf( fileName , Qt::CaseInsensitive) != -1 )
                continue;
            QString newSrcFolder = srcFolder + "/" + fileName;
            QString newDstFolder = dstFolder + "/" + fileName;
            QFileInfoList fileList = getFileInfoList(newSrcFolder);
            copyFolder( newSrcFolder , newDstFolder ,fileList);
        }
    }
}
/**************************************************************************
* 函数名:   copyFolder
* 功能:     
* 参数:   
*....@[in]  const QString & srcFolder
*....@[in]  const QString & dstFolder
*....@[out] void
* 返回值:   
* 时间:     2017/03/14 11:02
* 作者:   Leo
*/
void FileUtils::copyFolder( const QString &srcFolder,const QString & dstFolder )
{
    if (srcFolder.isEmpty() || dstFolder.isEmpty())
        return;
    QDir dir(srcFolder);
    QFileInfoList excludeFiles = getFileInfoList(srcFolder);
    QStringList sl;
    foreach (QFileInfo info, excludeFiles)
    {
        sl.append(info.filePath());
    }

    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();
    int count = list.count();
    for (int index = 0; index < count; index++)
    {
        QFileInfo fileInfo = list.at(index);
        QString fileName = fileInfo.fileName();
        if( excludeFiles.indexOf( fileName , Qt::CaseInsensitive) != -1 )
            continue;
        if( !dir.exists(dstFolder) )
            dir.mkpath(dstFolder);
        QString newSrcFileName = srcFolder + "/" + fileInfo.fileName();
        QString newDstFileName = dstFolder + "/" + fileInfo.fileName();
        QFile::copy(newSrcFileName, newDstFileName);
    }

    dir.setFilter(QDir::Dirs);
    list = dir.entryInfoList();
    count = list.count();
    for (int index = 0; index < count; index++)
    {
        QFileInfo fileInfo = list.at(index);
        QString fileName = fileInfo.fileName();
        if (fileName == "." || fileName == "..")
            continue;
        if( fileInfo.isDir())
        {
            if( excludeFiles.indexOf( fileName , Qt::CaseInsensitive) != -1 )
                continue;
            QString newSrcFolder = srcFolder + "/" + fileName;
            QString newDstFolder = dstFolder + "/" + fileName;
            QFileInfoList fileList = getFileInfoList(newSrcFolder);
            copyFolder( newSrcFolder , newDstFolder ,fileList);
        }
    } 
}

QString FileUtils::FindAppointFile(const QString& srcFolder, const QString& suffix)
{
    QDir dir(srcFolder );
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();
    int count = list.count();
    for (int index = 0; index < count; index++)
    {
        QFileInfo fileInfo = list.at(index);
        QString fileName = fileInfo.fileName();
        if( fileName.indexOf(suffix , Qt::CaseInsensitive) != -1 )
        {
            
            return fileInfo.filePath();
        }
    }

    dir.setFilter(QDir::Dirs);
    list = dir.entryInfoList();
    count = list.count();
    for (int index = 0; index < count; index++)
    {
        QFileInfo fileInfo = list.at(index);
        QString qs = fileInfo.filePath();
        QString fileName = fileInfo.fileName();
        if (fileName == "." || fileName == "..")
            continue;
        if( fileInfo.isDir() )
        {
            QString qs = FindAppointFile(fileInfo.filePath(), suffix);
            if (!qs.isEmpty())
                return qs;
            else
                continue;
        }
    }
    return "";
}
/**************************************************************************
* 函数名:   getFileName
* 功能:     
* 参数:   
*....@[in]  const QString & filePath
*....@[out] QT_NAMESPACE::QString
* 返回值:   
* 时间:     2017/02/27 21:25
* 作者:   Leo
*/
QString FileUtils::getFileName( const QString& filePath )
{
    QFileInfo info(filePath);
    return info.fileName();
}
/**************************************************************************
* 函数名:   getBaseName
* 功能:     
* 参数:   
*....@[in]  const QString & filePath
*....@[out] QT_NAMESPACE::QString
* 返回值:   
* 时间:     2017/02/27 21:51
* 作者:   Leo
*/
QString FileUtils::getBaseName( const QString& filePath )
{
    QFileInfo info(filePath);
    return info.baseName();
}
/**************************************************************************
* 函数名:   getSuffix
* 功能:     
* 参数:   
*....@[in]  const QString & filePath
*....@[out] QT_NAMESPACE::QString
* 返回值:   
* 时间:     2017/02/27 21:51
* 作者:   Leo
*/
QString FileUtils::getSuffix( const QString& filePath )
{
    QFileInfo info(filePath);
    return info.suffix();
}
/**************************************************************************
* 函数名:   getParentBaseName
* 功能:     
* 参数:   
*....@[in]  const QString & filePath
*....@[out] QT_NAMESPACE::QString
* 返回值:   
* 时间:     2017/02/27 21:53
* 作者:   Leo
*/
QString FileUtils::getParentBaseName( const QString& filePath )
{
    QString parentPath = FileUtils::cdUp(filePath);
    QFileInfo info(parentPath);
    return info.fileName();
}
//bool FileUtils::verifyMD5(const QString& filePath, const QString& qsMD5)
//{
//    QFile zipFile(filePath);
//
//    QString zipFileMD5;
//    if (zipFile.open(QIODevice::ReadOnly))
//    {
//        zipFileMD5 = QCryptographicHash::hash(zipFile.readAll(),QCryptographicHash::Md5).toHex();
//    }
//
//    zipFile.close();
//
//    //return true;
//    return (zipFileMD5.compare(qsMD5, Qt::CaseInsensitive) == 0);
//}
//
//bool FileUtils::verifyAllPathMD5(const QString& filePath, const QString& qsMd5, QString& correctFilePath)
//{
//    QFileInfoList fileInfoList = FileUtils::getFileInfoList(filePath);
//    foreach(QFileInfo fileInfo, fileInfoList)
//    {
//        correctFilePath = fileInfo.filePath();
//        if (FileUtils::verifyMD5(correctFilePath, qsMd5))
//        {
//            return true;
//        }
//    }
//    return false;
//}
bool FileUtils::copyFileToPath(const QString& sourceDir ,const QString& toDir, bool coverFileIfExist)
{
    if (sourceDir == toDir)
    {
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }
    QDir *createfile = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }//end if
     delete createfile;
    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}

bool FileUtils::deleteFile(const QString& fileName)
{
    QFileInfo info(fileName);
    if (info.exists())
    {
        QFile::remove(fileName);
        return true;
    }
    return false;
}

/**************************************************************************
* 函数名:   getMd5
* 功能:
* 参数:
*....@[in]  const QString & filePath
*....@[out] const QString
* 返回值:
* 时间:     2017/11/02 20:07
* 作者:   Leo
*/
const QString FileUtils::getMd5(const QString & filePath)
{
    QFile theFile(filePath);
    theFile.open(QIODevice::ReadOnly);
    QByteArray ba = QCryptographicHash::hash(theFile.readAll(), QCryptographicHash::Md5);
    theFile.close();
    return QString(ba.toHex());
}

