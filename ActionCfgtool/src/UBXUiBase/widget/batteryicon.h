/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CBatteryIcon.h
* 创建时间：20160805
* 文件标识：
* 文件摘要：the batter icon ui
*
* 当前版本：1.0.0.0
* 作    者：asu
*/

#ifndef CBATTERYICON_H
#define CBATTERYICON_H
#include "ubxuibase_global.h"
#include <QToolButton>
#include <QMap>


class UBXUIBASE_EXPORT CBatteryIcon : public QToolButton
{
public:
    explicit CBatteryIcon(QWidget *parent = NULL);
    ~CBatteryIcon();

public:
    enum emBatteryStatus
    {
        eBatteryDischarg = 0,  //00/否 01/是 02/没有电池
        eBatteryCharging = 1,
        eBatteryLineOff = 2,

        eBatteryUnknown
    };

public:
    static QString getBatteryUnknownIcon();
    static QString getBatteryLineOffIcon();
    static QString getBatteryChargingIcon(const int& nBatteryValue);
    static QString getBatteryDischargIcon(const int& nBatteryValue);

public slots:
    void setBattery(int eBatteryStatus, int nBatteryValue);

private:
    QString getIconPath(const int& eBatteryStatus,const int &nBatteryValue)const;
    void setBatteryIcon(const QString& iconPath);

private:

};

#endif // CBATTERYICON_H
