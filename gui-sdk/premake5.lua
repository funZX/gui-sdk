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
		defines { "TARGET_OS_WINDOWS" }

	filter {"system:linux"}
		defines { "TARGET_OS_LINUX" }
		links { "pthread", "rt", "dl", "X11" }

	filter {"platforms:arm*"}
		defines { "TARGET_OS_LINUX_ARM" }
		links { "EGL", "GLESv2" }

--------------------------------------------------