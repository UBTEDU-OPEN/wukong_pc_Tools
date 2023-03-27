#include "ledswidget.h"
#include "ui_ledswidget.h"


CLedsWidget::CLedsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLedsWidget)
{
    ui->setupUi(this);

    //init connect button
    for(int index = 0; index < LEDS_NUMBERS; ++index)
    {
        QPushButton* pBtn = getBtn(index);
        if(pBtn)
        {
            pBtn->setObjectName(QString::number(index));

            connect(pBtn, &QPushButton::toggled, this, &CLedsWidget::onButtonPress);
        }
    }

    //init ui
    for(int index = 0; index < LEDS_NUMBERS; ++index)
    {
        setBackgroundColor(index, eBlackColor);
    }

}

CLedsWidget::~CLedsWidget()
{
    delete ui;
}

QPushButton *CLedsWidget::getBtn(const int &index)const
{
    QPushButton* pBtn = NULL;
    switch (index) {
    case 0:
        pBtn = ui->btn0;
        break;

    case 1:
        pBtn = ui->btn1;
        break;

    case 2:
        pBtn = ui->btn2;
        break;

    case 3:
        pBtn = ui->btn3;
        break;

    case 4:
        pBtn = ui->btn4;
        break;

    case 5:
        pBtn = ui->btn5;
        break;

    case 6:
        pBtn = ui->btn6;
        break;

    case 7:
        pBtn = ui->btn7;
        break;

    default:
        break;
    }

    return pBtn;
}

bool CLedsWidget::isChecked(const int &index)const
{
    QPushButton* pBtn = NULL;
    pBtn = getBtn(index);
    if(pBtn)
    {
        return pBtn->isChecked();
    }
    
    return false;
}

void CLedsWidget::setBackgroundColor(const int &btnId, const int &colorId)
{    
    QPushButton* pBtn = NULL;
    pBtn = getBtn(btnId);
    if(pBtn)
    {
        QString strPng = getBackgroundIconPath(colorId);
        pBtn->setIcon(QIcon(strPng));
        pBtn->setIconSize(QSize(16,16));
    }
}

char CLedsWidget::getLedsSettings()const
{
    char charLedsSettings = 0;
    for(int index = 0; index < LEDS_NUMBERS; ++index)
    {
        bool bCheck = isChecked(index);
        charLedsSettings |= bCheck ? (1<<index) : (0<<index);
    }

    return charLedsSettings;
}

void CLedsWidget::setLedsSettings(const int &nSettings)
{
    for(int index = 0; index < LEDS_NUMBERS; ++index)
    {
        bool bSet = (nSettings>>index & 0x0001) ? true : false;
        QPushButton* pBtn = getBtn(index);
        if(pBtn)
        {
            pBtn->setChecked(bSet);
        }
    }
}

void CLedsWidget::onButtonPress()
{
    QPushButton* senderBtn = dynamic_cast<QPushButton*>(sender());
    if(senderBtn)
    {
        const int nIdBtn = senderBtn->objectName().toInt();
        const bool bCheck = isChecked(nIdBtn);
        emit sigButtonToggled(nIdBtn, bCheck);
    }
}

QString CLedsWidget::getBackgroundIconPath(const int &colorId)
{
    QString strPng;
    switch (colorId) {
    case eBlackColor:
        strPng = ":/res/images/BlackBlock.png";
        break;
        
    case eRedColor:
        strPng = ":/res/images/RedBlock.png";
        break;
        
    case eGreenColor:
        strPng = ":/res/images/GreenBlock.png";
        break;
        
    case eBlueColor:
        strPng = ":/res/images/BlueBlock.png";
        break;
        
    case eYellowColor:
        strPng = ":/res/images/YellowBlock.png";
        break;

    case ePinkColor:
        strPng = ":/res/images/PinkBlock.png";
        break;

    case eIndigoColor:
        strPng = ":/res/images/IndigoBlock.png";
        break;

    case eWhiteColor:
        strPng = ":/res/images/WhiteBlock.png";
        break;
    default:
        break;
    }
    
    return strPng;
}
