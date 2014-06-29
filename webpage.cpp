#include "webpage.h"
#include <QDebug>

WebPage::WebPage(QObject *parent) : QWebPage(parent)
{
    connect(this, SIGNAL(windowCloseRequested()), this, SLOT(closeWindow()));
    connect(this, SIGNAL(printRequested(QWebFrame *)), this, SLOT(denyPrintRequest()));
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
            return false;
    }

    return QWebPage::supportsExtension(extension);
}

bool WebPage::extension(Extension extension, const ExtensionOption *option, ExtensionReturn *output)
{
    if (extension == ErrorPageExtension) {
        const ErrorPageExtensionOption *errorOption = static_cast<const ErrorPageExtensionOption *>(option);
        ErrorPageExtensionReturn *errorOutput = static_cast<ErrorPageExtensionReturn *>(output);

        errorOutput->baseUrl = errorOption->url;

        QString errorPage;
        errorPage += "<html>";
        errorPage += "  <body>";
        errorPage += "    <div style=\"margin: 30px; text-align: center; font-family: arial; font-size: 50pt;\">";
        errorPage += "      <p style=\"color: #aaa; margin: 5px;\">%1 #%2:</p>";
        errorPage += "      <p style=\"margin: 5px;\">%3</p>";
        errorPage += "      <p style=\"margin: 10px; font-size: 100pt;\">&#9785;</p>";
        errorPage += "    </div>";
        errorPage += "  </body>";
        errorPage += "</html>";

        switch (errorOption->domain) {
            case QtNetwork:
                errorPage = errorPage.arg("Network error");
                break;
            case Http:
                errorPage = errorPage.arg("HTTP error");
                break;
            case WebKit:
                errorPage = errorPage.arg("WebKit error");
                break;
            default:
                errorPage = errorPage.arg("Error");
                break;
        }

        errorOutput->content = errorPage.arg(errorOption->error).arg(errorOption->errorString).toUtf8();

        return true;
    }

    return false;
}

QString WebPage::chooseFile(QWebFrame *parentFrame, const QString &suggestedFile)
{
    Q_UNUSED(parentFrame);
    Q_UNUSED(suggestedFile);

    emit statusBarMessage(tr("You can not upload files from this display kiosk."));
    return QString();
}

void WebPage::denyPrintRequest()
{
    emit statusBarMessage(tr("You can not print from this display kiosk."));
}
