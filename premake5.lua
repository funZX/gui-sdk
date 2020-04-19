--------------------------------------------------
local action  	= _ACTION
local options 	= _OPTIONS
local cfg		= premake.config
--------------------------------------------------
if action == "clean" then
	os.rmdir("build")
	os.exit(1)
end
--------------------------------------------------
if action == "gmake" or action == "ninja" then
	if nil == options["cc"] then
		print("Choose a C/C++ compiler set.")
		os.exit(1)
	end
end
--------------------------------------------------
workspace "gui-sdk"
	location "build/prj"
	characterset "MBCS"
	staticruntime "On"
	omitframepointer "On"
	
    configurations { "Debug", "Release" }
	filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"
--------------------------------------------------
filter {"system:windows"}
	platforms { "x64" }
	defines { "TARGET_OS_WINDOWS", "WIN32_LEAN_AND_MEAN" }
	entrypoint "mainCRTStartup"
filter {"system:linux"}
	platforms { "x64", "arm32", "arm64" }
	defines { "TARGET_OS_LINUX" }
--------------------------------------------------	
filter {"platforms:x64"}
	architecture "x86_64"
	
filter {"platforms:arm32"}
	architecture "ARM"
	--gccprefix "armv7a-linux-gnueabihf-"
	
filter {"platforms:arm64"}
	architecture "ARM64"
	--gccprefix "aarch64-linux-gnueabi-"
--------------------------------------------------
dofile("3rdparty/glfw/premake5.lua")
dofile("3rdparty/imgui/premake5.lua")
dofile("gui-sdk/premake5.lua")
--------------------------------------------------