#include "../gamemorrowind.h"

using namespace MOBase;

QList<ExecutableInfo> GameMorrowind::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("MWSE (Launcher Method)",
                           findInGameFolder("MWSE Launcher.exe"))
         << ExecutableInfo("Morrowind", findInGameFolder(binaryName()))
         << ExecutableInfo("Morrowind Launcher", findInGameFolder(getLauncherName()))
         << ExecutableInfo("MGE XE", findInGameFolder("MGEXEgui.exe"))
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Morrowind\"");
}
