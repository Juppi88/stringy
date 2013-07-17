-- A generic C string library

project "Lib-Stringy"
	kind "StaticLib"
	language "C"
	files { "**.h", "**.c", "premake4.lua" }
	vpaths { [""] = { "../Libraries/Stringy" } }
	includedirs { "." }
	location ( "../../Projects/" .. os.get() .. "/" .. _ACTION )
	
	-- Linux specific stuff
	configuration "linux"
		targetextension ".a"
		configuration "Debug" targetname "libstringyd"
		configuration "Release" targetname "libstringy"
	
	-- Windows specific stuff
	configuration "windows"
		targetextension ".lib"
		configuration "Debug" targetname "stringyd"
		configuration "Release" targetname "stringy"
