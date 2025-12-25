import QtQuick

Rectangle {
    id: root
    width: 300
    height: 300

    Text {
        id: thisLabel
        text: "thisLabel"
        horizontalAlignment: Text.AlignHCenter
        focus: true
        KeyNavigation.tab: thatLabel

        font {
            family: "JetBrainsMonoNL Nerd Font"
            pointSize: 20
            bold: true
        }
    }

    Text {
        id: thatLabel
        text: "thatLabel"
        focus: true
        KeyNavigation.tab: thisLabel

        y: thisLabel.height

        font {
            family: "JetBrainsMonoNL Nerd Font"
            pointSize: 20
            bold: true
        }
    }
}
