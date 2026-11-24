#ifndef LR1_EXPLORER_TEXTVIEWER_HPP
#define LR1_EXPLORER_TEXTVIEWER_HPP

#include <QListWidget>

#include <LR1-Explorer/ui/viewer.hpp>

#include <LR1/formats/srf.hpp>

namespace LR1_Explorer {
    class TextViewer : public QListWidget, public virtual LR1Viewer {
        Q_OBJECT

        public:
            Q_DISABLE_COPY_MOVE(TextViewer)

            explicit TextViewer(QWidget* parent = nullptr);
            ~TextViewer() override = default;

            bool load(const LR1::FileWrapper &file) override;
            bool save(const LR1::FileWrapper &file, const std::filesystem::path &outFile) override;
            void unload() override;
            QWidget *getWidget() override {return this;}

        private:
            LR1::SrfDecoder decoder;
    };
}

#endif //LR1_EXPLORER_TEXTVIEWER_HPP
