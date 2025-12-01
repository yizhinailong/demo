add_rules("mode.debug", "mode.release")

add_requires("boost", {
    configs = {
        cmake = false
    }
})

includes("boost_*")

add_includedirs("utils")

set_rundir("$(projectdir)")
