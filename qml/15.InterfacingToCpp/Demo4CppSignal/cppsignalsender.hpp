#pragma once

#include <QObject>
#include <QTimer>

#include <QtQml/qqml.h>

class CppSignalSender : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit CppSignalSender(QObject* parent = nullptr);

    static CppSignalSender* create(QQmlEngine* qmlEngine, QJSEngine* jsEngine);

signals:
    void callQml(QString paramter);
    void cppTimer(QString value);

public slots:
    void CppSlot();

private:
    QTimer m_timer;
    int m_value;
};
