/*
  210403:
  QMLとC++のバインディング

  https://qiita.com/DAR/items/400a1725ddedcaa6ef0e

  Signal & Slot
  C++でConnect
*/
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0

Rectangle {
    width: 640
    height: 480
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
        x: 200; y: 10
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
        x: 400; y: 10
        onClicked:{
            fileNameKey++
            if(fileNameKey >= filesMax){
                fileNameKey = 0
            }
        }
    }
    Text{
        x: 10; y: 80
        text: "index: " + fileNameKey
    }

    Image{
        x:10; y:100
        source: "image://imagedata/" + fileNames[fileNameKey] ;
    }
}
