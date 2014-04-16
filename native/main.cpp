#include <QApplication>

#include "kiosk.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Kiosk kiosk;
    kiosk.setResetText("hello");
    kiosk.setUrl(QUrl("http://example.com"));
    kiosk.show();

    return app.exec();
}
