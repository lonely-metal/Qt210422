#include "myclass.h"

std::string folderName {"E:\\work\\dvlp\\Qt\\images"};  // ２つ以上のクラスで使用してるので、とりあえずここで定義

MyClass::MyClass(QGuiApplication& app, QObject* parent):QObject(parent)
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

    // 文字列型connect
    QObject::connect(mpQmlWindow, SIGNAL(mainToSub1ButtonSignal()),
                     this,   SLOT(mainToSub1ButtonSlot()));

    mEngine.addImageProvider(QLatin1String("imagedata"),new ImageProvider);
}
void MyClass::mainToSub1ButtonSlot() {
    DEBUG_LOG("\n");

    QObject* obj = qobject_cast<QObject*>(mEngine.rootObjects().first());

    std::vector<std::string>  vec;
    getFiles(folderName, vec);

    QVariantList qvlist;
    for(int i=0; i<vec.size(); i++){
        qvlist.push_back(vec[i].c_str());
    }
    QMetaObject::invokeMethod((QObject*)obj, "getFileNames",
            Q_ARG(QVariant, QVariant::fromValue(qvlist)));
}
void MyClass::getFiles(const std::string& st, std::vector<std::string>& vec){
    for (const std::filesystem::directory_entry& i : std::filesystem::directory_iterator(st)) {
        if (! i.is_directory()) {
            vec.emplace_back(i.path().filename().string());
        }
    }
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize& requestedSize){

    QImage image(static_cast<QString>(folderName.c_str()) + "\\" + id);
    return image;
}
