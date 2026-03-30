#include "../gameenderal.h"

#include "executableinfo.h"
#include "steamutility.h"
#include "utility.h"

#include <QFileInfo>

using namespace MOBase;

QList<ExecutableInfo> GameEnderal::executables() const
{
  return QList<ExecutableInfo>()
         //<< ExecutableInfo("SKSE",
         // findInGameFolder(feature<ScriptExtender>()->loaderName()))
         //<< ExecutableInfo("SBW", findInGameFolder("SBW.exe"))
         << ExecutableInfo("Enderal (SKSE)", findInGameFolder(binaryName()))
         << ExecutableInfo("Enderal Launcher", findInGameFolder(getLauncherName()))
         << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Enderal\"")
         << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
                .withSteamAppId("202480");
}
