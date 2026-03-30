#include "../gameskyrimvr.h"

#include <executableinfo.h>
#include <scriptextender.h>

#include <QFileInfo>
#include <QList>
#include <QString>

using namespace MOBase;

QString GameSkyrimVR::identifyGamePath() const
{
  return parseSteamLocation(steamAPPId(), gameShortName());
}

QList<ExecutableInfo> GameSkyrimVR::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("SKSE",
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Skyrim VR", findInGameFolder(binaryName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Skyrim VR\"");
}
