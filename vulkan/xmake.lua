add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"})

-- 添加 Vulkan 核心依赖
add_requires("vulkan-headers", "vulkan-loader")
-- 添加窗口系统依赖（Vulkan 需要窗口来呈现画面）
add_requires("glfw")
-- 添加 Slang 着色器编译器
add_requires("slang")

target("test")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("vulkan-headers", "vulkan-loader", "glfw", "slang")
    set_languages("c++17")

    before_build(function (target)
        import("lib.detect.find_program")

        local shader_dir = path.join(target:targetdir(), "shaders")
        os.mkdir(shader_dir)

        local slangc = find_program("slangc", {
            paths = target:pkg("slang"):installdir("bin"),
            check = "-v",
        })
        assert(slangc, "slangc was not found in the slang package")
        local source = path.join(os.projectdir(), "shaders", "triangle.slang")

        for _, stage in ipairs({
            { name = "vertex", entry = "vertMain", output = "triangle.vert.spv" },
            { name = "fragment", entry = "fragMain", output = "triangle.frag.spv" },
        }) do
            os.execv(slangc, {
                source,
                "-target", "spirv",
                "-entry", stage.entry,
                "-stage", stage.name,
                "-fvk-use-entrypoint-name",
                -- xmake 仓库的 slang 包未启用 slang_glslang，缺少 slang-glslang.dll，
                -- 默认的 -O1 会调用其中的 spirv-opt 而失败，因此关闭优化。
                "-O0",
                "-o", path.join(shader_dir, stage.output),
            })
        end
    end)

    on_clean(function (target)
        os.tryrm(path.join(target:targetdir(), "shaders"))
    end)
