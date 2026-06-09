add_rules("mode.debug", "mode.release")

set_languages("c++26")
set_rundir("$(projectdir)")
set_policy("build.c++.modules", true)

add_requires(
    "opencv 4.13.0", {
        configs = {
            ffmpeg = false,
        }
    },
    "nlohmann_json"
)

target(
    "xmake-opencv",
    function ()
        set_kind("binary")

        add_files("src/*.cpp")

        add_packages("opencv", "nlohmann_json")
    end
)
