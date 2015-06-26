#include <QFileDialog>

#include "addcomponent.h"
#include "ui_addcomponent.h"

#include "mainwindow.h"

extern "C" {
#include "libcbfstool.h"
}

AddComponent::AddComponent(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::AddComponent)
{
        ui->setupUi(this);
        fill_cb_type();
}

AddComponent::~AddComponent()
{
        delete ui;
}

void AddComponent::on_b_sel_component_clicked()
{
        QString component_name;

        component_path = QFileDialog::getOpenFileName(this, tr("Select component"), ".", "All files (*.*)");
        component_name = component_path.section('/', -1);
        ui->b_sel_component->setVisible(false);
        ui->l_component_name->setText(component_name);
}

void AddComponent::on_b_add_component_clicked()
{
        char **cbfs_params;
        /* PROGNAME(1) + NAME(1) + COMMAND(1) */
        int param_count = 3;

        QString params[9];
        params[0] = "flash_tool";
        params[1] = w->get_flash_rom_path();
        params[2] = "add";

        if (!component_path.isEmpty()) {
                params[param_count] = "-f";
                params[param_count + 1] = component_path;
                param_count += 2;
        }
        if (!ui->edit_name->text().isEmpty()) {
                params[param_count] = "-n";
                params[param_count + 1] = ui->edit_name->text();
                param_count += 2;
        }

        params[param_count] = "-t";
        params[param_count + 1] = ui->cb_sel_type->currentText();
        param_count += 2;

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

void AddComponent::fill_cb_type()
{
        ui->cb_sel_type->addItem("stage");
        ui->cb_sel_type->addItem("payload");
        ui->cb_sel_type->addItem("optionrom");
        ui->cb_sel_type->addItem("bootsplash");
        ui->cb_sel_type->addItem("raw");
        ui->cb_sel_type->addItem("vsa");
        ui->cb_sel_type->addItem("mbi");
        ui->cb_sel_type->addItem("microcode");
        ui->cb_sel_type->addItem("fsp");
        ui->cb_sel_type->addItem("mrc");
        ui->cb_sel_type->addItem("cmos default");
        ui->cb_sel_type->addItem("cmos layout");
        ui->cb_sel_type->addItem("spd");
        ui->cb_sel_type->addItem("mrc_cache");
        ui->cb_sel_type->addItem("mbi");
        ui->cb_sel_type->addItem("deleted");
        ui->cb_sel_type->addItem("null");
}
