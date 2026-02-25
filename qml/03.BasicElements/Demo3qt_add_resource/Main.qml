import QtQuick

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Loading Images through Qt6 CMake facilities")

    Item {
        id: containerItemId
        // x: 150; y: 50
        width: 600
        height: 300
        anchors.centerIn: parent

        Rectangle {
            color: "gray"
            anchors.fill: parent
        }

        Image {
            x: 50
            y: 50
            width: 100
            height: 100
            //Method1: Loading image from through qt_add_resource in the CMakeLists.txt file
            // source: "qrc:/image/LearnQt.png"

            //Method2: Loading image through qt_add_qml_module in the CMakeLists.txt file
            source: "image/LearnQt.png"
        }
    }
}
