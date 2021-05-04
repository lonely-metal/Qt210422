#include "myclass.h"

// ２つ以上のクラスで使用してるので、とりあえずここで定義
QString folderName {"E:\\work\\dvlp\\Qt\\images"};
//std::string folderName {};

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
    DEBUG_LOG("MainClass start id:%d\n", QThread::currentThreadId());

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

    QObject::connect(mpQmlWindow, SIGNAL(folderCheckSignal(QString, QString)),
                     this,        SLOT(folderCheckSlot(QString, QString)));
    QObject::connect(mpQmlWindow, SIGNAL(quitButtonSignal()),
                     this,        SLOT(quitButtonSlot()));

    QObject::connect(mpQmlWindow, SIGNAL(setShuffleWaitTimeSignal(bool, int)),
                     this,        SLOT(setShuffleWaitTimeSlot(bool, int)));
    QObject::connect(mpQmlWindow, SIGNAL(setShuffleWaitTimeRestartSignal(int)),
                     this,        SLOT(setShuffleWaitTimeRestartSlot(int)));

    QObject::connect(mpQmlWindow, SIGNAL(sleepSignal(int)),
                     this,        SLOT(sleepSlot(int)));

    mEngine.addImageProvider(QLatin1String("imagedata"),new ImageProvider);
    mEngine.addImageProvider(QLatin1String("thumbnaildata"),new ThumbnailProvider);

    folderThread = std::make_unique<FolderThread>();
    //moveToThread(folderThread.get());
    folderThread->initialize(this);
    folderThread->start();       //スレッドを開始させる．

    shuffleThread = std::make_unique<ShuffleThread>();
    //moveToThread(shuffleThread.get());
    shuffleThread->initialize(this);

    invokeFolderNames(mpObj, folderName);

    mpApp = &app;
}

void MainClass::folderCheckSlot(QString msg1, QString msg2)
{
    QTextCodec* codec = QTextCodec::codecForName("Shift-JIS");
    QByteArray encoded = codec->fromUnicode(msg1);
    std::filesystem::path folder = encoded.toStdString();

    DEBUG_LOG("%s\n", msg1.toStdString().c_str());
    if(false == std::filesystem::exists(folder)){
        DEBUG_LOG("%s err\n", msg1.toStdString().c_str());
        return;
    }

    QString foldername{};
    if("." == msg2.toStdString()){
        foldername = msg1;
    }else if(".." == msg2.toStdString()){
        folder = folder.parent_path();
        foldername = codec->toUnicode(folder.string().c_str());
        //DEBUG_LOG("%s\n", foldername.c_str());
    }else{
        foldername = msg1 + "\\" + msg2;
        //DEBUG_LOG("%s\n", foldername.c_str());
    }

    invokeFolderNames(mpObj, foldername);
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

void MainClass::invokeFolderNames(QObject* object, QString& foldername)
{
    //DEBUG_LOG("%s\n", foldername.c_str());

    folderName = foldername;
    QVariant qv = folderName;

    std::vector<QString>  folderNames{};
    getFolders(folderName, folderNames);

    QVariantList qvlist;
    for(int i=0; i<folderNames.size(); i++){
        qvlist.push_back(folderNames[i]);
    }
    QMetaObject::invokeMethod((QObject*)object, "getFolderNames",
            Q_ARG(QVariant, QVariant::fromValue(qv)),
            Q_ARG(QVariant, QVariant::fromValue(qvlist)));
}
void MainClass::getFolders(const QString& folderName, std::vector<QString>& foldernames)
{
    foldernames.emplace_back(".");
    foldernames.emplace_back("..");

    QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
    QByteArray encoded = codec->fromUnicode(folderName);

    for (const std::filesystem::directory_entry& i : std::filesystem::directory_iterator(encoded.toStdString())) {
        if (i.is_directory()) {
            foldernames.emplace_back(codec->toUnicode(i.path().filename().string().c_str()));
        }
    }
}
void MainClass::invokeFileNames(QObject* object, QString& foldername){

    //DEBUG_LOG("%s\n", foldername.c_str());

    std::vector<QString>  fileNames;
    getFiles(folderName, fileNames);

    QVariantList qvlist;
    for(int i=0; i<fileNames.size(); i++){
        qvlist.push_back(fileNames[i]);
    }
    QMetaObject::invokeMethod((QObject*)object, "getFileNames",
            Q_ARG(QVariant, QVariant::fromValue(qvlist)));
}
void MainClass::getFiles(const QString& folderName, std::vector<QString>& fileNames)
{
    QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
    QByteArray encoded = codec->fromUnicode(folderName);

    int count = 0;
    for (const std::filesystem::directory_entry& i : std::filesystem::directory_iterator(encoded.toStdString())) {
        if (! i.is_directory()) {
            std::filesystem::path ex = i.path().extension();
            if(ex == ".jpg" || ex == ".jpeg" || ex == ".png" || ex == ".gif"
            || ex == ".JPG" || ex == ".JPEG" || ex == ".PNG" || ex == ".GIF")
            {
                fileNames.emplace_back(codec->toUnicode(i.path().filename().string().c_str()));
                count++;
                if(count >= 300){    // とりあえず300ファイルで切り上げる
                    break;
                }
            }
        }
    }
}
void MainClass::folderSearchSlot() {
    //DEBUG_LOG("%s\n", folderName.c_str());
    invokeFileNames(mpObj, folderName);
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
    DEBUG_LOG("FolderThread run() start id:%d\n", QThread::currentThreadId());

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
    DEBUG_LOG("Shufflethread run() start id:%d\n", QThread::currentThreadId());

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

    QImage image(static_cast<QString>(folderName) + "\\" + id);
    int height = image.height();
    int width = image.width();
    if(height > 800 || width > 1580)
    {
        QImage img = image.scaled(1580, 800, Qt::KeepAspectRatio);
        return img;
    }
    return image;
}
QImage ThumbnailProvider::requestImage(const QString &id, QSize *size, const QSize& requestedSize){

    QImage image(static_cast<QString>(folderName) + "\\" + id);
    int height = image.height();
    int width = image.width();
    if(height > 90 || width > 160)
    {
        QImage img = image.scaled(160, 90, Qt::KeepAspectRatio);
        return img;
    }
    return image;
}
