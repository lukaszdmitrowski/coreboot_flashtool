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
        case ERR_FILE_SELECTED:
                ui->l_info->setText("Can't open file!");
                break;
        case ERR_FILE_WRITE:
                ui->l_info->setText("Can't write to file!");
                break;
        case ERR_FILE_HW_XML:
                ui->l_info->setText("Can't open file hardware_info.xml!");
                break;
        case ERR_FILE_CONFIG:
                ui->l_info->setText("Can't access config file!");
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
                ui->l_info->setText("Can't execute: 'tar -cf hardware_data.tar hardware_data'");
                break;
        case ERR_CMD_TAR_UNPACK_NOT_EXEC:
                ui->l_info->setText("Can't execute: 'tar -xf'");
                break;
        case ERR_CONFIG_NOT_KNOWN:
                ui->l_info->setText("No configuration for your system! Please send hardware_data.tar to"
                                    " lukasz.dmitrowski@gmail.com");
                break;
        case ERR_COREBOOT_COPY_ROM:
                ui->l_info->setText("Can't copy coreboot rom file!");
                break;
        case ERR_COREBOOT_COPY_CONFIG:
                ui->l_info->setText("Can't copy coreboot .config file!");
                break;
        case ERR_COREBOOT_MAKE:
                ui->l_info->setText("Coreboot compilation failed!");
                break;
        case ERR_COREBOOT_NEED_FACTORY_BIOS:
                ui->l_info->setText("This configuration requires factory VGABIOS, plase use 'Set factory BIOS button'");
                break;
        case ERR_COREBOOT_NOROM:
                ui->l_info->setText("Can't find coreboot rom!");
                break;
        case ERR_COREBOOT_WRONG_DIR:
                ui->l_info->setText("Can't find coreboot! Check path in preferences.");
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

