#ifndef TIMESETTINGDIALOG_H
#define TIMESETTINGDIALOG_H

#include "motioneditor_global.h"
#include <QDialog>

namespace Ui {
class CTimeSettingDialog;
}

class MOTIONEDITOR_EXPORT CTimeSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CTimeSettingDialog(QWidget *parent = 0);
    ~CTimeSettingDialog();
public:
    /**************************************************************************
    * 函数名：SetTimeScale
    * 功能:设置时间粒度
    * 参数:
    *    @[in ] nTimeScale: 时间粒度
    * 返回值: 无
    * 时间: 2017/1/16
    * 作者: Joker
    */
    void SetTimeScale(int nTimeScale);

    /**************************************************************************
    * 函数名：GetTimeScale
    * 功能:获取时间粒度
    * 参数: 无
    * 返回值: int 返回时间粒度
    * 时间: 2017/1/16
    * 作者: Joker
    */
    int GetTimeScale();

protected:
    virtual void paintEvent(QPaintEvent *);

private slots:
    void on_pbClose_clicked();

    void on_pbCancel_clicked();

    void on_pbOK_clicked();

private:
    Ui::CTimeSettingDialog *ui;
//    int m_nTimeScale;
};

#endif // TIMESETTINGDIALOG_H
