add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"})

add_requires("boost", {
    configs = {
        cmake = false
    }
})

includes("boost_*")

add_includedirs("utils")

set_rundir("$(projectdir)")
