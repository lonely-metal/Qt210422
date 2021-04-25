import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
//import QtQuick.Layouts 2.12     // GridLayout // ����import�Ɏ��s���āA�A������C++����mEngine.load(url);�����s���Ă�
import QtQuick.Layouts 1.12

Rectangle {
    width: windowWidth
    height: windowHeight
    visible: true
    state: "sub1State"
    color: backGroundColor

    TextInput{
        id: folderName
        x:150; y:10
        color: "#FFFFFF"
    }
    Button{
        visible: false
        id: prevButtonToMain
        x:10; y: 80
        text: "Prev"
        onClicked: rectangle1.state = "mainState"
    }
    ComboBox{
        id: folderSelect
        x: 150; y: 80
    }
    Button{
        id: nextButtonToSub2
        text: "Next"
        x: 350; y: 80
        onClicked: rectangle1.state = "sub2State"
    }
    Button{
        id: quitButton
        x: 1000; y: 80
        text: "Quit"
        onClicked: {
            quitButtonSignal()
        }
    }

    GridLayout{
        columns: windowWidth / 110
        x:10; y:160
        Repeater{
            model: filesMax
            Rectangle{
                width: 100; height: 50
                //border.width: 30
                Image{
                    anchors.fill: parent
                    source: "image://imagedata/" + fileNames[index] ;
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            fileNameKey = index     // ������index�l���擾�o����̂��f���炵���I
                            rectangle1.state = "sub2State"
                        }
                    }
                }
            }
        }
    }
}
