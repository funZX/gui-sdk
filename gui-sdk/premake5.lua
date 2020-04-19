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
		"../3rdparty/opengles/include",
	}

    files { "**.h", "**.cpp" }
	links { "glfw", "imgui" }

	filter {"system:windows"}
		links { "libEGL", "libGLESv2"}
	
	filter {"system:linux"}
		links { "pthread", "rt", "dl", "X11", "EGL", "GLESv2" }

--------------------------------------------------