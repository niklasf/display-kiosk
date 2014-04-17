#include <QApplication>

#include "qcommandlineparser.h"
#include "kiosk.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("display-kiosk");
    app.setApplicationVersion("0.3.0");


    Kiosk kiosk;
    kiosk.setResetText("hello");
    kiosk.setUrl(QUrl("http://example.com"));
    kiosk.setPreventClose(true);
    kiosk.setAutoScroll(true);
    kiosk.setAutoReload(3);
    kiosk.show();

    return app.exec();
}
