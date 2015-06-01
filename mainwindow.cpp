#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDebug>

extern "C" {
#include "libflashrom.h"
}



fl_log_callback_t *my_log_callback;


int my_log(fl_log_level_t log_level, const char *format, va_list vl)
{
    QString str;
    str.sprintf(format, vl);
    return 1;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fill_progr_cb();
    my_log_callback = &my_log;
    fl_set_log_callback(my_log_callback);
    fl_init(0);
    fl_supported_programmers();
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
    //qDebug() << programmer_table[0].name;
    ui->cb_sel_progr->addItem("internal");
    ui->cb_sel_progr->addItem("dummy");
    ui->cb_sel_progr->addItem("nic3com");
    ui->cb_sel_progr->addItem("nicrealtek");
    ui->cb_sel_progr->addItem("gfxnvidia");
    ui->cb_sel_progr->addItem("drkaiser");
    ui->cb_sel_progr->addItem("satasii");
    ui->cb_sel_progr->addItem("ft2232_spi");
    ui->cb_sel_progr->addItem("serprog");
    ui->cb_sel_progr->addItem("buspirate_spi");
    ui->cb_sel_progr->addItem("rayer_spi");
    ui->cb_sel_progr->addItem("nicintel");
    ui->cb_sel_progr->addItem("nicintel_spi");
    ui->cb_sel_progr->addItem("ogp_spi");
    ui->cb_sel_progr->addItem("satamv");
}
