#include "cppworker.hpp"

#include <QDebug>

CppWorker::CppWorker(QObject* parent)
    : QObject{ parent } {
}

void CppWorker::RegisterMethod() {
    qDebug() << "This is C++ talking, RegisterMethod is Called";
}

QString CppWorker::RegisterMethodWithReturn(QString name, int age) {
    qDebug() << "This is C++ talking, name: " << name << " age: " << age;
    return QString{ name + ": " + QString::number(age) + " years" };
}

void CppWorker::CppSlot() {
    qDebug() << "This is talking, CppSlot is Called";
}
