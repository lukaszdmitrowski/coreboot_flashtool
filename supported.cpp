#include "supported.h"
#include "ui_supported.h"

#include "memory.h"

#include <QDebug>

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
    fl_flashchip_info_t *flashchip_info = 0;

    flashchip_info = fl_supported_flash_chips();

    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Vendor")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Name")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Test OK")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Broken")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QString("Size [kB]")));

    for (unsigned int i = 0; flashchip_info[i].name; ++i) {
        QList<QStandardItem*> flashchip_row;
        QString tested;
        QString known_broken;

        if (flashchip_info[i].tested.probe == FL_TESTED_OK)
            tested.append("P");
        else if (flashchip_info[i].tested.probe == FL_TESTED_NA)
            tested.append("-");
        else
            tested.append(" ");
        if (flashchip_info[i].tested.read == FL_TESTED_OK)
            tested.append("R");
        else if (flashchip_info[i].tested.read == FL_TESTED_NA)
            tested.append("-");
        else
            tested.append(" ");
        if (flashchip_info[i].tested.erase == FL_TESTED_OK)
            tested.append("E");
        else if (flashchip_info[i].tested.erase == FL_TESTED_NA)
            tested.append("-");
        else
            tested.append(" ");
        if (flashchip_info[i].tested.write == FL_TESTED_OK)
            tested.append("W");
        else if (flashchip_info[i].tested.write == FL_TESTED_NA)
            tested.append(" ");

        if (flashchip_info[i].tested.probe == FL_TESTED_BAD)
            known_broken.append("P");
        else
            known_broken.append(" ");
        if (flashchip_info[i].tested.read == FL_TESTED_BAD)
            known_broken.append("R");
        else
            known_broken.append(" ");
        if (flashchip_info[i].tested.erase == FL_TESTED_BAD)
            known_broken.append("E");
        else
            known_broken.append(" ");
        if (flashchip_info[i].tested.write == FL_TESTED_BAD)
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
    ui->tableView->setColumnWidth(4, 70);
}

void Supported::on_b_show_boards_clicked()
{
    /*unsigned int boards_list_size = 0;
    fl_board_info_t *boards_list;

    boards_list_size = fl_supported_boards_number();
    boards_list = new fl_board_info_t[boards_list_size];
    memset(boards_list, 0, sizeof(boards_list) * boards_list_size);
    fl_supported_boards(boards_list);

    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Vendor")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Board")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Status")));

    for (unsigned int i = 0; i < boards_list_size - 1; ++i) {
        QList<QStandardItem*> board_row;
        QString status;

        switch (boards_list[i].working) {
        case FL_TESTED_OK:
            status.append("OK");
            break;
        case FL_TESTED_NT:
            status.append("NT");
            break;
        case FL_TESTED_DEP:
            status.append("DEP");
            break;
        case FL_TESTED_NA:
            status.append("NA");
            break;
        case FL_TESTED_BAD:
        default:
            status.append("BAD");
            break;
        }

        board_row.append(new QStandardItem(QString(boards_list[i].vendor)));
        board_row.append(new QStandardItem(QString(boards_list[i].name)));
        board_row.append(new QStandardItem(status));
        model->appendRow(board_row);
    }

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(0, 155);
    ui->tableView->setColumnWidth(1, 400);
    ui->tableView->setColumnWidth(2, 70);

    delete [] boards_list;*/
}

void Supported::on_b_show_chipsets_clicked()
{
    /*unsigned int chipsets_list_size = 0;
    fl_chipset_info_t *chipsets_list;

    chipsets_list_size = fl_supported_chipsets_number();
    chipsets_list = new fl_chipset_info_t[chipsets_list_size];
    memset(chipsets_list, 0, sizeof(chipsets_list) * chipsets_list_size);
    fl_supported_chipsets(chipsets_list);

    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Vendor")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Chipset")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("PCI IDs")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Status")));

    for (unsigned int i = 0; i < chipsets_list_size - 1; ++i) {
        QList<QStandardItem*> chipset_row;

        chipset_row.append(new QStandardItem(QString(chipsets_list[i].vendor)));
        chipset_row.append(new QStandardItem(QString(chipsets_list[i].chipset)));
        chipset_row.append(new QStandardItem(QString("xxxx::xxxx")));
        chipset_row.append(new QStandardItem(test_state_to_qstring(chipsets_list[i].status)));
        model->appendRow(chipset_row);
    }

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(0, 155);
    ui->tableView->setColumnWidth(1, 350);
    ui->tableView->setColumnWidth(2, 90);
    ui->tableView->setColumnWidth(3, 140);*/
}

QString Supported::test_state_to_qstring(fl_test_state test_state)
{
    switch (test_state) {
    case FL_TESTED_OK:
        return QString("OK");
    case FL_TESTED_BAD:
        return QString("Not working");
    case FL_TESTED_NA:
        return QString("N/A");
    case FL_TESTED_DEP:
        return QString("Config-dependent");
    case FL_TESTED_NT:
    default:
        return QString("Untested");
    }
}
