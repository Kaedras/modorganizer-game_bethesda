#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T15:36:33
#
#-------------------------------------------------


TARGET = gameSkyrim
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll
DEFINES += GAMESKYRIM_LIBRARY

SOURCES += gameskyrim.cpp \
    skyrimbsainvalidation.cpp \
    skyrimscriptextender.cpp \
    skyrimdataarchives.cpp \
    skyrimsavegame.cpp \
    skyrimsavegameinfo.cpp

HEADERS += gameskyrim.h \
    skyrimbsainvalidation.h \
    skyrimscriptextender.h \
    skyrimdataarchives.h \
    skyrimsavegame.h \
    skyrimsavegameinfo.h

CONFIG(debug, debug|release) {
  LIBS += -L"$${OUT_PWD}/../gameGamebryo/debug"
  PRE_TARGETDEPS += \
    $$OUT_PWD/../gameGamebryo/debug/gameGamebryo.lib
} else {
  LIBS += -L"$${OUT_PWD}/../gameGamebryo/release"
  PRE_TARGETDEPS += \
    $$OUT_PWD/../gameGamebryo/release/gameGamebryo.lib
}

include(../plugin_template.pri)

INCLUDEPATH += "$${BOOSTPATH}" "$${PWD}/../gamefeatures" "$${PWD}/../gamegamebryo"

LIBS += -ladvapi32 -lole32 -lgameGamebryo

OTHER_FILES += \
    gameskyrim.json\
    SConscript \
    CMakeLists.txt

