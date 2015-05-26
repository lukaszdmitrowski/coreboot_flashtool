#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_b_sel_payload_clicked();

private:
    void fill_progr_cb();
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
