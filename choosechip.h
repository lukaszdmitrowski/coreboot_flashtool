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

        /**
         * @brief Add chip name to chip list
         * @param chip_name [in] Name of a chip
         */
        void add_chip(QString chip_name);

        /**
         * @brief Set pointer to flash context pointer
         * @param ctx_ptr [in] pointer to flash context pointer
         */
        void set_flash_ctx_ptr(fl_flashctx_t **ctx_ptr);

private slots:
        /**
         * @brief Called when b_chip_ok button is clicked
         * Probes for a chip selected from a list of chips found and sets chip name in MainWindow
         */
        void on_b_chip_ok_clicked();

private:
        Ui::ChooseChip *ui;

        /**
         * @brief Pointer to flash context pointer
         */
        fl_flashctx_t **flash_context_ptr;
};

#endif // CHOOSECHIP_H
