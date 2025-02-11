#include "gameskyrimse.h"

#include "skyrimsedataarchives.h"
#include "skyrimsemoddatachecker.h"
#include "skyrimsemoddatacontent.h"
#include "skyrimsesavegame.h"
#include "skyrimsescriptextender.h"
#include "skyrimseunmanagedmods.h"

#include "versioninfo.h"
#include <creationgameplugins.h>
#include <executableinfo.h>
#include <gamebryolocalsavegames.h>
#include <gamebryosavegameinfo.h>
#include <pluginsetting.h>
#include <utility.h>

#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QJsonDocument>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

#include "scopeguard.h"
#include <memory>

using namespace MOBase;
using namespace Qt::Literals::StringLiterals;

GameSkyrimSE::GameSkyrimSE() {}

void GameSkyrimSE::setVariant(QString variant)
{
  m_GameVariant = variant;
}

void GameSkyrimSE::checkVariants()
{
  QFileInfo gog_dll(m_GamePath % u"/Galaxy64.dll"_s);
  QFileInfo epic_dll(m_GamePath % u"/EOSSDK-Win64-Shipping.dll"_s);
  if (gog_dll.exists())
    setVariant(u"GOG"_s);
  else if (epic_dll.exists())
    setVariant(u"Epic Games"_s);
  else
    setVariant(u"Steam"_s);
}

QDir GameSkyrimSE::documentsDirectory() const
{
  return m_MyGamesPath;
}

void GameSkyrimSE::detectGame()
{
  m_GamePath = identifyGamePath();
  checkVariants();
  m_MyGamesPath = determineMyGamesPath(gameDirectoryName());
}

QString GameSkyrimSE::identifyGamePath() const
{
  QMap<QString, QString> paths = {
      {u"Software\\Bethesda Softworks\\"_s % gameName(), u"Installed Path"_s},
      {u"Software\\GOG.com\\Games\\1162721350"_s, u"path"_s},
      {u"Software\\GOG.com\\Games\\1711230643"_s, u"path"_s},
  };

  QString result;
  for (auto& path : paths.toStdMap()) {
    result = findInRegistry(HKEY_LOCAL_MACHINE, path.first.toStdWString().c_str(),
                            path.second.toStdWString().c_str());
    if (!result.isEmpty())
      break;
  }

  // Check Epic Games Manifests
  // AppName: ac82db5035584c7f8a2c548d98c86b2c
  //      AE Update: 5d600e4f59974aeba0259c7734134e27
  if (result.isEmpty()) {
    result = parseEpicGamesLocation(
        {u"ac82db5035584c7f8a2c548d98c86b2c"_s, u"5d600e4f59974aeba0259c7734134e27"_s});
  }

  return result;
}

void GameSkyrimSE::setGamePath(const QString& path)
{
  m_GamePath = path;
  checkVariants();
  m_MyGamesPath = determineMyGamesPath(gameDirectoryName());
}

QDir GameSkyrimSE::savesDirectory() const
{
  return QDir(m_MyGamesPath + u"/Saves"_s);
}

QString GameSkyrimSE::myGamesPath() const
{
  return m_MyGamesPath;
}

bool GameSkyrimSE::isInstalled() const
{
  return !m_GamePath.isEmpty();
}

bool GameSkyrimSE::init(IOrganizer* moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  registerFeature(std::make_shared<SkyrimSEScriptExtender>(this));
  registerFeature(std::make_shared<SkyrimSEDataArchives>(this));
  registerFeature(std::make_shared<GamebryoLocalSavegames>(this, u"SkyrimCustom.ini"_s));
  registerFeature(std::make_shared<SkyrimSEModDataChecker>(this));
  registerFeature(
      std::make_shared<SkyrimSEModDataContent>(m_Organizer->gameFeatures()));
  registerFeature(std::make_shared<GamebryoSaveGameInfo>(this));
  registerFeature(std::make_shared<CreationGamePlugins>(moInfo));
  registerFeature(std::make_shared<SkyrimSEUnmangedMods>(this));

  return true;
}

QString GameSkyrimSE::gameName() const
{
  return u"Skyrim Special Edition"_s;
}

QString GameSkyrimSE::gameDirectoryName() const
{
  if (selectedVariant() == u"GOG"_s)
    return u"Skyrim Special Edition GOG"_s;
  else if (selectedVariant() == u"Epic Games"_s)
    return u"Skyrim Special Edition EPIC"_s;
  else
    return u"Skyrim Special Edition"_s;
}

QList<ExecutableInfo> GameSkyrimSE::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo(u"SKSE"_s,
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
         << ExecutableInfo(u"Skyrim Special Edition"_s, findInGameFolder(binaryName()))
         << ExecutableInfo(u"Skyrim Special Edition Launcher"_s,
                           findInGameFolder(getLauncherName()))
         << ExecutableInfo(u"Creation Kit"_s, findInGameFolder(u"CreationKit.exe"_s))
                .withSteamAppId(u"1946180"_s)
         << ExecutableInfo(u"LOOT"_s, QFileInfo(getLootPath()))
                .withArgument(u"--game=\"Skyrim Special Edition\""_s);
}

QList<ExecutableForcedLoadSetting> GameSkyrimSE::executableForcedLoads() const
{
  return QList<ExecutableForcedLoadSetting>();
}

QFileInfo GameSkyrimSE::findInGameFolder(const QString& relativePath) const
{
  return QFileInfo(m_GamePath % '/' % relativePath);
}

QString GameSkyrimSE::name() const
{
  return u"Skyrim Special Edition Support Plugin"_s;
}

QString GameSkyrimSE::localizedName() const
{
  return tr("Skyrim Special Edition Support Plugin");
}

QString GameSkyrimSE::author() const
{
  return u"MO2 Team, Orig: Archost & ZachHaber"_s;
}

QString GameSkyrimSE::description() const
{
  return tr("Adds support for the game Skyrim Special Edition.");
}

MOBase::VersionInfo GameSkyrimSE::version() const
{
  return VersionInfo(1, 7, 1, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameSkyrimSE::settings() const
{
  return {PluginSetting(u"enderal_downloads"_s, u"allow Enderal and Enderal SE downloads"_s,
                        QVariant(false))};
}

void GameSkyrimSE::initializeProfile(const QDir& path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() % '/' % gameDirectoryName(), path, u"plugins.txt"_s);
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS) ||
        !QFileInfo::exists(myGamesPath() + u"/Skyrim.ini"_s)) {
      copyToProfile(gameDirectory().absolutePath(), path, u"Skyrim_default.ini"_s,
                    u"Skyrim.ini"_s);
    } else {
      copyToProfile(myGamesPath(), path, u"Skyrim.ini"_s);
    }

    copyToProfile(myGamesPath(), path, u"SkyrimPrefs.ini"_s);
    copyToProfile(myGamesPath(), path, u"SkyrimCustom.ini"_s);
  }
}

QString GameSkyrimSE::savegameExtension() const
{
  return u"ess"_s;
}

QString GameSkyrimSE::savegameSEExtension() const
{
  return u"skse"_s;
}

std::shared_ptr<const GamebryoSaveGame>
GameSkyrimSE::makeSaveGame(QString filePath) const
{
  return std::make_shared<const SkyrimSESaveGame>(filePath, this);
}

QString GameSkyrimSE::steamAPPId() const
{
  if (selectedVariant() == u"Steam"_s)
    return u"489830"_s;
  return u""_s;
}

QStringList GameSkyrimSE::primaryPlugins() const
{
  QStringList plugins = {u"Skyrim.esm"_s, u"Update.esm"_s, u"Dawnguard.esm"_s, u"HearthFires.esm"_s,
                         u"Dragonborn.esm"_s};

  plugins.append(CCPlugins());

  return plugins;
}

QStringList GameSkyrimSE::gameVariants() const
{
  return {u"Steam"_s, u"GOG"_s, u"Epic Games"_s};
}

QString GameSkyrimSE::gameShortName() const
{
  return u"SkyrimSE"_s;
}

QStringList GameSkyrimSE::validShortNames() const
{
  QStringList shortNames{u"Skyrim"_s};
  if (m_Organizer->pluginSetting(name(), u"enderal_downloads"_s).toBool()) {
    shortNames.append({u"Enderal"_s, u"EnderalSE"_s});
  }
  return shortNames;
}

QString GameSkyrimSE::gameNexusName() const
{
  return u"skyrimspecialedition"_s;
}

QStringList GameSkyrimSE::iniFiles() const
{
  return {u"Skyrim.ini"_s, u"SkyrimPrefs.ini"_s, u"skyrimcustom.ini"_s};
}

QStringList GameSkyrimSE::DLCPlugins() const
{
  return {u"Dawnguard.esm"_s, u"HearthFires.esm"_s, u"Dragonborn.esm"_s};
}

QStringList GameSkyrimSE::CCPlugins() const
{
  QStringList plugins;
  std::set<QString> pluginsLookup;

  const QString path = gameDirectory().filePath(u"Skyrim.ccc"_s);

  MOBase::forEachLineInFile(path, [&](QString s) {
    const auto lc = s.toLower();
    if (!pluginsLookup.contains(lc)) {
      pluginsLookup.insert(lc);
      plugins.append(std::move(s));
    }
  });

  return plugins;
}

IPluginGame::LoadOrderMechanism GameSkyrimSE::loadOrderMechanism() const
{
  return IPluginGame::LoadOrderMechanism::PluginsTxt;
}

int GameSkyrimSE::nexusModOrganizerID() const
{
  return 6194;  //... Should be 0?
}

int GameSkyrimSE::nexusGameID() const
{
  return 1704;  // 1704
}

QDir GameSkyrimSE::gameDirectory() const
{
  return QDir(m_GamePath);
}

// Not to delete all the spaces...
MappingType GameSkyrimSE::mappings() const
{
  MappingType result;

  for (const QString& profileFile : {u"plugins.txt"_s, u"loadorder.txt"_s}) {
    result.push_back({m_Organizer->profilePath() % '/' % profileFile,
                      localAppFolder() % '/' % gameDirectoryName() % '/' % profileFile,
                      false});
  }

  return result;
}
