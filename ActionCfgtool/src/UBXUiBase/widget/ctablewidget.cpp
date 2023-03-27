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

#include "ctablewidget.h"

#include <QTimerEvent>

CTableWidget::CTableWidget(QWidget *parent): QTableWidget(parent)
  ,m_nCurrentRowCount(0)
  ,m_nTimerIDUpdateRowChange(-1)
{

}

CTableWidget::~CTableWidget()
{

}

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
void CTableWidget::setAllCheckBoxs(QTableWidget *pTab, int nCheckBoxColPosition, Qt::CheckState state)
{
    if(pTab)
    {
        for(int i = 0; i < pTab->rowCount(); i++)
        {
            QCheckBox* pChkbox = dynamic_cast<QCheckBox*>(pTab->cellWidget(i, nCheckBoxColPosition));
            if(pChkbox)
            {
                pChkbox->setCheckState(state);
            }
        }
    }
}

/************************************
* 名称: isAllCheckBoxsChecked
* 功能: get the checkboxs status
* 参数: [in]pTab
*      [in]nCheckBoxColPosition
* 返回:   bool
* 时间:   20161108
* 作者:   asu
************************************/
bool CTableWidget::isAllCheckBoxsChecked(const QTableWidget * const pTab, const int nCheckBoxColPosition)
{
    if(!pTab || (pTab->rowCount() == 0))
    {
        return false;
    }

    bool bAllSelect = true;

    if(pTab)
    {
        for(int i = 0; i < pTab->rowCount(); i++)
        {
            QCheckBox* pCheckBox = dynamic_cast<QCheckBox*>(pTab->cellWidget(i, nCheckBoxColPosition));
            if(pCheckBox && !pCheckBox->isChecked())
            {
                bAllSelect = false;
                break;
            }
        }
    }

    return bAllSelect;
}

void CTableWidget::startRowChangeTimer(const int nMsec)
{
    if (m_nTimerIDUpdateRowChange > 0)
    {
        killTimer(m_nTimerIDUpdateRowChange);
    }

    m_nTimerIDUpdateRowChange = this->startTimer(nMsec);
}

void CTableWidget::timerEvent(QTimerEvent *event)
{
    if(event && (event->timerId() == m_nTimerIDUpdateRowChange))
    {
        if(m_nCurrentRowCount != this->rowCount())
        {
            m_nCurrentRowCount = this->rowCount();
            emit sigRowCountChanged(m_nCurrentRowCount);
        }
    }
}
