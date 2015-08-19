#include "preferences.h"
#include "ui_preferences.h"
#include "mainwindow.h"

#include <QTextStream>
#include <QFileDialog>
#include <QFile>

Preferences::Preferences(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Preferences)
{
        ui->setupUi(this);
        ui->edit_cor_path->setText(w->coreboot_dir);
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
        QFile config_file("preferences.cfg");
        QString new_coreboot_path = "coreboot_path: " + ui->edit_cor_path->text();

        if (config_file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
                QTextStream config_stream(&config_file);
                config_stream << new_coreboot_path;
        } else {
                w->info_dialog->show_message(ERR_FILE_CONFIG);
        }
        close();
}
