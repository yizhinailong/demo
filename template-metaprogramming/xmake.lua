add_rules("mode.debug", "mode.release")

set_languages("c++26")
set_policy("build.c++.modules", true)

function create_targets_for_cpp_files(directory)
    for _, file in ipairs(os.files(directory .. "/*.cpp")) do
        local name = path.basename(file)
        target(name)
            set_kind("binary")
            add_files(directory .. "/" .. name .. ".cpp")
    end
end

create_targets_for_cpp_files("1.Introduction")
create_targets_for_cpp_files("2.TemplateFundamentals")
create_targets_for_cpp_files("3.VariadicTemplates")
create_targets_for_cpp_files("4.AdvancedTemplateConcepts")
create_targets_for_cpp_files("5.TypeTraitsConditionalCompilation")
create_targets_for_cpp_files("6.ConceptsConstraints")
create_targets_for_cpp_files("7.PatternsIdioms")
create_targets_for_cpp_files("8.RangesAlgorithms")
create_targets_for_cpp_files("9.Ranges")
