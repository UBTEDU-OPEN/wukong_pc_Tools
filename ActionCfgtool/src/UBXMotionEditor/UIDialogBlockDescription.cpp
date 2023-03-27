#include "UIDialogBlockDescription.h"
#include "ui_UIDialogBlockDescription.h"

DialogBlockDescription::DialogBlockDescription(QWidget *parent, QString strDescripionInfo) :
    QDialog(parent),
    ui(new Ui::DialogBlockDescription)
{
    ui->setupUi(this);
    if(strDescripionInfo.isEmpty())
    {
        m_strBlockDescriptionInfo = tr("DefaultName");
    }
    else
    {
        m_strBlockDescriptionInfo = strDescripionInfo;
    }
    m_nStartTime = 0;
    m_nEndTime = 0;
    ui->labelStartTime->setText(tr("StartTime"));
    ui->labelEndTime->setText(tr("EndTime"));
    ui->labelDescription->setText(tr("Description"));
    ui->lineEditStartime->setEnabled(false);
    ui->lineEditEndTime->setEnabled(false);

    ui->lineEditStartime->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEditEndTime->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->lineEditDescription->setAttribute(Qt::WA_MacShowFocusRect, false);
}

DialogBlockDescription::~DialogBlockDescription()
{
    delete ui;
}

void DialogBlockDescription::SetStartTime(int nStartTime)
{
    m_nStartTime = nStartTime;
    QString strStartTime = QString("%1").arg(nStartTime);
    ui->lineEditStartime->setText(strStartTime);
}

void DialogBlockDescription::SetEndTime(int nEndTime)
{
    m_nEndTime = nEndTime;
    QString strEndTime = QString("%1").arg(nEndTime);
    ui->lineEditEndTime->setText(strEndTime);
}
void DialogBlockDescription::SetDescription(QString  strDescription)
{
  m_strBlockDescriptionInfo = strDescription;
  ui->lineEditDescription->setText(strDescription);
}

void DialogBlockDescription::on_btnOK_clicked()
{
    QString strText = ui->lineEditDescription->text();
    m_strBlockDescriptionInfo = strText;
    done(QDialog::Accepted);
}

void DialogBlockDescription::on_btnCancel_clicked()
{
    done(QDialog::Rejected);
}
