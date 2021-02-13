--------------------------------------------------
project "ofx"
    kind "StaticLib"
    language "C++"

	targetdir "../../build/bin/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}"
	objdir "../../build/obj/%{cfg.system}/%{cfg.prjname}"
	includedirs {
		"../glfw/include",
		"../imgui/include",
		"include",
	}
	
    files { "**.h", "**.cpp" }
	
	dependson {"glfw", "imgui"}
--------------------------------------------------	

