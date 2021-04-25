/*
  210421:
  特定フォルダに格納されてる画像表示ツール作成

  Rpeater参考URL： https://doc.qt.io/archives/qtquick-components-symbian-1.1/examples-native-scalability-positioners-grid-layout.html

  210422 13:58
  ついに思った通りの画像ビューワが完成

  210423 19:08
  別スレッドから定期的にフォルダ監視する処理に成功

  210424 15:56
  シャッフル画像表示と表示時間の設定OK
*/
import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    property int windowWidth: 1280
    property int windowHeight: 720

    width: windowWidth
    height: windowHeight
    visible: true

    property string backGroundColor: "#aaaaaa"
    property var fileNames: []
    property int fileNameKey: 0
    property int filesMax: 0
    property bool imageVisible: false

    signal mainToSub1ButtonSignal();
    signal quitButtonSignal();

    property bool shuffleComboVisible: false
    property int shuffleWaitTime: 5
    property var waitTimes: [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]

    signal setShuffleWaitTimeSignal(bool shuffleComboVisible, int shuffleWaitTime);
    signal setShuffleWaitTimeRestartSignal(int shuffleWaitTime);
    signal sleepSignal(int msec)

    function getFileNames(obj) {
        fileNames = obj
        for (var key in obj) {
            //console.log(key, fileNames[key]);
        }
        filesMax = fileNames.length
        //console.log(filesMax)
        imageVisible = true
    }
    function shufflePlay() {
        fileNameKey = Math.random() * 100000 % filesMax   // Math.random()で返ってくる値は1未満の小数なので100000くらい掛けとく
        console.log("fileNameKey " + fileNameKey)
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
