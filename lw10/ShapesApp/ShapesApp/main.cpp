#include <QApplication>
#include <QLabel>
#include <QWidget>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Проверка Qt");
    window.resize(300, 150);

    QLabel label("Привет, Qt работает! 🎉", &window);
    label.move(70, 60);

    window.show();

    return app.exec();
}