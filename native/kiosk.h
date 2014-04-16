#include <QMainWindow>
#include <QWebView>
#include <QWebFrame>
#include <QTimer>
#include <QProgressBar>
#include <QStatusBar>
#include <QAction>
#include <QToolBar>
#include <QStyle>
#include <QEvent>
#include <QCloseEvent>

class Kiosk : public QMainWindow {
    Q_OBJECT

public:
    Kiosk(QWidget *parent = 0);
    ~Kiosk();

    bool eventFilter(QObject *watched, QEvent *event);
    void closeEvent(QCloseEvent *event);

public slots:
    void updateIcon();
    void back();
    void reset();
    void notIdle();

private slots:
    void reloadTick();

private:
    QAction *m_resetAction;
    QAction *m_backAction;
    QAction *m_reloadAction;

    QProgressBar *m_progressBar;
    QWebView *m_view;

    QTimer *m_reloadTimer;
    int m_reloadCountdown;

    QObject *m_pageHolder;
};