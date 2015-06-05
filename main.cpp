#include <QApplication>
#include <QTest>

#include "mainwindow.h"
#include "utests.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Test_1 test_1;
    QTest::qExec(&test_1, argc, argv);

    return a.exec();
}
