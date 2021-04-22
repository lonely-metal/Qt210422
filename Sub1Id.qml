/*
  210403:
  QMLとC++のバインディング

  https://qiita.com/DAR/items/400a1725ddedcaa6ef0e

  Signal & Slot
  C++でConnect
*/
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
//import QtQuick.Layouts 2.12     // GridLayout // このimportに失敗して、連動してC++側のmEngine.load(url);が失敗してた
import QtQuick.Layouts 1.12

Rectangle {
    width: 640
    height: 480
    visible: false
    state: "sub1State"
    color: backGroundColor

    Button{
        id: prevButtonToMain
        text: "Prev"
        x: 10; y: 10
        onClicked: rectangle1.state = "mainState"
    }
    Button{
        id: nextButtonToSub2
        text: "Next"
        x: 200; y: 10
        onClicked: rectangle1.state = "sub2State"
    }
    GridLayout{
        columns: 5
        x:10; y:100
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
