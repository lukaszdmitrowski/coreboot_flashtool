#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "supported.h"
#include "about.h"
#include <cstdarg>

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDebug>

extern "C" {
#include "libflashrom.h"
int bios_extract_log(const char* text);
#include "bios_extract/bios_extract.h"
}

fl_log_callback_t *my_log_callback;


int my_log(fl_log_level_t log_level, const char *format, va_list vl)
{
        QString text;
        text.sprintf(format, va_arg(vl, char*));
        qDebug() << text;
        //w->ui->log_flash->append(text);
        return 1;
}

int bios_extract_log(const char* format)
{
        w->ui->log_extract->append(text);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
        ui->setupUi(this);
        my_log_callback = &my_log;
        fl_init(0);
        fill_cb_arch();
        fill_cb_programmers();
        fl_set_log_callback(my_log_callback);
}

MainWindow::~MainWindow()
{
        fl_shutdown();
        delete ui;
}

void MainWindow::on_b_sel_payload_clicked()
{
        open_select_rom_window();
}

void MainWindow::on_b_read_clicked()
{
}

void MainWindow::on_b_sel_bios_rom_clicked()
{
        open_select_bios_rom_window();
}

void MainWindow::on_b_sel_bios_out_clicked()
{
        open_select_bios_out_window();
}

void MainWindow::on_b_extract_clicked()
{
        extract_bios();
}

void MainWindow::on_cb_sel_progr_currentIndexChanged(const QString &programmer)
{
        fl_programmer_init(programmer.toStdString().c_str(), "");
}

void MainWindow::fill_cb_programmers()
{
        unsigned int programmers_number = 0;
        const char **programmers = 0;

        programmers_number = fl_supported_programmers_number();
        programmers = new const char*[programmers_number];
        fl_supported_programmers(programmers);

        for(unsigned int i = 0; i < programmers_number; ++i) {
                ui->cb_sel_progr->addItem(programmers[i]);
        }

        delete [] programmers;
}

void MainWindow::fill_cb_arch()
{
        ui->cb_sel_arch->addItem("x86");
        ui->cb_sel_arch->addItem("mips");
        ui->cb_sel_arch->addItem("arm");
        ui->cb_sel_arch->addItem("arm64");
}

void MainWindow::open_select_rom_window()
{
        QString rom_name;

        flash_rom_path = QFileDialog::getOpenFileName(this, tr("Select ROM"), ".", "All files (*.*)");
        rom_name = flash_rom_path.section('/', -1);
        ui->b_sel_payload->setVisible(false);
        ui->l_payload_name->setText(rom_name);
}

void MainWindow::open_select_bios_rom_window()
{
        QString rom_name;

        bios_rom_path = QFileDialog::getOpenFileName(this, tr("Select ROM"), ".", "All files (*.*)");
        rom_name = bios_rom_path.section('/', -1);
        ui->b_sel_bios_rom->setVisible(false);
        ui->l_ex_rom_name->setText(rom_name);
}

void MainWindow::open_select_bios_out_window()
{
        QString rom_path = QFileDialog::getOpenFileName(this, tr("Select ROM"), ".", "All files (*.*)");
        QString rom_name;
        rom_name = rom_path.section('/', -1);
        ui->b_sel_bios_out->setVisible(false);
        ui->l_ex_out_name->setText(rom_name);
}


void MainWindow::on_act_sel_payload_triggered()
{
        open_select_rom_window();
}

void MainWindow::on_act_supported_list_triggered()
{             
        Supported supported_window;
        supported_window.setModal(true);
        supported_window.exec();
}

void MainWindow::on_act_about_triggered()
{
        About about_window;
        about_window.setModal(true);
        about_window.exec();
}

int MainWindow::extract_bios()
{
        qDebug() << bios_rom_path.toStdString().c_str();
        start_bios_extract(bios_rom_path.toStdString().c_str());
        return 1;
}
