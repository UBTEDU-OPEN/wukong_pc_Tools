#include "phonenumregexp.h"
#include "regexp.h"

CPhoneNumRegExp::CPhoneNumRegExp(QWidget *parent):
    QLineEdit(parent)
{
    this->setValidator(new QRegExpValidator(CRegExp::getRegExpPhoneNum(), this));
}

CPhoneNumRegExp::~CPhoneNumRegExp()
{
}
