workspace "bfjit"
    configurations { "debug", "release" }

project "bfjit"
    language "C++"
    kind "ConsoleApp"

    files {
	"src/**.cpp",
	"include/**.h",
    }

    includedirs { "include" }

    filter "configurations:debug"
	defines { "DEBUG" }
	symbols "On"

    filter "configurations:release"
	defines { "NDEBUG" }
	optimize "On"
