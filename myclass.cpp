#include "myclass.h"

std::string folderName {"E:\\work\\dvlp\\Qt\\images"};  // ２つ以上のクラスで使用してるので、とりあえずここで定義

MainClass::MainClass(QObject* parent)
{
    DEBUG_LOG("MainClass start\n");
}
MainClass::~MainClass(){
    DEBUG_LOG("MainClass end\n");
    //folderThread->quit();     // 意味ない
}
void MainClass::initialize(QGuiApplication& app)
{
    DEBUG_LOG("MainClass start\n");

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

    folderThread = std::make_unique<FolderThread>();
    folderThread->initialize(this);
    folderThread->start();       //スレッドを開始させる．

    shuffleThread = std::make_unique<ShuffleThread>();
    shuffleThread->initialize(this);

    mpApp = &app;
}

void MainClass::mainToSub1ButtonSlot() {
    DEBUG_LOG("\n");
    invokeFileNames(mpObj);
}
void MainClass::quitButtonSlot(){
    folderThread->quit();
    shuffleThread->quit();
    mpApp->quit();      // アプリ終了
}

void MainClass::setShuffleWaitTimeSlot(bool  shuffleComboVisible, int shuffleWaitTime)
{
    if(shuffleComboVisible){
        shuffleThread->setWaitTime(shuffleWaitTime);
        shuffleThread->start();
    }else{
        shuffleThread->quit();
    }
}
void MainClass::setShuffleWaitTimeRestartSlot(int shuffleWaitTime)
{
    shuffleThread->quit();
    while(1){
        if(! shuffleThread->isRunning()){
            break;
        }
        std::chrono::milliseconds(static_cast<long long>(100));     // 0.1秒待ち合わせ
    }
    shuffleThread->setWaitTime(shuffleWaitTime);
    shuffleThread->start();
}
void MainClass::sleepSlot(int msec)
{
    std::chrono::milliseconds(static_cast<long long>(msec));
}

void MainClass::folderSearchSlot(){
    invokeFileNames(mpObj);
}
void MainClass::invokeFileNames(QObject* object){

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
void MainClass::getFiles(const std::string& st, std::vector<std::string>& vec){
    for (const std::filesystem::directory_entry& i : std::filesystem::directory_iterator(st)) {
        if (! i.is_directory()) {
            vec.emplace_back(i.path().filename().string());
        }
    }
}
void MainClass::shufflePlaySlot(){
    QMetaObject::invokeMethod((QObject*)mpObj, "shufflePlay");
}
QObject* MainClass::getMainclassObject()
{
    return mpObj;
}
void MainClass::setMainclassObject(QObject* obj)
{
    mpObj = obj;
}

FolderThread::FolderThread(QObject* parent):QThread(parent)
{
}
void FolderThread::initialize(MainClass* mainclass)
{
    mpMainclass = std::make_shared<MainClass>(mainclass);
    mpMainclass->setMainclassObject(mainclass->getMainclassObject());  // この代入が重要？
}
void FolderThread::run()
{
    DEBUG_LOG("FolderThread run() start\n");

    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()), mpMainclass.get(), SLOT(folderSearchSlot()), Qt::DirectConnection);
    timer.setInterval(500);
    timer.start();

    exec();

    timer.stop();

    DEBUG_LOG("Mythread run() end\n");
}

ShuffleThread::ShuffleThread(QObject* parent):QThread(parent)
{
}
void ShuffleThread::initialize(MainClass* mainclass)
{
    mpMainclass = std::make_shared<MainClass>(mainclass);
    mpMainclass->setMainclassObject(mainclass->getMainclassObject());  // この代入が重要？
}
void ShuffleThread::run()
{
    DEBUG_LOG("Shufflethread run() start\n");

    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()), mpMainclass.get(), SLOT(shufflePlaySlot()), Qt::DirectConnection);
    timer.setInterval(mWaitTime * 1000);
    timer.start();

    exec();

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
