#include "../gamenehrim.h"

#include <executableinfo.h>
#include <gamebryogameplugins.h>
#include <gamebryolocalsavegames.h>
#include <steamutility.h>

#include <QFileInfo>

using namespace MOBase;

QList<ExecutableInfo> GameNehrim::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("Nehrim", findInGameFolder("Oblivion.exe"))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Nehrim Launcher", findInGameFolder("NehrimLauncher.exe"))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Oblivion Mod Manager",
                           findInGameFolder("OblivionModManager.exe"))
         << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Nehrim\"")
         << ExecutableInfo("Construction Set",
                           findInGameFolder("TESConstructionSet.exe"));
}

QString GameNehrim::identifyGamePath() const
{
  QString location = findSteamGame("Nehrim", "NehrimFiles/Data/Nehrim.esm");

  if (!location.isEmpty()) {
    location.append("/NehrimFiles");
  }
  return location;
}
