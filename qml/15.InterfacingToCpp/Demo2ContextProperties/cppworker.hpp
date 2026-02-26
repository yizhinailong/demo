#pragma once

#include <QObject>
#include <QtQml/qqml.h>

class CppWorker : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit CppWorker(QObject* parent = nullptr);

    static CppWorker* create(QQmlEngine* qmlEngine, QJSEngine* jsEngine);

    Q_INVOKABLE void RegisterMethod();
    Q_INVOKABLE QString RegisterMethodWithReturn(QString name, int age);

public slots:
    void CppSlot();
};
