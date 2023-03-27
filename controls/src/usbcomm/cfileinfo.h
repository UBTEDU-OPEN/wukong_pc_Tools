#ifndef CFILEINFO_H
#define CFILEINFO_H

#include <QString>
#include <QList>
#include "usbcomm_global.h"

class USBCOMMSHARED_EXPORT CFileInfo
{
public:
    CFileInfo();
    CFileInfo(const QString& strFilePath);
    virtual ~CFileInfo ();

public:
    void setFilePath (const QString& strFilePath){
        m_strFilePath = strFilePath;
    }

    QString getFilePath () const {
        return m_strFilePath;
    }

    void setSendEnable (bool bSendEnable){
        m_bSendEnable = bSendEnable;
    }

    bool getSendEnable () const {
        return m_bSendEnable;
    }

    QString getZipPath () const {
        return m_strZipPath;
    }

    QString getZipName () const {
        return m_strZipName;
    }

public:
    bool  getFileInfo ();
    bool  getFileContent (const QString& strFilePath, QByteArray& arry);
    QString md5ToHex();
    bool  delZip ();
    bool  compress (const QString& strZipPath,
                    const QString& strPath,
                    bool isDir = false);

public:
    QString  m_strFileName;     /**< 文件名称，包含后缀 */
    int      m_nFileSize;       /**< 文件大小，单位：byte */
    QByteArray  m_strMd5;       /**< 文件内容MD5校验码 */
    int      m_nRobotPathType;  /**< 机器人路径类型, 由机器人端提供 */
    QString  m_strLocalPath;    /**< 本地文件路径，不包含文件名称 */

protected:
    QString  m_strFilePath;     /**< 文件全路径 */
    bool     m_bSendEnable;
    QString  m_strZipName;
    QString  m_strZipPath;      /**< 文件夹压缩路径 */
};

typedef QList<CFileInfo*>  CFileInfoList;

class USBCOMMSHARED_EXPORT CFileInfos
{
public:
    CFileInfos ();
    virtual ~CFileInfos ();

public:
    int    count();
    void   clear();
    void   add (const QString& strFilePath);
    void   add (const QStringList& strFilePathList);
    void   add (CFileInfo* p);
    bool   remove(const QString& strFilePath);
    bool   update(const QString& strFilePath);
    CFileInfo* find (const QString& strFilePath);
    CFileInfo* findName (const QString& strFileName);
    CFileInfo* at(int i);

public:
    CFileInfoList m_listFileInfo;
};

#endif // CFILEINFO_H
