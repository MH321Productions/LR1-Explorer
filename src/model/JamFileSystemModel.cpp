#include <qicon.h>
#include <LR1-Explorer/model/JamFileSystemModel.hpp>

using namespace std;
using namespace std::filesystem;

namespace LR1_Explorer {
    JamFileSystemModel::JamFileSystemModel(const path& p, QObject* parent) : QAbstractItemModel(parent), root(make_unique<JamFSEntry>(p)) {
        setupData(p, root.get());
    }

    JamFSEntry* JamFileSystemModel::getEntry(const QModelIndex& index) const {
        return index.isValid() ? static_cast<JamFSEntry*>(index.internalPointer()) : root.get();
    }

    QModelIndex JamFileSystemModel::index(const int row, const int column, const QModelIndex& parent) const {
        if (!hasIndex(row, column, parent)) return {};

        if (const auto* childItem = getEntry(parent)->child(row))
            return createIndex(row, column, childItem);

        return {};
    }

    QModelIndex JamFileSystemModel::parent(const QModelIndex &index) const {
        if (!index.isValid())
            return {};

        const auto* childItem = static_cast<JamFSEntry*>(index.internalPointer());
        const JamFSEntry* parentItem = childItem->parentItem();

        return parentItem != root.get() ? createIndex(parentItem->row(), 0, parentItem) : QModelIndex{};
    }

    int JamFileSystemModel::rowCount(const QModelIndex &parent) const {
        if (parent.column() > 0) return 0;

        return getEntry(parent)->childCount();
    }

    int JamFileSystemModel::columnCount(const QModelIndex&) const {
        return JamFSEntry::columnCount;
    }

    std::optional<LR1::FileWrapper> JamFileSystemModel::fileData(const QModelIndex& index) const {
        const auto* entry = getEntry(index);
        return entryData.contains(entry->getPath()) ? entryData.at(entry->getPath()) : std::optional<LR1::FileWrapper>(nullopt);
    }

    QVariant JamFileSystemModel::data(const QModelIndex& index, const int role) const {
        if (!index.isValid() || role != Qt::DisplayRole) return {};

        const auto* entry = static_cast<JamFSEntry*>(index.internalPointer());
        return entry->data(index.column());
    }

    Qt::ItemFlags JamFileSystemModel::flags(const QModelIndex &index) const {
        return index.isValid() ? QAbstractItemModel::flags(index) : Qt::ItemFlags(Qt::NoItemFlags);
    }

    QVariant JamFileSystemModel::headerData(const int section, const Qt::Orientation orientation, const int role) const {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};

        switch (section) {
            case 0: return "File/JAM Entry";
            case 1: return "Type";
            default: return {};
        }
    }

    void JamFileSystemModel::setupData(const path& rootDir, JamFSEntry* parentNode) {
        vector<directory_entry> children;
        for (const directory_entry& e: directory_iterator(rootDir)) children.push_back(e);
        ranges::sort(children, cmpDirEntry);

        for (const directory_entry& e: children) {
            auto child = make_unique<JamFSEntry>(e.path(), parentNode);
            const auto rawPtr = parentNode->appendChild(std::move(child));

            if (e.is_directory()) setupData(e.path(), rawPtr);
            else if (e.is_regular_file() && e.path().extension() == ".JAM") {
                const optional<LR1::JamDirectory> jam = jamExtractor.loadJam(e.path());
                if (jam.has_value()) loadJamDir(jam.value(), e.path(), rawPtr);
            } else {
                entryData.insert({e.path(), LR1::FileWrapper(e.path(), LR1::getResourceType(e.path()))});
            }
        }
    }

    void JamFileSystemModel::loadJamDir(const LR1::JamDirectory& dir, const path& basePath, JamFSEntry* parentNode) {
        for (const LR1::JamDirectory& childDir: dir.childDirectories) {
            path childPath = basePath / childDir.name;
            auto child = make_unique<JamFSEntry>(childPath, parentNode);
            loadJamDir(childDir, childPath, parentNode->appendChild(std::move(child)));
        }

        for (const LR1::JamFile& childFile: dir.childFiles) {
            const path childPath = basePath / childFile.filename;
            parentNode->appendChild(make_unique<JamFSEntry>(childPath, parentNode));
            entryData.insert({childPath, LR1::FileWrapper(childFile)});
        }
    }

    bool JamFileSystemModel::cmpDirEntry(const directory_entry &e1, const directory_entry &e2) {
        if (e1.is_directory() && !e2.is_directory()) return true;
        if (!e1.is_directory() && e2.is_directory()) return false;

        return e1.path().filename() < e2.path().filename();
    }
}
