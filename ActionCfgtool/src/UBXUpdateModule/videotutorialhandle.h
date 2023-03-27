/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CVideoTutorialHandle
* 创建时间：20160822
* 文件标识：
* 文件摘要：
*
* 当前版本：1.0.0.0
* 作    者：asu
*/

#ifndef CVIDEOTUTORIALHANDLE_H
#define CVIDEOTUTORIALHANDLE_H


#include <QObject>

#include "ubxupdatemodule_global.h"
#include "global.h"


class UBXUPDATEMODULESHARED_EXPORT CVideoTutorialHandle : QObject
{
    Q_OBJECT

public:
    explicit CVideoTutorialHandle();
    ~CVideoTutorialHandle();

public:
    int getUpdateCheckStatus();

};

#endif // CVIDEOTUTORIALHANDLE_H
