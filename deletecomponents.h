#ifndef DELETECOMPONENTS_H
#define DELETECOMPONENTS_H

#include <QDialog>

namespace Ui {
class DeleteComponents;
}

class DeleteComponents : public QDialog
{
        Q_OBJECT

public:
        explicit DeleteComponents(QWidget *parent = 0);
        ~DeleteComponents();

private slots:
        void on_b_remove_comp_clicked();

private:
        Ui::DeleteComponents *ui;
};

#endif // DELETECOMPONENTS_H
