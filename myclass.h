#ifndef MYCLASS_H
#define MYCLASS_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickView>
#include <QThread>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>  // テキストファイル読み込み・書き込み
#include <cstdio>   // sprintf_s() sscanf_s()
#include <time.h>

#define DEBUG_LOG(fmt, ...) printf("[%s] %05d: "fmt, __FILE__, __LINE__, __VA_ARGS__);fflush(stdout)

class MyClass : public QObject
{
    Q_OBJECT
public:
    explicit MyClass(QGuiApplication& app, QObject* parent=0);
    void getFiles(const std::string& st, std::vector<std::string>& vec);
    ~MyClass(){
        DEBUG_LOG("MyClass end\n");
    }

public slots:
    void mainToSub1ButtonSlot();

private:
    QQmlApplicationEngine   mEngine;
    QQuickWindow*           mpQmlWindow;
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
