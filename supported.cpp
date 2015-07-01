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
        sortFilterProxy = new QSortFilterProxyModel(this);
        sortFilterProxy->setSourceModel(model);
        sortFilterProxy->setDynamicSortFilter(true);
        flashchip_info = fl_supported_flash_chips();
        boards_list = fl_supported_boards();
        chipsets_list = fl_supported_chipsets();
        ui->cb_sel_hardware->addItem("Chips");
        ui->cb_sel_hardware->addItem("Boards");
        ui->cb_sel_hardware->addItem("Chipsets");
}

Supported::~Supported()
{
        fl_supported_info_free(flashchip_info);
        fl_supported_info_free(boards_list);
        fl_supported_info_free(chipsets_list);
        delete ui;
}

void Supported::show_flash_chips()
{
        QString last_vendor;

        model->clear();
        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Vendor")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Name")));
        model->setHorizontalHeaderItem(2, new QStandardItem(QString("Test OK")));
        model->setHorizontalHeaderItem(3, new QStandardItem(QString("Broken")));
        model->setHorizontalHeaderItem(4, new QStandardItem(QString("Size [kB]")));
        ui->cb_sel_vendor->addItem("");

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

                if (last_vendor.compare(flashchip_info[i].vendor)) {
                        last_vendor = flashchip_info[i].vendor;
                        ui->cb_sel_vendor->addItem(last_vendor);
                }

                flashchip_row.append(new QStandardItem(QString(flashchip_info[i].vendor)));
                flashchip_row.append(new QStandardItem(QString(flashchip_info[i].name)));
                flashchip_row.append(new QStandardItem(tested));
                flashchip_row.append(new QStandardItem(known_broken));
                flashchip_row.append(new QStandardItem(QString::number(flashchip_info[i].total_size)));
                model->appendRow(flashchip_row);
        }

        ui->tableView->setModel(sortFilterProxy);
        ui->tableView->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
        ui->tableView->setColumnWidth(0, 155);
        ui->tableView->setColumnWidth(1, 400);
        ui->tableView->setColumnWidth(2, 70);
        ui->tableView->setColumnWidth(3, 70);
        ui->tableView->setColumnWidth(4, 70);
}

void Supported::show_boards()
{
        model->clear();
        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Vendor")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Board")));
        model->setHorizontalHeaderItem(2, new QStandardItem(QString("Status")));

        for (unsigned int i = 0; boards_list[i].vendor; ++i) {
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
}

void Supported::show_chipsets()
{
        model->clear();
        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Vendor")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Chipset")));
        model->setHorizontalHeaderItem(2, new QStandardItem(QString("PCI IDs")));
        model->setHorizontalHeaderItem(3, new QStandardItem(QString("Status")));

        for (unsigned int i = 0; chipsets_list[i].vendor; ++i) {
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
        ui->tableView->setColumnWidth(3, 140);
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

void Supported::on_cb_sel_hardware_currentIndexChanged(int index)
{
        ui->cb_sel_vendor->clear();
        switch (index) {
        case 0:
                show_flash_chips();
                break;
        case 1:
                show_boards();
                break;
        case 2:
                show_chipsets();
                break;
    }
}

void Supported::on_cb_sel_vendor_currentIndexChanged(int index)
{
        if (index != 0) {
                sortFilterProxy->setFilterWildcard(ui->cb_sel_vendor->currentText());
                sortFilterProxy->setFilterKeyColumn(0);
        }
}

void Supported::on_edit_name_textChanged(const QString &arg1)
{
    sortFilterProxy->setFilterWildcard(arg1);
    sortFilterProxy->setFilterKeyColumn(1);
}
