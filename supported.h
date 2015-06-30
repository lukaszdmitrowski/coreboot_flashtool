#ifndef SUPPORTED_H
#define SUPPORTED_H

#include <QDialog>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

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
        void show_flash_chips();
        void show_boards();
        void show_chipsets();
        void on_cb_sel_hardware_currentIndexChanged(int index);

        void on_cb_sel_vendor_currentIndexChanged(int index);

private:
        QString test_state_to_qstring(fl_test_state test_state);
        Ui::Supported *ui;
        QStandardItemModel *model;
        QSortFilterProxyModel *sortFilterProxy;
        fl_flashchip_info *flashchip_info;
        fl_board_info *boards_list;
        fl_chipset_info *chipsets_list;
};

#endif // SUPPORTED_H
