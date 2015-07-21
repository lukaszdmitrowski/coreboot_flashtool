#ifndef CHOOSECHIP_H
#define CHOOSECHIP_H

#include <QDialog>

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

private slots:
        void on_b_chip_ok_clicked();

private:
        Ui::ChooseChip *ui;
};

#endif // CHOOSECHIP_H
