#include "webpage.h"

WebPage::WebPage(QObject *parent) : QWebPage(parent)
{
}

QWebPage *WebPage::createWindow(WebWindowType type)
{
    Q_UNUSED(type);

    WebPage *page = new WebPage(parent());
    page->setNetworkAccessManager(networkAccessManager());
    emit windowCreated(page);

    return page;
}
