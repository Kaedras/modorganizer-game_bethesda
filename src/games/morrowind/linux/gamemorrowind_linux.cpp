#include "../gamemorrowind.h"

using namespace MOBase;

QList<ExecutableInfo> GameMorrowind::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("MWSE (Launcher Method)",
                           findInGameFolder("MWSE Launcher.exe"))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Morrowind", findInGameFolder(binaryName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Morrowind Launcher", findInGameFolder(getLauncherName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("MGE XE", findInGameFolder("MGEXEgui.exe"))
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Morrowind\"");
}
