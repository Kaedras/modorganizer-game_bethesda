cmake_minimum_required(VERSION 3.16)

if(WIN32)
	set(os_name win32)
else()
	set(os_name linux)
endif()

add_library(game_skyrimse SHARED
	gameskyrimse.cpp
	${os_name}/gameskyrimse_${os_name}.cpp
	gameskyrimse.h
	skyrimsedataarchives.cpp
	skyrimsedataarchives.h
	skyrimsemoddatachecker.h
	skyrimsemoddatacontent.h
	skyrimsesavegame.cpp
	skyrimsesavegame.h
	skyrimsescriptextender.cpp
	skyrimsescriptextender.h
	skyrimseunmanagedmods.cpp
	skyrimseunmanagedmods.h
)
mo2_configure_plugin(game_skyrimse NO_SOURCES WARNINGS 4)
mo2_default_source_group()
target_link_libraries(game_skyrimse PRIVATE game_creation)
mo2_install_plugin(game_skyrimse)
