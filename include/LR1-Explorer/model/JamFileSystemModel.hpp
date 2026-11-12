#ifndef LR1_EXPLORER_JAMFILESYSTEMMODEL_HPP
#define LR1_EXPLORER_JAMFILESYSTEMMODEL_HPP

#include <filesystem>
#include <map>
#include <set>

#include <QAbstractItemModel>

#include <LR1/jam/extractor.hpp>

namespace LR1_Explorer {
    class JamFSEntry;

    class JamFileSystemModel: public QAbstractItemModel {
        public:
            [[nodiscard]] bool hasChildren(const QModelIndex &parent) const override;
            [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
            [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent) const override;
            [[nodiscard]] QModelIndex parent(const QModelIndex &child) const override;
            [[nodiscard]] int columnCount(const QModelIndex &parent) const override;
            [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

            [[nodiscard]] QModelIndex index(const std::filesystem::path& p) const;
            QModelIndex setRootPath(const std::filesystem::path& path);

        private:
            static const std::set<std::string> fileFilter;

            LR1::JamExtractor extractor;
            std::map<std::filesystem::path, JamFSEntry> entries;
            std::map<QString, LR1::JamDirectory> loadedJams;

            void loadDir(const std::filesystem::path& dir);
            void loadJamDir(const LR1::JamDirectory& jamDir, const std::filesystem::path& baseFile);
    };

    class JamFSEntry {
        public:
            explicit JamFSEntry(const std::filesystem::path& realPath) : jamDir(nullptr), jamFile(nullptr), p(realPath) {}
            JamFSEntry(const LR1::JamFile* jamFile, const std::filesystem::path& baseFile) : jamDir(nullptr), jamFile(jamFile), p(getVirtualPath(jamFile->parent, baseFile) / jamFile->filename) {}
            JamFSEntry(const LR1::JamDirectory* jamDir, const std::filesystem::path& baseFile) : jamDir(jamDir), jamFile(nullptr), p(getVirtualPath(jamDir, baseFile)) {}

            [[nodiscard]] bool isReal() const {return std::filesystem::exists(p);}
            [[nodiscard]] bool isJamEntry() const {return !std::filesystem::exists(p);}
            [[nodiscard]] bool isFile() const {return isReal() ? std::filesystem::is_regular_file(p) : jamFile != nullptr;}
            [[nodiscard]] bool isDirectory() const {return isReal() ? std::filesystem::is_directory(p) : jamDir != nullptr;}

        private:
            const LR1::JamDirectory* jamDir;
            const LR1::JamFile* jamFile;
            const std::filesystem::path p;

            static std::filesystem::path getVirtualPath(const LR1::JamDirectory* jamDir, const std::filesystem::path& baseFile);

        friend class JamFileSystemModel;
    };

}

#endif //LR1_EXPLORER_JAMFILESYSTEMMODEL_HPP
