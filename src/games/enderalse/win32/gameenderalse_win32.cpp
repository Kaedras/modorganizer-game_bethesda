#include "../gameenderalse.h"

#include "steamutility.h"

#include "versioninfo.h"
#include <executableinfo.h>
#include <gamebryosavegameinfo.h>
#include <ipluginlist.h>
#include <pluginsetting.h>
#include <utility.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

using namespace MOBase;

QString GameEnderalSE::identifyGamePath() const
{
  QMap<QString, QString> paths = {
      {"Software\\Bethesda Softworks\\" + gameName(), "Installed Path"},
      {"Software\\GOG.com\\Games\\1708684988", "path"},
  };
  QString result;
  try {
    for (auto& path : paths.toStdMap()) {
      result = findInRegistry(HKEY_LOCAL_MACHINE, path.first.toStdWString().c_str(),
                              path.second.toStdWString().c_str());
      if (!result.isEmpty())
        break;
    }
  } catch (MOBase::MyException) {
    result = MOBase::findSteamGame("Enderal Special Edition",
                                   "Data/Enderal - Forgotten Stories.esm");
  }
  return result;
}

QList<ExecutableInfo> GameEnderalSE::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo("Enderal Special Edition (SKSE)",
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
         << ExecutableInfo("Enderal Special Edition Launcher",
                           findInGameFolder(getLauncherName()))
         << ExecutableInfo("Creation Kit", findInGameFolder("CreationKit.exe"))
                .withSteamAppId("1946180")
         << ExecutableInfo("LOOT", QFileInfo(getLootPath()))
                .withArgument("--game=\"Enderal Special Edition\"");
}
