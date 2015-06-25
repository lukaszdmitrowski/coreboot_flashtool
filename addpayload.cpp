#include <QFileDialog>
#include <QDebug>

#include "addpayload.h"
#include "ui_addpayload.h"
#include "mainwindow.h"

extern "C" {
#include "libcbfstool.h"
}

AddPayload::AddPayload(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::AddPayload)
{
        ui->setupUi(this);
        ui->cb_sel_compression->addItem("lzma");
        ui->cb_sel_compression->addItem("none");
}

AddPayload::~AddPayload()
{
        delete ui;
}

void AddPayload::on_b_sel_payload_clicked()
{
        QString payload_name;

        payload_path = QFileDialog::getOpenFileName(this, tr("Select payload"), ".", "All files (*.*)");
        payload_name = payload_path.section('/', -1);
        ui->b_sel_payload->setVisible(false);
        ui->l_payload_name->setText(payload_name);
}

void AddPayload::on_b_add_payload_clicked()
{
        char **cbfs_params;
        /* PROGNAME(1) + NAME(1) + COMMAND(1) */
        int param_count = 3;

        QString params[6];
        params[0] = "flash_tool";
        params[1] = w->get_flash_rom_path();
        params[2] = "add-payload";

        if (!payload_path.isEmpty()) {
                param_count += 2;
                params[3] = "-f ";
                params[4] = payload_path;
        }
        if (!ui->edit_name->text().isEmpty()) {
                param_count += 2;
                params[5] = "-n ";
                params[6] = ui->edit_name->text();
        }

        cbfs_params = new char*[param_count];
        for (int i = 0; i < param_count; ++i) {
                cbfs_params[i] = new char[params[i].length() + 1];
                strcpy(cbfs_params[i], params[i].toStdString().c_str());
        }

        start_cbfs(param_count, cbfs_params);

        qDebug() << "param_count: " << param_count;
        for (int i = 0; i < param_count; ++i) {
                qDebug() << "cbfs_param: " << cbfs_params[i];
                delete [] cbfs_params[i];
        }
        delete [] cbfs_params;
}
