/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：CMenu
* 创建时间：20160819
* 文件标识：
* 文件摘要：CMenu for adding alert to menu by show not same the text.
*
* 当前版本：1.0.0.0
* 作    者：asu
*/

#ifndef CMENU_H
#define CMENU_H


#include "ubxuibase_global.h"
#include <QMenu>
#include <QColor>
#include <QFont>
#include <QImage>

const QString MENU_DATA_ALERT_KEY = "menuDataAlertKey";

class UBXUIBASE_EXPORT CMenu : public QMenu
{
    Q_OBJECT

public:
    explicit CMenu(const QString txt , QWidget* parent = 0);
    virtual ~CMenu();

public:
    //for set the alert text
    void setTextAlert(const QString &strAlert);
    //for set the alert color
    void setColorAlert(const QColor &colorAlert);
    //for set the  alert position
    void setAlertRightMargin(int nAlertRightMargin);
    //for set the  alert Font
    void setAlertFont(const QFont &font);
    //whether have alert mark flag
    bool isAlertMark()const;
    //for set the alert image
    void setAlertImage(const QString& strImgPath);

signals:
    void sigAlertMark(bool bAlertMark);


public slots:
    void setEnableAlert(bool bAlert);

protected:
    virtual  void paintEvent(QPaintEvent *);
    bool getAlertMarkAction(QList<QAction *>& actionList)const;
    static bool isActionAlertMark(QAction *pAction);

private:
    //repaint the item action for alert
    void repainItemAction(QMenu* pMenu, QAction* pAction);

private:
    QColor m_colorAlert;
    QFont m_font;
    QString m_strAlert;    
    QImage m_imagAlert;


    int m_nAlertRightMargin;
    bool m_bAlert;
};

#endif // CMENU_H
