#ifndef CCHECKBOXLABEL_H
#define CCHECKBOXLABEL_H

#include "ubxuibase_global.h"
#include <QCheckBox>


#include "cimagelabel.h"


class CCheckBoxLabel : public CImageLabel
{
    Q_OBJECT

public:
    explicit CCheckBoxLabel(QWidget *parent = 0);
    virtual ~CCheckBoxLabel();

public:
    void setCheckBoxPosition(const QPoint& position);
    QCheckBox* getCheckBox();

public:
   bool isChecked()const;

signals:
    void stateChanged(int);

private:
    QCheckBox m_checkBox;

};

#endif // CCHECKBOXLABEL_H
