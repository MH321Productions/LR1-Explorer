#ifndef LR1_EXPLORER_MAINWINDOW_HPP
#define LR1_EXPLORER_MAINWINDOW_HPP

#include <QMainWindow>

#include <LR1-Explorer/model/JamFileSystemModel.hpp>

class QTreeView;
class QSplitter;
class QStackedWidget;

namespace LR1_Explorer {
    class MainWindow : public QMainWindow {
        public:
            MainWindow();

        private:
            QMenu* menuFile;
            QAction* miFileOpen;
            QAction* miFileQuit;

            QSplitter* splitterMain;
            QStackedWidget* stackedViewer;

            QTreeView* treeView;
            JamFileSystemModel* fileModel;

            void onOpen();
    };
}

#endif //LR1_EXPLORER_MAINWINDOW_HPP
