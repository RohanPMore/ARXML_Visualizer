import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Flatcontrols

Item {
    height: paramList.defaultLineHeight
    width: paramList.width
    RowLayout {
        anchors.fill: parent
        Label {
            Layout.minimumWidth: paramList.parameterNameWidth
            Layout.maximumWidth: paramList.parameterNameWidth
            font.pixelSize: paramList.defaultFontPixels
            text: model.name + ":"
        }
        CheckBox {
            padding: 0
            spacing: 0
            checked: model.value
        }
        // Filler
        Item {
            Layout.fillWidth: true
        }
    }
}
