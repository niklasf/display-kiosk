#include <QApplication>

#include "qcommandlineparser.h"
#include "kiosk.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("display-kiosk");
    app.setApplicationVersion("0.3.0");

    QCommandLineParser parser;
    parser.setApplicationDescription(
        QCoreApplication::translate("main", "Displays a website in a stripped down kiosk"));

    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("url",
        QCoreApplication::translate("main", "The URL of the website to display."));

    QCommandLineOption preventCloseOption("prevent-close",
        QCoreApplication::translate("main", "Ignore close events. A window manager might still provide ways to kill a process or minimize a window and must be locked down seperately."));
    parser.addOption(preventCloseOption);

    parser.process(app);

    Kiosk kiosk;
    kiosk.setResetText("hello");
    kiosk.setUrl(QUrl("http://example.com"));
    kiosk.setPreventClose(parser.isSet(preventCloseOption));
    kiosk.setAutoScroll(true);
    kiosk.setAutoReload(3);
    kiosk.show();

    return app.exec();
}
