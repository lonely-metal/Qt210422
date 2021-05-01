import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
//import QtQuick.Layouts 2.12     // GridLayout // このimportに失敗して、連動してC++側のmEngine.load(url);が失敗してた
import QtQuick.Layouts 1.12

Rectangle {
    width: windowWidth
    height: windowHeight
    visible: true
    state: "sub1State"
    color: backGroundColor

    TextField{
        id: textFieldId
        x:10; y:10
        width: 500; height: 30
        color: folderNameTextColor
        font.pixelSize: 20
        text: folderName
        /*
        onAccepted:{
            folderNameTextColor = "#000000"
        }
        */
        onEditingFinished:{
            console.log("TextField")
            folderNameTextColor = "#000000"
            folderSelectBlock = false
            folderCheckSignal(textFieldId.text, folderNames[0])
        }
    }
    Button{
        visible: false
        id: prevButtonToMain
        x:10; y: 50
        text: "Prev"
        onClicked: rectangle1.state = "mainState"
    }
    ComboBox{
        id: folderSelectId
        x: 10; y: 50
        model: folderNames
        onCurrentIndexChanged:{
            console.log("ComboBox: folderCheckSignal " + currentIndex)
            if(folderSelectBlock){
                console.log("ComboBox: folderCheckSignal " + currentIndex)
                folderSelectBlock = false
                //imageLayoutVisible = false
                //folderSelectCurrentIndex = currentIndex
                folderCheckSignal(folderName, folderNames[currentIndex])
            }
        }
    }
    Button{
        id: nextButtonToSub2
        text: "Next"
        x: 200; y: 50
        onClicked: rectangle1.state = "sub2State"
    }
    Button{
        id: quitButton
        x: 1000; y: 50
        text: "Quit"
        onClicked: {
            quitButtonSignal()
        }
    }

    GridLayout{
        columns: windowWidth / 110
        x:10; y:120
        Repeater{
            model: filesMax
            Rectangle{
                width: 100; height: 50
                //border.width: 30
                Image{
                    //visible: imageVisible
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
