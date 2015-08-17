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
        default:
                ui->l_info->setText("Unknown error!");
        }
        show();
}

void InfoDialog::on_b_ok_clicked()
{
        close();
}

