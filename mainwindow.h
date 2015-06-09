#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void on_b_sel_payload_clicked();
    void on_cb_sel_progr_currentIndexChanged(const QString &arg1);
    void on_b_read_clicked();

    void on_act_sel_payload_triggered();

    void on_act_supported_list_triggered();

private:
    void fill_cb_programmers();
    void fill_cb_arch();
    void open_select_rom_window();
};

extern MainWindow *w;

#endif // MAINWINDOW_H
