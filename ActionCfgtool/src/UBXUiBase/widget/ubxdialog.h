/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CUBXDialog
* 创建时间：20160816
* 文件标识：
* 文件摘要：the base dialog
*
* 当前版本：1.0.0.0
* 作    者：asu
*/

#ifndef CUBXDIALOG_H
#define CUBXDIALOG_H
#include "ubxuibase_global.h"
#include <QDialog>

class UBXUIBASE_EXPORT CUBXDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CUBXDialog(QWidget *parent = 0);
    virtual ~CUBXDialog();

public:
    //设置窗体默认样式
    void setEnableMove(bool bEnableMove);
    void setTitleBarHeight(const int& titleBarHeight);

protected:
    void paintEvent(QPaintEvent *);

    //重写以下函数实现移动窗体功能
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);



private:
    bool m_bLeftDown;
    bool m_bEnableMove;
    int m_nTitleBarHeight;

    QPoint m_ptPre;
};

#endif // CUBXDIALOG_H



