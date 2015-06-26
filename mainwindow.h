#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>

extern "C" {
#include "libflashrom.h"
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;
    QTextEdit *cbfs_log_out;
    QString get_flash_rom_path();

private slots:
    void on_b_sel_payload_clicked();
    void on_cb_sel_progr_currentIndexChanged(const QString &arg1);
    void on_b_read_clicked();
    void on_act_sel_payload_triggered();
    void on_act_supported_list_triggered();
    void on_act_about_triggered();
    void on_b_sel_bios_rom_clicked();
    void on_b_sel_bios_out_clicked();

    void on_b_extract_clicked();
    void on_b_create_rom_clicked();
    void on_b_sel_boot_block_clicked();

    void on_b_add_payload_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_b_add_component_clicked();

    void on_b_remove_comp_clicked();

private:
    void fill_cb_programmers();
    void fill_cb_arch();
    void open_select_rom_window();
    void open_select_bios_rom_window();
    void open_select_bios_out_window();
    int extract_bios();
    void print_rom();
    QString flash_rom_path;
    QString bios_rom_path;
    QString bootblock_path;
};

extern MainWindow *w;

#endif // MAINWINDOW_H
