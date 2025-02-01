import QtQuick
import QtQuick.Templates as T

import Flatcontrols

T.Label {
    id: control

    color: enabled ? FlatcontrolsStyle.text : FlatcontrolsStyle.toolTipText
    linkColor: FlatcontrolsStyle.link
}
