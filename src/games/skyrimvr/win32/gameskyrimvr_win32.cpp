#include "../gameskyrimvr.h"

#include <executableinfo.h>
#include <scriptextender.h>

#include <QFileInfo>
#include <QList>
#include <QString>

using namespace MOBase;

QString GameSkyrimVR::identifyGamePath() const
{
  QString path = "Software\\Bethesda Softworks\\" + gameName();
  return findInRegistry(HKEY_LOCAL_MACHINE, path.toStdWString().c_str(),
                        L"Installed Path");
}

QList<ExecutableInfo> GameSkyrimVR::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("SKSE",
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
         << ExecutableInfo("Skyrim VR", findInGameFolder(binaryName()))
         << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Skyrim VR\"");
}
