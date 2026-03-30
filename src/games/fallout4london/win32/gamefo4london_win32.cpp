#include "../gamefo4london.h"

#include <creationgameplugins.h>
#include <executableinfo.h>
#include <scriptextender.h>

#include <QFileInfo>
#include <QList>
#include <QString>

#include <memory>

using namespace MOBase;

QString GameFallout4London::identifyGamePath() const
{
  // TODO: Add GOG support
  QString path = "Software\\Bethesda Softworks\\Fallout4";
  return findInRegistry(HKEY_LOCAL_MACHINE, path.toStdWString().c_str(),
                        L"Installed Path");
}

QList<ExecutableInfo> GameFallout4London::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("F4SE",
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
         << ExecutableInfo("Fallout 4 London", findInGameFolder(binaryName()))
         << ExecutableInfo("Fallout Launcher", findInGameFolder(getLauncherName()))
         << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
                .withSteamAppId("1946160")
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Fallout4\"");
}
