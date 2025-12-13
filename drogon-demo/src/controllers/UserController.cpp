#include "UserController.hpp"

void UserController::GetUser(const drogon::HttpRequestPtr& req, Callback&& callback) {
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setBody("Hello World!");
    resp->addHeader("Content-Type", "text/plain");
    std::move(callback)(resp);
}
