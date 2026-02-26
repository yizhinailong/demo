#include "cppsignalsender.hpp"

CppSignalSender::CppSignalSender(QObject* parent)
    : QObject{ parent },
      m_timer(new QTimer(this)),
      m_value(0) {

    connect(&m_timer, &QTimer::timeout, [=]() {
        m_value++;
        emit cppTimer(QString::number(m_value));
    });

    m_timer.start(1000);
}

CppSignalSender* CppSignalSender::create(QQmlEngine* qmlEngine, QJSEngine* jsEngine) {
    Q_UNUSED(qmlEngine)
    Q_UNUSED(jsEngine)
    static CppSignalSender instance;
    return &instance;
}

void CppSignalSender::CppSlot() {
    emit callQml("Information from C++");
}
