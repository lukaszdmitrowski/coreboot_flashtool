#include "about.h"
#include "ui_about.h"

extern "C" {
#include "libflashrom.h"
}

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    /*fl_flashrom_info_t flashrom_info;

    ui->setupUi(this);
    fl_version(&flashrom_info);
    ui->l_flashrom_ver->setText(flashrom_info.version);*/
}

About::~About()
{
    delete ui;
}

void About::on_b_about_close_clicked()
{
    close();
}
