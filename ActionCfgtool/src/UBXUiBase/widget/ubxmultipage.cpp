/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：ubxmultipage.cpp
* 创建时间：20160720
* 文件标识：
* 文件摘要：多页控件
*
* 当前版本：1.0.0.0
* 作    者：hwx
*/

#include <QPainter>
#include <QCheckBox>

#include "ubxmultipage.h"
#include "ui_ubxmultipage.h"
#include "ubxwidgetconst.h"

UBXMultiPage::UBXMultiPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UBXMultiPage)
{
    ui->setupUi(this);

    m_nCurrentPage = -1;
    m_nTotalPage = -1;
    m_nPagebtns = 10;

    InitBtns();    

    ui->pbNumDot1->hide();
    ui->pbNumDot2->hide();

    UpdatePages(1);
}

UBXMultiPage::~UBXMultiPage()
{
    delete ui;
}

/************************************
* 名称: InitBtns
* 功能: 初始化页码按钮组
* 返回:   void
* 时间:   2016/06/13
* 作者:   asu
************************************/
void UBXMultiPage::InitBtns()
{
    m_btnGroupForPages.addButton(ui->pbNum1, UBXUiBaseConstants::BTN_PAGES_NUM_1);
    m_btnGroupForPages.addButton(ui->pbNum2, UBXUiBaseConstants::BTN_PAGES_NUM_2);
    m_btnGroupForPages.addButton(ui->pbNum3, UBXUiBaseConstants::BTN_PAGES_NUM_3);
    m_btnGroupForPages.addButton(ui->pbNum4, UBXUiBaseConstants::BTN_PAGES_NUM_4);
    m_btnGroupForPages.addButton(ui->pbNum5, UBXUiBaseConstants::BTN_PAGES_NUM_5);
    m_btnGroupForPages.addButton(ui->pbNum6, UBXUiBaseConstants::BTN_PAGES_NUM_6);
    m_btnGroupForPages.addButton(ui->pbNum7, UBXUiBaseConstants::BTN_PAGES_NUM_7);
    m_btnGroupForPages.addButton(ui->pbNum8, UBXUiBaseConstants::BTN_PAGES_NUM_8);
    m_btnGroupForPages.addButton(ui->pbNum9, UBXUiBaseConstants::BTN_PAGES_NUM_9);
    m_btnGroupForPages.addButton(ui->pbNum10, UBXUiBaseConstants::BTN_PAGES_NUM_10);

    //记录按钮对应的真实页码到按钮属性中
    // > 0 真实页码值, = -1 不显示
    int nCount = m_btnGroupForPages.buttons().count();
    for(int i = 0; i < nCount; i++)
    {
        QAbstractButton* btn = m_btnGroupForPages.buttons().at(i);
        btn->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, -1);
    }
    ui->pbNumDot1->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, -1);
    ui->pbNumDot2->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, -1);

    connect(&m_btnGroupForPages, static_cast<void (QButtonGroup::*)(int, bool)>(&QButtonGroup::buttonToggled),
            this, &UBXMultiPage::onBtnsButtonToggled);

    connect(ui->pbPrevPage, &QPushButton::clicked, this, &UBXMultiPage::onPrevPageClicked);
    connect(ui->pbNextPage, &QPushButton::clicked, this, &UBXMultiPage::onNextPageClicked);
}

/************************************
* 名称: UpdatePages
* 功能: 更新页码显示
* 参数: [in]nTotalPage 总页数
* 返回:   void
* 时间:   2016/04/14
* 作者:   hels
************************************/
void UBXMultiPage::UpdatePages(int nTotalPage)
{
    if (m_nTotalPage != nTotalPage)
    {
        m_nTotalPage = nTotalPage;
    }

    // > 0 真实页码值, = -1 不显示
    //先清空页码真实值
    int nCount = m_btnGroupForPages.buttons().count();
    for(int i = 0; i < nCount; i++)
    {
        QAbstractButton* btn = m_btnGroupForPages.buttons().at(i);
        btn->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, -1);
    }
    ui->pbNumDot1->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, -1);
    ui->pbNumDot2->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, -1);

    if(m_nCurrentPage <= 0)
    {
        ui->pbPrevPage->hide();
        ui->pbNextPage->hide();
        foreach (QAbstractButton *btn, m_btnGroupForPages.buttons())
        {
            btn->hide();
        }
        return;
    }

    ui->pbPrevPage->show();
    ui->pbNextPage->show();
    if(m_nCurrentPage > m_nTotalPage)
    {
        m_nCurrentPage = m_nTotalPage;
    }

    // > 0 真实页码值, = -1 不显示
    if(m_nTotalPage <= m_nPagebtns) //总页码数小于页码按钮个数，不显示"..."
    {
        for(int i = 0; i < m_nTotalPage; i++)
        {
            QAbstractButton* btn = m_btnGroupForPages.buttons().at(i);
            btn->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, i+1);
        }
        ui->pbNumDot1->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, -1);
        ui->pbNumDot2->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, -1);
    }
    else //总页码大于按钮个数，需要显示"..."
    {
        int nleftPageCount = (m_nPagebtns / 2);//左边的页码个数
        int nRightPageCount = (m_nPagebtns / 2) -1;//右边的页码个数

        //第一种情况< 1 ... 3 4 5 6 7 8 9 10... 30 >即左右两边都有...出现
        if(m_nCurrentPage > nleftPageCount + 1 && m_nCurrentPage < m_nTotalPage - nRightPageCount)
        {
            QAbstractButton* btn1 = m_btnGroupForPages.buttons().at(0);

            btn1->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, 1);//第一页
            //设置...显示
            ui->pbNumDot1->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, 0);//...

            int i = 1; //第二个页码按钮开始
            for(; i <= m_nPagebtns - 2; i++)
            {
                QAbstractButton* btn = m_btnGroupForPages.buttons().at(i);
                btn->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, m_nCurrentPage - nleftPageCount + i);
            }

            ui->pbNumDot2->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, 0);//最后一页前的...
            QAbstractButton* btn2 = m_btnGroupForPages.buttons().at(i);
            btn2->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, m_nTotalPage);//最后一页
        }
        //第二种情况 < 1 ... 22 23 24 25 26 26 28 29 30 >
        else if(m_nCurrentPage >= m_nTotalPage - nRightPageCount)
        {
            QAbstractButton* btn1 = m_btnGroupForPages.buttons().at(0);
            btn1->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, 1);//第一页
            //设置...显示
            ui->pbNumDot1->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, 0);//...
            //设置不显示...
            ui->pbNumDot2->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, -1);

            int i = 1;
            for(; i <= m_nPagebtns - 1; i++)
            {
                QAbstractButton* btn = m_btnGroupForPages.buttons().at(i);
                btn->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, m_nTotalPage - m_nPagebtns + 1 + i);
            }
        }
        else
        {
            //设置不显示
            ui->pbNumDot1->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, -1);
            //设置显示...
            ui->pbNumDot2->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, 0);
            int i = 0;
            for(; i <= m_nPagebtns - 2; i++)
            {
                QAbstractButton* btn = m_btnGroupForPages.buttons().at(i);
                btn->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, i + 1);
            }
            QAbstractButton* btn1 = m_btnGroupForPages.buttons().at(i);
            btn1->setProperty(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER, m_nTotalPage);
        }
    }

    //更新界面
    int nNumDot1 = ui->pbNumDot1->property(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER).toInt();
    if(-1 == nNumDot1)
    {
        ui->pbNumDot1->hide();
    }
    else
    {
        ui->pbNumDot1->show();
    }

    int nNumDot2 = ui->pbNumDot2->property(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER).toInt();
    if(-1 == nNumDot2)
    {
        ui->pbNumDot2->hide();
    }
    else
    {
        ui->pbNumDot2->show();
    }

    foreach (QAbstractButton* btn, m_btnGroupForPages.buttons())
    {
        int nNum = btn->property(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER).toInt();
        if(-1 == nNum)
        {
            btn->hide();
        }
        else
        {
            btn->setText(QString::number(nNum));

            if(m_nCurrentPage == nNum)
            {
                if(!btn->isChecked())
                {
                    btn->setChecked(true);
                }
            }
            btn->show();
        }
    }
}

/************************************
* 名称: getCurrentPage
* 功能: get the current page num
    * 返回:   int 当前页数
* 时间:   2016/06/13
* 作者:   asu
************************************/
int UBXMultiPage::getCurrentPage() const
{
    return m_nCurrentPage;
}

/************************************
* 名称: setCurrentPage
* 功能: 设置当前页
* 参数: [in]nCurPage 当前页
* 返回:   void
* 时间:   2016/06/13
* 作者:   hwx
************************************/
void UBXMultiPage::setCurrentPage(int nCurPage)
{
    m_nCurrentPage = nCurPage;
}

/************************************
* 名称: getTotalPage
* 功能: 获取总页数
* 返回:   int 总页数
* 时间:   2016/06/13
* 作者:   hwx
************************************/
int UBXMultiPage::getTotalPage() const
{
    return m_nTotalPage;
}

/************************************
* 名称: OnBtnsButtonToggled
* 功能: 第一排按钮组槽函数
* 参数: [in]int-id 发送信号的按钮id
* 参数: [in]bool-bChecked 按钮是否选中
* 返回:   void
* 时间:   2016/06/13
* 作者:   asu
************************************/
void UBXMultiPage::onBtnsButtonToggled(int id, bool bChecked)
{
    if (!bChecked)
    {
        return;
    }

    int nNum = m_nCurrentPage;
    QAbstractButton* btn = m_btnGroupForPages.button(id);
    if(btn)
    {
        nNum = btn->property(UBXUiBaseConstants::PROPERTY_PAGE_REAL_NUMBER).toInt();
    }

    if(m_nCurrentPage != nNum)
    {
        m_nCurrentPage = nNum;
        emit sigCurrentPageChanged(m_nCurrentPage);
    }
}

/************************************
* 名称: OnPrevPageClicked
* 功能: 上一页按钮槽函数
* 返回:   void
* 时间:   2016/06/14
* 作者:   asu
************************************/
void UBXMultiPage::onPrevPageClicked()
{
    if(m_nCurrentPage <= 1)
    {
        return;
    }

    --m_nCurrentPage;
    UpdatePages(m_nTotalPage);

    emit sigPrevPageClicked();
}

/************************************
* 名称: OnNextPageClicked
* 功能: 下一页按钮槽函数
* 返回:   void
* 时间:   2016/06/14
* 作者:   asu
************************************/
void UBXMultiPage::onNextPageClicked()
{
    if(m_nCurrentPage >= m_nTotalPage)
    {
        return;
    }

    ++m_nCurrentPage;
    UpdatePages(m_nTotalPage);
    emit sigNextPageClicked();
}
