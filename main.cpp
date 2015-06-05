#include <QApplication>
#include <QTest>

#include "mainwindow.h"
#include "utests.h"

MainWindow *w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w = new MainWindow;
    w->show();

#if UTEST
    Test_1 test_1;
    QTest::qExec(&test_1, argc, argv);
#endif

    return a.exec();
}
