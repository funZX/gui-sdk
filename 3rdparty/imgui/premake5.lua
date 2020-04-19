--------------------------------------------------
project "imgui"
    kind "StaticLib"
    language "C++"

	targetdir "../../build/bin/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}"
	objdir "../../build/obj/%{cfg.system}/%{cfg.prjname}"
	includedirs { "include" }
	
    files { "**.h", "**.cpp" }
--------------------------------------------------	

