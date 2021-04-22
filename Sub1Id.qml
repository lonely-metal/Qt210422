/*
  210403:
  QML��C++�̃o�C���f�B���O

  https://qiita.com/DAR/items/400a1725ddedcaa6ef0e

  Signal & Slot
  C++��Connect
*/
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
//import QtQuick.Layouts 2.12     // GridLayout // ����import�Ɏ��s���āA�A������C++����mEngine.load(url);�����s���Ă�
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
                            fileNameKey = index     // ������index�l���擾�o����̂��f���炵���I
                            rectangle1.state = "sub2State"
                        }
                    }
                }
            }
        }
    }
}
