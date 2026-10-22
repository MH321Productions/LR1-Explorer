#include <QApplication>

#include <LR1-Explorer/ui/mainwindow.hpp>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("LR1-Explorer"));

    LR1_Explorer::MainWindow window;
    window.show();

    return QApplication::exec();
}
