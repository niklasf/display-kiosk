#include "kiosk.h"

Kiosk::Kiosk(QWidget *parent) : QMainWindow(parent)
{
    // Create the status bar.
    m_progressBar = new QProgressBar();
    statusBar()->addPermanentWidget(m_progressBar);
    // TODO: Hide option

    // Create the central widget.
    m_view = new QWebView();
    connect(m_view, SIGNAL(titleChanged(const QString &)), this, SLOT(setWindowTitle(const QString &)));
    connect(m_view, SIGNAL(iconChanged()), this, SLOT(updateIcon()));
    connect(m_view, SIGNAL(loadStarted()), m_progressBar, SLOT(show()));
    connect(m_view, SIGNAL(loadFinished(bool)), m_progressBar, SLOT(hide()));
    connect(m_view, SIGNAL(loadProgress(int)), m_progressBar, SLOT(setValue(int)));
    connect(m_view, SIGNAL(statusBarMessage(const QString &)), statusBar(), SLOT(showMessage(const QString &)));
    setCentralWidget(m_view);

    // Create actions for the tool bar.
    m_resetAction = new QAction("Reset", this);
    m_resetAction->setIcon(style()->standardIcon(QStyle::SP_BrowserStop));
    connect(m_resetAction, SIGNAL(triggered()), this, SLOT(reset()));

    m_backAction = new QAction(this);
    m_backAction->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    connect(m_backAction, SIGNAL(triggered()), this, SLOT(back()));

    m_reloadAction = new QAction(this);
    m_reloadAction->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    connect(m_reloadAction, SIGNAL(triggered()), m_view, SLOT(reload()));

    // Create the tool bar.
    QToolBar *toolBar = addToolBar("Kiosk");
    // TODO: Hide option
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->addAction(m_resetAction);
    toolBar->addSeparator();
    toolBar->addAction(m_backAction);
    toolBar->addAction(m_reloadAction);

    // Pages are children of this page holder.
    m_pageHolder = new QObject(this);

    // Initial page load.
    reset();

    // Start reload countdown.
    m_reloadTimer = new QTimer(this);
    m_reloadTimer->start(1000);
    connect(m_reloadTimer, SIGNAL(timeout()), this, SLOT(reloadTick()));

    // Reset reload countdown if not idle.
    this->installEventFilter(this);
    statusBar()->installEventFilter(this);
    toolBar->installEventFilter(this);
    m_view->installEventFilter(this);
}

Kiosk::~Kiosk()
{
}

void Kiosk::updateIcon()
{
    setWindowIcon(m_view->icon());
}

void Kiosk::back()
{
    m_view->back();
}

void Kiosk::reset()
{
    while (!m_pageHolder->children().isEmpty()) {
        delete m_pageHolder->children().last();
    }

    QWebPage *page = new QWebPage(m_pageHolder);
    m_view->setPage(page);
    page->mainFrame()->load(QUrl("http://iserv-trg-oha.de/"));

    notIdle();
}

void Kiosk::notIdle()
{
    m_reloadCountdown = 35; // TODO: Option
    m_resetAction->setText("Reset");
}

void Kiosk::reloadTick()
{
    m_reloadCountdown--;

    if (m_reloadCountdown < 0) {
        // TODO: Do not if auto scrolling
        reset();
    } else if (m_reloadCountdown <= 30) {
        m_resetAction->setText(QString("Reset (%1)").arg(m_reloadCountdown));
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
    // TODO: Option
    if (false) {
        event->ignore();
    } else {
        QMainWindow::closeEvent(event);
    }
}
