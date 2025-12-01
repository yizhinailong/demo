#include <boost/pfr.hpp>

import std;

struct Person {
    std::string name;
    unsigned birth_year;
};

int main() {
    Person val{
        .name = "Edgar Allan Poe",
        .birth_year = 1809
    };

    std::println("Name: {}, Birth Year: {}", boost::pfr::get<0>(val), boost::pfr::get<1>(val));

    std::cout << boost::pfr::io(val) << std::endl;

    auto display = [](auto&& field) {
        std::println("Field: {}", field);
    };
    boost::pfr::for_each_field(val, display);

    return 0;
}
