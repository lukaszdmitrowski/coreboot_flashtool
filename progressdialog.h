#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QProgressDialog>

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QProgressDialog
{
        Q_OBJECT

public:
        explicit ProgressDialog(QWidget *parent = 0);
        ~ProgressDialog();
        void setText(QString text);

private:
        Ui::ProgressDialog *ui;
};

#endif // PROGRESSDIALOG_H
