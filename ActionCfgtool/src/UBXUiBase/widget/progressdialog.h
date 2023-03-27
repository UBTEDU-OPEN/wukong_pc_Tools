/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CProgressDialog
* 创建时间：20161009
* 文件标识：
* 文件摘要：Progress Dialog
*
* 当前版本：1.0.0.0
* 作    者：asu
*/


#ifndef CPROGRESSDIALOG_H
#define CPROGRESSDIALOG_H
#include "ubxuibase_global.h"
#include <QProgressDialog>
class QKeyEvent;
class QString;
class UBXUIBASE_EXPORT CProgressDialog : public QProgressDialog
{
public:
    CProgressDialog(QWidget * parent = 0, Qt::WindowFlags flags = 0);
    CProgressDialog(const QString &labelText, const QString &cancelButtonText,
                    int minimum, int maximum, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~CProgressDialog();

public:
    void setKeyPressEnable(bool bKeyEnable);

protected:
    void keyPressEvent(QKeyEvent *) override;
    virtual bool eventFilter(QObject *, QEvent *)override;

private:
    void setUpEventFilter();

private:
    bool m_bKeyEnable;
};

#endif // CPROGRESSDIALOG_H
