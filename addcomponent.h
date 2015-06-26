#ifndef ADDCOMPONENT_H
#define ADDCOMPONENT_H

#include <QDialog>

namespace Ui {
class AddComponent;
}

class AddComponent : public QDialog
{
        Q_OBJECT

public:
        explicit AddComponent(QWidget *parent = 0);
        ~AddComponent();

private slots:
        void on_b_sel_component_clicked();
        void on_b_add_component_clicked();

private:
        void fill_cb_type();
        Ui::AddComponent *ui;
        QString component_path;
};

#endif // ADDCOMPONENT_H
