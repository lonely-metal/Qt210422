import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
//import QtQuick.Layouts 2.12     // GridLayout // このimportに失敗して、連動してC++側のmEngine.load(url);が失敗してた
import QtQuick.Layouts 1.12

Rectangle {
    width: windowWidth
    height: windowHeight
    visible: false
    state: "sub1State"
    color: backGroundColor

    TextInput{
        id: folderName
        x:150; y:10
        color: "#FFFFFF"
    }
    Button{
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
                            fileNameKey = index     // ここでindex値を取得出来るのが素晴らしい！
                            rectangle1.state = "sub2State"
                        }
                    }
                }
            }
        }
    }
}
