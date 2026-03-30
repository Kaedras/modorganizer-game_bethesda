cmake_minimum_required(VERSION 3.16)

if (UNIX)
	set(os_name linux)
else()
	set(os_name win32)
endif()

add_library(game_morrowind SHARED
	gamemorrowind.cpp
	${os_name}/gamemorrowind_${os_name}.cpp
	gamemorrowind.h
	morrowindbsainvalidation.cpp
	morrowindbsainvalidation.h
	morrowinddataarchives.cpp
	morrowinddataarchives.h
	morrowindgameplugins.cpp
	morrowindgameplugins.h
	morrowindlocalsavegames.cpp
	morrowindlocalsavegames.h
	morrowindmoddatachecker.h
	morrowindmoddatacontent.h
	morrowindsavegame.cpp
	morrowindsavegame.h
	morrowindsavegameinfo.cpp
	morrowindsavegameinfo.h
	morrowindsavegameinfowidget.cpp
	morrowindsavegameinfowidget.h
)
mo2_configure_plugin(game_morrowind NO_SOURCES WARNINGS 4)
mo2_default_source_group()
target_link_libraries(game_morrowind PRIVATE game_gamebryo)
mo2_install_plugin(game_morrowind)
