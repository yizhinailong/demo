import QtQuick

Rectangle {
    id: root
    width: 300
    height: 300

    Text {
        id: thisLabel
        text: "thisLabel"
        horizontalAlignment: Text.AlignHCenter
        focus: false
        color: focus ? "red" : "black"
        KeyNavigation.tab: thatLabel

        property int time: 24

        font {
            family: "JetBrainsMonoNL Nerd Font"
            pointSize: 20
            bold: true
        }
    }

    Text {
        id: thatLabel
        text: "thatLabel" + " " + thisLabel.time
        focus: false
        color: focus ? "red" : "black"
        KeyNavigation.tab: thisLabel

        y: thisLabel.height

        font {
            family: "JetBrainsMonoNL Nerd Font"
            pointSize: 20
            bold: true
        }

        Keys.onSpacePressed: {
            increment()
        }

        function increment() {
            thisLabel.time += 1
        }
    }
}
