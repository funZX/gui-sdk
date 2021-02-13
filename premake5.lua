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
if action == "gmake" then
	if nil == options["cc"] then
		print("Choose a C/C++ compiler set.")
		os.exit(1)
	end
end
--------------------------------------------------
workspace "gui-sdk"
	location( "build/project/" .. _ACTION )
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
	buildoptions {"-fpermissive"}

filter {"platforms:arm*"}	
	defines { "TARGET_OS_LINUX_ARM" }

--------------------------------------------------	
filter {"platforms:x64"}
	architecture "x86_64"
	
filter {"platforms:arm32"}
	architecture "arm"
	--gccprefix "armv7a-linux-gnueabihf-"
	
filter {"platforms:arm64"}
	architecture "aarch64"
	--gccprefix "aarch64-linux-gnueabi-"
--------------------------------------------------

include("main/premake5.lua")
--------------------------------------------------
