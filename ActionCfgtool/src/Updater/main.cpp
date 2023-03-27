#include "updater.h"
#include <QtWidgets/QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString qsTmp, qsPath, qsId;
    for (int i = 0; i < argc; ++i) {
        qsTmp = argv[i];
        if (qsTmp.contains("path:", Qt::CaseInsensitive)) {
            qsTmp.remove(QRegExp("path:*", Qt::CaseInsensitive));
            qsPath = qsTmp;
        }
        else if (qsTmp.contains("id:", Qt::CaseInsensitive)) {
            qsTmp.remove(QRegExp("id:*", Qt::CaseInsensitive));
            qsId = qsTmp;
        }
    }
    Updater w;
    w.show();
    w.setParentProcessPathAndId(qsPath, qsId.toInt());
    w.start();
    return a.exec();
}
