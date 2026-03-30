cmake_minimum_required(VERSION 3.16)

if (UNIX)
	set(os_name linux)
else()
	set(os_name win32)
endif()

add_library(game_fallout3 SHARED
	fallout3bsainvalidation.cpp
	fallout3bsainvalidation.h
	fallout3dataarchives.cpp
	fallout3dataarchives.h
	fallout3moddatachecker.h
	fallout3moddatacontent.h
	fallout3savegame.cpp
	fallout3savegame.h
	fallout3scriptextender.cpp
	fallout3scriptextender.h
	gamefallout3.cpp
	${os_name}/gamefallout3_${os_name}.cpp
	gamefallout3.h
)
mo2_configure_plugin(game_fallout3 NO_SOURCES WARNINGS 4)
mo2_default_source_group()
target_link_libraries(game_fallout3 PRIVATE game_gamebryo)
mo2_install_plugin(game_fallout3)
