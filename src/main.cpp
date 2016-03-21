#include <QApplication>
#include "tf2rconmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Tf2RconMainWindow window;
    window.show();

    return a.exec();
}
