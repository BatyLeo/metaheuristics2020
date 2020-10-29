#include <QApplication>
#include <ctime>
#include <cstdlib>

#include <interface/mainwindow.h>

#include <iostream>

using namespace std;


int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    MainWindow main_window;
    main_window.showMaximized();

    return app.exec();

    return 0;

}
