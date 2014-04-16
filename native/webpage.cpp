#include "webpage.h"
#include <QDebug>

WebPage::WebPage(QObject *parent) : QWebPage(parent)
{
    connect(this, SIGNAL(windowCloseRequested()), this, SLOT(closeWindow()));
}

QWebPage *WebPage::createWindow(WebWindowType type)
{
    Q_UNUSED(type);

    WebPage *page = new WebPage(parent());
    page->setNetworkAccessManager(networkAccessManager());
    emit windowCreated(page);

    return page;
}

void WebPage::closeWindow()
{
    if (parent()->children().indexOf(this) != 0) {
        setParent(QApplication::instance());
        emit windowClosed(this);
        deleteLater();
    }
}

bool WebPage::supportsExtension(Extension extension) const
{
    switch (extension) {
        case ErrorPageExtension:
            return true;

        case ChooseMultipleFilesExtension:
            // TODO: Option.
            return false;
    }

    return QWebPage::supportsExtension(extension);
}

bool WebPage::extension(Extension extension, const ExtensionOption *option, ExtensionReturn *output)
{
    const ErrorPageExtensionOption *error;

    switch (extension) {
        case ErrorPageExtension:
            error = static_cast<const ErrorPageExtensionOption *>(option);

            switch (error->domain) {
                case QtNetwork:
                    emit statusBarMessage(QString("Network error: %1").arg(error->errorString));
                    break;

                case Http:
                    emit statusBarMessage(QString("HTTP error: %1").arg(error->errorString));
                    break;

                case WebKit:
                    emit statusBarMessage(QString("WebKit error: %1").arg(error->errorString));
                    break;
            }

            return false;

        case ChooseMultipleFilesExtension:
            break;
    }

    return QWebPage::extension(extension, option, output);
}
