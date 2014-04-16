#include <QApplication>
#include <QWebPage>

class WebPage : public QWebPage {
    Q_OBJECT

public:
    WebPage(QObject *parent = 0);
    QWebPage *createWindow(WebWindowType type);

public slots:
    void closeWindow();

signals:
    void windowCreated(WebPage *page);
    void windowClosed(WebPage *page);
};
