#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H

#include "ubxuibase_global.h"
#include <QComboBox>
#include <QWidget>
#include <QPair>


typedef QPair<QString, QVariant>  ComboBoxPair; //<show text, user data>

class MyComboBox : public QComboBox
{
public:
    explicit MyComboBox(QWidget* parent);
    ~MyComboBox();

public:
    //add the items for the ui and the user data
    //asu
    void addItems(const QList<ComboBoxPair>& list);

    //
    void setCurrentText(const QString &strUserData);
};

#endif // MYCOMBOBOX_H
