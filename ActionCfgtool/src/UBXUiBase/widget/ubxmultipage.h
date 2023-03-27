/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：ubxmultipage.h
* 创建时间：20160720
* 文件标识：
* 文件摘要：多页控件
*
* 当前版本：1.0.0.0
* 作    者：hwx
*/

#ifndef UBXMULTIPAGE_H
#define UBXMULTIPAGE_H
#include "ubxuibase_global.h"
#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class UBXMultiPage;
}

class UBXUIBASE_EXPORT UBXMultiPage : public QWidget
{
    Q_OBJECT

public:
    explicit UBXMultiPage(QWidget *parent = 0);
    ~UBXMultiPage();

public:
    /************************************
    * 名称: UpdatePages
    * 功能: 更新页码显示
    * 参数: [in]nTotalPage 总页数
    * 返回:   void
    * 时间:   2016/04/14
    * 作者:   hels
    ************************************/
    void UpdatePages(int nTotalPage);

    /************************************
    * 名称: getCurrentPage
    * 功能: get the current page num
    * 返回:   int 当前页数
    * 时间:   2016/06/13
    * 作者:   asu
    ************************************/
    int getCurrentPage()const;

    /************************************
    * 名称: setCurrentPage
    * 功能: 设置当前页
    * 参数: [in]nCurPage 当前页
    * 返回:   void
    * 时间:   2016/06/13
    * 作者:   hwx
    ************************************/
   void setCurrentPage(int nCurPage);

   /************************************
   * 名称: getTotalPage
   * 功能: 获取总页数
   * 返回:   int 总页数
   * 时间:   2016/06/13
   * 作者:   hwx
   ************************************/
   int getTotalPage() const;

protected:
    /************************************
    * 名称: InitBtns
    * 功能: 初始化页码按钮组
    * 返回:   void
    * 时间:   2016/06/13
    * 作者:   asu
    ************************************/
    void InitBtns();

signals:
    void sigCurrentPageChanged(int nCurPage);
    void sigPrevPageClicked();
    void sigNextPageClicked();

protected slots:
    /************************************
    * 名称: OnBtnsButtonToggled
    * 功能: 第一排按钮组槽函数
    * 参数: [in]int-id 发送信号的按钮id
    * 参数: [in]bool-bChecked 按钮是否选中
    * 返回:   void
    * 时间:   2016/06/13
    * 作者:   asu
    ************************************/
    void onBtnsButtonToggled(int id, bool bChecked);

    /************************************
    * 名称: OnPrevPageClicked
    * 功能: 上一页按钮槽函数
    * 返回:   void
    * 时间:   2016/06/14
    * 作者:   asu
    ************************************/
    void onPrevPageClicked();

    /************************************
    * 名称: OnNextPageClicked
    * 功能: 下一页按钮槽函数
    * 返回:   void
    * 时间:   2016/04/14
    * 作者:   hels
    ************************************/
    void onNextPageClicked();

private:
     int m_nCurrentPage;//记录当前浏览的下载项页码
     int m_nTotalPage;
     int m_nPagebtns;//页码按钮个数

     QButtonGroup m_btnGroupForPages;//分页按钮组

private:
    Ui::UBXMultiPage *ui;
};

#endif // UBXMULTIPAGE_H
