#ifndef LR1_EXPLORER_UNSUPPORTEDVIEWER_HPP
#define LR1_EXPLORER_UNSUPPORTEDVIEWER_HPP

#include <QLabel>

#include <LR1-Explorer/ui/viewer.hpp>

namespace LR1_Explorer {
    class UnsupportedViewer : public QLabel, public virtual LR1Viewer {
        Q_OBJECT

        public:
            Q_DISABLE_COPY_MOVE(UnsupportedViewer)

            explicit UnsupportedViewer(QWidget* parent = nullptr) : LR1Viewer(LR1::ResourceType::Unsupported, {}), QLabel(QStringLiteral("The selected file is not (yet) supported"), parent) {
                setAlignment(Qt::AlignCenter);
            }
            ~UnsupportedViewer() override = default;

            bool load(const LR1::FileWrapper &file) override {return true;}
            bool save(const LR1::FileWrapper &file, const std::filesystem::path &outFile) override {return false;}
            bool canSave(const LR1::FileWrapper &file) override {return false;}
            void unload() override {}
            QWidget* getWidget() override {return this;}
    };
}

#endif //LR1_EXPLORER_UNSUPPORTEDVIEWER_HPP
