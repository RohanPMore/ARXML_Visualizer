import QtQuick
import FontAwesome

Item {
    id: awesome

    property alias icons: variables
    readonly property bool loaded: (loader_solid.status === FontLoader.Ready) && (loader_reg.status === FontLoader.Ready)

    readonly property string family: "Font Awesome 6 Free"
    readonly property int weight_fa_solid: 900
    readonly property int weight_fa_regular: 400

    FontLoader {
        id: loader_solid
        source: "qrc:/resource/fa-solid-900.ttf"
    }

    FontLoader {
        id: loader_reg
        source: "qrc:/resource/fa-regular-400.ttf"
    }

    Variables {
        id: variables
    }
}