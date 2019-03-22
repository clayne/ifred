#include <widgets/qpalettecontainer.h>

static void centerWidgets(QWidget* widget, QWidget* host = nullptr) {
    if (!host)
        host = widget->parentWidget();

    if (host) {
        auto hostRect = host->geometry();
        widget->move(hostRect.center() - widget->rect().center());
    }
    else {
        QRect screenGeometry = QApplication::desktop()->screenGeometry();
        int x = (screenGeometry.width() - widget->width()) / 2;
        int y = (screenGeometry.height() - widget->height()) / 2;
        widget->move(x, y);
    }
}

QPaletteContainer::QPaletteContainer()
        : inner_stacked_(new QStackedWidget(this)), shadow_observer_(new ShadowObserver(this)) {
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground); //enable MainWindow to be transparent

    setCentralWidget(inner_stacked_);
    connect(this, &QPaletteContainer::show, this, &QPaletteContainer::onShow, Qt::QueuedConnection);
}

void QPaletteContainer::onShow(const QString &name, const QVector<Action> &actions_, ActionHandler func) {
    while (inner_stacked_->count())
        inner_stacked_->removeWidget(inner_stacked_->widget(0));

    auto delegate = new QPaletteInner(this, name, actions_);
    connect(delegate, &QPaletteInner::enter_callback, func);
    inner_stacked_->addWidget(delegate);
    centerWidgets(this);

    QMainWindow::show();
    activate();
}

void QPaletteContainer::activate() {
    activateWindow();

    if (!inner_stacked_->count())
        return;

    auto * inner = static_cast<QPaletteInner*>(inner_stacked_->widget(0));

    inner->searchbox().selectAll();
    inner->searchbox().setFocus();
}
