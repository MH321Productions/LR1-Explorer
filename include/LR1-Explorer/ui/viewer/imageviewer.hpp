#ifndef LR1_EXPLORER_IMAGEVIEWER_HPP
#define LR1_EXPLORER_IMAGEVIEWER_HPP

#include <QLabel>

#include <LR1/formats/bmp.hpp>

#include <LR1-Explorer/ui/viewer/viewer.hpp>

namespace LR1_Explorer {
    class ImageViewer : public QLabel, public virtual LR1Viewer {
        Q_OBJECT

        public:
            Q_DISABLE_COPY_MOVE(ImageViewer)

            explicit ImageViewer(QWidget* parent = nullptr);
            ~ImageViewer() override = default;

            bool load(const LR1::FileWrapper &file) override;
            bool save(const LR1::FileWrapper &file, const std::filesystem::path &outFile) override;
            void unload() override;
            QWidget *getWidget() override;

        private:
            LR1::BmpDecoder decoder;
    };
}

#endif //LR1_EXPLORER_IMAGEVIEWER_HPP
