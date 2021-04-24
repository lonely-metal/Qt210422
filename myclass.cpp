#include "myclass.h"

std::string folderName {"E:\\work\\dvlp\\Qt\\images"};  // ２つ以上のクラスで使用してるので、とりあえずここで定義

MyClass::MyClass(QObject* parent)
{
    DEBUG_LOG("MyClass start\n");
}
MyClass::~MyClass(){
    DEBUG_LOG("MyClass end\n");
    //mythread->quit();     // 意味ない
}
void MyClass::initialize(QGuiApplication& app)
{
    DEBUG_LOG("MyClass start\n");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&mEngine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    mEngine.load(url);

    // Windowポインタ獲得
    mpQmlWindow = dynamic_cast<QQuickWindow*>(mEngine.rootObjects().first());

    mpObj = qobject_cast<QObject*>(mEngine.rootObjects().first());

    QObject::connect(mpQmlWindow, SIGNAL(mainToSub1ButtonSignal()),
                     this,        SLOT(mainToSub1ButtonSlot()));
    QObject::connect(mpQmlWindow, SIGNAL(quitButtonSignal()),
                     this,        SLOT(quitButtonSlot()));

    QObject::connect(mpQmlWindow, SIGNAL(setShuffleWaitTimeSignal(bool, int)),
                     this,        SLOT(setShuffleWaitTimeSlot(bool, int)));
    QObject::connect(mpQmlWindow, SIGNAL(setShuffleWaitTimeRestartSignal(int)),
                     this,        SLOT(setShuffleWaitTimeRestartSlot(int)));

    QObject::connect(mpQmlWindow, SIGNAL(sleepSignal(int)),
                     this,        SLOT(sleepSlot(int)));

    mEngine.addImageProvider(QLatin1String("imagedata"),new ImageProvider);

    mythread = std::make_unique<MyThread>();
    mythread->initialize(this);
    mythread->start();       //スレッドを開始させる．

    shufflethread = std::make_unique<ShuffleThread>();
    shufflethread->initialize(this);

    mpApp = &app;
}

void MyClass::mainToSub1ButtonSlot() {
    DEBUG_LOG("\n");
    invokeFileNames(mpObj);
}
void MyClass::quitButtonSlot(){
    mythread->quit();
    shufflethread->quit();
    mpApp->quit();      // アプリ終了
}

void MyClass::setShuffleWaitTimeSlot(bool  shuffleComboVisible, int shuffleWaitTime)
{
    if(shuffleComboVisible){
        shufflethread->setWaitTime(shuffleWaitTime);
        shufflethread->start();
    }else{
        shufflethread->quit();
    }
}
void MyClass::setShuffleWaitTimeRestartSlot(int shuffleWaitTime)
{
    shufflethread->quit();
    while(1){
        if(! shufflethread->isRunning()){
            break;
        }
        std::chrono::milliseconds(static_cast<long long>(100));     // 0.1秒待ち合わせ
    }
    shufflethread->setWaitTime(shuffleWaitTime);
    shufflethread->start();
}
void MyClass::sleepSlot(int msec)
{
    std::chrono::milliseconds(static_cast<long long>(msec));
}

void MyClass::folderSearchSlot(){
    invokeFileNames(mpObj);
}
void MyClass::invokeFileNames(QObject* object){

    //DEBUG_LOG("\n");

    std::vector<std::string>  vec;
    getFiles(folderName, vec);

    QVariantList qvlist;
    for(int i=0; i<vec.size(); i++){
        qvlist.push_back(vec[i].c_str());
    }
    QMetaObject::invokeMethod((QObject*)object, "getFileNames",
            Q_ARG(QVariant, QVariant::fromValue(qvlist)));
}
void MyClass::getFiles(const std::string& st, std::vector<std::string>& vec){
    for (const std::filesystem::directory_entry& i : std::filesystem::directory_iterator(st)) {
        if (! i.is_directory()) {
            vec.emplace_back(i.path().filename().string());
        }
    }
}
void MyClass::shufflePlaySlot(){
    QMetaObject::invokeMethod((QObject*)mpObj, "shufflePlay");
}
QObject* MyClass::getMyObject()
{
    return mpObj;
}
void MyClass::setMyObject(QObject* obj)
{
    mpObj = obj;
}

MyThread::MyThread(QObject* parent):QThread(parent)
{
}
void MyThread::initialize(MyClass* myclass)
{
    mpMyclass = std::make_shared<MyClass>(myclass);
    mpMyclass->setMyObject(myclass->getMyObject());  // この代入が重要？
}
void MyThread::run(){   //スレッドが始まるときに，これが呼ばれる

    DEBUG_LOG("MyThread run() start\n");

    QTimer timer;             //タイマーを作成する．
    QObject::connect(&timer, SIGNAL(timeout()), mpMyclass.get(), SLOT(folderSearchSlot()), Qt::DirectConnection);
    timer.setInterval(500);
    timer.start();

    exec();     //Start event loop          スレッド動作を開始する．スレッドが終了するまで以下は実行されない．

    timer.stop();

    DEBUG_LOG("Mythread run() end\n");
}

ShuffleThread::ShuffleThread(QObject* parent):QThread(parent)
{
}
void ShuffleThread::initialize(MyClass* myclass)
{
    mpMyclass = std::make_shared<MyClass>(myclass);
    mpMyclass->setMyObject(myclass->getMyObject());  // この代入が重要？
}
void ShuffleThread::run()
{
    DEBUG_LOG("Shufflethread run() start\n");

    QTimer timer;             //タイマーを作成する．
    QObject::connect(&timer, SIGNAL(timeout()), mpMyclass.get(), SLOT(shufflePlaySlot()), Qt::DirectConnection);
    timer.setInterval(mWaitTime * 1000);
    timer.start();

    exec();     //Start event loop          スレッド動作を開始する．スレッドが終了するまで以下は実行されない．

    timer.stop();

    DEBUG_LOG("Shufflethread run() end\n");
}
void ShuffleThread::setWaitTime(int waitTime){
    mWaitTime = waitTime;
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize& requestedSize){

    QImage image(static_cast<QString>(folderName.c_str()) + "\\" + id);
    return image;
}
