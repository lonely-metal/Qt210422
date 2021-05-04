#ifndef MYCLASS_H
#define MYCLASS_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickView>
#include <QThread>
#include <QTimer>
#include <QTextCodec>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>  // テキストファイル読み込み・書き込み
#include <cstdio>   // sprintf_s() sscanf_s()
#include <time.h>

#define DEBUG_LOG(fmt, ...) printf("[%s] %05d: "fmt, __FILE__, __LINE__, __VA_ARGS__);fflush(stdout)

class FolderThread;
class ShuffleThread;

class MainClass : public QObject
{
    Q_OBJECT
public:
    explicit MainClass(QObject* parent=0);
    ~MainClass();
    void initialize(QGuiApplication& app);
    void invokeFolderNames(QObject* object, QString& foldername);
    void invokeFileNames(QObject* object, QString& folderame);
    void getFolders(const QString& folderName, std::vector<QString>& foldernames);
    void getFiles(const QString& folderName, std::vector<QString>& filenames);
    QObject* getMainclassObject();
    void setMainclassObject(QObject* obj);

public slots:
    void folderCheckSlot(QString msg1, QString msg2);
    void quitButtonSlot();
    void setShuffleWaitTimeSlot(bool shuffleComboVisible, int shuffleWaitTime);
    void setShuffleWaitTimeRestartSlot(int shuffleWaitTime);
    void sleepSlot(int msec);

    void folderSearchSlot();
    void shufflePlaySlot();

private:
    QGuiApplication*                mpApp;
    QQmlApplicationEngine           mEngine;
    QQuickWindow*                   mpQmlWindow;
    QObject*                        mpObj;
    std::unique_ptr<FolderThread>   folderThread;
    std::unique_ptr<ShuffleThread>  shuffleThread;
    //QTextCodec*                     mpCodec;
};

class FolderThread : public QThread
{
    Q_OBJECT
public:
    explicit FolderThread(QObject* parent=0);
    void initialize(MainClass* mainclass);
    void run();

private:
    std::shared_ptr<MainClass>    mpMainclass;
};

class ShuffleThread : public QThread
{
    Q_OBJECT
public:
    explicit ShuffleThread(QObject* parent=0);
    void initialize(MainClass* mainclass);
    void run();
    void setWaitTime(int waitTime);

private:
    std::shared_ptr<MainClass>  mpMainclass;
    int                         mWaitTime;
};

#include <QQuickImageProvider>

QT_BEGIN_NAMESPACE
class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider() :QQuickImageProvider(QQuickImageProvider::Image){}
    virtual QImage requestImage(const QString &id, QSize *size, const QSize& requestedSize);
};

QT_BEGIN_NAMESPACE
class ThumbnailProvider : public QQuickImageProvider
{
public:
    ThumbnailProvider() :QQuickImageProvider(QQuickImageProvider::Image){}
    virtual QImage requestImage(const QString &id, QSize *size, const QSize& requestedSize);
};

#endif // MYCLASS_H
