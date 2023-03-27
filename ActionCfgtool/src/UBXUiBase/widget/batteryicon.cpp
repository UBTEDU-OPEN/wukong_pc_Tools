/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CBatteryIcon
* 创建时间：20160805
* 文件标识：
* 文件摘要：the batter icon ui
*
* 当前版本：1.0.0.0
* 作    者：asu
*/

#include "batteryicon.h"


const QString BATTERY_ICON_PATH_PREFIX = ":/res/images/";

const QString BATTERY_ICON_UNKNOWN = BATTERY_ICON_PATH_PREFIX + "icon_battery_unknown.png";
const QString BATTERY_ICON_LINEOFF = BATTERY_ICON_PATH_PREFIX + "icon_battery_lineoff.png";

const QString BATTERY_ICON_CHARGING_LOW = BATTERY_ICON_PATH_PREFIX + "icon_battery_chargin_low.png";
const QString BATTERY_ICON_CHARGING_20 = BATTERY_ICON_PATH_PREFIX + "icon_battery_chargin20.png";
const QString BATTERY_ICON_CHARGING_40 = BATTERY_ICON_PATH_PREFIX + "icon_battery_chargin40.png";
const QString BATTERY_ICON_CHARGING_60 = BATTERY_ICON_PATH_PREFIX + "icon_battery_chargin60.png";
const QString BATTERY_ICON_CHARGING_80 = BATTERY_ICON_PATH_PREFIX + "icon_battery_chargin80.png";

const QString BATTERY_ICON_DISCHARG_LOW = BATTERY_ICON_PATH_PREFIX + "icon_battery_discharg_low.png";
const QString BATTERY_ICON_DISCHARG_20 = BATTERY_ICON_PATH_PREFIX + "icon_battery_discharg20.png";
const QString BATTERY_ICON_DISCHARG_40 = BATTERY_ICON_PATH_PREFIX + "icon_battery_discharg40.png";
const QString BATTERY_ICON_DISCHARG_60 = BATTERY_ICON_PATH_PREFIX + "icon_battery_discharg60.png";
const QString BATTERY_ICON_DISCHARG_80 = BATTERY_ICON_PATH_PREFIX + "icon_battery_discharg80.png";
const QString BATTERY_ICON_DISCHARG_100 = BATTERY_ICON_PATH_PREFIX + "icon_battery_discharg100.png";



CBatteryIcon::CBatteryIcon(QWidget *parent):
    QToolButton(parent)
{
    this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->setStyleSheet("border: 0px solid #8f8f91;}");
}

CBatteryIcon::~CBatteryIcon()
{

}


void CBatteryIcon::setBattery(int eBatteryStatus, int nBatteryValue)
{
    setBatteryIcon(getIconPath(eBatteryStatus, nBatteryValue));

    QString strBatteryTip;
    strBatteryTip.clear();

    switch (eBatteryStatus)
    {
    case eBatteryUnknown:
        strBatteryTip = tr("");
        break;

    case eBatteryLineOff:
        strBatteryTip = tr("");
        break;

    case eBatteryCharging:
    case eBatteryDischarg:
        strBatteryTip = QString::number(nBatteryValue);
        break;

    default:
        strBatteryTip = tr("Battery err");;
        break;
    }

    setText(strBatteryTip);
}

QString CBatteryIcon::getIconPath(const int& eBatteryStatus,const int &nBatteryValue)const
{
    QString strIconPath;
    strIconPath.clear();

    switch (eBatteryStatus)
    {
    case eBatteryUnknown:
        strIconPath = getBatteryUnknownIcon();
        break;

    case eBatteryLineOff:
        strIconPath = getBatteryLineOffIcon();
        break;

    case eBatteryCharging:
        strIconPath = getBatteryChargingIcon(nBatteryValue);
        break;

    case eBatteryDischarg:
        strIconPath = getBatteryDischargIcon(nBatteryValue);
        break;

    default:
        strIconPath = getBatteryUnknownIcon();
        break;
    }

    return strIconPath;
}

QString CBatteryIcon::getBatteryUnknownIcon()
{
    //todo
    return BATTERY_ICON_UNKNOWN;
}

QString CBatteryIcon::getBatteryLineOffIcon()
{
    return BATTERY_ICON_LINEOFF;
}

QString CBatteryIcon::getBatteryChargingIcon(const int &nBatteryValue)
{
    //todo
    QString strIconPath;
    strIconPath.clear();

    if((0 > nBatteryValue) || (nBatteryValue > 100))
    {
        return strIconPath;
    }

    if((0 <= nBatteryValue) || (nBatteryValue < 20))
    {
        strIconPath = BATTERY_ICON_CHARGING_LOW;
    }

    else if((20 <= nBatteryValue) || (nBatteryValue < 40))
    {
        strIconPath = BATTERY_ICON_CHARGING_20;
    }

    else if((40 <= nBatteryValue) || (nBatteryValue < 60))
    {
        strIconPath = BATTERY_ICON_CHARGING_40;
    }

    else if((60 <= nBatteryValue) || (nBatteryValue < 80))
    {
        strIconPath = BATTERY_ICON_CHARGING_60;
    }

    else if((80 <= nBatteryValue) || (nBatteryValue < 100))
    {
        strIconPath = BATTERY_ICON_CHARGING_80;
    }

    else if(nBatteryValue == 100)
    {
        strIconPath = BATTERY_ICON_DISCHARG_100;
    }

    return strIconPath;
}

QString CBatteryIcon::getBatteryDischargIcon(const int &nBatteryValue)
{
    //todo
    QString strIconPath;
    strIconPath.clear();

    if((0 > nBatteryValue) || (nBatteryValue > 100))
    {
        return strIconPath;
    }

    if((0 <= nBatteryValue) || (nBatteryValue < 20))
    {
        strIconPath = BATTERY_ICON_DISCHARG_LOW;
    }

    else if((20 <= nBatteryValue) || (nBatteryValue < 40))
    {
        strIconPath = BATTERY_ICON_DISCHARG_20;
    }

    else if((40 <= nBatteryValue) || (nBatteryValue < 60))
    {
        strIconPath = BATTERY_ICON_DISCHARG_40;
    }

    else if((60 <= nBatteryValue) || (nBatteryValue < 80))
    {
        strIconPath = BATTERY_ICON_DISCHARG_60;
    }

    else if((80 <= nBatteryValue) || (nBatteryValue < 100))
    {
        strIconPath = BATTERY_ICON_DISCHARG_80;
    }

    else if(nBatteryValue == 100)
    {
        strIconPath = BATTERY_ICON_DISCHARG_100;
    }

    return strIconPath;
}

void CBatteryIcon::setBatteryIcon(const QString &iconPath)
{
    this->setIcon(QIcon(iconPath));
}
