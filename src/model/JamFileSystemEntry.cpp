#include <LR1-Explorer/model/JamFileSystemModel.hpp>

using namespace std;
using namespace std::filesystem;

namespace LR1_Explorer {
    const int JamFSEntry::columnCount = 2;
    const array<QString, static_cast<uint8_t>(LR1::ResourceType::Unsupported) + 1> JamFSEntry::resourceTypeNames = {
        QStringLiteral("Image"),
        QStringLiteral("Audio"),
        QStringLiteral("Text"),
        QStringLiteral("Model"),
        QStringLiteral("Mesh"),
        QStringLiteral("Material"),

        //Insert more type names here

        QStringLiteral("Unsupported"),
    };

    JamFSEntry::JamFSEntry(const path& p, const JamFSEntry* parent) : p(p), parent(parent) {}

    JamFSEntry* JamFSEntry::appendChild(std::unique_ptr<JamFSEntry>&& child) {
        children.push_back(std::move(child));
        return children.back().get();
    }

    JamFSEntry* JamFSEntry::child(const int& row) const {
        return row >= 0 && row < childCount() ? children.at(row).get() : nullptr;
    }

    int JamFSEntry::childCount() const {
        return static_cast<int>(children.size());
    }

    QVariant JamFSEntry::data(const int& column) const {
        switch (column) {
            case 0: return QString::fromStdString(p.filename().string());
            case 1:
                if (p.extension().string() == ".JAM") return "JAM File";
                if (!children.empty() || (exists(p) && is_directory(p))) return "Directory";
                return resourceTypeNames.at(static_cast<uint8_t>(LR1::getResourceType(p)));

            default: return {};
        }
    }

    int JamFSEntry::row() const {
        if (parent == nullptr) return 0;

        const auto it = std::ranges::find_if(
            std::as_const(parent->children),
            [this](const std::unique_ptr<JamFSEntry> &treeItem) {
                return treeItem.get() == this;
            }
        );

        if (it != parent->children.cend())
            return static_cast<int>(std::distance(parent->children.cbegin(), it));

        Q_ASSERT(false); // should not happen
    }

    const JamFSEntry* JamFSEntry::parentItem() const {
        return parent;
    }
}
