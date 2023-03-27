#include "cfileinfo.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QCryptographicHash>

#include <QCoreApplication>
#include "JlCompress.h"

CFileInfo::CFileInfo()
{
    m_strFileName = "";
    m_nFileSize = 0;
    m_strMd5 = "";
    m_nRobotPathType = 0;
    m_strLocalPath = "";
    m_strFilePath = "";
    m_strZipPath = "";
    m_strZipName = "";
    m_bSendEnable = true;
}

CFileInfo::CFileInfo(const QString& strFilePath)
    : m_strFilePath (strFilePath)
{
    m_strFileName = "";
    m_nFileSize = 0;
    m_strMd5 = "";
    m_nRobotPathType = 0;
    m_strLocalPath = "";
    m_bSendEnable = true;
    m_strZipPath = "";
    m_strZipName = "";
    getFileInfo();
}

CFileInfo::~CFileInfo ()
{
    delZip ();
}

bool CFileInfo::getFileInfo ()
{
    QString strZipPath = QCoreApplication::applicationDirPath() + "/temp";
    QDir dir(strZipPath);
    if ( !dir.exists() ){
        dir.mkdir(strZipPath);
    }

    if ( m_strFilePath.isEmpty() ){
        return false;
    }

    bool isDir = false;
    QString strPath = m_strFilePath;
    QFileInfo fileInfo (strPath);
    if ( fileInfo.isDir() )
    {
        isDir = true;
    }

    QString strZipName = fileInfo.fileName();
    strZipPath += "/" + strZipName + ".zip";
    if ( !compress (strZipPath, strPath, isDir) )
    {
        return false;
    }
    m_strZipName = strZipName + ".zip";
    m_strZipPath = strZipPath;

    QByteArray contentArray;
    if ( !getFileContent (m_strZipPath, contentArray) ){
        return false;
    }

    m_strFileName = fileInfo.fileName();
    m_strLocalPath = fileInfo.path();
    m_nFileSize = contentArray.toStdString().length();
    m_strMd5 = QCryptographicHash::hash(contentArray, QCryptographicHash::Md5);

    return true;
}

bool CFileInfo::getFileContent (const QString& strFilePath, QByteArray& arry)
{
    QFile file(strFilePath);
    if ( !file.exists() )
    {
        qDebug()<<"--Warnning, can't find file "<< strFilePath;
        return false;
    }

    if ( !file.open(QIODevice::ReadOnly) )
    {
        qDebug()<<"--Warnning, can't open file "<< strFilePath;
        return false;
    }

    arry = file.readAll();
    file.close();
    return true;
}

QString CFileInfo::md5ToHex()
{
    return QString(m_strMd5.toHex());
}

bool CFileInfo::delZip ()
{
    QFile file(m_strZipPath);
    if ( file.exists() )
    {
        file.remove();
    }
    return true;
}

bool CFileInfo::compress (const QString& strZipPath,
                           const QString& strPath,
                           bool isDir/* = false*/)
{
    if ( isDir )
    {
        /**
         * 注意： JlCompress::compressDir压缩文件夹时，只能压缩文件夹内的文件，不能包含文件夹名称。此处需要修改 Quazip源码。
         * 修改如下： JlCompress.cpp  bool JlCompress::compressDir(QString fileCompressed, QString dir, bool recursive, QDir::Filters filters)
         *
         *     // Aggiungo i file e le sotto cartelle
         *      QFileInfo info (dir);                       // 此处增加
         *      QString strParentDir = info.path();         // 此处增加
         *      if (!compressSubDir(&zip,dir,strParentDir,recursive, filters)) {
         *          QFile::remove(fileCompressed);
         *          return false;
         *      }
         */
        if ( !JlCompress::compressDir (strZipPath, strPath) )
        {
            return false;
        }
    }
    else
    {
        if ( !JlCompress::compressFile (strZipPath, strPath) )
        {
            return false;
        }
    }

    return true;
}

CFileInfos::CFileInfos ()
{
    m_listFileInfo.clear();
}

CFileInfos::~CFileInfos ()
{
    clear();
}

int CFileInfos::count()
{
    return m_listFileInfo.count();
}

void CFileInfos::clear()
{
    qDeleteAll (m_listFileInfo);
    m_listFileInfo.clear();
}

CFileInfo* CFileInfos::find (const QString& strFilePath)
{
    int nCount = m_listFileInfo.count();
    for ( int i = 0; i < nCount; i ++ )
    {
        CFileInfo* p = m_listFileInfo.at(i);
        if ( strFilePath == p->getFilePath() )
        {
           return p;
        }
    }
    return  NULL;
}

CFileInfo* CFileInfos::findName (const QString& strFileName)
{
    int nCount = m_listFileInfo.count();
    for ( int i = 0; i < nCount; i ++ )
    {
        CFileInfo* p = m_listFileInfo.at(i);
        if ( strFileName == p->m_strFileName )
        {
           return p;
        }
    }
    return  NULL;
}

CFileInfo* CFileInfos::at(int i)
{
    if ( i >= count() ) {
        return NULL;
    }

    return m_listFileInfo.at(i);
}

bool CFileInfos::remove (const QString& strFilePath)
{
    CFileInfo* p = find(strFilePath);
    if ( p )
    {
        m_listFileInfo.removeOne(p);
        delete p;
        p = NULL;
    }
    return true;
}

bool CFileInfos::update (const QString& strFilePath)
{
    CFileInfo* p = find(strFilePath);
    if ( p )
    {
        p->setFilePath(strFilePath);
        p->getFileInfo();
    }
    return true;
}

void CFileInfos::add (const QString& strFilePath)
{
    m_listFileInfo.append(new CFileInfo(strFilePath));
}

void CFileInfos::add (const QStringList& strFilePathList)
{
    int nCount = strFilePathList.count();
    for ( int i = 0; i < nCount; i ++ )
    {
        QString strFilePath = strFilePathList.at(i);
        add (strFilePath);
    }
}

void CFileInfos::add (CFileInfo* p)
{
    m_listFileInfo.append(p);
}
