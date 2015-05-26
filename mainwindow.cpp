#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //creates user interaface
    ui->setupUi(this);
    ui->pushButton->setText("Select");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSelect_ROM_triggered()
{

}

void MainWindow::on_pushButton_clicked()
{
    QString rom_path = QFileDialog::getOpenFileName(this, tr("Select ROM"), ".", "All files (*.*)");
    QString rom_name;
    rom_name = rom_path.section('/', -1);
    ui->romNameLabel->setText(rom_name);
}
