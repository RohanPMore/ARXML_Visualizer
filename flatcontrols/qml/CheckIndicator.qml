import QtQuick

import Flatcontrols
import FontAwesome

Rectangle {
    id: indicatorItem
    property Item control
    //property int checkState: control.checkState

    FontAwesome {
        id: awesome
    }

    implicitWidth: 20
    implicitHeight: 20
    color: FlatcontrolsStyle.base
    border.color: control.hovered ? FlatcontrolsStyle.highlightedOutline : FlatcontrolsStyle.outline
    border.width: 1
    radius: 2

    Behavior on border.color {
        ColorAnimation {
            duration: 100
            easing.type: Easing.OutCubic
        }
    }

    Text {
        id: checkImage
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        font.pointSize: 9
        font.weight: awesome.weight_fa_solid
        font.family: awesome.family
        text: awesome.icons.fas_check
        color: FlatcontrolsStyle.text
        scale: control.checkState === Qt.Checked ? 1 : 0
        Behavior on scale { NumberAnimation { duration: 100 } }
    }

    Rectangle {
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 12
        height: 3

        scale: control.checkState === Qt.PartiallyChecked ? 1 : 0
        Behavior on scale { NumberAnimation { duration: 100 } }
    }

    states: [
        State {
            name: "checked"
            when: control.checkState === Qt.Checked
        },
        State {
            name: "partiallychecked"
            when: control.checkState === Qt.PartiallyChecked
        }
    ]

    transitions: Transition {
        SequentialAnimation {
            NumberAnimation {
                target: indicatorItem
                property: "scale"
                // Go down 2 pixels in size.
                to: 1 - 2 / indicatorItem.width
                duration: 60
            }
            NumberAnimation {
                target: indicatorItem
                property: "scale"
                to: 1
                duration: 60
            }
        }
    }
}
