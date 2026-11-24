#include <QQmlContext>
#include <QQuickItem>

#include <LR1-Explorer/ui/viewer/meshviewer.hpp>
#include "meshgeometry.hpp"

using namespace std;
using namespace std::filesystem;

namespace LR1_Explorer {
    MeshViewer::MeshViewer(QWidget* parent) : LR1Viewer(LR1::ResourceType::Mesh, QStringLiteral("OBJ files (*.obj)")), QQuickWidget(parent) {
        setSource(QUrl(QStringLiteral("qrc:/scenes/meshViewer.qml")));
    }

    bool MeshViewer::load(const LR1::FileWrapper& file) {
        auto* child = rootObject()->findChild<LR1MeshGeometry*>("modelGeometry");
        if (child) qDebug() << child->objectName();

        child->loadModel({{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}});

        return true;
    }

    bool MeshViewer::save(const LR1::FileWrapper& file, const path& outFile) {
        return false;
    }

    void MeshViewer::unload() {

    }
}
