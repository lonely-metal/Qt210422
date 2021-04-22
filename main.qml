/*
  210421:
  特定フォルダに格納されてる画像表示ツール作成

  Rpeater参考URL： https://doc.qt.io/archives/qtquick-components-symbian-1.1/examples-native-scalability-positioners-grid-layout.html

  210422 13:58
  ついに思った通りの画像ビューワが完成！！

  sub2IdにComboBox選択を追加する
*/
import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: 640
    height: 480
    visible: true

    property string backGroundColor: "#aaaaaa"
    property var fileNames: []
    property int fileNameKey: 0
    property int filesMax: 0
    property bool imageVisible: false

    signal mainToSub1ButtonSignal();

    function getFileNames(obj) {
        fileNames = obj
        for (var key in obj) {
            console.log(key, fileNames[key]);
        }
        filesMax = fileNames.length
        console.log(filesMax)
        imageVisible = true
    }

    Rectangle {
        id:rectangle1
        color: backGroundColor

        MainId{
            id: mainId
        }
        Sub1Id{
            id: sub1Id
        }
        Sub2Id{
            id: sub2Id
        }

        states: [
            State {
                name: "mainState"
                PropertyChanges {
                    target: mainId
                    visible:true
                }
            },
            State{
                name: "sub1State"
                PropertyChanges {
                    target: sub1Id
                    visible:true
                }
            },
            State{
                name: "sub2State"
                PropertyChanges {
                    target: sub2Id
                    visible:true
                }
            }
        ]
    }
}
