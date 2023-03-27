#include <QApplication>
#include <QProcess>
#include "updateManager.h"
#include <downloadmanager.h>
#include "urldef.h"
/**
 * @brief exitApp 退出指定应用程序
 * @param strAppName 应用程序名称
 */
void exitApp (QString strAppName)
{
    QProcess p;
    QString strCloseApp = QString("taskkill /im %1 /f").arg(strAppName);
    p.execute(strCloseApp);
    p.close();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStringList arguments = app.arguments();
    arguments.takeFirst(); // remove the first argument, which is the program's name;
    // 关闭主进程
    exitApp (PROGRAM_NAME);
    // 如果外部调用，例如主程序调用更新程序，则根据外部传入参数进行处理。否则更新程序自己向后台请求数据
    if ( arguments.isEmpty() ){
        // 更新程序单独运行，更新程序向后台请求
        UpdateManager updateManager;
        updateManager.checkUpdate();
        QObject::connect(&updateManager, SIGNAL(finished()), &app, SLOT(quit()));
        app.exec();
    }else{
        // 外部调用升级程序
        DownloadManager downloadManger;
        UpdateParam* param = downloadManger.getServerParam();
        param->setPackageUrl(arguments.takeFirst());  // 第一位保存升级包URL
        param->setPackageMd5(arguments.takeFirst());  // 第二位保存升级包MD5
        downloadManger.startDownLoad();
        QObject::connect(&downloadManger, SIGNAL(finished()), &app, SLOT(quit()));
        app.exec();
    }

    return 0;
}
