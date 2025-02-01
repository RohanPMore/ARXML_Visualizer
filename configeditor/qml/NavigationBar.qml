import QtQuick
import Flatcontrols
import ConfigEditor
import FontAwesome

Item {
    id: navBarRoot
    property color background: FlatcontrolsStyle.alternateBase
    property color textColor: FlatcontrolsStyle.text
    property bool backEnabled
    property bool forwardEnabled

    signal forwardClicked()
    signal backClicked()
    signal addressChanged(ecucPath: string)
    signal linkActivated(ecucPath: string)

    onAddressChanged: (ecucPath) => {
        if (ecucPath.length > 0) {
            // Add hyperlinks to the path and update the text in the textedit

            // The ecucPath might also contain a reference to a Bswmd container (/a/b/c:xyz)
            const ecucBswmdSplit = ecucPath.split(":")

            const ecuc = ecucBswmdSplit[0]
            const arr = ecuc.split("/").slice(1)
            let current_path = "";
            let output = "<style>a:link { color: " + FlatcontrolsStyle.link + "; text-decoration: none; }</style>";
            let combineWithNext = false;

            // Usually the first entry refers to the autosar package. It should link to the next entry as well:
            if(arr.length > 1) {
                current_path = current_path.concat("/", arr[0]);
                const link = "<a href=\""+current_path+"/"+arr[1]+"\">";
                output = output.concat("/", link, arr[0], "</a>");
            }

            for (let i = 1; i < arr.length; i++) {
                current_path = current_path.concat("/", arr[i]);
                const link = "<a href=\""+current_path+"\">";
                output = output.concat("/", link, arr[i], "</a>");
            } 
            
            // Add Bswmd reference at the end if needed.
            if(ecucBswmdSplit.length > 1)
            {
                output = output.concat(" : ", ecucBswmdSplit[1]);
            }

            addressField.text = output;
        }
    }

    FontAwesome {
        id: awesome
    }

    Rectangle {
        anchors.fill: parent
        color: navBarRoot.background
    }

    Rectangle {
            height: 1
            color: FlatcontrolsStyle.button
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
    }

    NavigationButton {
        id: back
        background: navBarRoot.background
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        text: awesome.icons.fas_arrow_left
        height: parent.height*0.8
        width: parent.height*0.8
        onClicked: navBarRoot.backClicked()
    }

    NavigationButton {
        id: forward
        background: navBarRoot.background
        anchors.left: back.right
        anchors.verticalCenter: parent.verticalCenter
        text: awesome.icons.fas_arrow_right
        height: parent.height*0.8
        width: parent.height*0.8
        onClicked: navBarRoot.forwardClicked()
    }
    Rectangle {
        anchors.left: forward.right
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 5
        anchors.leftMargin: 5

        height: parent.height*0.7
        radius: 3
        color: FlatcontrolsStyle.base
        border.color: FlatcontrolsStyle.outline
        border.width: 1

        TextEdit {
            id:addressField
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 5
            anchors.bottomMargin: 5

            font.pixelSize: 12
            color: navBarRoot.textColor
            selectionColor: FlatcontrolsStyle.highlight
            readOnly: true
            textFormat: TextEdit.RichText

            onLinkActivated: (ecucPath) => {
                navBarRoot.linkActivated(ecucPath);
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.NoButton
                cursorShape: Qt.PointingHandCursor
            }
        }
    }
    
}