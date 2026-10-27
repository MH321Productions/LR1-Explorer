#include <LR1-Explorer/ui/mainwindow.hpp>

#include <QApplication>
#include <QStatusBar>
#include <QMenuBar>
#include <QFileDialog>

namespace LR1_Explorer {
    MainWindow::MainWindow() {
        setWindowTitle(QStringLiteral("LR1-Explorer"));
        resize(1280, 720);

        menuFile = menuBar()->addMenu(QStringLiteral("File"));

        miFileOpen = new QAction(tr("&Open"), this);
        miFileOpen->setShortcut(QKeySequence::Open);
        connect(miFileOpen, &QAction::triggered, this, &MainWindow::onOpen);

        miFileQuit = new QAction(tr("&Quit"), this);
        connect(miFileQuit, &QAction::triggered, this, &QApplication::quit);

        menuFile->addAction(miFileOpen);
        menuFile->addSeparator();
        menuFile->addAction(miFileQuit);

        statusBar()->showMessage(QStringLiteral("Open a folder to begin"));
    }

    void MainWindow::onOpen() {
        const QString dirName = QFileDialog::getExistingDirectory(this, QStringLiteral("Select the data directory"));
        if (dirName.isEmpty()) return;


    }
}
