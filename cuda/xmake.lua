add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", { outputdir = "build" })

set_rundir("$(projectdir)")

set_runtimes(is_mode("debug") and "MDd" or "MD")
if is_plat("windows") then
	set_warnings("all", "extra")
end

target("cuda_vector_add_cuda", function()
	set_kind("static")

	set_languages("c++20")
	set_values("cuda.rdc", false)

	add_files("src/cuda_vector_add.cu")
	add_includedirs("src")
	add_cugencodes("native")
end)

target("cuda_vector_add", function()
	set_kind("binary")

	set_languages("c++23")

	add_files("src/main.cpp")
	add_includedirs("src")
	add_deps("cuda_vector_add_cuda")
end)
