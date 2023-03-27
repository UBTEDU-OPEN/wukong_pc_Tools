/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
* 
* 文件名称：UpdateProgressDlg.h
* 创建时间：2015/06/02 16:17
* 文件标识：
* 文件摘要：更新进度窗体类
* 
* 当前版本：1.0.0.0
* 作    者：ZDJ
* 完成时间：2015/06/02 16:17
* 版本摘要：
*/
#ifndef UPDATEPROGRESSDLG_H
#define UPDATEPROGRESSDLG_H

#include "ubxupdatemodule_global.h"
#include "Global.h"

#include <QDialog>

namespace Ui {
class UpdateProgressDlg;
}

class MyDownloadProc : public QObject
{
    Q_OBJECT
public:
    explicit MyDownloadProc(QWidget* pParent);
    ~MyDownloadProc();

public slots:
    void start();

public:
    /**************************************************************************
    * 函数名: DownProc
    * 功能: 下载执行函数
    * 时间: 2015/06/02 16:39
    * 作者: ZDJ
    */
    void DownProc(LPVOID lParam);
private:
    QWidget* m_pParent;

};

class UBXUPDATEMODULESHARED_EXPORT CUpdateProgressDlg : public QDialog
{
    Q_OBJECT    
public:
    explicit CUpdateProgressDlg(QWidget* pParent = NULL);
    ~CUpdateProgressDlg();

    /**************************************************************************
    * 函数名: OnDownloadResult
    * 功能: 下载完成通知
    * 参数:
    *    @[in ] bRet: 下载结果，成功true，失败false
    *    @[in ] pData: 通知数据
    *    @[in ] nDataLen: 通知数据长度
    * 返回值: void
    * 时间: 2016/03/09 16:30
    * 作者: ZDJ
    */
    void OnDownloadResult(bool bRet, char* pData, int nDataLen);

	/**************************************************************************
	* 函数名: DoCheck
	* 功能: 检查更新
	* 时间: 2015/06/02 16:30
	* 作者: ZDJ
	*/
    void DoCheck();

protected:
    virtual void customEvent(QEvent* event);

private slots:
    void OnCancel();

public:
    bool            m_bShowTipOnFailed;//未检测到更新时候是否提示	

    QString          m_sUpdateURL;//更新地址
    QString          m_sLocalVer;//本地版本
    emCheckUpdateType  m_eChkType;
    FILEINFO        m_LatestVersionInfo;//下载的文件信息


    Ui::UpdateProgressDlg *ui;

    MyDownloadProc* m_pDownloadTask;
};

#endif
