#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QApplication>
#include <QWebPage>

class WebPage : public QWebPage {
    Q_OBJECT

public:
    WebPage(QObject *parent = 0);
    QWebPage *createWindow(WebWindowType type);
    bool supportsExtension(Extension extension) const;
    bool extension(Extension extension, const ExtensionOption *option = 0, ExtensionReturn *output = 0);

public slots:
    void closeWindow();

protected slots:
    void denyPrintRequest();

protected:
    QString chooseFile(QWebFrame *parentFrame, const QString &suggestedFile);

signals:
    void windowCreated(WebPage *page);
    void windowClosed(WebPage *page);
};

#endif
