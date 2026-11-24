#ifndef LR1_EXPLORER_MESHVIEWER_HPP
#define LR1_EXPLORER_MESHVIEWER_HPP

#include <QQuickWidget>

#include <LR1-Explorer/ui/viewer.hpp>

namespace LR1_Explorer {
    class MeshViewer : public QQuickWidget, public virtual LR1Viewer {
        Q_OBJECT

        public:
            Q_DISABLE_COPY_MOVE(MeshViewer)

            explicit MeshViewer(QWidget *parent = nullptr);
            ~MeshViewer() override = default;

            bool load(const LR1::FileWrapper &file) override;
            bool save(const LR1::FileWrapper &file, const std::filesystem::path &outFile) override;
            void unload() override;
            QWidget* getWidget() override {return this;}

    };
}

#endif //LR1_EXPLORER_MESHVIEWER_HPP
