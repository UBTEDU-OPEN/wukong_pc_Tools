#ifndef CTHREADCOPYFILE_H
#define CTHREADCOPYFILE_H

#include <QThread>

class CThreadCopyFile : public QThread
{
public:
    explicit CThreadCopyFile(QObject * parent = 0);
    ~CThreadCopyFile();

    // 设置机器人内存根目录
    void setRobotRootPath(const QString &strPath) { m_strRobotRootPath = strPath; }
    void setBinFilePath(const QString &strPath) { m_strBinFileLocalPath = strPath; }

    void setLanguageFlag(bool bFlag) { m_bIsChinese = bFlag; }
    void setRobotFlag(bool bIs1P) { m_bIs1P = bIs1P; }

protected:
    virtual void run() override;

private:
    // 拷贝内置动作到机器人
    bool copyDefaultActionToRobot();

    // 拷贝指定路径下的所有文件到指定目录
    bool copyDir(const QString &strSrcDir, const QString &strDestDir);

    // 拷贝bin文件到机器人
    bool copyBinFile();

private:
    QString m_strRobotRootPath;
    QString m_strBinFileLocalPath;

    bool m_bIsChinese; // 中英文标志
    bool m_bIs1P;   // 1P/1S标志

};

#endif // CTHREADCOPYFILE_H
