#ifndef InfoDialog_H
#define InfoDialog_H

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
        void setText(QString text);


private:
        Ui::InfoDialog *ui;
};

#endif // InfoDialog_H
