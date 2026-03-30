#include "../gameskyrimse.h"

#include <creationgameplugins.h>
#include <executableinfo.h>
#include <scriptextender.h>
#include <steamutility.h>

#include <QFileInfo>
#include <QList>
#include <QString>

using namespace MOBase;

QString GameSkyrimSE::identifyGamePath() const
{
  QString result = findSteamGame(gameName(), "Data/Skyrim.esm");
  // Check Epic Games Manifests
  // AppName: ac82db5035584c7f8a2c548d98c86b2c
  //      AE Update: 5d600e4f59974aeba0259c7734134e27
  if (result.isEmpty()) {
    result = parseEpicGamesLocation(
        {"ac82db5035584c7f8a2c548d98c86b2c", "5d600e4f59974aeba0259c7734134e27"});
  }

  return result;
}

QList<ExecutableInfo> GameSkyrimSE::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("SKSE",
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Skyrim Special Edition", findInGameFolder(binaryName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Skyrim Special Edition Launcher",
                           findInGameFolder(getLauncherName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
                .withSteamAppId("1946180")
                .withSteamAPI()
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Skyrim Special Edition\"");
}
