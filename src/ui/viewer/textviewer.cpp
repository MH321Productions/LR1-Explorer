#include <LR1-Explorer/ui/viewer/textviewer.hpp>

using namespace std;
using namespace std::filesystem;

namespace LR1_Explorer {
    TextViewer::TextViewer(QWidget* parent) : LR1Viewer(LR1::ResourceType::Text, QStringLiteral("JSON files (*.json)")), QListWidget(parent) {}

    bool TextViewer::load(const LR1::FileWrapper& file) {
        if (file.type != LR1::ResourceType::Text) return false;

        const optional<vector<string>> entries = decoder.decodeWrapper(file);
        if (!entries) return false;

        clear();
        for (const string& e: entries.value()) {
            addItem(QString::fromStdString(e));
        }

        return true;
    }

    bool TextViewer::save(const LR1::FileWrapper& file, const path& outFile) {
        return decoder.convertWrapper(file, outFile);
    }

    void TextViewer::unload() {
        clear();
    }


}
