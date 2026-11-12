#include <filesystem>

#include <QApplication>
#include <QFileDialog>

#include <LR1-Explorer/ui/mainwindow.hpp>

using namespace std::filesystem;

static path getRootDir();

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("LR1-Explorer"));

    const path rootDir = getRootDir();
    if (rootDir.empty()) return 1;

    LR1_Explorer::MainWindow window(rootDir);
    window.show();

    return QApplication::exec();
}

path getRootDir() {
    if (QApplication::arguments().count() > 1) return QDir(QApplication::arguments().at(1)).filesystemAbsolutePath();

    const QString selected = QFileDialog::getExistingDirectory(nullptr, QStringLiteral("Select the data directory"));
    if (selected.isEmpty()) return "";
    return QDir(selected).filesystemAbsolutePath();
}
