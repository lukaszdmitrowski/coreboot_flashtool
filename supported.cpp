#include "supported.h"
#include "ui_supported.h"

#include "memory.h"

#include <QDebug>

extern "C" {
#include "libflashrom.h"
}

Supported::Supported(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Supported)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);
}

Supported::~Supported()
{
    delete ui;
}

void Supported::on_b_show_chips_clicked()
{
    unsigned int flashchip_info_size = 0;
    fl_flashchip_info_t *flashchip_info = 0;

    flashchip_info_size = fl_supported_flash_chips_size();
    flashchip_info = new fl_flashchip_info_t[flashchip_info_size];
    memset(flashchip_info, 0, sizeof(flashchip_info) * flashchip_info_size);
    fl_supported_flash_chips(flashchip_info);

    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Vendor")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Name")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Test OK")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Broken")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QString("Size [kB]")));


    for (unsigned int i = 0; i < flashchip_info_size - 1; ++i) {
        QList<QStandardItem*> flashchip_row;
        QString tested;
        QString known_broken;

        if (flashchip_info[i].tested.probe == FL_CHIP_TEST_OK)
            tested.append("P");
        else if (flashchip_info[i].tested.probe == FL_CHIP_TEST_NA)
            tested.append("-");
        else
            tested.append(" ");
        if (flashchip_info[i].tested.read == FL_CHIP_TEST_OK)
            tested.append("R");
        else if (flashchip_info[i].tested.read == FL_CHIP_TEST_NA)
            tested.append("-");
        else
            tested.append(" ");
        if (flashchip_info[i].tested.erase == FL_CHIP_TEST_OK)
            tested.append("E");
        else if (flashchip_info[i].tested.erase == FL_CHIP_TEST_NA)
            tested.append("-");
        else
            tested.append(" ");
        if (flashchip_info[i].tested.write == FL_CHIP_TEST_OK)
            tested.append("W");
        else if (flashchip_info[i].tested.write == FL_CHIP_TEST_NA)
            tested.append(" ");

        if (flashchip_info[i].tested.probe == FL_CHIP_TEST_BAD)
            known_broken.append("P");
        else
            known_broken.append(" ");
        if (flashchip_info[i].tested.read == FL_CHIP_TEST_BAD)
            known_broken.append("R");
        else
            known_broken.append(" ");
        if (flashchip_info[i].tested.erase == FL_CHIP_TEST_BAD)
            known_broken.append("E");
        else
            known_broken.append(" ");
        if (flashchip_info[i].tested.write == FL_CHIP_TEST_BAD)
            known_broken.append("W");
        else
            known_broken.append(" ");

        flashchip_row.append(new QStandardItem(QString(flashchip_info[i].vendor)));
        flashchip_row.append(new QStandardItem(QString(flashchip_info[i].name)));
        flashchip_row.append(new QStandardItem(tested));
        flashchip_row.append(new QStandardItem(known_broken));
        flashchip_row.append(new QStandardItem(QString::number(flashchip_info[i].total_size)));

        model->appendRow(flashchip_row);
    }

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(0, 155);
    ui->tableView->setColumnWidth(1, 400);
    ui->tableView->setColumnWidth(2, 70);
    ui->tableView->setColumnWidth(3, 70);
    ui->tableView->setColumnWidth(4, 65);

    delete [] flashchip_info;
}

void Supported::on_b_show_boards_clicked()
{
    model->clear();
}

void Supported::on_b_show_chipsets_clicked()
{
    model->clear();
}
