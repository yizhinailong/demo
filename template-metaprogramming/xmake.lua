add_rules("mode.debug", "mode.release")

set_languages("c++26")
set_policy("build.c++.modules", true)

local all_targets = {}

function create_targets_for_cpp_files(directory)
    for _, file in ipairs(os.files(directory .. "/*.cpp")) do
        local name = path.basename(file)
        local target_name = directory .. "-" .. name
        table.insert(all_targets, target_name)
        target(target_name)
            set_kind("binary")
            set_default(false)
            add_files(directory .. "/" .. name .. ".cpp")
    end
end

create_targets_for_cpp_files("1.Introduction")
create_targets_for_cpp_files("2.TemplateFundamentals")

table.sort(all_targets)
if #all_targets > 0 then
    target(all_targets[#all_targets])
        set_default(true)
end
