--------------------------------------------------
project "application"
    kind "WindowedApp"
    language "C++"

	targetdir "../build/bin/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}"
	objdir "../build/obj/%{cfg.system}/%{cfg.prjname}"
	
	libdirs { "../libs/opengles/lib" }	
	includedirs {
		"include",
		"../libs/glfw/include",
		"../libs/imgui/include",
		"../libs/ofx/include",
	}

    files { "**.h", "**.cpp" }
	links { "ofx", "glfw", "imgui" }

	filter {"system:linux"}
		links { "pthread", "rt", "dl", "X11" }

	filter {"platforms:arm*"}
		links { "EGL", "GLESv2" }

	group "libs"
		include "../libs/glfw"
		include "../libs/imgui"
		include "../libs/ofx"
	group "" -- end of "Dependensies"
--------------------------------------------------