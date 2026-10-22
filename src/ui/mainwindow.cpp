#include <LR1-Explorer/ui/mainwindow.hpp>

#include <QStatusBar>

namespace LR1_Explorer {
    MainWindow::MainWindow() {
        setWindowTitle(QStringLiteral("LR1-Explorer"));
        resize(1280, 720);


        statusBar()->showMessage(QStringLiteral("Open a folder to begin"));
    }
}
