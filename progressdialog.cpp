#include "progressdialog.h"
#include "ui_progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::ProgressDialog)
{
        ui->setupUi(this);
}

ProgressDialog::~ProgressDialog()
{
        delete ui;
}

void ProgressDialog::setText(QString &text)
{
        ui->l_info->setText(text);
}
