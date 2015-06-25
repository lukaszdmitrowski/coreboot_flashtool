#ifndef ADDPAYLOAD_H
#define ADDPAYLOAD_H

#include <QDialog>

namespace Ui {
class AddPayload;
}

class AddPayload : public QDialog
{
        Q_OBJECT

public:
        explicit AddPayload(QWidget *parent = 0);
        ~AddPayload();

private slots:
        void on_b_sel_payload_clicked();

        void on_b_add_payload_clicked();

private:
        Ui::AddPayload *ui;
        QString payload_path;
};

#endif // ADDPAYLOAD_H
