workspace "string_view"
    configurations { "Release", "Debug" }
    architecture "x64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "string_view_static"
	location "string_view_static"
    kind "StaticLib"
    language "C"
    staticruntime "on"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files { "src/string_view.c", "src/string_view.h" }


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"


project "string_view_shared"
	location "string_view_shared"
    kind "SharedLib"
    language "C"
    staticruntime "on"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files { "src/string_view.c", "src/string_view.h" }
	postBuildCommand { ("{COPY} %{cfg.buildtarget.relpath}../bin/"..outputdir.."test")}

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"


project "tests"
	loaction "tests"
	kind "ConsoleApp"
	language "C"
    staticruntime "on"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files { "test/test.c" }
	includedirs { "src" }
	links { "string_view"}

	filter "configurations:Debug"
	runtime "Debug"
	symbols "on"

	filter "configurations:Release"
	runtime "Release"
	optimize "on"