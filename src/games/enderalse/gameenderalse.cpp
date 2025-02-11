#include "gameenderalse.h"

#include "enderalsedataarchives.h"
#include "enderalsegameplugins.h"
#include "enderalselocalsavegames.h"
#include "enderalsemoddatachecker.h"
#include "enderalsemoddatacontent.h"
#include "enderalsesavegame.h"
#include "enderalsescriptextender.h"
#include "enderalseunmanagedmods.h"
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

#include "scopeguard.h"
#include <memory>

using namespace MOBase;
using namespace Qt::Literals::StringLiterals;

GameEnderalSE::GameEnderalSE() {}

void GameEnderalSE::setVariant(QString variant)
{
  m_GameVariant = variant;
}

void GameEnderalSE::checkVariants()
{
  QFileInfo gog_dll(m_GamePath % u"/Galaxy64.dll"_s);
  if (gog_dll.exists())
    setVariant(u"GOG"_s);
  else
    setVariant(u"Steam"_s);
}

QDir GameEnderalSE::documentsDirectory() const
{
  return m_MyGamesPath;
}

void GameEnderalSE::detectGame()
{
  m_GamePath = identifyGamePath();
  checkVariants();
  m_MyGamesPath = determineMyGamesPath(gameDirectoryName());
}

QString GameEnderalSE::identifyGamePath() const
{
  QMap<QString, QString> paths = {
      {u"Software\\Bethesda Softworks\\"_s % gameName(), u"Installed Path"_s},
      {u"Software\\GOG.com\\Games\\1708684988"_s, "path"},
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
    result = MOBase::findSteamGame(u"Enderal Special Edition"_s,
                                   u"Data/Enderal - Forgotten Stories.esm"_s);
  }
  return result;
}

void GameEnderalSE::setGamePath(const QString& path)
{
  m_GamePath = path;
  checkVariants();
  m_MyGamesPath = determineMyGamesPath(gameDirectoryName());
}

QDir GameEnderalSE::savesDirectory() const
{
  return QDir(m_MyGamesPath % u"/Saves"_s);
}

bool GameEnderalSE::isInstalled() const
{
  return !m_GamePath.isEmpty();
}

bool GameEnderalSE::init(IOrganizer* moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  auto dataArchives = std::make_shared<EnderalSEDataArchives>(this);
  registerFeature(std::make_shared<EnderalSEScriptExtender>(this));
  registerFeature(dataArchives);
  registerFeature(std::make_shared<EnderalSELocalSavegames>(this, u"Enderal.ini"_s));
  registerFeature(std::make_shared<EnderalSEModDataChecker>(this));
  registerFeature(std::make_shared<EnderalSEModDataContent>(moInfo->gameFeatures()));
  registerFeature(std::make_shared<GamebryoSaveGameInfo>(this));
  registerFeature(std::make_shared<EnderalSEGamePlugins>(moInfo));
  registerFeature(std::make_shared<EnderalSEUnmangedMods>(this));

  return true;
}

QString GameEnderalSE::gameName() const
{
  return u"Enderal Special Edition"_s;
}

QString GameEnderalSE::gameDirectoryName() const
{
  if (selectedVariant() == u"GOG"_s)
    return u"Enderal Special Edition GOG"_s;
  else
    return u"Enderal Special Edition"_s;
}

QIcon GameEnderalSE::gameIcon() const
{
  return MOBase::iconForExecutable(gameDirectory().absoluteFilePath(getLauncherName()));
}

QList<ExecutableInfo> GameEnderalSE::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo(u"Enderal Special Edition (SKSE)"_s,
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
         << ExecutableInfo(u"Enderal Special Edition Launcher"_s,
                           findInGameFolder(getLauncherName()))
         << ExecutableInfo(u"Creation Kit"_s, findInGameFolder(u"CreationKit.exe"_s))
                .withSteamAppId(u"1946180"_s)
         << ExecutableInfo(u"LOOT"_s, QFileInfo(getLootPath()))
                .withArgument(u"--game=\"Enderal Special Edition\""_s);
}

QList<ExecutableForcedLoadSetting> GameEnderalSE::executableForcedLoads() const
{
  return QList<ExecutableForcedLoadSetting>();
}

QString GameEnderalSE::binaryName() const
{
  return u"skse64_loader.exe"_s;
}

QString GameEnderalSE::getLauncherName() const
{
  return u"Enderal Launcher.exe"_s;
}

bool GameEnderalSE::looksValid(const QDir& folder) const
{
  // we need to check both launcher and binary because the binary also exists for
  // Skyrim SE and the launcher for Enderal LE
  return folder.exists(getLauncherName()) && folder.exists(binaryName());
}

QFileInfo GameEnderalSE::findInGameFolder(const QString& relativePath) const
{
  return QFileInfo(m_GamePath % '/' % relativePath);
}

QString GameEnderalSE::name() const
{
  return u"Enderal Special Edition Support Plugin"_s;
}

QString GameEnderalSE::localizedName() const
{
  return tr("Enderal Special Edition Support Plugin");
}

QString GameEnderalSE::author() const
{
  return u"Archost, ZachHaber & MO2 Team"_s;
}

QString GameEnderalSE::description() const
{
  return tr("Adds support for the game Enderal Special Edition.");
}

MOBase::VersionInfo GameEnderalSE::version() const
{
  return VersionInfo(1, 2, 0, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameEnderalSE::settings() const
{
  return QList<PluginSetting>();
}

void GameEnderalSE::initializeProfile(const QDir& path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() % gameDirectoryName(), path, u"plugins.txt"_s);
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS) ||
        !QFileInfo::exists(myGamesPath() % u"/Enderal.ini"_s)) {

      // there is no default ini, actually they are going to put them in for us!
      copyToProfile(gameDirectory().absolutePath(), path, u"Enderal_default.ini"_s,
                    u"Enderal.ini"_s);
      copyToProfile(gameDirectory().absolutePath(), path, u"EnderalPrefs_default.ini"_s,
                    u"EnderalPrefs.ini"_s);
    } else {
      copyToProfile(myGamesPath(), path, u"Enderal.ini"_s);
      copyToProfile(myGamesPath(), path, u"EnderalPrefs.ini"_s);
    }
  }
}

QString GameEnderalSE::savegameExtension() const
{
  return u"ess"_s;
}

QString GameEnderalSE::savegameSEExtension() const
{
  return u"skse"_s;
}

std::shared_ptr<const GamebryoSaveGame>
GameEnderalSE::makeSaveGame(QString filePath) const
{
  return std::make_shared<const EnderalSESaveGame>(filePath, this);
}

QString GameEnderalSE::steamAPPId() const
{
  if (selectedVariant() == u"Steam"_s)
    return u"976620"_s;
  return "";
}

QStringList GameEnderalSE::primaryPlugins() const
{
  return {u"Skyrim.esm"_s, u"Update.esm"_s, u"Dawnguard.esm"_s, u"HearthFires.esm"_s,
          u"Dragonborn.esm"_s,

          // these two plugins are considered "primary" for users but are not
          // automatically loaded by the game so we need to force-write them
          // to the plugin list
          u"Enderal - Forgotten Stories.esm"_s, u"SkyUI_SE.esp"_s};
}

QStringList GameEnderalSE::DLCPlugins() const
{
  return {};
}

QStringList GameEnderalSE::gameVariants() const
{
  return {u"Steam"_s, u"GOG"_s};
}

QString GameEnderalSE::gameShortName() const
{
  return u"EnderalSE"_s;
}

QStringList GameEnderalSE::validShortNames() const
{
  return {u"Skyrim"_s, u"SkyrimSE"_s, u"Enderal"_s};
}

QString GameEnderalSE::gameNexusName() const
{
  return u"enderalspecialedition"_s;
}

QStringList GameEnderalSE::iniFiles() const
{
  return {u"Enderal.ini"_s, u"EnderalPrefs.ini"_s};
}

QStringList GameEnderalSE::CCPlugins() const
{
  return {};
}

IPluginGame::LoadOrderMechanism GameEnderalSE::loadOrderMechanism() const
{
  return IPluginGame::LoadOrderMechanism::PluginsTxt;
}

int GameEnderalSE::nexusModOrganizerID() const
{
  return 0;
}

int GameEnderalSE::nexusGameID() const
{
  return 3685;
}

QDir GameEnderalSE::gameDirectory() const
{
  return QDir(m_GamePath);
}

// Not to delete all the spaces...
MappingType GameEnderalSE::mappings() const
{
  MappingType result;

  for (const auto& profileFile : {u"plugins.txt"_s, u"loadorder.txt"_s}) {
    result.push_back({m_Organizer->profilePath() % '/' % profileFile,
                      localAppFolder() % '/' % gameDirectoryName() % '/' % profileFile,
                      false});
  }

  return result;
}
