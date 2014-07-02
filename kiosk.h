#ifndef KIOSK_H
#define KIOSK_H

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

#include "webpage.h"

class Kiosk : public QMainWindow {
    Q_OBJECT
    Q_PROPERTY(QString resetText READ resetText WRITE setResetText)
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(bool preventClose READ preventClose WRITE setPreventClose)
    Q_PROPERTY(bool autoScroll READ autoScroll WRITE setAutoScroll)
    Q_PROPERTY(int autoScrollDelta READ autoScrollDelta WRITE setAutoScrollDelta)
    Q_PROPERTY(int autoScrollInterval READ autoScrollInterval WRITE setAutoScrollInterval)
    Q_PROPERTY(int autoReload READ autoReload WRITE setAutoReload)

public:
    Kiosk(QWidget *parent = 0);
    // ~Kiosk();

    bool eventFilter(QObject *watched, QEvent *event);
    void closeEvent(QCloseEvent *event);

    QToolBar *toolBar() const;

    QString resetText() const;
    void setResetText(const QString &resetText);

    QUrl url() const;
    void setUrl(const QUrl &url);

    bool preventClose() const;
    void setPreventClose(bool preventClose);

    bool autoScroll() const;
    void setAutoScroll(bool autoScroll);

    int autoScrollDelta() const;
    void setAutoScrollDelta(int delta);

    int autoScrollInterval() const;
    void setAutoScrollInterval(int msec);

    int autoReload() const;
    void setAutoReload(int sec);

public slots:
    void updateIcon();
    void back();
    void reset();
    void notIdle();
    void showLastPage();

private:
    void resetFrameScrollBars(QWebFrame *frame);
    bool scrollFrame(QWebFrame *frame, int delta);

private slots:
    void reloadTick();
    void scrollTick();
    void addPage(WebPage *page);

private:
    QAction *m_resetAction;
    QAction *m_backAction;
    QAction *m_reloadAction;

    QProgressBar *m_progressBar;
    QToolBar *m_toolBar;
    QWebView *m_view;

    QTimer *m_reloadTimer;
    QTimer *m_scrollTimer;
    int m_reloadCountdown;

    QObject *m_pageHolder;

    QString m_resetText;
    QUrl m_url;
    bool m_preventClose;
    int m_autoReload;
    int m_autoScrollDelta;
};

#endif
