#include "../gamestarfield.h"

#include <executableinfo.h>
#include <scriptextender.h>

#include <QFileInfo>
#include <QList>
#include <QString>

using namespace MOBase;

QList<ExecutableInfo> GameStarfield::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("SFSE",
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Starfield", findInGameFolder(binaryName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
                .withSteamAppId("2722710")
                .withSteamAPI()
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Starfield\"");
}
