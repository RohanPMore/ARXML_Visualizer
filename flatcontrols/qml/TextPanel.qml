import QtQuick
import QtQuick.Controls.impl
import Flatcontrols

Rectangle {
    id: panel

    property bool highlighted

    visible: !parent.flat || control.down || control.checked
    radius: 3
    border.color: highlighted ? FlatcontrolsStyle.highlightedOutline : FlatcontrolsStyle.outline
    border.width: 1 
    color: backgroundColor
}
