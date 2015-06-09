#include "supported.h"
#include "ui_supported.h"

#include <QStandardItemModel>
#include <QDebug>

extern "C" {
#include "libflashrom.h"
}

Supported::Supported(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Supported)
{
    ui->setupUi(this);
    QStandardItemModel *model = new QStandardItemModel(2,3,this); //2 Rows and 3 Columns

    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Column1 Header")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Column2 Header")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));

    ui->tableView->setModel(model);

    QStandardItem *firstRow = new QStandardItem(QString("ColumnValue"));
    model->setItem(0,0,firstRow);

    fl_flashchip_t const **myflashchip;
    fl_supported_flash_chips(myflashchip);
    qDebug() << (*myflashchip)->vendor;
}

Supported::~Supported()
{
    delete ui;
}
