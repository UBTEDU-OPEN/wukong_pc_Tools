#include "menu.h"


#include <QPaintEvent>
#include <QPainter>
#include <QFont>
#include <QApplication>


#include <QDebug>

CMenu::CMenu(const QString txt, QWidget *parent):QMenu(txt, parent)  
  ,m_bAlert(false)
{
    m_font = QApplication::font();
    m_font.setPointSize(20);
    m_font.setBold(true);

    m_colorAlert = Qt::red;
    m_nAlertRightMargin = 18;
    m_strAlert.clear();
}

CMenu::~CMenu()
{
}

void CMenu::paintEvent(QPaintEvent *paintEvent)
{
    QMenu::paintEvent(paintEvent);

    if(m_bAlert)
    {
        QList<QAction *> actionList;
        bool bAlertMark = getAlertMarkAction(actionList);

        for(int index = 0; index < actionList.count(); ++index)
        {
            repainItemAction(this, actionList.at(index));
        }

        emit sigAlertMark(bAlertMark);
    }

    return;
}

//repaint the item action for alert
void CMenu::repainItemAction(QMenu *pMenu, QAction *pAction)
{
    if(pMenu && pAction)
    {
        QRect rect = pMenu->actionGeometry(pAction);
        const int nLeftOld = rect.left();
        const int nRightOld = rect.right();
        const int nRightNew = rect.right() - m_nAlertRightMargin;
        rect.setRight((nLeftOld > nRightNew)?nRightOld:nRightNew);

        QPainter painter(this);
        if(!m_strAlert.isEmpty())
        {
            painter.setFont(m_font);
            painter.setPen(m_colorAlert);

            painter.drawText(rect, Qt::AlignRight|Qt::AlignTop, m_strAlert);
        }

        QRect rcImage = m_imagAlert.rect();
        const int nLeftNew = rect.right()- rcImage.width();
        rect.setLeft((nLeftNew > rect.right())?nLeftOld:nLeftNew);
        rect.setHeight(rcImage.height());

        painter.drawImage(rect, m_imagAlert);

        emit sigAlertMark(true);
    }
}

bool CMenu::getAlertMarkAction(QList<QAction *>& actionList)const
{
    bool bGetAlertMark = false;
    actionList.clear();

    if(m_bAlert)
    {
        QList<QAction *> listAction = this->actions();
        for(int index = 0; index < listAction.count(); ++index)
        {
            QAction *pAction = listAction.at(index);

            //this is menu
            if(pAction && pAction->menu())
            {
                qDebug() << "menu:"<< pAction->menu()->title()<<"\n";
                QList<QAction *> listAction = pAction->menu()->actions();

                bool bAlertForChild = false;
                for(int index = 0; (index < listAction.count()) && !bAlertForChild; ++index)
                {
                    QAction *pActionChild = listAction.at(index);
                    if(pAction)
                    {
                        qDebug() << "pActionChild:"<<pActionChild->text()<<"\n";

                        bAlertForChild = CMenu::isActionAlertMark(pActionChild);
                    }
                }

                if(bAlertForChild)
                {
                    actionList.append(pAction);

                    qDebug() << "pAction:"<<pAction->text()<<"\n";
                    if(pAction->menu())
                    {
                        qDebug() << "menu:"<< pAction->menu()->title()<<"\n";
                    }
                }
            }

            //this is action
            else if(pAction)
            {
                bool bAlertMark = CMenu::isActionAlertMark(pAction);
                if(bAlertMark)
                {
                    actionList.append(pAction);
                }
            }
        }
    }

    if(!actionList.isEmpty())
    {
        bGetAlertMark = true;
    }

    return bGetAlertMark;
}

bool CMenu::isActionAlertMark(QAction *pAction)
{
    bool bAlert = false;

    if(pAction)
    {
        QVariant variantIsAlert = pAction->property(MENU_DATA_ALERT_KEY.toStdString().c_str());
        if(variantIsAlert.isValid())
        {
            if(variantIsAlert.toBool())
            {
                bAlert = true;
            }
        }
    }

    return bAlert;
}

//for set the  alert Font
void CMenu::setAlertFont(const QFont &font)
{
    m_font = font;
}

//whether have alert mark flag
bool CMenu::isAlertMark() const
{
    QList<QAction *> actionList;
    bool bAlertMark = getAlertMarkAction(actionList);

    return bAlertMark;
}

void CMenu::setAlertImage(const QString &strImgPath)
{
    m_imagAlert = QImage(strImgPath);
}

void CMenu::setEnableAlert(bool bAlert)
{
    m_bAlert = bAlert;
}

//for set the  alert position
void CMenu::setAlertRightMargin(int nAlertRightMargin)
{
    m_nAlertRightMargin = nAlertRightMargin;
}

//for set the alert color
void CMenu::setColorAlert(const QColor &colorAlert)
{
    m_colorAlert = colorAlert;
}

//for set the alert text
void CMenu::setTextAlert(const QString &strAlert)
{
    m_strAlert = strAlert;
}
