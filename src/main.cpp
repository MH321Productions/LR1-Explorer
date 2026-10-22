#include <QApplication>
#include <QMainWindow>
#include <QStatusBar>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("LR1-Explorer"));

    QMainWindow window;
    window.setWindowTitle(QStringLiteral("LR1-Explorer"));
    window.resize(1280, 720);

    window.statusBar()->showMessage(QStringLiteral("Open a folder to begin"));
    window.show();
    return QApplication::exec();
}
