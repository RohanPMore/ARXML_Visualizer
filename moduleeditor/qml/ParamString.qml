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
        TextField {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.minimumWidth: 20
            Layout.preferredWidth: 500
            topPadding: 0
            bottomPadding: 0
            font.pixelSize: paramList.defaultFontPixels
            text: model.value
        }
    }
}
