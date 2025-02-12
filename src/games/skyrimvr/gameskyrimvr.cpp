#include "gameskyrimvr.h"

#include "skyrimvrdataarchives.h"
#include "skyrimvrgameplugins.h"
#include "skyrimvrmoddatachecker.h"
#include "skyrimvrmoddatacontent.h"
#include "skyrimvrsavegame.h"
#include "skyrimvrscriptextender.h"
#include "skyrimvrunmanagedmods.h"

#include "versioninfo.h"
#include <executableinfo.h>
#include <gamebryolocalsavegames.h>
#include <gamebryosavegameinfo.h>
#include <pluginsetting.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

#include "scopeguard.h"
#include <memory>

using namespace MOBase;
using namespace Qt::Literals::StringLiterals;

GameSkyrimVR::GameSkyrimVR() {}

void GameSkyrimVR::setGamePath(const QString& path)
{
  m_GamePath = path;
}

QDir GameSkyrimVR::documentsDirectory() const
{
  return m_MyGamesPath;
}

QString GameSkyrimVR::identifyGamePath() const
{
  QString path = u"Software\\Bethesda Softworks\\"_s % gameName();
  return findInRegistry(HKEY_LOCAL_MACHINE, path.toStdWString().c_str(),
                        L"Installed Path");
}

QDir GameSkyrimVR::savesDirectory() const
{
  return QDir(m_MyGamesPath % u"/Saves"_s);
}

QString GameSkyrimVR::myGamesPath() const
{
  return m_MyGamesPath;
}

bool GameSkyrimVR::isInstalled() const
{
  return !m_GamePath.isEmpty();
}

bool GameSkyrimVR::init(IOrganizer* moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  registerFeature(std::make_shared<SkyrimVRScriptExtender>(this));
  registerFeature(std::make_shared<SkyrimVRDataArchives>(this));
  registerFeature(std::make_shared<GamebryoLocalSavegames>(this, u"SkyrimVR.ini"_s));
  registerFeature(std::make_shared<SkyrimVRModDataChecker>(this));
  registerFeature(std::make_shared<GamebryoSaveGameInfo>(this));
  registerFeature(
      std::make_shared<SkyrimVRModDataContent>(m_Organizer->gameFeatures()));
  registerFeature(std::make_shared<SkyrimVRGamePlugins>(moInfo));
  registerFeature(std::make_shared<SkyrimVRUnmangedMods>(this));

  return true;
}

QString GameSkyrimVR::gameName() const
{
  return u"Skyrim VR"_s;
}

QList<ExecutableInfo> GameSkyrimVR::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo(u"SKSE"_s,
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
         << ExecutableInfo(u"Skyrim VR"_s, findInGameFolder(binaryName()))
         << ExecutableInfo(u"Creation Kit"_s, findInGameFolder(u"CreationKit.exe"_s))
         << ExecutableInfo(u"LOOT"_s, QFileInfo(getLootPath()))
                .withArgument(u"--game=\"Skyrim VR\""_s);
}

QList<ExecutableForcedLoadSetting> GameSkyrimVR::executableForcedLoads() const
{
  return QList<ExecutableForcedLoadSetting>();
}

QFileInfo GameSkyrimVR::findInGameFolder(const QString& relativePath) const
{
  return QFileInfo(m_GamePath % '/' % relativePath);
}

QString GameSkyrimVR::name() const
{
  return u"Skyrim VR Support Plugin"_s;
}

QString GameSkyrimVR::localizedName() const
{
  return tr("Skyrim VR Support Plugin");
}

QString GameSkyrimVR::author() const
{
  return u"Brixified & MO2 Team"_s;
}

QString GameSkyrimVR::description() const
{
  return tr("Adds support for the game Skyrim VR.");
}

MOBase::VersionInfo GameSkyrimVR::version() const
{
  return VersionInfo(1, 5, 1, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameSkyrimVR::settings() const
{
  return {PluginSetting(u"enderal_downloads"_s, u"allow Enderal and Enderal SE downloads"_s,
                        QVariant(false))};
}

void GameSkyrimVR::initializeProfile(const QDir& path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() % u"/Skyrim VR"_s, path, u"plugins.txt"_s);
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS) ||
        !QFileInfo::exists(myGamesPath() % u"/SkyrimVR.ini"_s)) {
      copyToProfile(gameDirectory().absolutePath(), path, u"Skyrim.ini"_s, u"SkyrimVR.ini"_s);
    } else {
      copyToProfile(myGamesPath(), path, u"SkyrimVR.ini"_s);
    }

    copyToProfile(myGamesPath(), path, u"SkyrimPrefs.ini"_s);
  }
}

QString GameSkyrimVR::savegameExtension() const
{
  return u"ess"_s;
}

QString GameSkyrimVR::savegameSEExtension() const
{
  return u"skse"_s;
}

std::shared_ptr<const GamebryoSaveGame>
GameSkyrimVR::makeSaveGame(QString filePath) const
{
  return std::make_shared<const SkyrimVRSaveGame>(filePath, this);
}

QString GameSkyrimVR::steamAPPId() const
{
  return u"611670"_s;
}

QStringList GameSkyrimVR::primaryPlugins() const
{
  QStringList plugins = {u"Skyrim.esm"_s,      u"Update.esm"_s,     u"Dawnguard.esm"_s,
                         u"HearthFires.esm"_s, u"Dragonborn.esm"_s, u"SkyrimVR.esm"_s};

  plugins.append(CCPlugins());

  return plugins;
}

QStringList GameSkyrimVR::gameVariants() const
{
  return {u"Regular"_s};
}

QString GameSkyrimVR::gameShortName() const
{
  return u"SkyrimVR"_s;
}

QStringList GameSkyrimVR::primarySources() const
{
  return {u"SkyrimSE"_s};
}

QStringList GameSkyrimVR::validShortNames() const
{
  QStringList shortNames{u"Skyrim"_s, u"SkyrimSE"_s};
  if (m_Organizer->pluginSetting(name(), u"enderal_downloads"_s).toBool()) {
    shortNames.append({u"Enderal"_s, u"EnderalSE"_s});
  }
  return shortNames;
}

QString GameSkyrimVR::gameNexusName() const
{
  return QString();
}

QStringList GameSkyrimVR::iniFiles() const
{
  return {u"SkyrimVR.ini"_s, u"SkyrimPrefs.ini"_s};
}

QStringList GameSkyrimVR::DLCPlugins() const
{
  return {u"Dawnguard.esm"_s, u"HearthFires.esm"_s, u"Dragonborn.esm"_s};
}

QStringList GameSkyrimVR::CCPlugins() const
{
  QStringList plugins = {};
  QFile file(gameDirectory().filePath(u"Skyrim.ccc"_s));
  if (file.open(QIODevice::ReadOnly)) {
    ON_BLOCK_EXIT([&file]() {
      file.close();
    });

    if (file.size() == 0) {
      return plugins;
    }
    while (!file.atEnd()) {
      QByteArray line = file.readLine().trimmed();
      QString modName;
      if ((line.size() > 0) && (line.at(0) != '#')) {
        modName = QString::fromUtf8(line.constData()).toLower();
      }

      if (modName.size() > 0) {
        if (!plugins.contains(modName, Qt::CaseInsensitive)) {
          plugins.append(modName);
        }
      }
    }
  }
  return plugins;
}

IPluginGame::LoadOrderMechanism GameSkyrimVR::loadOrderMechanism() const
{
  return IPluginGame::LoadOrderMechanism::PluginsTxt;
}

MOBase::IPluginGame::SortMechanism GameSkyrimVR::sortMechanism() const
{
  return SortMechanism::LOOT;
}

int GameSkyrimVR::nexusModOrganizerID() const
{
  return 0;
}

int GameSkyrimVR::nexusGameID() const
{
  return 0;
}

QString GameSkyrimVR::getLauncherName() const
{
  return binaryName();  // Skyrim VR has no Launcher, so we just return the name of the
                        // game binary
}

QDir GameSkyrimVR::gameDirectory() const
{
  return QDir(m_GamePath);
}

// Not to delete all the spaces...
MappingType GameSkyrimVR::mappings() const
{
  MappingType result;

  for (const QString& profileFile : {u"plugins.txt"_s, u"loadorder.txt"_s}) {
    result.push_back({m_Organizer->profilePath() % '/' % profileFile,
                      localAppFolder() % '/' % gameName() % '/' % profileFile, false});
  }

  return result;
}
