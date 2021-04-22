import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0

Rectangle{
    width: 640
    height: 480
    visible: true
    state: "mainState"
    color: backGroundColor

    Button{
        id: startButtonToSub1
        x: 10; y: 10
        text: "Start"
        onClicked: {
            mainToSub1ButtonSignal()
            rectangle1.state = "sub1State"
        }
    }
}
