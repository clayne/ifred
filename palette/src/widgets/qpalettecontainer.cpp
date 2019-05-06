#include <widgets/qpalettecontainer.h>
#include <widgets/qpalette_inner.h>

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
    : QMainWindow(nullptr), inner_stacked_(new QStackedWidget(this)), shadow_observer_(new ShadowObserver(this)) {
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
        setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TranslucentBackground); //enable MainWindow to be transparent

    setCentralWidget(inner_stacked_);
}

void QPaletteContainer::show(const QString & name, const QString & placeholder, const QVector<Action> & actions, const QString & closeKey, ActionHandler func) {
    while (inner_stacked_->count())
        inner_stacked_->removeWidget(inner_stacked_->widget(0));

    auto inner = new QPaletteInner(this, name, actions, closeKey);
    connect(inner, &QPaletteInner::itemClicked, std::move(func));
    connect(inner, &QPaletteInner::closed, [=]() {
        close();
        });
    inner_stacked_->addWidget(inner);
    centerWidgets(this);

    inner->setPlaceholderText(placeholder);

    shadow_observer_->activate();
    QMainWindow::show();
    activateWindow();
}
