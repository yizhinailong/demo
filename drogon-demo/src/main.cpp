#include <print>

#include <drogon/drogon.h>

using Callback = std::function<void(const drogon::HttpResponsePtr&)>;

auto main() -> int {
    trantor::Logger::setLogLevel(trantor::Logger::kTrace);

    std::println("Drogon version: {}", drogon::getVersion());
    std::println("Web server is listening on http://127.0.0.1:8080");
    std::println("The index page is http://127.0.0.1:8080/hello");
    std::println("Drogon is running...");

    auto Hello = [](const drogon::HttpRequestPtr& req, Callback&& callback) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setBody("Hello with PostgreSQL support!");
        resp->setContentTypeCode(drogon::CT_TEXT_PLAIN);
        std::move(callback)(resp);
    };

    drogon::app().registerHandler("/hello2", Hello);

    drogon::app().registerHandler(
        "/hello",
        [](const drogon::HttpRequestPtr& req, Callback&& callback) {
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setBody("Hello with PostgreSQL support!");
            resp->setContentTypeCode(drogon::CT_TEXT_PLAIN);
            std::move(callback)(resp);
        });

    drogon::app()
        .setLogPath("./")
        .setLogLevel(trantor::Logger::kInfo)
        .setUploadPath("./build/upload")
        .addListener("0.0.0.0", 8080)
        .setThreadNum(4)
        .run();

    return 0;
}
