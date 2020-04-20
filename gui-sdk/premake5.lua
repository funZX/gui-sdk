--------------------------------------------------
project "gui-sdk"
    kind "WindowedApp"
    language "C++"

	targetdir "../build/bin/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}"
	objdir "../build/obj/%{cfg.system}/%{cfg.prjname}"
	
	libdirs { "../3rdparty/opengles/lib" }	
	includedirs {
		"include",
		"../3rdparty/glfw/include",
		"../3rdparty/imgui/include",
	}

    files { "**.h", "**.cpp" }
	links { "glfw", "imgui" }

	filter {"system:linux"}
		links { "pthread", "rt", "dl", "X11" }

	filter {"platforms:arm*"}
		links { "EGL", "GLESv2" }

--------------------------------------------------