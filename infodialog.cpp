#include "infodialog.h"
#include "ui_infodialog.h"

InfoDialog::InfoDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::InfoDialog)
{
        ui->setupUi(this);
        setModal(true);
        setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
}

InfoDialog::~InfoDialog()
{
        delete ui;
}

void InfoDialog::show_message(RET_VAL ret_val)
{
        switch(ret_val) {
        case SUCCESS:
                ui->l_info->setText("Success!");
                break;
        case ERR_CANT_OPEN_FILE:
                ui->l_info->setText("Can't open file!");
                break;
        case ERR_WRITE_FAILED:
                ui->l_info->setText("Write failed!");
                break;
        case ERR_CHIP_NOT_READ:
                ui->l_info->setText("Can't read chip!");
                break;
        case ERR_CHIP_NOT_VERIFIED:
                ui->l_info->setText("Chip content is different!");
                break;
        case ERR_PROG_NOT_INIT:
                ui->l_info->setText("Programmer not initialized!");
                break;
        case ERR_DIR_NOT_SEL:
                ui->l_info->setText("Output directory not selected!");
                break;
        case ERR_MEM_ALLOC:
                ui->l_info->setText("Memory allocation failed!");
                break;
        case ERR_BUF_SIZE_DIFF:
                ui->l_info->setText("Buffer size different than chip size!");
                break;
        case ERR_CMD_LSPCI_NOT_EXEC:
                ui->l_info->setText("Cant't execute: 'lspci -nn > hardware_data/lspci_output.txt'");
                break;
        case ERR_CMD_EDID_NOT_EXEC:
                ui->l_info->setText("Can't execute: 'cat /sys/class/drm/card0-LVDS-1/edid | edid-decode > hardware_data/edid-decode_output.txt'");
                break;
        case ERR_CMD_DMI_NOT_EXEC:
                ui->l_info->setText("Can't execute: 'dmidecode -t 2 > hardware_data/dmidecode_output.txt'");
                break;
        case ERR_CMD_VGBABIOS_NOT_EXEC:
                ui->l_info->setText("Can't execute: extract_vga_bios.sh");
                break;
        case ERR_CMD_TAR_PACK_NOT_EXEC:
                ui->l_info->setText("Can't execute: tar -cf hardware_data.tar hardware_data");
                break;
        default:
                ui->l_info->setText("Unknown error!");
        }
        exec();
}

void InfoDialog::on_b_ok_clicked()
{
        close();
}

