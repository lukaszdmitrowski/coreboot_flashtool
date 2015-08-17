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
        void show_message(RET_VAL ret_val);

private slots:
        void on_b_ok_clicked();

private:
        Ui::InfoDialog *ui;
};

#endif // InfoDialog_H
