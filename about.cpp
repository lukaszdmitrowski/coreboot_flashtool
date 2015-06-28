#include "about.h"
#include "ui_about.h"

extern "C" {
#include "libflashrom.h"
}

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->l_flashrom_ver->setText(fl_version());
}

About::~About()
{
    delete ui;
}

void About::on_b_about_close_clicked()
{
    close();
}
