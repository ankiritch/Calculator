workspace "Calculator"
    configurations { "Debug", "Release" }
    targetdir "bin/%{cfg.buildcfg}"

project "Calculator"
    filter {}
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        toolset "msc"
        buildoptions { 
            "/W4",
            "/WX",
            "/EHsc",
        }

        files { 
            "src/**.cpp",
            "src/**.h",
            "src/**.hpp"
        }

        links { } -- link external libraries
        libdirs { "libs" } -- directories for libraries

    filter "configurations:Debug"
        defines { "DEBUG" } -- define preprocessor symbol
        symbols "On"

    filter "configurations:Release"
        buildoptions { "-O2" }
        defines { "NDEBUG" }
        optimize "On"
