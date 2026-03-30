#include "../gameenderalse.h"

#include <executableinfo.h>
#include <scriptextender.h>
#include <steamutility.h>

using namespace MOBase;

QString GameEnderalSE::identifyGamePath() const
{
  return findSteamGame("Enderal Special Edition",
                       "Data/Enderal - Forgotten Stories.esm");
}

QList<ExecutableInfo> GameEnderalSE::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("Enderal Special Edition (SKSE)",
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Enderal Special Edition Launcher",
                           findInGameFolder(getLauncherName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
                .withSteamAppId("1946180")
                .withSteamAPI()
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Enderal Special Edition\"");
}
