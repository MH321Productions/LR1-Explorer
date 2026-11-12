#include <filesystem>

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

    bool JamFileSystemModel::hasChildren(const QModelIndex &parent) const {
        return false; //TODO: Implement
    }

    int JamFileSystemModel::rowCount(const QModelIndex &parent) const {
        return 0; //TODO: Implement
    }

    QModelIndex JamFileSystemModel::index(int row, int column, const QModelIndex &parent) const {
        return {}; //TODO: Implement
    }

    QModelIndex JamFileSystemModel::index(const path &p) const {
    }

    QModelIndex JamFileSystemModel::parent(const QModelIndex &child) const {
        return {}; //TODO: Implement
    }

    int JamFileSystemModel::columnCount(const QModelIndex &parent) const {
        return parent.column() > 0 ? 0 : 2;
    }

    QVariant JamFileSystemModel::data(const QModelIndex &index, int role) const {
        return {}; //TODO: Implement
    }

    QModelIndex JamFileSystemModel::setRootPath(const path& path) {
        loadedJams.clear();
        entries.clear();

        entries.insert({path, JamFSEntry(path)});
        loadDir(path);

        return createIndex(0, 0, &(entries.at(path)));
    }

    void JamFileSystemModel::loadDir(const path& dir) {
        for (const directory_entry& e: directory_iterator(dir)) {
            if (e.is_directory()) loadDir(e.path());
            else if (!e.is_regular_file() || !fileFilter.contains(e.path().extension())) continue;

            entries.insert({e.path(), JamFSEntry(e.path())});
        }
    }

    void JamFileSystemModel::loadJamDir(const LR1::JamDirectory &jamDir, const path& baseFile) {

    }

    path JamFSEntry::getVirtualPath(const LR1::JamDirectory* jamDir, const std::filesystem::path& baseFile) {
        if (!jamDir) return baseFile;
        return getVirtualPath(jamDir->parent, baseFile) / jamDir->name;
    }
}
