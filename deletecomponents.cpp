#include "deletecomponents.h"
#include "ui_deletecomponents.h"
#include "mainwindow.h"

extern "C" {
#include "libcbfstool.h"
}

DeleteComponents::DeleteComponents(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DeleteComponents)
{
        ui->setupUi(this);
}

DeleteComponents::~DeleteComponents()
{
        delete ui;
}

void DeleteComponents::on_b_remove_comp_clicked()
{
        char **cbfs_params;
        /* PROGNAME(1) + NAME(1) + COMMAND(1) */
        int param_count = 3;

        QString params[7];
        params[0] = "flash_tool";
        params[1] = w->get_flash_rom_path();
        params[2] = "remove";

        if (!ui->edit_name->text().isEmpty()) {
                params[param_count] = "-n";
                params[param_count + 1] = ui->edit_name->text();
                param_count += 2;
        }

        cbfs_params = new char*[param_count];
        for (int i = 0; i < param_count; ++i) {
                cbfs_params[i] = new char[params[i].length() + 1];
                strcpy(cbfs_params[i], params[i].toStdString().c_str());
        }

        start_cbfs(param_count, cbfs_params);

        for (int i = 0; i < param_count; ++i) {
                delete [] cbfs_params[i];
        }
        delete [] cbfs_params;
}
