import QtQuick3D
import LR1Explorer

View3D {
    id: view
    anchors.fill: parent

    //! [environment]
    environment: SceneEnvironment {
        clearColor: "skyblue"
        backgroundMode: SceneEnvironment.Color
    }
    //! [environment]

    //! [camera]
    PerspectiveCamera {
        position: Qt.vector3d(0, 200, 300)
        eulerRotation.x: -30
    }
    //! [camera]

    //! [light]
    DirectionalLight {
        eulerRotation.x: -30
        eulerRotation.y: -70
    }
    //! [light]

    //! [objects]
    Model {
        id: lrMeshModel
        position: Qt.vector3d(0, 0, 0)
        geometry: LR1MeshGeometry {
            objectName: "modelGeometry"
        }
        materials: [ PrincipledMaterial {
            baseColor: "red"
        }
        ]
    }
    //! [objects]
}