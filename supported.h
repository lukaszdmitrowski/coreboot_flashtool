#ifndef SUPPORTED_H
#define SUPPORTED_H

#include <QDialog>
#include <QStandardItemModel>

extern "C" {
#include "libflashrom.h"
}

namespace Ui {
class Supported;
}

class Supported : public QDialog
{
    Q_OBJECT

public:
    explicit Supported(QWidget *parent = 0);
    ~Supported();

private slots:
    void on_b_show_chips_clicked();

    void on_b_show_boards_clicked();

    void on_b_show_chipsets_clicked();

private:
    Ui::Supported *ui;
    QStandardItemModel *model;
    QString test_state_to_qstring(fl_test_state test_state);
};

#endif // SUPPORTED_H
