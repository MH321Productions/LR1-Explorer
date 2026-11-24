#ifndef LR1_EXPLORER_NEWFSMODEL_HPP
#define LR1_EXPLORER_NEWFSMODEL_HPP

#include <memory>
#include <filesystem>
#include <vector>
#include <map>
#include <array>

#include <QAbstractItemModel>

#include <LR1/jam/extractor.hpp>
#include <LR1/decoder.hpp>

namespace LR1_Explorer {
    class JamFSEntry {
        public:
            static const int columnCount;

            explicit JamFSEntry(const std::filesystem::path& p, const JamFSEntry* parent = nullptr);

            JamFSEntry* appendChild(std::unique_ptr<JamFSEntry>&& child);

            [[nodiscard]] JamFSEntry* child(const int& row) const;
            [[nodiscard]] int childCount() const;
            [[nodiscard]] QVariant data(const int& column) const;
            [[nodiscard]] int row() const;
            [[nodiscard]] const JamFSEntry* parentItem() const;
            [[nodiscard]] std::filesystem::path getPath() const {return p;}

        private:
            static const std::array<QString, static_cast<uint8_t>(LR1::ResourceType::Unsupported) + 1> resourceTypeNames;

            std::vector<std::unique_ptr<JamFSEntry>> children;
            const std::filesystem::path p;
            const JamFSEntry* parent;
    };

    class JamFileSystemModel : public QAbstractItemModel {
        Q_OBJECT

        public:
            Q_DISABLE_COPY_MOVE(JamFileSystemModel)

            explicit JamFileSystemModel(const std::filesystem::path& p, QObject* parent = nullptr);
            ~JamFileSystemModel() override = default;

            [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
            [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;
            [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
            [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent) const override;
            [[nodiscard]] QModelIndex parent(const QModelIndex &index) const override;
            [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
            [[nodiscard]] int columnCount(const QModelIndex &parent) const override;
            [[nodiscard]] std::optional<LR1::FileWrapper> fileData(const QModelIndex& index) const;
            [[nodiscard]] std::optional<LR1::FileWrapper> fileData(const std::filesystem::path& path) const;

        private:
            static bool cmpDirEntry(const std::filesystem::directory_entry& e1, const std::filesystem::directory_entry& e2);

            LR1::JamExtractor jamExtractor;
            std::unique_ptr<JamFSEntry> root;
            std::map<std::filesystem::path, LR1::FileWrapper> entryData;

            [[nodiscard]] JamFSEntry* getEntry(const QModelIndex& index) const;
            void setupData(const std::filesystem::path& rootDir, JamFSEntry* parentNode);
            void loadJamDir(const LR1::JamDirectory& dir, const std::filesystem::path& basePath, JamFSEntry* parentNode);
    };
}

#endif //LR1_EXPLORER_NEWFSMODEL_HPP
