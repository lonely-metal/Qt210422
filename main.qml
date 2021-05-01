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

  210428 16:35
  フォルダ選択、フォルダ名入力追加

  210501 16:42
  読み込むファイル数を300ファイルまでに制限
  image画像表示領域を超えるサイズは縮小する
*/
import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    property int windowWidth: 1600
    property int windowHeight: 900

    width: windowWidth
    height: windowHeight
    visible: true

    property string backGroundColor: "#aaaaaa"
    property string folderNameTextColor: "#000000"
    //property string folderName: "E:\\work\\dvlp\\Qt\\images"
    property string folderName: ""
    property bool folderSelectBlock: false
    property int folderSelectIndex: 0
    property int folderSelectCurrentIndex: -1
    property var folderNames: []
    property int folderNameKry: 0
    property int foldersMax: 0
    //property bool imageLayoutVisible: false
    property var fileNames: []
    property int fileNameKey: 0
    property int filesMax: 0
    property bool imageVisible: false

    signal folderCheckSignal(string msg1, string msg2);
    signal quitButtonSignal();

    property bool shuffleComboVisible: false
    property int shuffleWaitTime: 5
    property var waitTimes: [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]

    signal setShuffleWaitTimeSignal(bool shuffleComboVisible, int shuffleWaitTime);
    signal setShuffleWaitTimeRestartSignal(int shuffleWaitTime);
    signal sleepSignal(int msec)

    function getFolderNames(obj1, obj2){
        imageVisible = false
        folderName = obj1
        foldersMax = obj2.length
        folderNames = obj2
        folderSelectBlock = true
        //folderSelectId.currentIndex = folderSelectCurrentIndex
        //folderSelectCurrentIndex = 0    //folderSelectId.currentIndex
        for (var key in obj2) {
            console.log(key, obj2[key]);
        }
        folderNameTextColor = "#0000FF"
    }

    function getFileNames(obj1) {
        filesMax = obj1.length
        fileNames = obj1
        /*
        for (var key in obj1) {
            console.log(key, obj1[key]);
        }
        */
        //imageLayoutVisible = true
        //console.log("filesMax: " + fileNames.length)
        //console.log(filesMax)
        imageVisible = true
        console.log("filesMax:" + filesMax)
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
