import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Universal

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Dictionary")
    minimumWidth: 300
    minimumHeight: 300

    Universal.theme: Universal.Dark
    Universal.accent: Universal.Cyan

    Component {
        id: listViewDelegate

        Rectangle {
            width: parent ? parent.width : 0
            height: childrenRect ? childrenRect.height : 0
            color: "#00000000"
            border.color: Universal.accent
            border.width: 1

            ListView {
                width: parent ? parent.width : 0
                height: contentItem ? (contentItem.childrenRect ? contentItem.childrenRect.height : 0) : 0
                model: wordElements
                delegate: Component {
                    ColumnLayout {
                        width: parent ? parent.width : 0
                        visible: (value.length > 0)

                        Label {
                            text: name
                            visible: (value.length > 0)
                            font.italic: true
                            font.pixelSize: valueId ? valueId.font.pixelSize * 0.9 : 0
                            Layout.margins: 0
                            Layout.leftMargin: 2
                            Layout.rightMargin: 2
                        }

                        Label {
                            id: valueId
                            text: value
                            visible: (value.length > 0)
                            font.bold: true
                            //readOnly: true
                            //selectByMouse: true
                            wrapMode: Label.WrapAnywhere
                            Layout.fillWidth: true
                            Layout.margins: 0
                            Layout.leftMargin: 5
                            Layout.rightMargin: 5
                            Layout.bottomMargin: 2
                            Keys.onBackPressed: { deselect(); focus = false; }
                            Keys.onEscapePressed: { deselect(); focus = false; }
                        }
                    }
                }
                clip: true
                spacing: 0
                interactive: false
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5

        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            Layout.margins: 5

            TextField {
                id: searchField
                focus: true
                font.bold: true
                placeholderText: qsTr("Search...")
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                Keys.onReturnPressed: search.clicked()
                Keys.onEnterPressed: search.clicked()
                Keys.onPressed: {
                    if (Qt.platform.os === "android")
                        search.clicked()
                }
            }

            Button {
                id: search
                text: qsTr("Search")
                Layout.alignment: Qt.AlignRight
                onClicked: wordsListModel.search(searchField.text)
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignBottom
            model: wordsListModel
            delegate: listViewDelegate
            clip: true
            spacing: 5
        }
    }
}
