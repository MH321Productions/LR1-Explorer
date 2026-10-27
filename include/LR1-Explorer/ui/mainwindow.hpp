#ifndef LR1_EXPLORER_MAINWINDOW_HPP
#define LR1_EXPLORER_MAINWINDOW_HPP

#include <QMainWindow>

namespace LR1_Explorer {
    class MainWindow : public QMainWindow {
        public:
            MainWindow();

        private:
            QMenu* menuFile;
            QAction* miFileOpen;
            QAction* miFileQuit;

            void onOpen();
    };
}

#endif //LR1_EXPLORER_MAINWINDOW_HPP
