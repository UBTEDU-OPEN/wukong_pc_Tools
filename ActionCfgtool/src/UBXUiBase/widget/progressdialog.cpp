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

#include "progressdialog.h"


#include <QKeyEvent>

CProgressDialog::CProgressDialog(QWidget *parent, Qt::WindowFlags flags) : QProgressDialog(parent, flags)
        ,m_bKeyEnable(true)
{
    setUpEventFilter();
}

CProgressDialog::CProgressDialog(const QString &labelText, const QString &cancelButtonText,
                                 int minimum, int maximum, QWidget *parent, Qt::WindowFlags flags)
    :QProgressDialog(labelText, cancelButtonText, minimum, maximum, parent, flags)
    ,m_bKeyEnable(true)
{
    setUpEventFilter();
}

CProgressDialog::~CProgressDialog()
{

}

void CProgressDialog::setKeyPressEnable(bool bKeyEnable)
{
    m_bKeyEnable = bKeyEnable;
}

void CProgressDialog::keyPressEvent(QKeyEvent *keyEvent)
{
   if(m_bKeyEnable)
   {
       QProgressDialog::keyPressEvent(keyEvent);
   }
}

bool CProgressDialog::eventFilter(QObject *pObject, QEvent *pEvent)
{
    if(!m_bKeyEnable)
    {
        if(QEvent::KeyPress == pEvent->type())
        {
            return true;
        }

        QKeyEvent *pKeyEvent = dynamic_cast<QKeyEvent*>(pEvent);
        if (pKeyEvent && (pKeyEvent->key() == Qt::Key_Escape))
        {
            pEvent->accept();
            return true;
        }
    }

    return QProgressDialog::eventFilter(pObject, pEvent);
}

void CProgressDialog::setUpEventFilter()
{
    QObjectList list = this->children();
    for(int i = 0; i < list.count(); ++i)
    {
        list.at(i)->installEventFilter(this);
    }
}

