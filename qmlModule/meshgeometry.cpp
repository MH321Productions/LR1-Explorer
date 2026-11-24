#include <utility>
#include <filesystem>

#include <QFileInfo>

#include "meshgeometry.hpp"

using namespace std;
using namespace std::filesystem;

namespace LR1_Explorer {
    LR1MeshGeometry::LR1MeshGeometry() = default;

    void LR1MeshGeometry::loadModel(const ModelData& model) {
        qDebug() << "Loading " << model.vertices.size() << " vertices";

        clear();
    }
}