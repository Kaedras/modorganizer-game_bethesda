#include "../gamefallout4.h"

#include <creationgameplugins.h>
#include <executableinfo.h>
#include <scriptextender.h>

#include <QFileInfo>
#include <QList>
#include <QString>

using namespace MOBase;

QList<ExecutableInfo> GameFallout4::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("F4SE",
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
         << ExecutableInfo("Fallout 4", findInGameFolder(binaryName()))
         << ExecutableInfo("Fallout Launcher", findInGameFolder(getLauncherName()))
         << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
                .withSteamAppId("1946160")
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Fallout4\"");
}
