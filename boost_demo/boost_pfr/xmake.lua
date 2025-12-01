target(
    "boost_pfr",
    function()
        set_kind("binary")
        set_languages("c++23")
        set_policy("build.c++.modules", true)

        add_files("boost_pfr.cpp")

        add_packages("boost")
    end
)
