#pragma once

#include <drogon/HttpController.h>

class UserController : public drogon::HttpController<UserController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(UserController::GetUser, "/getuser", drogon::Get);
    METHOD_LIST_END

    using Callback = std::function<void(const drogon::HttpResponsePtr&)>;

    void GetUser(const drogon::HttpRequestPtr& req, Callback&& callback);
};
