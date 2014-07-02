#include "kiosk.h"

Kiosk::Kiosk(QWidget *parent) : QMainWindow(parent), m_resetText(tr("Reset")), m_url()
{
    // Initialize members.
    m_preventClose = false;
    m_autoScrollDelta = 300;
    m_autoReload = 0;

    // Create the status bar.
    m_progressBar = new QProgressBar();
    statusBar()->addPermanentWidget(m_progressBar);

    // Create the central widget.
    m_view = new QWebView();
    m_view->setContextMenuPolicy(Qt::NoContextMenu);
    connect(m_view, SIGNAL(titleChanged(const QString &)), this, SLOT(setWindowTitle(const QString &)));
    connect(m_view, SIGNAL(iconChanged()), this, SLOT(updateIcon()));
    connect(m_view, SIGNAL(loadStarted()), m_progressBar, SLOT(show()));
    connect(m_view, SIGNAL(loadFinished(bool)), m_progressBar, SLOT(hide()));
    connect(m_view, SIGNAL(loadProgress(int)), m_progressBar, SLOT(setValue(int)));
    connect(m_view, SIGNAL(statusBarMessage(const QString &)), statusBar(), SLOT(showMessage(const QString &)));
    setCentralWidget(m_view);

    // Create actions for the tool bar.
    m_resetAction = new QAction(m_resetText, this);
    m_resetAction->setIcon(style()->standardIcon(QStyle::SP_BrowserStop));
    connect(m_resetAction, SIGNAL(triggered()), this, SLOT(reset()));

    m_backAction = new QAction(this);
    m_backAction->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    connect(m_backAction, SIGNAL(triggered()), this, SLOT(back()));

    m_reloadAction = new QAction(this);
    m_reloadAction->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    connect(m_reloadAction, SIGNAL(triggered()), m_view, SLOT(reload()));

    // Create the tool bar.
    m_toolBar = addToolBar("Kiosk");
    m_toolBar->setFloatable(false);
    m_toolBar->setMovable(false);
    m_toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_toolBar->addAction(m_resetAction);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_backAction);
    m_toolBar->addAction(m_reloadAction);

    // Pages are children of this page holder.
    m_pageHolder = new QObject(this);

    // Initial page load.
    reset();

    // Setup auto scroll timer.
    m_scrollTimer = new QTimer(this);
    m_scrollTimer->setInterval(5000);
    connect(m_scrollTimer, SIGNAL(timeout()), this, SLOT(scrollTick()));

    // Start reload countdown.
    m_reloadTimer = new QTimer(this);
    m_reloadTimer->start(1000);
    connect(m_reloadTimer, SIGNAL(timeout()), this, SLOT(reloadTick()));

    // Reset reload countdown if not idle.
    this->installEventFilter(this);
    m_toolBar->installEventFilter(this);
    m_view->installEventFilter(this);
}

/* Kiosk::~Kiosk()
{
} */

void Kiosk::updateIcon()
{
    setWindowIcon(m_view->icon());
}

void Kiosk::back()
{
    if (m_view->pageAction(QWebPage::Back)->isEnabled()) {
        m_view->back();
    } else if (m_pageHolder->children().count() > 1) {
        WebPage *page = qobject_cast<WebPage *>(m_view->page());
        page->closeWindow();
    } else {
        reset();
    }
}

void Kiosk::reset()
{
    while (!m_pageHolder->children().isEmpty()) {
        delete m_pageHolder->children().last();
    }

    WebPage *page = new WebPage(m_pageHolder);
    addPage(page);
    if (m_url.isValid()) {
        page->mainFrame()->load(m_url);
    }

    notIdle();
}

void Kiosk::notIdle()
{
    m_reloadCountdown = m_autoReload;
    m_resetAction->setText(m_resetText);
}

void Kiosk::reloadTick()
{
    if (!autoReload()) {
        return;
    }

    m_reloadCountdown--;

    if (m_reloadCountdown < 0) {
        if (!autoScroll()) {
            reset();
        }
    } else if (m_reloadCountdown <= 30) {
        m_resetAction->setText(QString("%1 (%2)").arg(m_resetText).arg(m_reloadCountdown));
    }
}

void Kiosk::scrollTick()
{
    if (!scrollFrame(m_view->page()->mainFrame(), m_autoScrollDelta)) {
        if (autoReload() && m_reloadCountdown < 0) {
            reset();
        } else {
            resetFrameScrollBars(m_view->page()->mainFrame());
        }
    }
}

void Kiosk::resetFrameScrollBars(QWebFrame *frame)
{
    frame->setScrollBarValue(Qt::Vertical, 0);

    for (int i = 0; i < frame->childFrames().count(); i++) {
        resetFrameScrollBars(frame->childFrames().at(i));
    }
}

bool Kiosk::scrollFrame(QWebFrame *frame, int delta)
{
    int maximum = frame->scrollBarMaximum(Qt::Vertical);

    if (maximum == 0) {
        for (int i = 0; i < frame->childFrames().count(); i++) {
            if (scrollFrame(frame->childFrames().at(i), delta)) {
                return true;
            }
        }
        return false;
    } else {
        int value = frame->scrollBarValue(Qt::Vertical);

        if (value >= maximum) {
            return false;
        } else {
            frame->setScrollBarValue(Qt::Vertical, value + delta);
            return true;
        }
    }
}

bool Kiosk::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::MouseMove) {
        notIdle();
    }

    return QMainWindow::eventFilter(watched, event);
}

void Kiosk::closeEvent(QCloseEvent *event)
{
    if (m_preventClose) {
        reset();
        event->ignore();
    } else {
        QMainWindow::closeEvent(event);
    }
}

void Kiosk::addPage(WebPage *page)
{
    connect(page, SIGNAL(windowCreated(WebPage *)), this, SLOT(addPage(WebPage *)));
    connect(page, SIGNAL(windowClosed(WebPage *)), this, SLOT(showLastPage()));
    m_view->setPage(page);
}

void Kiosk::showLastPage()
{
    WebPage *lastPage = qobject_cast<WebPage *>(m_pageHolder->children().last());
    m_view->setPage(lastPage);
}

QToolBar *Kiosk::toolBar() const
{
    return m_toolBar;
}

QString Kiosk::resetText() const
{
    return m_resetText;
}

void Kiosk::setResetText(const QString &resetText)
{
    m_resetAction->setText(resetText);
    m_resetText = resetText;
}

QUrl Kiosk::url() const
{
    return m_url;
}

void Kiosk::setUrl(const QUrl &url)
{
    if (m_url != url) {
        m_url = url;
        reset();
    }
}

bool Kiosk::preventClose() const
{
    return m_preventClose;
}

void Kiosk::setPreventClose(bool preventClose)
{
    m_preventClose = preventClose;
}

bool Kiosk::autoScroll() const
{
   return m_scrollTimer->isActive();
}

void Kiosk::setAutoScroll(bool autoScroll)
{
    if (autoScroll) {
        m_scrollTimer->start();
    } else {
        m_scrollTimer->stop();
    }
}

int Kiosk::autoScrollInterval() const
{
    return m_scrollTimer->interval();
}

void Kiosk::setAutoScrollInterval(int msec)
{
    m_scrollTimer->setInterval(msec);
}

int Kiosk::autoScrollDelta() const
{
    return m_autoScrollDelta;
}

void Kiosk::setAutoScrollDelta(int delta)
{
    m_autoScrollDelta = delta;
}

int Kiosk::autoReload() const
{
    return m_autoReload;
}

void Kiosk::setAutoReload(int sec)
{
    if (sec != m_autoReload) {
        m_autoReload = sec;
        m_reloadCountdown = m_autoReload;
    }

    if (m_autoReload) {
        m_reloadTimer->start();
    } else {
        m_reloadTimer->stop();
    }
}
