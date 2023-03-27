#include "dialogcommandbox.h"
#include "ui_dialogcommandbox.h"

DialogCommandBox::DialogCommandBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCommandBox)
{
    ui->setupUi(this);
    m_strCommandBoxName = "";
    ui->lineEditBlockName->setAttribute(Qt::WA_MacShowFocusRect, false);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

DialogCommandBox::~DialogCommandBox()
{
    delete ui;
}

void DialogCommandBox::on_btnOK_clicked()
{
    QString strText = ui->lineEditBlockName->text();
    m_strCommandBoxName = strText;
    done(QDialog::Accepted);
}

void DialogCommandBox::on_btnCancel_clicked()
{
    done(QDialog::Rejected);
}

void DialogCommandBox::on_pbClose_clicked()
{
    close();
}
