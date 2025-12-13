#include <print>

#include <drogon/drogon.h>

int main() {
    std::println("Drogon version: {}", drogon::getVersion());
    std::println("Web server is listening on http://127.0.0.1:8080");
    std::println("The index page is http://127.0.0.1:8080/hello");
    std::println("Drogon is running...");

    drogon::app().loadConfigFile("config.json").run();

    return 0;
}
