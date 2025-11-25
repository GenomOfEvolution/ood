#include <QApplication>
#include <qscreen.h>
#include "Views/MainView/MainView.h"

using namespace UI;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainView view;

    const QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    view.resize(1200, 700);
    view.move((screenGeometry.width() - view.width()) / 2,
        (screenGeometry.height() - view.height()) / 2);

    // 4. Показываем UI
    view.show();

    return app.exec();
}