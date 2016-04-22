#include <QApplication>
#include "tf2rconmainwindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName(QStringLiteral("TF2Rcon"));
    QCoreApplication::setOrganizationName(QStringLiteral("TF2RconTeam"));
    QCoreApplication::setApplicationVersion(QStringLiteral("1.0"));
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    
    QApplication a(argc, argv);

    Tf2RconMainWindow window;
    window.show();

    return a.exec();
}
