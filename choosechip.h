#ifndef CHOOSECHIP_H
#define CHOOSECHIP_H

#include <QDialog>

extern "C" {
#include "libflashrom.h"
}

namespace Ui {
class ChooseChip;
}

class ChooseChip : public QDialog
{
        Q_OBJECT

public:
        explicit ChooseChip(QWidget *parent = 0);
        ~ChooseChip();
        void add_chip(QString chip_name);
        void set_flash_ctx_ptr(fl_flashctx_t **ctx_ptr);

private slots:
        void on_b_chip_ok_clicked();

private:
        Ui::ChooseChip *ui;
        fl_flashctx_t **flash_context_ptr;
};

#endif // CHOOSECHIP_H
