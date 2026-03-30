#include "../gamefallout3.h"

#include <executableinfo.h>
#include <gamebryogameplugins.h>
#include <scriptextender.h>

#include <QFileInfo>
#include <QList>
#include <QString>

using namespace MOBase;

QList<ExecutableInfo> GameFallout3::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("FOSE",
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Fallout 3", findInGameFolder(binaryName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("Fallout Mod Manager", findInGameFolder("fomm/fomm.exe"))
         << ExecutableInfo("Construction Kit", findInGameFolder("geck.exe"))
         << ExecutableInfo("Fallout Launcher", findInGameFolder(getLauncherName()))
                .withSteamAPI()
                .withSteamOverlay()
         << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Fallout3\"");
}
