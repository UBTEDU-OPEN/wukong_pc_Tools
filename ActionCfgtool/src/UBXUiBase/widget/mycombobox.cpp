#include "mycombobox.h"


MyComboBox::MyComboBox(QWidget *parent):
    QComboBox(parent)
{

}

MyComboBox::~MyComboBox()
{

}


//add the items for the ui and the user data
//asu
void MyComboBox::addItems(const QList<ComboBoxPair>& listPair)
{
    if(listPair.isEmpty())
    {
        return;
    }

    for (int i = 0; i < listPair.size(); ++i)
    {
       this->addItem(listPair.at(i).first, listPair.at(i).second);
    }
}

void MyComboBox::setCurrentText(const QString &strUserData)
{
    const int nCount = this->count();
    for(int index = 0; index < nCount; ++index)
    {
        if( this->itemData(index).toString() == strUserData)
        {
            this->setCurrentIndex(index);

            return;
        }
    }

    QComboBox::setCurrentText(strUserData);
}
