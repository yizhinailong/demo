#pragma once

#include <drogon/HttpController.h>

class HelloController : public drogon::HttpController<HelloController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(HelloController::hello, "/hello", drogon::Get);
    METHOD_LIST_END

    using Callback = std::function<void(const drogon::HttpResponsePtr&)>;

    void hello(const drogon::HttpRequestPtr& req, Callback&& callback);
};
