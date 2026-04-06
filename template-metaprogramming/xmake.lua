add_rules("mode.debug", "mode.release")

set_languages("c++26")
set_policy("build.c++.modules", true)

function create_targets_for_cpp_files(directory)
    for _, file in ipairs(os.files(directory .. "/*.cpp")) do
        local name = path.basename(file)
        target(directory .. "-" .. name)
            set_kind("binary")
            add_files(directory .. "/" .. name .. ".cpp")
    end
end

create_targets_for_cpp_files("1.Introduction")
create_targets_for_cpp_files("2.TemplateFundamentals")
