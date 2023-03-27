/********************************************************************************
** Form generated from reading UI file 'systemembeddedupdatewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYSTEMEMBEDDEDUPDATEWIDGET_H
#define UI_SYSTEMEMBEDDEDUPDATEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SystemEmbeddedUpdateWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayoutTitle;
    QPushButton *pbCloseMac;
    QSpacerItem *horizontalSpacer;
    QLabel *labelTitle;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pbCloseWin;
    QHBoxLayout *horizontalLayout_3;
    QGridLayout *gridLayout;
    QLabel *labLatestVersion;
    QLabel *labLatestVerionText;
    QLabel *labLocalVersionText;
    QLabel *labLocalVersion;
    QHBoxLayout *horizontalLayout_4;
    QTextEdit *versiontextEdit;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pbUpdate;

    void setupUi(QWidget *SystemEmbeddedUpdateWidget)
    {
        if (SystemEmbeddedUpdateWidget->objectName().isEmpty())
            SystemEmbeddedUpdateWidget->setObjectName(QStringLiteral("SystemEmbeddedUpdateWidget"));
        SystemEmbeddedUpdateWidget->setWindowModality(Qt::WindowModal);
        SystemEmbeddedUpdateWidget->resize(341, 289);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SystemEmbeddedUpdateWidget->sizePolicy().hasHeightForWidth());
        SystemEmbeddedUpdateWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(SystemEmbeddedUpdateWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayoutTitle = new QHBoxLayout();
        horizontalLayoutTitle->setSpacing(6);
        horizontalLayoutTitle->setObjectName(QStringLiteral("horizontalLayoutTitle"));
        horizontalLayoutTitle->setContentsMargins(20, 9, 20, -1);
        pbCloseMac = new QPushButton(SystemEmbeddedUpdateWidget);
        pbCloseMac->setObjectName(QStringLiteral("pbCloseMac"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pbCloseMac->sizePolicy().hasHeightForWidth());
        pbCloseMac->setSizePolicy(sizePolicy1);
        pbCloseMac->setMinimumSize(QSize(12, 12));
        pbCloseMac->setMaximumSize(QSize(12, 12));

        horizontalLayoutTitle->addWidget(pbCloseMac);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutTitle->addItem(horizontalSpacer);

        labelTitle = new QLabel(SystemEmbeddedUpdateWidget);
        labelTitle->setObjectName(QStringLiteral("labelTitle"));
        labelTitle->setAlignment(Qt::AlignCenter);

        horizontalLayoutTitle->addWidget(labelTitle);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutTitle->addItem(horizontalSpacer_2);

        pbCloseWin = new QPushButton(SystemEmbeddedUpdateWidget);
        pbCloseWin->setObjectName(QStringLiteral("pbCloseWin"));
        sizePolicy1.setHeightForWidth(pbCloseWin->sizePolicy().hasHeightForWidth());
        pbCloseWin->setSizePolicy(sizePolicy1);
        pbCloseWin->setMinimumSize(QSize(12, 12));
        pbCloseWin->setMaximumSize(QSize(12, 12));

        horizontalLayoutTitle->addWidget(pbCloseWin);


        verticalLayout->addLayout(horizontalLayoutTitle);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(20, -1, 20, -1);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(20);
        gridLayout->setVerticalSpacing(5);
        gridLayout->setContentsMargins(0, -1, 0, -1);
        labLatestVersion = new QLabel(SystemEmbeddedUpdateWidget);
        labLatestVersion->setObjectName(QStringLiteral("labLatestVersion"));
        sizePolicy1.setHeightForWidth(labLatestVersion->sizePolicy().hasHeightForWidth());
        labLatestVersion->setSizePolicy(sizePolicy1);
        labLatestVersion->setMinimumSize(QSize(0, 32));
        labLatestVersion->setMaximumSize(QSize(16777215, 32));
        labLatestVersion->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labLatestVersion, 1, 0, 1, 1);

        labLatestVerionText = new QLabel(SystemEmbeddedUpdateWidget);
        labLatestVerionText->setObjectName(QStringLiteral("labLatestVerionText"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(labLatestVerionText->sizePolicy().hasHeightForWidth());
        labLatestVerionText->setSizePolicy(sizePolicy2);
        labLatestVerionText->setMinimumSize(QSize(0, 32));
        labLatestVerionText->setMaximumSize(QSize(16777215, 32));
        labLatestVerionText->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(labLatestVerionText, 1, 1, 1, 1);

        labLocalVersionText = new QLabel(SystemEmbeddedUpdateWidget);
        labLocalVersionText->setObjectName(QStringLiteral("labLocalVersionText"));
        sizePolicy2.setHeightForWidth(labLocalVersionText->sizePolicy().hasHeightForWidth());
        labLocalVersionText->setSizePolicy(sizePolicy2);
        labLocalVersionText->setMinimumSize(QSize(0, 32));
        labLocalVersionText->setMaximumSize(QSize(16777215, 32));
        labLocalVersionText->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(labLocalVersionText, 0, 1, 1, 1);

        labLocalVersion = new QLabel(SystemEmbeddedUpdateWidget);
        labLocalVersion->setObjectName(QStringLiteral("labLocalVersion"));
        sizePolicy1.setHeightForWidth(labLocalVersion->sizePolicy().hasHeightForWidth());
        labLocalVersion->setSizePolicy(sizePolicy1);
        labLocalVersion->setMinimumSize(QSize(0, 32));
        labLocalVersion->setMaximumSize(QSize(16777215, 32));
        labLocalVersion->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labLocalVersion, 0, 0, 1, 1);


        horizontalLayout_3->addLayout(gridLayout);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(20, -1, 20, -1);
        versiontextEdit = new QTextEdit(SystemEmbeddedUpdateWidget);
        versiontextEdit->setObjectName(QStringLiteral("versiontextEdit"));

        horizontalLayout_4->addWidget(versiontextEdit);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(20, -1, 20, 24);
        pbUpdate = new QPushButton(SystemEmbeddedUpdateWidget);
        pbUpdate->setObjectName(QStringLiteral("pbUpdate"));
        sizePolicy1.setHeightForWidth(pbUpdate->sizePolicy().hasHeightForWidth());
        pbUpdate->setSizePolicy(sizePolicy1);
        pbUpdate->setMinimumSize(QSize(0, 32));
        pbUpdate->setMaximumSize(QSize(16777215, 32));

        horizontalLayout_2->addWidget(pbUpdate);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(SystemEmbeddedUpdateWidget);

        QMetaObject::connectSlotsByName(SystemEmbeddedUpdateWidget);
    } // setupUi

    void retranslateUi(QWidget *SystemEmbeddedUpdateWidget)
    {
        SystemEmbeddedUpdateWidget->setWindowTitle(QApplication::translate("SystemEmbeddedUpdateWidget", "Form", 0));
        pbCloseMac->setText(QString());
        labelTitle->setText(QApplication::translate("SystemEmbeddedUpdateWidget", "Title", 0));
        pbCloseWin->setText(QString());
        labLatestVersion->setText(QApplication::translate("SystemEmbeddedUpdateWidget", "LatestVersion", 0));
        labLatestVerionText->setText(QString());
        labLocalVersionText->setText(QString());
        labLocalVersion->setText(QApplication::translate("SystemEmbeddedUpdateWidget", "LocalVersion", 0));
        pbUpdate->setText(QApplication::translate("SystemEmbeddedUpdateWidget", "Update", 0));
    } // retranslateUi

};

namespace Ui {
    class SystemEmbeddedUpdateWidget: public Ui_SystemEmbeddedUpdateWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYSTEMEMBEDDEDUPDATEWIDGET_H
