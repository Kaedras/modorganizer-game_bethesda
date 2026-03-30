#include "../gameoblivion.h"

#include <QFileInfo>
#include <executableinfo.h>
#include <gamebryogameplugins.h>
#include <gamebryolocalsavegames.h>

using namespace MOBase;

QList<ExecutableInfo> GameOblivion::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("Oblivion", findInGameFolder(binaryName()))
         << ExecutableInfo("Oblivion Launcher", findInGameFolder(getLauncherName()))
         << ExecutableInfo("Oblivion Mod Manager",
                           findInGameFolder("OblivionModManager.exe"))
         << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Oblivion\"")
         << ExecutableInfo("Construction Set",
                           findInGameFolder("TESConstructionSet.exe"));
}
