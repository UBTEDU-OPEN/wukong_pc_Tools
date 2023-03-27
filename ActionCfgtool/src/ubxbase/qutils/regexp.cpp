#include "regexp.h"

CRegExp::CRegExp(QRegExp parent) :
    QRegExp(parent)
{

}

CRegExp::~CRegExp()
{
    
}

QRegExp CRegExp::getRegExpAnyCharts(const int &nMinLen, const int &nMaxLen)
{
    QString strReg = QString("^.{%1,%2}").arg(nMinLen)
                                             .arg(nMaxLen);

    return QRegExp(strReg);
}

QRegExp CRegExp::getRegExpAnyLetters(const int &nMinLen, const int &nMaxLen)
{
    QString strReg = QString("^[A-Za-z]{%1,%2}").arg(nMinLen)
                                                      .arg(nMaxLen);

    return QRegExp(strReg);
}

QRegExp CRegExp::getRegExpAnyNumbers(const int &nMinLen, const int &nMaxLen)
{
    QString strReg = QString("^[0-9]{%1,%2}").arg(nMinLen)
                                                   .arg(nMaxLen);

    return QRegExp(strReg);
}

QRegExp CRegExp::getRegExpPhoneNum()
{
    return CRegExp::getRegExpAnyNumbers(0, PHONE_NUM_MAX_LENGTH);
}
