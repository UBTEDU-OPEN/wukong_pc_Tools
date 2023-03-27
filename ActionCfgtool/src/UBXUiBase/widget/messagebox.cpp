#include "messagebox.h"


#include <QLayout>
#include <QPushButton>

#include "mytitlebar.h"



CMessageBox::CMessageBox(QWidget *parent):
    QWidget(parent),
    m_pMyTitleBar(NULL),
    m_pMessageBox(NULL),
    m_pVLayout(NULL)
{
    m_pVLayout = new QVBoxLayout(this);

    m_pMyTitleBar = new MyTitleBar();
    m_pMessageBox = new QMessageBox();

    m_pVLayout->addWidget(m_pMyTitleBar);
    m_pVLayout->addWidget(m_pMessageBox);

    this->setLayout(m_pVLayout);

    m_pMyTitleBar->show();
    m_pMessageBox->show();
}

CMessageBox::~CMessageBox()
{
    if(m_pMyTitleBar)
    {
        delete m_pMyTitleBar;
        m_pMyTitleBar = NULL;
    }
}

MyTitleBar *CMessageBox::getMyTitleBar() const
{
    return m_pMyTitleBar;
}

QMessageBox *CMessageBox::getQMBox() const
{
    return m_pMessageBox;
}

void CMessageBox::setMyTitleBarVisible(bool bVisible)
{
    if(m_pMyTitleBar)
    {
        m_pMyTitleBar->setVisible(bVisible);
    }
}


//asu, for warning box
QMessageBox::StandardButton CMessageBox::warning(QWidget *parent,
                                                 const QString &title,
                                                 const QString &text,
                                                 QMessageBox::StandardButtons buttons,
                                                 QMessageBox::StandardButton defaultButton)
{


    return CMessageBox::execMBox(QMessageBox::Warning,
                                 parent,
                                 title,
                                 text,
                                 buttons,
                                 defaultButton);
}

//asu, for Information box
QMessageBox::StandardButton CMessageBox::information(QWidget *parent,
                                                     const QString &title,
                                                     const QString &text,
                                                     QMessageBox::StandardButtons buttons,
                                                     QMessageBox::StandardButton defaultButton)
{
    return CMessageBox::execMBox(QMessageBox::Information,
                                 parent,
                                 title,
                                 text,
                                 buttons,
                                 defaultButton);
}

//asu, for Question box
QMessageBox::StandardButton CMessageBox::question(QWidget *parent,
                                                  const QString &title,
                                                  const QString &text,
                                                  QMessageBox::StandardButtons buttons,
                                                  QMessageBox::StandardButton defaultButton)
{
    return CMessageBox::execMBox(QMessageBox::Question,
                                 parent,
                                 title,
                                 text,
                                 buttons,
                                 defaultButton);
}


//asu, for Critical box
QMessageBox::StandardButton CMessageBox::critical(QWidget *parent,
                                                  const QString &title,
                                                  const QString &text,
                                                  QMessageBox::StandardButtons buttons,
                                                  QMessageBox::StandardButton defaultButton)
{
    return CMessageBox::execMBox(QMessageBox::Critical,
                                 parent,
                                 title,
                                 text,
                                 buttons,
                                 defaultButton);
}

//asu, for exec an message box
QMessageBox::StandardButton CMessageBox::execMBox(QMessageBox::Icon icon,
                                                  QWidget *parent,
                                                  const QString &title,
                                                  const QString &text,
                                                  QMessageBox::StandardButtons buttons,
                                                  QMessageBox::StandardButton defaultButton)
{
    QMessageBox* pBox = new QMessageBox(QMessageBox::Warning, title, text,
                                        buttons, parent);

    if(defaultButton != QMessageBox::NoButton)
    {
        pBox->setDefaultButton(defaultButton);
    }

    QList<QAbstractButton *> listBtns = pBox->buttons();

    for(int i = 0; i < listBtns.count(); i++)
    {

        QAbstractButton *pBtn = listBtns.at(i);
        if(pBtn)
        {
            const QMessageBox::StandardButton standardButton = pBox->standardButton(pBtn);
            CMessageBox::setBtnText(pBtn, standardButton);
        }
    }

    return (QMessageBox::StandardButton)pBox->exec();
}

//asu, set btn show text
void CMessageBox::setBtnText(QAbstractButton *pBtn, QMessageBox::StandardButton standardButton)
{
    if(pBtn)
    {
        switch (standardButton)
        {
        case QMessageBox::Ok:
            pBtn->setText(QObject::tr("Ok"));
            break;

        case QMessageBox::Save:
            pBtn->setText(QObject::tr("Save"));
            break;

        case QMessageBox::SaveAll:
            pBtn->setText(QObject::tr("SaveAll"));
            break;

        case QMessageBox::Open:
            pBtn->setText(QObject::tr("Open"));
            break;

        case QMessageBox::Yes:
            pBtn->setText(QObject::tr("Yes"));
            break;

        case QMessageBox::YesToAll:
            pBtn->setText(QObject::tr("YesToAll"));
            break;

        case QMessageBox::No:
            pBtn->setText(QObject::tr("No"));
            break;

        case QMessageBox::NoToAll:
            pBtn->setText(QObject::tr("NoToAll"));
            break;

        case QMessageBox::Abort:
            pBtn->setText(QObject::tr("Abort"));
            break;

        case QMessageBox::Retry:
            pBtn->setText(QObject::tr("Retry"));
            break;

        case QMessageBox::Ignore:
            pBtn->setText(QObject::tr("Ignore"));
            break;

        case QMessageBox::Close:
            pBtn->setText(QObject::tr("Close"));
            break;

        case QMessageBox::Cancel:
            pBtn->setText(QObject::tr("Cancel"));
            break;

        case QMessageBox::Discard:
            pBtn->setText(QObject::tr("Discard"));
            break;

        case QMessageBox::Help:
            pBtn->setText(QObject::tr("Help"));
            break;

        case QMessageBox::Apply:
            pBtn->setText(QObject::tr("Apply"));
            break;

        case QMessageBox::Reset:
            pBtn->setText(QObject::tr("Reset"));
            break;

        case QMessageBox::RestoreDefaults:
            pBtn->setText(QObject::tr("RestoreDefaults"));
            break;

        default:
            break;
        }
    }
}


