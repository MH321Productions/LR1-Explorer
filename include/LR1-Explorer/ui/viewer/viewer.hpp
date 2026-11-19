#ifndef LR1_EXPLORER_VIEWER_HPP
#define LR1_EXPLORER_VIEWER_HPP

#include <filesystem>

#include <QWidget>

namespace LR1 {
    struct FileWrapper;
}

namespace LR1_Explorer {
    class LR1Viewer {
        public:
            virtual ~LR1Viewer() = default;

            virtual bool load(const LR1::FileWrapper& file) = 0;
            virtual bool save(const LR1::FileWrapper& file, const std::filesystem::path& outFile) = 0;
            virtual bool canSave(const LR1::FileWrapper& file) = 0;
            virtual void unload() = 0;
            virtual QWidget* getWidget() = 0;
            virtual QString getSaveFilter() = 0;
    };
}

#endif //LR1_EXPLORER_VIEWER_HPP
