#include "cppworker.hpp"

#include <QDebug>
#include <QQmlEngine>

CppWorker::CppWorker(QObject* parent)
    : QObject{ parent } {
}

CppWorker* CppWorker::create(QQmlEngine* qmlEngine, QJSEngine* jsEngine) {
    Q_UNUSED(qmlEngine)
    Q_UNUSED(jsEngine)
    static CppWorker instance;
    return &instance;
}

void CppWorker::RegisterMethod() {
    qDebug() << "This is C++ talking, RegisterMethod is Called";
}

QString CppWorker::RegisterMethodWithReturn(QString name, int age) {
    qDebug() << "This is C++ talking, name: " << name << " age: " << age;
    return QString{ name + ": " + QString::number(age) + " years" };
}

void CppWorker::CppSlot() {
    qDebug() << "This is C++ talking, CppSlot is Called";
}
