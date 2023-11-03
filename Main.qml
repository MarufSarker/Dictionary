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
            radius: 5
            border.width: 1
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#151515" }
                GradientStop { position: 1.0; color: "#1A1A1A" }
            }

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
                            Layout.margins: 5
                            Layout.bottomMargin: 0
                        }

                        Label {
                            id: valueId
                            text: value
                            visible: (value.length > 0)
                            font.bold: true
                            wrapMode: Label.WrapAnywhere
                            Layout.fillWidth: true
                            Layout.margins: 10
                            Layout.topMargin: 0
                            Layout.bottomMargin: 5
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

            Rectangle {
                Layout.fillWidth: true
                implicitHeight: searchField.height + 5
                color: "#1A1A1A"
                radius: height * 0.5
                border.width: 2
                border.color: "#FFFFFF"

                RowLayout {
                    width: parent.width - 5
                    anchors.centerIn: parent

                    TextField {
                        id: searchField
                        focus: true
                        font.bold: true
                        color: "#FFFFFF"
                        placeholderTextColor: "#FFFFFF"
                        placeholderText: qsTr("Search...")
                        Layout.alignment: Qt.AlignCenter
                        Layout.fillWidth: true
                        Keys.onReturnPressed: doSearch()
                        Keys.onEnterPressed: doSearch()
                        Keys.onPressed: {
                            if (Qt.platform.os === "android")
                                doSearch()
                        }
                        background: Rectangle { color: "transparent" }
                        function doSearch() {
                            wordsListModel.search(searchField.text)
                        }
                    }

                    Rectangle {
                        implicitWidth: searchField.height - 5
                        implicitHeight: searchField.height - 5
                        color: "transparent"

                        Image {
                            source: "qrc:/qt/qml/Dictionary/assets/images/x-white.svg"
                            anchors.fill: parent
                            anchors.margins: 3
                            sourceSize.width: width
                            sourceSize.height: height
                            fillMode: Image.PreserveAspectFit
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                searchField.forceActiveFocus()
                                searchField.text = ""
                            }
                        }
                    }

                    Rectangle {
                        implicitWidth: searchField.height - 5
                        implicitHeight: searchField.height - 5
                        Layout.rightMargin: 5
                        color: "transparent"

                        Image {
                            source: "qrc:/qt/qml/Dictionary/assets/images/search-white.svg"
                            anchors.fill: parent
                            anchors.margins: 3
                            sourceSize.width: width
                            sourceSize.height: height
                            fillMode: Image.PreserveAspectFit
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                forceActiveFocus()
                                searchField.doSearch()
                            }
                        }
                    }
                }
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
