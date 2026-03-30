#include "../gamenehrim.h"

#include <executableinfo.h>
#include <gamebryogameplugins.h>
#include <gamebryolocalsavegames.h>

#include <QFileInfo>

using namespace MOBase;

QList<ExecutableInfo> GameNehrim::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("Nehrim", findInGameFolder("Oblivion.exe"))
         << ExecutableInfo("Nehrim Launcher", findInGameFolder("NehrimLauncher.exe"))
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
  QString path     = "Software\\Bethesda Softworks\\Oblivion";
  QString location = findInRegistry(HKEY_LOCAL_MACHINE, path.toStdWString().c_str(),
                                    L"Installed Path");

  if (!location.isEmpty()) {
    location.append("/NehrimFiles");
  }
  return location;
}
