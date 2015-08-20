#ifndef InfoDialog_H
#define InfoDialog_H

#include "constants.h"
#include <QDialog>

namespace Ui {
class InfoDialog;
}

class InfoDialog : public QDialog
{
        Q_OBJECT

public:
        explicit InfoDialog(QWidget *parent = 0);
        ~InfoDialog();

        /**
         * @brief Show dialog with info(error) message
         * @param ret_val [in] error type
         */
        void show_message(RET_VAL ret_val);

        /**
         * @brief Show dialog with provided message
         * @param message [in] - message to be shown in dialog
         */
        void show_message(QString message);

private slots:
        /**
         * @brief Called when b_ok button is clicked
         * Closes dialog
         */
        void on_b_ok_clicked();

private:
        Ui::InfoDialog *ui;
};

#endif // InfoDialog_H
