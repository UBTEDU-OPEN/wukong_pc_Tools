#include "cubxintvalidator.h"

CUBXIntValidator::CUBXIntValidator(QString &strLastValidValue, QObject *parent) : QIntValidator(parent)
  ,m_strLastValidValue(strLastValidValue)
{

}

CUBXIntValidator::CUBXIntValidator(int minimum, int maximum, QString &strLastValidValue, QObject *parent) : QIntValidator(minimum, maximum, parent)
  , m_strLastValidValue(strLastValidValue)
{

}

CUBXIntValidator::~CUBXIntValidator()
{

}

void CUBXIntValidator::fixup(QString &input) const
{
    input = m_strLastValidValue;
}

