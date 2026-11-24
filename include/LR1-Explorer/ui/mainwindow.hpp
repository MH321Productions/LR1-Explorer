#ifndef LR1_EXPLORER_MAINWINDOW_HPP
#define LR1_EXPLORER_MAINWINDOW_HPP

#include <filesystem>
#include <array>

#include <QMainWindow>
#include <QString>

#include <LR1-Explorer/model/JamFileSystemModel.hpp>
#include <LR1-Explorer/ui/viewer.hpp>

class QTreeView;
class QSplitter;
class QStackedWidget;

namespace LR1_Explorer {
    class MainWindow : public QMainWindow {
        Q_OBJECT

        public:
            static MainWindow* getInstance();

            explicit MainWindow(const std::filesystem::path& rootDir);

            [[nodiscard]] std::optional<LR1::FileWrapper> getFile(const std::filesystem::path& filePath) const;

        private:
            QMenu* menuFile;
            QAction* miFileOpen;
            QAction* miFileQuit;

            QSplitter* splitterMain;
            QStackedWidget* stackedViewer;
            std::array<std::unique_ptr<LR1Viewer>, static_cast<uint8_t>(LR1::ResourceType::Unsupported) + 1> viewers;

            QTreeView* treeView;
            JamFileSystemModel* fileModel;

            QString rootDir;

            void onSave();
            void onSelectFile(const QModelIndex& index) const;

            [[nodiscard]] std::optional<LR1::FileWrapper> currentFile() const;
            [[nodiscard]] const std::unique_ptr<LR1Viewer>& currentViewer() const;
    };
}

#endif //LR1_EXPLORER_MAINWINDOW_HPP
