add_rules("mode.debug", "mode.release")

set_languages("c++26")
set_rundir("$(projectdir)")
set_policy("build.c++.modules", true)

add_requires(
    "opencv", {
        configs = {
            ffmpeg = false,
        }
    }
)

target(
    "xmake-opencv",
    function ()
        set_kind("binary")

        add_files("src/*.cpp")

        add_packages("opencv")
    end
)
