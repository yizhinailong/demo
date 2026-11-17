import QtQuick
import QtQuick.Layouts

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Layouts")

    GridLayout {
        id: mGridLayoutId
        anchors.fill: parent // Attempt to force the Grid element to fill the parent
        columns: 3
        layoutDirection: Qt.RightToLeft

        Rectangle {
            id: topLeftRectId
            implicitWidth: 70
            implicitHeight: implicitWidth
            color: "green"
            Text {
                text: "1"
                anchors.centerIn: parent
                font.pointSize: 20
            }

            Layout.alignment: Qt.AlignRight | Qt.AlignBottom
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumWidth: 150
            Layout.maximumHeight: 150
        }
        Rectangle {
            id: topCenterRectId
            implicitWidth: 100
            implicitHeight: implicitWidth
            color: "red"
            Text {
                text: "2"
                anchors.centerIn: parent
                font.pointSize: 20
            }
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.columnSpan: 2
        }

        //Removing this because its space will be taken by rect 2 which will span two columns.

        /*
        Rectangle {
            id: topRightRectId
            implicitWidth: 100
            implicitHeight: implicitWidth
            color: "blue"
            Text{text: "3"; anchors.centerIn: parent; font.pointSize: 20}
            Layout.fillWidth: true
            Layout.fillHeight: true

        }
        */
        Rectangle {
            id: centerLeftRectId
            implicitWidth: 100
            implicitHeight: implicitWidth
            color: "beige"
            Text {
                text: "4"
                anchors.centerIn: parent
                font.pointSize: 20
            }
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 2
        }
        Rectangle {
            id: centerCenterRectId
            implicitWidth: 100
            implicitHeight: implicitWidth
            color: "pink"
            Text {
                text: "5"
                anchors.centerIn: parent
                font.pointSize: 20
            }
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Rectangle {
            id: centerRightRectId
            implicitWidth: 100
            implicitHeight: implicitWidth
            color: "yellow"
            Text {
                text: "6"
                anchors.centerIn: parent
                font.pointSize: 20
            }
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        //Romoving this because its space will be eaten up by rectangle 4, which will span two rows.

        /*
        Rectangle {
            id: bottomLeftRectId
            implicitWidth: 100
            implicitHeight: implicitWidth
            color: "magenta"
            Text{text: "7"; anchors.centerIn: parent; font.pointSize: 20}
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        */
        Rectangle {
            id: bottomCenterRectId
            implicitWidth: 100
            implicitHeight: implicitWidth
            color: "yellowgreen"
            Text {
                text: "8"
                anchors.centerIn: parent
                font.pointSize: 20
            }
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        Rectangle {
            id: bottomRightRectId
            implicitWidth: 100
            implicitHeight: implicitWidth
            color: "dodgerblue"
            Text {
                text: "9"
                anchors.centerIn: parent
                font.pointSize: 20
            }
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
