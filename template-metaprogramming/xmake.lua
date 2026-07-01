add_rules("mode.debug", "mode.release")

set_languages("c++26")
set_policy("build.c++.modules", true)

local all_targets = {}

function create_targets_for_cpp_files(directory)
    for _, file in ipairs(os.files(directory .. "/*.cpp")) do
        local name = path.basename(file)
        table.insert(all_targets, target_name)
        target(name)
            set_kind("binary")
            set_default(false)
            add_files(directory .. "/" .. name .. ".cpp")
    end
end

create_targets_for_cpp_files("1.Introduction")
create_targets_for_cpp_files("2.TemplateFundamentals")
create_targets_for_cpp_files("3.VariadicTemplates")
create_targets_for_cpp_files("4.AdvancedTemplateConcepts")

table.sort(all_targets)

if #all_targets > 0 then
    local last_target = all_targets[#all_targets]
    target("all")
        set_kind("phony")
        set_default(true)
        for _, t in ipairs(all_targets) do
            add_deps(t)
        end
        on_run(function(target)
            import("core.project.project")
            local t = project.target(last_target)
            os.exec(t:targetfile())
        end)
end
