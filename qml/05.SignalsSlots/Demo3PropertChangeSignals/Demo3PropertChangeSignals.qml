import QtQuick

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Property Change Handlers")

    Rectangle {
        id: rectId
        width: 300
        height: width + 50 // Property binding
        color: "dodgerblue"

        //Custom property
        property string descritption: "A rectangle to play with"

        //Property change handlers
        onWidthChanged: {
            console.log("Width changed to :" + rectId.width)
        }

        onHeightChanged: {
            console.log("Height changed to :" + rectId.height)
        }

        onColorChanged: {
            console.log("Color changed")
        }

        onVisibleChanged: {

        }

        onDescritptionChanged: {
            console.log("Description changed to: " + rectId.descritption)
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                rectId.width += 20
                rectId.descritption = "New data"
            }
        }
    }
}
