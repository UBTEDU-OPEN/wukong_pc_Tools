/********************************************************************************
** Form generated from reading UI file 'updateprogressdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPDATEPROGRESSDLG_H
#define UI_UPDATEPROGRESSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_UpdateProgressDlg
{
public:
    QProgressBar *progressBar;
    QPushButton *pushButton_Cancel;

    void setupUi(QDialog *UpdateProgressDlg)
    {
        if (UpdateProgressDlg->objectName().isEmpty())
            UpdateProgressDlg->setObjectName(QStringLiteral("UpdateProgressDlg"));
        UpdateProgressDlg->resize(416, 86);
        progressBar = new QProgressBar(UpdateProgressDlg);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(30, 30, 331, 16));
        progressBar->setValue(24);
        pushButton_Cancel = new QPushButton(UpdateProgressDlg);
        pushButton_Cancel->setObjectName(QStringLiteral("pushButton_Cancel"));
        pushButton_Cancel->setGeometry(QRect(280, 50, 115, 32));

        retranslateUi(UpdateProgressDlg);

        QMetaObject::connectSlotsByName(UpdateProgressDlg);
    } // setupUi

    void retranslateUi(QDialog *UpdateProgressDlg)
    {
        UpdateProgressDlg->setWindowTitle(QApplication::translate("UpdateProgressDlg", "Dialog", 0));
        pushButton_Cancel->setText(QApplication::translate("UpdateProgressDlg", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class UpdateProgressDlg: public Ui_UpdateProgressDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPDATEPROGRESSDLG_H
