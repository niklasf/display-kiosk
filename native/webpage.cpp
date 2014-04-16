#include "webpage.h"

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
