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

    QCommandLineOption hideCursorOption("hide-cursor",
        QCoreApplication::translate("main", "Hide the cursor."));
    parser.addOption(hideCursorOption);

    QCommandLineOption hideStatusOption("hide-status",
        QCoreApplication::translate("main", "Do not show a status bar."));
    parser.addOption(hideStatusOption);

    QCommandLineOption hideToolbarOption("hide-toolbar",
        QCoreApplication::translate("main", "Do not show a toolbar."));
    parser.addOption(hideToolbarOption);

    QCommandLineOption resetTextOption("reset-text",
        QCoreApplication::translate("main", "The label of the reset button in the toolbar."),
        "reset-text",
        QCoreApplication::translate("main", "Reset"));
    parser.addOption(resetTextOption);

    QCommandLineOption preventCloseOption("prevent-close",
        QCoreApplication::translate("main", "Ignore close events. A window manager might still provide ways to kill a process or minimize a window and must be locked down seperately."));
    parser.addOption(preventCloseOption);

    QCommandLineOption autoScrollOption("auto-scroll",
        QCoreApplication::translate("main", "Automatically scroll down."));
    parser.addOption(autoScrollOption);

    //QCommandLineOption autoScrollDeltaOption("auto-scroll-delta",
        //QCoreApplication::translate("main", "Number of pixels to scroll down per step. Ignored if --auto-scroll is not given. Defaults to 300."),
        //"pixels", "300);

    QCommandLineOption windowOption("window",
        QCoreApplication::translate("main", "Open in a window rather than fullscreen."));
    parser.addOption(windowOption);

    parser.process(app);

    if (parser.isSet(hideCursorOption)) {
        app.setOverrideCursor(Qt::BlankCursor);
    }

    Kiosk kiosk;
    kiosk.statusBar()->setVisible(!parser.isSet(hideStatusOption));
    kiosk.toolBar()->setVisible(!parser.isSet(hideToolbarOption));
    kiosk.setResetText(parser.value(resetTextOption));
    kiosk.setPreventClose(parser.isSet(preventCloseOption));
    kiosk.setAutoScroll(parser.isSet(autoScrollOption));
    //kiosk.setAutoScrollDelta(parser.value(autoScrollDeltaOption));
    kiosk.setUrl(QUrl("http://example.com"));
    kiosk.setAutoReload(3);

    if (parser.isSet(windowOption)) {
        kiosk.show();
    } else {
        kiosk.showFullScreen();
    }

    return app.exec();
}
