#include "preferences.h"
#include "ui_preferences.h"
#include "mainwindow.h"

#include <QFileDialog>

Preferences::Preferences(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Preferences)
{
        ui->setupUi(this);
}

Preferences::~Preferences()
{
        delete ui;
}

void Preferences::on_b_sel_cor_path_clicked()
{
        QString coreboot_dir = QFileDialog::getExistingDirectory(this,
                                                                tr("Select coreboot dir"),
                                                                ".",
                                                                QFileDialog::ShowDirsOnly
                                                                | QFileDialog::DontResolveSymlinks);
        coreboot_dir.append("/");
        ui->edit_cor_path->setText(coreboot_dir);
        w->coreboot_dir = coreboot_dir;
}

void Preferences::on_b_ok_clicked()
{
    close();
}
