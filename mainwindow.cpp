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
#include "bios_extract.h"
#include "libcbfstool.h"
}

fl_log_callback_t *my_log_callback;

int libflashrom_log(fl_log_level_t log_level, const char *format, va_list vl)
{
        QString text;

        text.vsprintf(format, vl);
        w->ui->log_flash->append(text);

        return 1;
}

int libbiosext_log(const char *const format, ...)
{
        int ret = 0;
        QString text;
        va_list args;

        va_start(args, format);
        text.vsprintf(format, args);
        w->ui->log_extract->insertPlainText(text);
        va_end(args);

        return ret;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
        ui->setupUi(this);

        ui->log_flash->setReadOnly(true);
        ui->log_rom_opt->setReadOnly(true);
        ui->log_extract->setReadOnly(true);
        ui->log_create_rom->setReadOnly(true);

        my_log_callback = &libflashrom_log;
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

void MainWindow::on_b_sel_boot_block_clicked()
{
        QString bootblock_name;

        bootblock_path = QFileDialog::getOpenFileName(this, tr("Select bootblock"), ".", "All files (*.*)");
        bootblock_name = bootblock_path.section('/', -1);
        ui->b_sel_bios_rom->setVisible(false);
        ui->l_bootblack_name->setText(bootblock_name);
}

void MainWindow::on_b_extract_clicked()
{
        extract_bios();
}

void MainWindow::on_b_create_rom_clicked()
{
        uint32_t arch = 0;
        uint32_t alignment = 64;
        uint32_t baseaddress = 0;

        QString selected_arch = ui->cb_sel_arch->currentText();
        qDebug() << "selected_arch: " << selected_arch << "currentIndex: " << ui->cb_sel_arch->currentIndex();

        if (selected_arch.compare("x86") == 0) {
                qDebug() << "x86";
                arch = CBFS_ARCHITECTURE_X86;
        } else if (selected_arch.compare("mips") == 0) {
                qDebug() << "mips";
                arch = CBFS_ARCHITECTURE_MIPS;
        } else if (selected_arch.compare("arm") == 0) {
                qDebug() << "arm";
                arch = CBFS_ARCHITECTURE_ARM;
        } else if (selected_arch.compare("arm64") == 0) {
                qDebug() << "arm64";
                arch = CBFS_ARCHITECTURE_AARCH64;
        } else {
                qDebug() << "unknown";
                arch = CBFS_ARCHITECTURE_UNKNOWN;
        }

        struct buffer bootblock;
        libcbfs_buffer_from_file(&bootblock, bootblock_path.toStdString().c_str());

        baseaddress = ui->edit_bootblock_off->text().toUInt();

        /*int cbfs_legacy_image_create(struct cbfs_image *image,
                                      uint32_t arch,
                                      uint32_t align,
                                      struct buffer *bootblock,
                                      uint32_t bootblock_offset,
                                      uint32_t header_offset,
                                      uint32_t entries_offset);*/
}

void MainWindow::on_cb_sel_progr_currentIndexChanged(const QString &programmer)
{
        fl_shutdown();
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
        QString rom_dir = QFileDialog::getExistingDirectory(this,
                                                            tr("Select output dir"),
                                                            ".",
                                                            QFileDialog::ShowDirsOnly
                                                            | QFileDialog::DontResolveSymlinks);
        rom_dir.append("/");
        ui->b_sel_bios_out->setVisible(false);
        ui->l_ex_out_name->setText(rom_dir);
        set_output_directory(rom_dir.toStdString().c_str());
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
        return start_bios_extract(bios_rom_path.toStdString().c_str());
}
