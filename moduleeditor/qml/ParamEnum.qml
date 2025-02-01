import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Flatcontrols

Item {
    property var choices
    property var value

    height: paramList.defaultLineHeight
    width: paramList.width
    id:item

    choices: model.choices
    value: model.value

    RowLayout {
        anchors.fill: parent
        Label {
            Layout.minimumWidth: paramList.parameterNameWidth
            Layout.maximumWidth: paramList.parameterNameWidth
            font.pixelSize: paramList.defaultFontPixels
            text: model.name + ":"
        }
        
        ComboBox {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.minimumWidth: 300
            Layout.maximumWidth: 300

            font.pixelSize: paramList.defaultFontPixels
            topPadding: 0
            bottomPadding: 0
            model: choices

            Component.onCompleted: currentIndex = find(item.value)
        }
        // Filler
        Item {
            Layout.fillWidth: true
        }
    }
}
