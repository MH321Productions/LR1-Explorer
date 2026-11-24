#ifndef LR1_EXPLORER_MESHGEOMETRY_HPP
#define LR1_EXPLORER_MESHGEOMETRY_HPP

#include <vector>

#include <QQuick3DGeometry>

namespace LR1_Explorer {

    struct ModelData {
        std::vector<float> vertices;
    };

    class LR1MeshGeometry : public QQuick3DGeometry {
        Q_OBJECT
        QML_NAMED_ELEMENT(LR1MeshGeometry)
        //Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

        public:
        Q_DISABLE_COPY_MOVE(LR1MeshGeometry)

        LR1MeshGeometry();

        void loadModel(const ModelData& model);
    };
}

#endif //LR1_EXPLORER_MESHGEOMETRY_HPP
