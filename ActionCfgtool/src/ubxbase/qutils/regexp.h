#ifndef CREGEXP_H
#define CREGEXP_H

#include "qutils_global.h"
#include <QRegExp>
#include <QRegExpValidator>


const int PHONE_NUM_MAX_LENGTH = 20;



class QUTILS_EXPORT CRegExp : public QRegExp
{
public:
    explicit CRegExp(QRegExp parent);
    ~CRegExp();

public:
    static QRegExp getRegExpAnyCharts(const int& nMinLen, const int& nMaxLen);
    static QRegExp getRegExpAnyLetters(const int& nMinLen, const int& nMaxLen);
    static QRegExp getRegExpAnyNumbers(const int& nMinLen, const int& nMaxLen);
    static QRegExp getRegExpPhoneNum();

};

#endif // CREGEXP_H
