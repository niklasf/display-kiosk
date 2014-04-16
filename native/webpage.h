#include <QWebPage>

class WebPage : public QWebPage {
    Q_OBJECT

public:
    WebPage(QObject *parent = 0);
    QWebPage *createWindow(WebWindowType type);

signals:
    void windowCreated(WebPage *page);
};
