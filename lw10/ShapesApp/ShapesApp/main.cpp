#include <QApplication>
#include <qscreen.h>
#include "Views/MainView/MainView.h"

using namespace UI;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/app-icon.ico"));

    MainView view;

    const QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    view.resize(1200, 700);
    view.move((screenGeometry.width() - view.width()) / 2,
        (screenGeometry.height() - view.height()) / 2);

    view.show();

    return app.exec();
}