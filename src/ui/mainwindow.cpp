#include <LR1-Explorer/ui/mainwindow.hpp>
#include <LR1-Explorer/model/TreeModel.hpp>

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

        //Test
        const QString testData = R"(
Getting Started				How to familiarize yourself with Qt Widgets Designer
    Launching Designer			Running the Qt Widgets Designer application
    The User Interface			How to interact with Qt Widgets Designer

Designing a Component			Creating a GUI for your application
    Creating a Dialog			How to create a dialog
    Composing the Dialog		Putting widgets into the dialog example
    Creating a Layout			Arranging widgets on a form
    Signal and Slot Connections		Making widget communicate with each other

Using a Component in Your Application	Generating code from forms
    The Direct Approach			Using a form without any adjustments
    The Single Inheritance Approach	Subclassing a form's base class
    The Multiple Inheritance Approach	Subclassing the form itself
    Automatic Connections		Connecting widgets using a naming scheme
        A Dialog Without Auto-Connect	How to connect widgets without a naming scheme
        A Dialog With Auto-Connect	Using automatic connections

Form Editing Mode			How to edit a form in Qt Widgets Designer
    Managing Forms			Loading and saving forms
    Editing a Form			Basic editing techniques
    The Property Editor			Changing widget properties
    The Object Inspector		Examining the hierarchy of objects on a form
    Layouts				Objects that arrange widgets on a form
        Applying and Breaking Layouts	Managing widgets in layouts
        Horizontal and Vertical Layouts	Standard row and column layouts
        The Grid Layout			Arranging widgets in a matrix
    Previewing Forms			Checking that the design works

Using Containers			How to group widgets together
    General Features			Common container features
    Frames				QFrame
    Group Boxes				QGroupBox
    Stacked Widgets			QStackedWidget
    Tab Widgets				QTabWidget
    Toolbox Widgets			QToolBox

Connection Editing Mode			Connecting widgets together with signals and slots
    Connecting Objects			Making connections in Qt Widgets Designer
    Editing Connections			Changing existing connections
)";
        char c = '\t';

        fileModel = new JamFileSystemModel();
        fileModel->setRootPath(rootDir);
        auto* model = new TreeModel(testData);
        treeView = new QTreeView(splitterMain);
        treeView->setModel(model);

        treeView->setModel(fileModel);
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
