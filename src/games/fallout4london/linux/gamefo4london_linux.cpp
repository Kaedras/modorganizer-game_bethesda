#include "../gamefo4london.h"

#include <creationgameplugins.h>
#include <executableinfo.h>
#include <scriptextender.h>
#include <steamutility.h>

#include <QFileInfo>
#include <QList>
#include <QString>

using namespace MOBase;

QString GameFallout4London::identifyGamePath() const
{
  // TODO: Add GOG support
  return findSteamGame("Fallout 4", "Data/londonworldspace.esm");
}

QList<ExecutableInfo> GameFallout4London::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("F4SE",
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Fallout 4 London", findInGameFolder(binaryName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Fallout Launcher", findInGameFolder(getLauncherName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
                .withSteamAppId("1946160")
                .withSteamAPI()
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Fallout4\"");
}
