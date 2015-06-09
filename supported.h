#ifndef SUPPORTED_H
#define SUPPORTED_H

#include <QDialog>

namespace Ui {
class Supported;
}

class Supported : public QDialog
{
    Q_OBJECT

public:
    explicit Supported(QWidget *parent = 0);
    ~Supported();

private:
    Ui::Supported *ui;
};

#endif // SUPPORTED_H
