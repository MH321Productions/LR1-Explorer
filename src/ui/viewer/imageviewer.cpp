#include <LR1/decoder.hpp>
#include <LR1/formats/bmp.hpp>

#include <LR1-Explorer/ui/viewer/imageviewer.hpp>

using namespace std;
using namespace std::filesystem;

namespace LR1_Explorer {
    ImageViewer::ImageViewer(QWidget* parent) : LR1Viewer(LR1::ResourceType::Image, QStringLiteral("PNG Images (*.png)")), QLabel(parent) {
        setAlignment(Qt::AlignCenter);
    }

    bool ImageViewer::load(const LR1::FileWrapper& file) {
        const optional<LR1::Bmp> bmp = decoder.decodeWrapper(file);
        if (!bmp.has_value()) return false;

        const QImage img(reinterpret_cast<const uint8_t*>(bmp->pixels.data()), bmp->width, bmp->height, QImage::Format_RGBA8888);
        setPixmap(QPixmap::fromImage(img));

        return true;
    }

    bool ImageViewer::save(const LR1::FileWrapper& file, const path& outFile) {
        return decoder.convertWrapper(file, outFile);
    }

    void ImageViewer::unload() {
        pixmap().detach();
        setPixmap({});
    }

    QWidget* ImageViewer::getWidget() {
        return this;
    }
}
