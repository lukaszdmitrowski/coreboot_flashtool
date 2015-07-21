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

private:
        Ui::ChooseChip *ui;
};

#endif // CHOOSECHIP_H
