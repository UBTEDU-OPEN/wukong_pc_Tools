#ifndef ASSIGNACTDEV_H
#define ASSIGNACTDEV_H

#include "ccustomdialog.h"

namespace Ui {
class CAssignActDev;
}

QT_BEGIN_NAMESPACE
class CTitleWidget;
class QTableWidget;
QT_END_NAMESPACE

/**
 * @brief 指定激活机器人
 */

class CAssignActDev : public XDialog
{
    Q_OBJECT

public:
    explicit CAssignActDev(QWidget *parent = 0);
    ~CAssignActDev();

public:
    void init ();
    void initVar ();
    void initSerialNumTableWidget ();
    void createTitleWidget ();
    void initWindows ();
    void initAction ();
    void layout();

protected:
    void refreshSerialNumTableWidget ();
    void loadSerialInfo();

public slots:
    void slotSave ();
    void slotClose ();

protected:
    CTitleWidget*     m_pTitleWidget;
    QStringList       m_listSerialInfo;

private:
    Ui::CAssignActDev *ui;
};

#endif // ASSIGNACTDEV_H
