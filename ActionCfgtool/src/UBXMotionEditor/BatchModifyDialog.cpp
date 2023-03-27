#include "BatchModifyDialog.h"
#include "ui_BatchModifyDialog.h"

BatchModifyDialog::BatchModifyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BatchModifyDialog)
{
    ui->setupUi(this);
    m_strModifyValue = "";

    QRegExp regx("[0-9]{1,5}");
    QValidator *validatorRunTime = new QRegExpValidator(regx, ui->lineEditModifyValue);
    ui->lineEditModifyValue->setValidator(validatorRunTime);
    ui->lineEditModifyValue->setAttribute(Qt::WA_MacShowFocusRect, false);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

BatchModifyDialog::~BatchModifyDialog()
{
    delete ui;
}

void BatchModifyDialog::on_btnOK_clicked()
{
    QString strText = ui->lineEditModifyValue->text();
    m_strModifyValue = strText;
    done(QDialog::Accepted);
}

void BatchModifyDialog::on_btnCancel_clicked()
{
    done(QDialog::Rejected);
}

void BatchModifyDialog::on_pbClose_clicked()
{
    close();
}
