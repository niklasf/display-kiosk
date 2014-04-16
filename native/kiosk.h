#include <QMainWindow>
#include <QWebView>
#include <QWebFrame>
#include <QTimer>
#include <QProgressBar>
#include <QStatusBar>

class Kiosk : public QMainWindow {
    Q_OBJECT

public:
    Kiosk(QWidget *parent = 0);
    ~Kiosk();

public slots:
    void updateIcon();
    void reset();

private:
    QProgressBar *m_progressBar;
    QWebView *m_view;
    QTimer *m_reloadTimer;
    QObject *m_pageHolder;
};
