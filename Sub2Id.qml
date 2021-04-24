import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0

Rectangle {
    width: windowWidth
    height: windowHeight
    visible: false
    state: "sub2State"
    color: backGroundColor

    Button{
        id: prevButtonToSub1
        text: "Prev"
        x: 10; y: 10
        onClicked: rectangle1.state = "sub1State"
    }
    Button{
        id: prevButton
        text: "<"
        x: 150; y: 10
        onClicked:{
            fileNameKey--
            if(fileNameKey < 0){
                fileNameKey = filesMax - 1
            }
        }
    }
    Button{
        id: nextButton
        text: ">"
        x: 300; y: 10
        onClicked:{
            fileNameKey++
            if(fileNameKey >= filesMax){
                fileNameKey = 0
            }
        }
    }
    ComboBox{
        x: 450; y: 10
        model: fileNames
        onCurrentIndexChanged:{
            fileNameKey = currentIndex
        }
    }
    CheckBox{
        x: 700; y: 10
        text: "Shuffle"
        onCheckedChanged:{
            if(checked){
                shuffleComboVisible = true
            }else{
                shuffleComboVisible = false
            }
            console.log("shuffleWaitTime " + shuffleWaitTime)
            setShuffleWaitTimeSignal(shuffleComboVisible, shuffleWaitTime)
            //setShuffleWaitTimeSignal()
        }
    }
    ComboBox{
        x: 800; y: 10
        width: 70
        visible: shuffleComboVisible
        model: waitTimes
        onCurrentIndexChanged:{
            shuffleWaitTime = waitTimes[currentIndex]
            console.log("shuffleWaitTime " + shuffleWaitTime)
            setShuffleWaitTimeRestartSignal(shuffleWaitTime)
        }
    }

    Text{
        x: 10; y: 80
        text: fileNames[fileNameKey]
    }
    Image{
        x:10; y:100
        source: "image://imagedata/" + fileNames[fileNameKey] ;
    }
}
