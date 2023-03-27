#include "timesettingdialog.h"
#include "ui_timesettingdialog.h"
#include <QFile>

CTimeSettingDialog::CTimeSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CTimeSettingDialog)
//    m_nTimeScale(0)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);

    QFile styleSheet(":/res/qss/layerdefault.qss");
    if(styleSheet.open(QIODevice::ReadOnly))
    {
        setStyleSheet(styleSheet.readAll());
    }
}

CTimeSettingDialog::~CTimeSettingDialog()
{
    delete ui;
}

/**************************************************************************
* 函数名：SetTimeScale
* 功能:设置时间粒度
* 参数:
*    @[in ] nTimeScale: 时间粒度
* 返回值: 无
* 时间: 2017/1/16
* 作者: Joker
*/
void CTimeSettingDialog::SetTimeScale(int nTimeScale)
{
//    m_nTimeScale = nTimeScale;
    QString strTimeScale = QString("%1").arg(nTimeScale);
    ui->lESettingTime->setText(strTimeScale);
}

/**************************************************************************
* 函数名：GetTimeScale
* 功能:获取时间粒度
* 参数: 无
* 返回值: int 返回时间粒度
* 时间: 2017/1/16
* 作者: Joker
*/
int CTimeSettingDialog::GetTimeScale()
{
      QString strTimeScale = ui->lESettingTime->text();
      int nTimeScale = strTimeScale.toInt();
      return nTimeScale;
}

void CTimeSettingDialog::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CTimeSettingDialog::on_pbClose_clicked()
{
    close();
}

void CTimeSettingDialog::on_pbCancel_clicked()
{
    done(QDialog::Rejected);
}

void CTimeSettingDialog::on_pbOK_clicked()
{
    done(QDialog::Accepted);
}
