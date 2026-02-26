#pragma once

#include <QObject>

class CppWorker : public QObject {
    Q_OBJECT

public:
    explicit CppWorker(QObject* parent = nullptr);

    // Q_INVOKABLE
    Q_INVOKABLE void RegisterMethod();
    Q_INVOKABLE QString RegisterMethodWithReturn(QString name, int age);

public slots:
    void CppSlot();
};
