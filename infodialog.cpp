#include "infodialog.h"
#include "ui_infodialog.h"

InfoDialog::InfoDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::InfoDialog)
{
        ui->setupUi(this);
        ui->b_ok->setVisible(false);
        setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
        setModal(true);
}

InfoDialog::~InfoDialog()
{
        delete ui;
}

void InfoDialog::setText(QString text)
{
        ui->l_info->setText(text);
}



