#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "selectprog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //creates user interaface
    ui->setupUi(this);
    fill_progr_cb();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_b_sel_payload_clicked()
{
    QString rom_path = QFileDialog::getOpenFileName(this, tr("Select ROM"), ".", "All files (*.*)");
    QString rom_name;
    rom_name = rom_path.section('/', -1);
    ui->l_payload_name->setText(rom_name);
}

void MainWindow::fill_progr_cb()
{
    ui->comboBox->addItem("internal");
    ui->comboBox->addItem("dummy");
    ui->comboBox->addItem("nic3com");
    ui->comboBox->addItem("nicrealtek");
    ui->comboBox->addItem("gfxnvidia");
    ui->comboBox->addItem("drkaiser");
    ui->comboBox->addItem("satasii");
    ui->comboBox->addItem("ft2232_spi");
    ui->comboBox->addItem("serprog");
    ui->comboBox->addItem("buspirate_spi");
    ui->comboBox->addItem("rayer_spi");
    ui->comboBox->addItem("nicintel");
    ui->comboBox->addItem("nicintel_spi");
    ui->comboBox->addItem("ogp_spi");
    ui->comboBox->addItem("satamv");
}
