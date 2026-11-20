#ifndef LR1_EXPLORER_VIEWER_HPP
#define LR1_EXPLORER_VIEWER_HPP

#include <filesystem>

#include <LR1/decoder.hpp>
#include <utility>

namespace LR1_Explorer {
    class LR1Viewer {
        public:
            virtual ~LR1Viewer() = default;

            virtual bool load(const LR1::FileWrapper& file) = 0;
            virtual bool save(const LR1::FileWrapper& file, const std::filesystem::path& outFile) = 0;
            virtual bool canSave(const LR1::FileWrapper& file) {return file.type == supportedType;}
            virtual void unload() = 0;
            virtual QWidget* getWidget() = 0;
            virtual QString getSaveFilter() {return saveFilter;}

        protected:
            explicit LR1Viewer(const LR1::ResourceType type, QString saveFilter) : supportedType(type), saveFilter(std::move(saveFilter)) {}

            const LR1::ResourceType supportedType;
            const QString saveFilter;
    };
}

#endif //LR1_EXPLORER_VIEWER_HPP
