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
#include <QMessageBox>

#include <LR1-Explorer/ui/mainwindow.hpp>
#include <LR1-Explorer/ui/viewer/imageviewer.hpp>
#include <LR1-Explorer/ui/viewer/unsupportedviewer.hpp>
#include <LR1-Explorer/ui/viewer/audioviewer.hpp>
#include <LR1-Explorer/ui/viewer/textviewer.hpp>

using namespace std;
using namespace std::filesystem;

namespace LR1_Explorer {
    MainWindow::MainWindow(const path& rootDir): fileModel(nullptr), rootDir(QDir(rootDir).absolutePath()) {
        setWindowTitle(QStringLiteral("LR1-Explorer"));
        resize(1280, 720);

        auto* central = new QWidget;
        auto* layout = new QVBoxLayout(central);

        splitterMain = new QSplitter;

        fileModel = new JamFileSystemModel(rootDir);
        treeView = new QTreeView(splitterMain);
        treeView->setModel(fileModel);
        connect(treeView, &QTreeView::clicked, this, &MainWindow::onSelectFile);

        stackedViewer = new QStackedWidget(splitterMain);
        viewers = {
            {
                make_unique<ImageViewer>(),
                make_unique<AudioViewer>(),
                make_unique<TextViewer>(),
                make_unique<UnsupportedViewer>(), //Model
                make_unique<UnsupportedViewer>(), //Mesh
                make_unique<UnsupportedViewer>(), //Material

                //...

                make_unique<UnsupportedViewer>(),
            }
        };
        for (const auto& viewer : viewers) {
            stackedViewer->addWidget(viewer->getWidget());
        }

        splitterMain->addWidget(treeView);
        splitterMain->addWidget(stackedViewer);

        layout->addWidget(splitterMain);
        setCentralWidget(central);

        menuFile = menuBar()->addMenu(QStringLiteral("File"));

        miFileOpen = new QAction(tr("&Save"), this);
        miFileOpen->setShortcut(QKeySequence::Save);
        connect(miFileOpen, &QAction::triggered, this, &MainWindow::onSave);

        miFileQuit = new QAction(tr("&Quit"), this);
        connect(miFileQuit, &QAction::triggered, this, &QApplication::quit);

        menuFile->addAction(miFileOpen);
        menuFile->addSeparator();
        menuFile->addAction(miFileQuit);

        statusBar()->showMessage(this->rootDir);
    }

    void MainWindow::onSave() {
        const std::optional<LR1::FileWrapper> file = currentFile();
        if (!file || !currentViewer()->canSave(file.value())) return;

        const QString outFile = QFileDialog::getSaveFileName(this, tr("Save File"), rootDir, currentViewer()->getSaveFilter());
        if (outFile.isEmpty()) return;

        if (currentViewer()->save(file.value(), QFileInfo(outFile).filesystemAbsoluteFilePath())) {
            QMessageBox::information(this, QStringLiteral("Save successful"),
                QStringLiteral("The file ") + QString::fromStdString(file.value().filename()) + QStringLiteral(" was saved as ") + outFile + QStringLiteral(".")
            );
        } else {
            QMessageBox::critical(this, QStringLiteral("Save failed"),
                QStringLiteral("The file ") + QString::fromStdString(file.value().filename()) + QStringLiteral(" couldn't be saved.")
            );
        }
    }

    void MainWindow::onSelectFile(const QModelIndex& index) const {
        std::optional<LR1::FileWrapper> data = fileModel->fileData(index);
        if (data) {
            currentViewer()->unload();
            const int newIndex = static_cast<int>(data->type);
            if (viewers.at(newIndex)->load(data.value())) stackedViewer->setCurrentIndex(newIndex);
        }
    }

    std::optional<LR1::FileWrapper> MainWindow::currentFile() const {return fileModel->fileData(treeView->currentIndex());}
    const std::unique_ptr<LR1Viewer>& MainWindow::currentViewer() const {return viewers.at(stackedViewer->currentIndex());}
}
