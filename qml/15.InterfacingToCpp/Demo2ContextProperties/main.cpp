#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "cppworker.hpp"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    // Create the object on the C++ side
    CppWorker cppWorker;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("CppWorker", &cppWorker);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
    );
    engine.loadFromModule("Demo2ContextProperties", "Main");

    return app.exec();
}
