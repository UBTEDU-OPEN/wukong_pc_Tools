/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CPhoneNumRegExp
* 创建时间：20160720
* 文件标识：
* 文件摘要：the widget title bar
*
* 当前版本：1.0.0.0
* 作    者：asu
*/

#ifndef CPHONENUMREGEXP_H
#define CPHONENUMREGEXP_H


#include "ubxuibase_global.h"
#include <QLineEdit>

class UBXUIBASE_EXPORT CPhoneNumRegExp : public QLineEdit
{
public:    
    explicit CPhoneNumRegExp(QWidget *parent = 0);
    virtual ~CPhoneNumRegExp();

public:

};

#endif // CPHONENUMREGEXP_H
