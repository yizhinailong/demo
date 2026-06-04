add_rules("mode.debug", "mode.release")
add_requires("imgui", {configs = {glfw_opengl3 = true}})
add_requires("glad")

target("xmake-imgui")
    set_kind("binary")
    set_languages("c++17")
    add_packages("imgui", "glad")
    add_files("src/*.cpp")
