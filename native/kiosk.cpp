#include "kiosk.h"

Kiosk::Kiosk(QWidget *parent) : QMainWindow(parent)
{
    QAction *resetAction = new QAction("Reset", this);
    connect(resetAction, SIGNAL(triggered()), this, SLOT(reset()));

    QToolBar *toolBar = addToolBar("Kiosk");
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    toolBar->addAction(resetAction);

    m_progressBar = new QProgressBar();
    statusBar()->addPermanentWidget(m_progressBar);

    m_view = new QWebView();
    connect(m_view, SIGNAL(titleChanged(const QString &)), this, SLOT(setWindowTitle(const QString &)));
    connect(m_view, SIGNAL(iconChanged()), this, SLOT(updateIcon()));
    connect(m_view, SIGNAL(loadStarted()), m_progressBar, SLOT(show()));
    connect(m_view, SIGNAL(loadFinished(bool)), m_progressBar, SLOT(hide()));
    connect(m_view, SIGNAL(loadProgress(int)), m_progressBar, SLOT(setValue(int)));
    connect(m_view, SIGNAL(statusBarMessage(const QString &)), statusBar(), SLOT(showMessage(const QString &)));
    setCentralWidget(m_view);

    m_pageHolder = new QObject(this);

    m_reloadTimer = new QTimer(this);
    m_reloadTimer->start(1000);
    // connect(m_reloadTimer, SIGNAL(timeout()), this, SLOT(reset()));

    reset();
}

Kiosk::~Kiosk()
{
}

void Kiosk::updateIcon()
{
    setWindowIcon(m_view->icon());
}

void Kiosk::reset()
{
    while (!m_pageHolder->children().isEmpty()) {
        delete m_pageHolder->children().last();
    }

    QWebPage *page = new QWebPage(m_pageHolder);
    m_view->setPage(page);
    page->mainFrame()->load(QUrl("http://trg-oha.de/cgi-bin/vplan/vertret-anzeige.htm"));
}
