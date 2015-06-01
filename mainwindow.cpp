#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdarg>

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
    str.sprintf(format, va_arg(vl, char*));
    qDebug() << str;
    return 1;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    my_log_callback = &my_log;
    fl_init(0);
    fl_set_log_callback(my_log_callback);
    fill_cb_programmers();
    fill_cb_arch();
}

MainWindow::~MainWindow()
{
    fl_shutdown();
    delete ui;
}

void MainWindow::on_b_sel_payload_clicked()
{
    QString rom_path = QFileDialog::getOpenFileName(this, tr("Select ROM"), ".", "All files (*.*)");
    QString rom_name;
    rom_name = rom_path.section('/', -1);
    ui->b_sel_payload->setVisible(false);
    ui->l_payload_name->setText(rom_name);
}

void MainWindow::fill_cb_programmers()
{
    const char **programmers = fl_supported_programmers();
    for(int i = 0; i < 21; ++i)
    {
        ui->cb_sel_progr->addItem(*programmers);
        ++programmers;
    }
}

void MainWindow::fill_cb_arch()
{
    ui->cb_sel_arch->addItem("x86");
    ui->cb_sel_arch->addItem("mips");
    ui->cb_sel_arch->addItem("arm");
    ui->cb_sel_arch->addItem("arm64");
}
