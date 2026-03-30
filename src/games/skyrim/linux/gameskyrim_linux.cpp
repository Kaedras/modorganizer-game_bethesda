#include "../gameskyrim.h"

#include <executableinfo.h>
#include <gamebryogameplugins.h>
#include <scriptextender.h>

#include <QFileInfo>
#include <QVersionNumber>

using namespace MOBase;

QList<ExecutableInfo> GameSkyrim::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("SKSE",
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("SBW", findInGameFolder("SBW.exe"))
         << ExecutableInfo("Skyrim", findInGameFolder(binaryName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Skyrim Launcher", findInGameFolder(getLauncherName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Skyrim\"")
         << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
                .withSteamAppId("202480")
                .withSteamAPI();
}
