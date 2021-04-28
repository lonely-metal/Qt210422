import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0

Rectangle {
    width: windowWidth
    height: windowHeight
    visible: false
    state: "sub2State"
    color: backGroundColor

    function shuffleCancel(){
        shuffleCheck.checked = false
        shuffleComboVisible = false
        setShuffleWaitTimeSignal(shuffleComboVisible, shuffleWaitTime)
    }

    Button{
        id: prevButtonToSub1
        text: "Prev"
        x: 10; y: 10
        onClicked: {
            rectangle1.state = "sub1State"
            shuffleCancel()
        }
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
            shuffleCancel()
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
            shuffleCancel()
        }
    }
    ComboBox{
        x: 450; y: 10
        model: fileNames
        onCurrentIndexChanged:{
            console.log("ComboBox: shuffleCancel()")
            fileNameKey = currentIndex
            shuffleCancel()
        }
    }
    CheckBox{
        id: shuffleCheck
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
        }
    }
    ComboBox{
        x: 800; y: 10
        width: 70
        visible: shuffleComboVisible
        model: waitTimes
        onCurrentIndexChanged:{
            shuffleWaitTime = waitTimes[currentIndex]
            console.log("ComboBox shuffleWaitTime " + shuffleWaitTime)
            setShuffleWaitTimeRestartSignal(shuffleWaitTime)
        }
    }
    Button{
        id: quitButton
        x: 1000; y: 10
        text: "Quit"
        onClicked: {
            quitButtonSignal()
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
