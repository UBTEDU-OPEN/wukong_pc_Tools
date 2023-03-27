/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：favoritespage.h
* 创建时间：2016/06/30
* 文件标识：
* 文件摘要：image label with checkbox
*
* 当前版本：1.0.0.0
* 作    者：asu
*/

#include "checkboxlabel.h"


const int CHECKBOX_DEFAULT_POSITOIN = 5;


CCheckBoxLabel::CCheckBoxLabel(QWidget *parent) :
    CImageLabel(parent)
{
    m_checkBox.setParent(this);
    setCheckBoxPosition(QPoint(CHECKBOX_DEFAULT_POSITOIN, CHECKBOX_DEFAULT_POSITOIN));

    connect(&m_checkBox, &QCheckBox::stateChanged, this, &CCheckBoxLabel::stateChanged);
}


CCheckBoxLabel::~CCheckBoxLabel()
{

}

void CCheckBoxLabel::setCheckBoxPosition(const QPoint &position)
{
    QRect rectVisual = this->rect();

    if(rectVisual.contains(position))
    {
        m_checkBox.move(position);
    }
    else
    {
        m_checkBox.move(CHECKBOX_DEFAULT_POSITOIN, CHECKBOX_DEFAULT_POSITOIN);
    }
}

QCheckBox *CCheckBoxLabel::getCheckBox()
{
    return &m_checkBox;
}

bool CCheckBoxLabel::isChecked() const
{
    return m_checkBox.isChecked();
}
