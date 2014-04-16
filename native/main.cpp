#include <QApplication>

#include "kiosk.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Kiosk kiosk;
    kiosk.show();

    return app.exec();
}
