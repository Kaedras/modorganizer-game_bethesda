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
         << ExecutableInfo("Fallout 3", findInGameFolder(binaryName()))
         << ExecutableInfo("Fallout Mod Manager", findInGameFolder("fomm/fomm.exe"))
         << ExecutableInfo("Construction Kit", findInGameFolder("geck.exe"))
         << ExecutableInfo("Fallout Launcher", findInGameFolder(getLauncherName()))
         << ExecutableInfo("BOSS", findInGameFolder("BOSS/BOSS.exe"))
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Fallout3\"");
}
