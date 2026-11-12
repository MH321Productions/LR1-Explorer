#include <algorithm>

#include <LR1-Explorer/model/JamFileSystemModel.hpp>

using namespace std;
using namespace std::filesystem;

namespace LR1_Explorer {
    const set<string> JamFileSystemModel::fileFilter {
        ".ADB",
        ".avi",
        ".BDB",
        ".BMP",
        ".BVB",
        ".CCB",
        ".CDB",
        ".CEB",
        ".CMB",
        ".CPB",
        ".CRB",
        ".DDB",
        ".EMB",
        ".EVB",
        ".FDB",
        ".GCB",
        ".GDB",
        ".GHB",
        ".HZB",
        ".IDB",
        ".JAM",
        ".LEB",
        ".LRS",
        ".LSB",
        ".MAB",
        ".MDB",
        ".MIB",
        ".MSB",
        ".PCB",
        ".PCM",
        ".PWB",
        ".RAB",
        ".RCB",
        ".RRB",
        ".SBK",
        ".SDB",
        ".SKB",
        ".SPB",
        ".SRF",
        ".TDB",
        ".TGA",
        ".TGB",
        ".TIB",
        ".TMB",
        ".TRB",
        ".tun",
        ".TUN",
        ".WDB",
    };

    Qt::ItemFlags JamFileSystemModel::flags(const QModelIndex &index) const {
        if (index.isValid()) return QAbstractItemModel::flags(index);
        return Qt::NoItemFlags;
    }

    bool JamFileSystemModel::hasChildren(const QModelIndex& parent) const {
        const auto* entry = static_cast<const JamFSEntry*>(parent.constInternalPointer());
        if (entry->isReal()) {
            if (entry->isFile()) return false;
            directory_iterator it(entry->p);
            return begin(it) != end(it);
        }
        return entry->isFile() ? false : !entry->jamDir->childDirectories.empty() || !entry->jamDir->childFiles.empty();
    }

    int JamFileSystemModel::rowCount(const QModelIndex& parent) const {
        if (parent.column() > 0) return 0;

        const auto* entry = static_cast<const JamFSEntry*>(parent.constInternalPointer());
        if (entry->isReal()) {
            if (entry->isFile()) return 0;

            int num = 0;
            for (const directory_entry& e: directory_iterator(entry->p)) num++; //TODO: Maybe find a more efficient way to count children
            return num;
        }
        return entry->isFile() ? 0 : static_cast<int>(entry->jamDir->childDirectories.size() + entry->jamDir->childFiles.size());
    }

    QModelIndex JamFileSystemModel::index(const int row, const int column, const QModelIndex& parent) const {
        if (column < 0 || column > 1 || row < 0) return {};

        const auto* parentEntry = static_cast<const JamFSEntry*>(parent.constInternalPointer());
        if (!indices.contains(parentEntry->p)) return {};

        const vector<path>& parentIndices = indices.at(parentEntry->p);
        if (row >= parentIndices.size()) return {};

        return createIndex(row, column, &entries.at(parentIndices.at(row)));
    }

    QModelIndex JamFileSystemModel::index(const path& p) const {
        if (!indices.contains(p.parent_path())) return {};

        const int i = indexOf(indices.at(p.parent_path()), p);
        if (i == -1) return {};

        return createIndex(i, 0, &entries.at(p));
    }

    QModelIndex JamFileSystemModel::parent(const QModelIndex& child) const {
        const auto* entry = static_cast<const JamFSEntry*>(child.constInternalPointer());
        return index(entry->p.parent_path());
    }

    int JamFileSystemModel::columnCount(const QModelIndex& parent) const {
        return parent.column() > 0 ? 0 : 2;
    }

    QVariant JamFileSystemModel::data(const QModelIndex& index, int role) const {
        return "Test"; //TODO: Implement
    }

    QModelIndex JamFileSystemModel::setRootPath(const path& path) {
        loadedJams.clear();
        entries.clear();
        indices.clear();

        indices.insert({path.parent_path(), {path}}); //Insert root path with its parent
        entries.insert({path, JamFSEntry(path)});
        loadDir(path);

        return createIndex(0, 0, &(entries.at(path)));
    }

    QVariant JamFileSystemModel::headerData(const int section, Qt::Orientation, int) const {
        if (section == 0) return "Name";
        if (section == 1) return "Type";
        return "";
    }

    void JamFileSystemModel::loadDir(const path& dir) {
        vector<directory_entry> children;
        for (const directory_entry& e: directory_iterator(dir)) children.push_back(e);
        ranges::sort(children, [](const directory_entry& e1, const directory_entry& e2) {return e1.path().filename() < e2.path().filename();});

        vector<path> childPaths(children.size());
        for (const directory_entry& e: children) childPaths.push_back(e.path());
        indices.insert({dir, childPaths});


        for (const directory_entry& e: children) {
            if (e.is_directory()) loadDir(e.path());
            else if (!e.is_regular_file() || !fileFilter.contains(e.path().extension().string())) continue;
            else if (e.path().extension() == ".JAM") {
                const optional<LR1::JamDirectory> jam = extractor.loadJam(e.path());
                if (!jam) continue;

                loadedJams.insert({e.path(), jam.value()});
                loadJamDir(jam.value(), e.path());
            }

            entries.insert({e.path(), JamFSEntry(e.path())});
        }
    }

    void JamFileSystemModel::loadJamDir(const LR1::JamDirectory& jamDir, const path& baseFile) {
        JamFSEntry entry(&jamDir, baseFile);
        entries.insert({entry.p, entry});

        for (const LR1::JamDirectory& d: jamDir.childDirectories) loadJamDir(d, baseFile);
        for (const LR1::JamFile& f: jamDir.childFiles) {
            JamFSEntry fileEntry(&f, baseFile);
            entries.insert({fileEntry.p, fileEntry});
        }
    }

    int JamFileSystemModel::indexOf(const std::vector<path> &list, const path &p) {
        int i = 0;
        for (const path& e: list) {
            if (e == p) return i;
            i++;
        }

        return -1;
    }

    path JamFSEntry::getVirtualPath(const LR1::JamDirectory* jamDir, const path& baseFile) {
        if (!jamDir) return baseFile;
        return getVirtualPath(jamDir->parent, baseFile) / jamDir->name;
    }
}
