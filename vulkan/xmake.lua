add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"})

-- 添加 Vulkan 核心依赖
add_requires("vulkan-headers", "vulkan-loader")
-- 添加窗口系统依赖（Vulkan 需要窗口来呈现画面）
add_requires("glfw")

target("test")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("vulkan-headers", "vulkan-loader", "glfw")
    set_languages("c++17")

    before_build(function (target)
        local shader_dir = path.join(target:targetdir(), "shaders")
        os.mkdir(shader_dir)
        for _, name in ipairs({ "triangle.vert.spv", "triangle.frag.spv" }) do
            os.cp(path.join(os.projectdir(), "shaders", name), shader_dir)
        end
    end)

    on_clean(function (target)
        os.tryrm(path.join(target:targetdir(), "shaders"))
    end)
