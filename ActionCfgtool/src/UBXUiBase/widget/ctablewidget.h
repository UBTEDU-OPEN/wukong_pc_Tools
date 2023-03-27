/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CTableWidget.h
* 创建时间：20161107
* 文件标识：
* 文件摘要：CTableWidget
*
* 当前版本：1.0.0.0
* 作    者：asu
*/

#ifndef CTABLEWIDGET_H
#define CTABLEWIDGET_H


#include "ubxuibase_global.h"
#include <QTableWidget>
#include <QCheckBox>


class UBXUIBASE_EXPORT CTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit CTableWidget(QWidget *parent = NULL);
    ~CTableWidget();

public:
    /************************************
    * 名称: setAllCheckBoxs
    * 功能:
    * 参数: [in]pTab
    *      [in]nCheckBoxColPosition
    *      [in]state
    * 返回:   void
    * 时间:   20161107
    * 作者:   asu
    ************************************/
    static void setAllCheckBoxs(QTableWidget* pTab, int nCheckBoxColPosition, Qt::CheckState state);

    /************************************
    * 名称: isAllCheckBoxsChecked
    * 功能: get the checkboxs status
    * 参数: [in]pTab
    *      [in]nCheckBoxColPosition
    * 返回:   bool
    * 时间:   20161108
    * 作者:   asu
    ************************************/
    static bool isAllCheckBoxsChecked(const QTableWidget* const pTab, const int nCheckBoxColPosition);

    void startRowChangeTimer(const int nMsec);

signals:
    void sigRowCountChanged(int newCount);

protected:
    virtual void timerEvent(QTimerEvent* event);

private:
    int m_nCurrentRowCount;
    int m_nTimerIDUpdateRowChange;
};

#endif // CTABLEWIDGET_H
