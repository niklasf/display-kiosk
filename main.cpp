#include <QApplication>
#include <QNetworkProxyFactory>
#include <QCommandLineParser>
#include <QWebSettings>

#include <stdio.h>

#include "kiosk.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("display-kiosk");
    app.setApplicationVersion("0.3.2");

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
        QCoreApplication::translate("main", "Ignore close events. A window manager might still provide ways to kill a process or minimize a window and must be locked down separately."));
    parser.addOption(preventCloseOption);

    QCommandLineOption autoScrollOption("auto-scroll",
        QCoreApplication::translate("main", "Automatically scroll down."));
    parser.addOption(autoScrollOption);

    QCommandLineOption autoScrollDeltaOption("auto-scroll-delta",
        QCoreApplication::translate("main", "Number of pixels to scroll down per step. Ignored if --auto-scroll is not given. Defaults to 300."),
        "pixels", "300");
    parser.addOption(autoScrollDeltaOption);

    QCommandLineOption autoScrollIntervalOption("auto-scroll-interval",
        QCoreApplication::translate("main", "Milliseconds to wait between auto scrolling steps. Ignored if --auto-scroll is not given. Defaults to 5000."),
        "millis", "5000");
    parser.addOption(autoScrollIntervalOption);

    QCommandLineOption autoReloadOption("auto-reload",
        QCoreApplication::translate("main", "Number of seconds ater which the page should be automatically reloaded. Will only reload when keyboard and mouse are idle. Will end all sessions. If auto scrolling is enabled, the reload will be deferred until scrolling reaches the end. The default is not to reload at all."),
        "seconds", "0");
    parser.addOption(autoReloadOption);

    QCommandLineOption windowOption("window",
        QCoreApplication::translate("main", "Open in a window rather than fullscreen."));
    parser.addOption(windowOption);

    QCommandLineOption mediaOption("media",
        QCoreApplication::translate("main", "Overwrite the default CSS @media type."),
        "css-media-type");
    parser.addOption(mediaOption);

    parser.process(app);

    bool ok;

    int autoScrollDelta = parser.value(autoScrollDeltaOption).toInt(&ok);
    if (autoScrollDelta < 0 || !ok) {
        fprintf(stderr, "%s\n", qPrintable(
            QCoreApplication::translate("main", "Expected a positive number of pixels for '--auto-scroll-delta'.")));
        return EXIT_FAILURE;
    }

    int autoScrollInterval = parser.value(autoScrollIntervalOption).toInt(&ok);
    if (autoScrollInterval <= 0 || !ok) {
        fprintf(stderr, "%s\n", qPrintable(
            QCoreApplication::translate("main", "Expected a positive number of milliseconds for '--auto-scroll-interval'.")));
        return EXIT_FAILURE;
    }

    int autoReload = parser.value(autoReloadOption).toInt(&ok);
    if (autoReload < 0 || !ok) {
        fprintf(stderr, "%s\n", qPrintable(
            QCoreApplication::translate("main", "Expected 0 or a positive number of seconds for '--auto-reload'.")));
        return EXIT_FAILURE;
    }

    if (parser.positionalArguments().count() != 1) {
        fprintf(stderr, "%s\n", qPrintable(
            QCoreApplication::translate("main", "Expected extacly one URL as an argument.")));
        return EXIT_FAILURE;
    }
    QUrl url(parser.positionalArguments().at(0));
    if (!url.isValid()) {
        fprintf(stderr, "%s\n", qPrintable(
            QCoreApplication::translate("main", "URL is invalid.")));
        return EXIT_FAILURE;
    }

    if (parser.isSet(hideCursorOption)) {
        app.setOverrideCursor(Qt::BlankCursor);
    }

    if (parser.isSet(mediaOption)) {
        QWebSettings::globalSettings()->setCSSMediaType(
            parser.value(mediaOption));
    }

    QNetworkProxyFactory::setUseSystemConfiguration(true);

    Kiosk kiosk;
    kiosk.statusBar()->setVisible(!parser.isSet(hideStatusOption));
    kiosk.toolBar()->setVisible(!parser.isSet(hideToolbarOption));
    kiosk.setResetText(parser.value(resetTextOption));
    kiosk.setPreventClose(parser.isSet(preventCloseOption));
    kiosk.setAutoScroll(parser.isSet(autoScrollOption));
    kiosk.setAutoScrollDelta(autoScrollDelta);
    kiosk.setAutoScrollInterval(autoScrollInterval);
    kiosk.setAutoReload(autoReload);
    kiosk.setUrl(url);

    if (parser.isSet(windowOption)) {
        kiosk.show();
    } else {
        kiosk.showFullScreen();
    }

    return app.exec();
}
