/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：MyTitleBar.h
* 创建时间：20160715
* 文件标识：
* 文件摘要：the widget title bar
*
* 当前版本：1.0.0.0
* 作    者：asu
*/

#ifndef UBXWIDGET_H
#define UBXWIDGET_H
#include "ubxuibase_global.h"
#include <QWidget>

const QString DEFAULT_ZXLIB_QSS_PATH  = ":/res/qss/widget.qss";

const int TITLE_BAR_HEIGHT = 40;

class UBXUIBASE_EXPORT UBXWidget: public QWidget
{
    Q_OBJECT

public:
    explicit UBXWidget(QWidget *parent = 0);
    virtual ~UBXWidget();

public:
    //设置窗体默认样式
    static bool setDefaultStyle(QWidget *pWidget, const QString& qssPath);
    void setEnableMove(bool bEnableMove);
    void setTitleBarHeight(const int& titleBarHeight);

    static void applyUBXWinFlags(QWidget *pWidget);
    //exec dialog modal
    static void execByAppModal(QWidget *pWidget);
    //show on top
    static void showAlwayOnTop(QWidget *pWidget);

    void paintEvent(QPaintEvent *);
    static void paintEvent(QWidget *pWidget);

    //窗口居中
    static void CenterWidget(const QWidget*const pParent, QWidget*const pChild);


protected:
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

#endif // UBXWIDGET_H
