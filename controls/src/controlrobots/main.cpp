#include "ucontroldialog.h"
#include <QApplication>
#include "singleinstance.h"
#include "sharelog.h"
SHARE_EASYLOGGINGPP(getStorage())

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set application information
    a.setApplicationName ("controlrobots");
    a.setApplicationVersion ("v2.0.1");

    // Prevent many instances of the app to be launched
    QString name = "controlrobots";
    SingleInstance instance;
    if (instance.hasPrevious (name)){
        return EXIT_SUCCESS;
    }
    instance.listen (name);

    //初始化日志记录模块
    initShareLog();

    UControlDialog w;
    w.show();

    return a.exec();
}
