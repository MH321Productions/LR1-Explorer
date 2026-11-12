#include <LR1-Explorer/ui/mainwindow.hpp>

#include <iostream>

#include <QApplication>
#include <QStatusBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QTreeView>
#include <QVBoxLayout>
#include <QSplitter>
#include <QStackedWidget>
#include <QFileSystemModel>

using namespace std::filesystem;

namespace LR1_Explorer {
    MainWindow::MainWindow(const path& rootDir): fileModel(nullptr) {
        setWindowTitle(QStringLiteral("LR1-Explorer"));
        resize(1280, 720);

        auto* central = new QWidget;
        auto* layout = new QVBoxLayout(central);

        splitterMain = new QSplitter;

        //fileModel = new JamFileSystemModel();
        auto* model = new QFileSystemModel;
        model->setRootPath("C:\\");
        treeView = new QTreeView(splitterMain);
        //treeView->setModel(fileModel);
        treeView->setModel(model);
        connect(treeView, &QTreeView::clicked, this, &MainWindow::onSelectFile);

        stackedViewer = new QStackedWidget(splitterMain);

        splitterMain->addWidget(treeView);
        splitterMain->addWidget(stackedViewer);

        layout->addWidget(splitterMain);
        setCentralWidget(central);

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

        statusBar()->showMessage(dirName);

        const path rootDir = QDir(dirName).filesystemAbsolutePath();
        /*fileModel->setRootPath(rootDir);
        treeView->setRootIndex(fileModel->index(rootDir));*/
    }

    void MainWindow::onSelectFile(const QModelIndex& index) {
        std::cout << index.column() << ", " << index.row() << std::endl;
    }
}
