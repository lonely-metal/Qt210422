#ifndef MYCLASS_H
#define MYCLASS_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickView>
#include <QThread>
#include <QTimer>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>  // テキストファイル読み込み・書き込み
#include <cstdio>   // sprintf_s() sscanf_s()
#include <time.h>

#define DEBUG_LOG(fmt, ...) printf("[%s] %05d: "fmt, __FILE__, __LINE__, __VA_ARGS__);fflush(stdout)

class MyThread;
class ShuffleThread;

class MyClass : public QObject
{
    Q_OBJECT
public:
    explicit MyClass(QObject* parent=0);
    ~MyClass();
    void initialize(QGuiApplication& app);
    void invokeFileNames(QObject* object);
    void getFiles(const std::string& st, std::vector<std::string>& vec);
    QObject* getMyObject();
    void setMyObject(QObject* obj);

public slots:
    void mainToSub1ButtonSlot();
    void quitButtonSlot();
    void setShuffleWaitTimeSlot(bool shuffleComboVisible, int shuffleWaitTime);
    void setShuffleWaitTimeRestartSlot(int shuffleWaitTime);
    void sleepSlot(int msec);

    void folderSearchSlot();
    void shufflePlaySlot();

private:
    QGuiApplication*            mpApp;
    QQmlApplicationEngine       mEngine;
    QQuickWindow*               mpQmlWindow;
    QObject*                    mpObj;
    std::unique_ptr<MyThread>       mythread;
    std::unique_ptr<ShuffleThread>  shufflethread;
};

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject* parent=0);
    void initialize(MyClass* myclass);
    void run();

private:
    std::shared_ptr<MyClass>    mpMyclass;
};

class ShuffleThread : public QThread
{
    Q_OBJECT
public:
    explicit ShuffleThread(QObject* parent=0);
    void initialize(MyClass* myclass);
    void run();
    void setWaitTime(int waitTime);

private:
    std::shared_ptr<MyClass>    mpMyclass;
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

#endif // MYCLASS_H
