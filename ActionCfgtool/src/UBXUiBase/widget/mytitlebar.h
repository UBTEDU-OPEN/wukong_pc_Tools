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

#ifndef MYTITLEBAR_H
#define MYTITLEBAR_H
#include "ubxuibase_global.h"
#include <QWidget>
#include <QLabel>
#include <QtUiPlugin/QDesignerExportWidget>

const QString MY_TITLE_BAR_OBJECT_NAME = "MyTitleBar";

namespace Ui {
class myTitleBar;
}

class UBXUIBASE_EXPORT  MyTitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit MyTitleBar(QWidget *parent = 0);
    ~MyTitleBar();

public:
    enum emTitleSystemStyle
    {
        eTitleStyleMac = 0,
        eTitleStyleWin
    };


public:
    QLabel* getTitleLabel()const;
    void setTitleLabel(const QString& title);
    void setTitleStyle(emTitleSystemStyle style);

    //for control the button and title position
    void setLayoutMargins(int left, int top, int right, int bottom);


signals:
    void sigClose();


protected:
    virtual void showEvent(QShowEvent *);
        void paintEvent(QPaintEvent *);

protected:
virtual void resizeEvent(QResizeEvent *);

private:
    void resizeToParent();


private:
    Ui::myTitleBar *ui;
};

#endif // MYTITLEBAR_H
