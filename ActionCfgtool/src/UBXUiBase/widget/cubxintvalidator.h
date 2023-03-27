/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CUBXIntValidator.h
* 创建时间：2016/11/22
* 文件标识：
* 文件摘要：Int类型合法限制
*
* 当前版本：1.0.0.0
* 作    者：hwx
*/


#ifndef CUBXINTVALIDATOR_H
#define CUBXINTVALIDATOR_H

#include "ubxuibase_global.h"
#include <QIntValidator>

class UBXUIBASE_EXPORT CUBXIntValidator : public QIntValidator
{
    Q_OBJECT
public:
    explicit CUBXIntValidator(QString &strLastValidValue, QObject *parent = 0);
    explicit CUBXIntValidator(int minimum, int maximum, QString &strLastValidValue, QObject * parent = 0);
    ~CUBXIntValidator();

public:
    virtual void	fixup(QString & input) const override;

signals:

public slots:

private:
    QString &m_strLastValidValue;  //记录上一个合法的值，当输入的值不合法时，恢复为上一个合法的值
};

#endif // CUBXINTVALIDATOR_H
