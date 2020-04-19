--------------------------------------------------
project "glfw"
    kind "StaticLib"
    language "C++"

	targetdir "../../build/bin/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}"
	objdir "../../build/obj/%{cfg.system}/%{cfg.prjname}"
	includedirs { "include" }
	
	filter { "system:*" }
		files {
			"include/**.h",
			"src/internal.h",
			"src/mappings.h",
			"src/context.c",
			"src/init.c",
			"src/input.c",
			"src/monitor.c",
			"src/vulkan.c",
			"src/window.c",
			"src/osmesa_context.h",
			"src/osmesa_context.c",
			"src/egl_context.h",
			"src/egl_context.c",
		}
		
	filter { "system:windows" }
		defines { "_GLFW_WIN32" }
		files {
			"src/win32_joystick.h",
			"src/win32_joystick.c",
			"src/win32_platform.h",
			"src/wgl_context.h",
			"src/win32_init.c",
			"src/win32_monitor.c",
			"src/win32_time.c",
			"src/win32_thread.c",
			"src/win32_window.c",
			"src/wgl_context.c",
		}		
		
	filter { "system:linux" }
		defines { "_GLFW_X11" }
		files {
			"src/linux_joystick.h",
			"src/linux_joystick.c",
			"src/x11_platform.h",
			"src/xkb_unicode.h",
			"src/posix_time.h",
			"src/posix_thread.h",
			"src/glx_context.h",
			"src/x11_init.c",
			"src/x11_monitor.c",
			"src/x11_window.c",
			"src/xkb_unicode.c",
			"src/posix_time.c",
			"src/posix_thread.c",
			"src/glx_context.c",
		}
		
--------------------------------------------------

