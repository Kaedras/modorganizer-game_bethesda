#include "gamefalloutnv.h"

#include "falloutnvbsainvalidation.h"
#include "falloutnvdataarchives.h"
#include "falloutnvmoddatachecker.h"
#include "falloutnvmoddatacontent.h"
#include "falloutnvsavegame.h"
#include "falloutnvscriptextender.h"

#include "executableinfo.h"
#include "pluginsetting.h"
#include "versioninfo.h"
#include <gamebryogameplugins.h>
#include <gamebryolocalsavegames.h>
#include <gamebryosavegameinfo.h>
#include <gamebryounmanagedmods.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

#include <memory>

using namespace MOBase;
using namespace Qt::Literals::StringLiterals;

GameFalloutNV::GameFalloutNV() {}

bool GameFalloutNV::init(IOrganizer* moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  auto dataArchives = std::make_shared<FalloutNVDataArchives>(this);
  registerFeature(std::make_shared<FalloutNVScriptExtender>(this));
  registerFeature(dataArchives);
  registerFeature(std::make_shared<FalloutNVBSAInvalidation>(dataArchives.get(), this));
  registerFeature(std::make_shared<GamebryoSaveGameInfo>(this));
  registerFeature(std::make_shared<GamebryoLocalSavegames>(this, u"fallout.ini"_s));
  registerFeature(std::make_shared<FalloutNVModDataChecker>(this));
  registerFeature(
      std::make_shared<FalloutNVModDataContent>(m_Organizer->gameFeatures()));
  registerFeature(std::make_shared<GamebryoGamePlugins>(moInfo));
  registerFeature(std::make_shared<GamebryoUnmangedMods>(this));

  return true;
}

void GameFalloutNV::setVariant(QString variant)
{
  m_GameVariant = variant;
}

void GameFalloutNV::checkVariants()
{
  bool gog_dll_exists = QFileInfo::exists(m_GamePath % u"/Galaxy.dll"_s);
  bool epic_dll_exists = QFileInfo::exists(m_GamePath % u"/EOSSDK-Win32-Shipping.dll"_s);
  if (gog_dll_exists)
    setVariant(u"GOG"_s);
  else if (epic_dll_exists)
    setVariant(u"Epic Games"_s);
  else
    setVariant(u"Steam"_s);
}

QDir GameFalloutNV::documentsDirectory() const
{
  return m_MyGamesPath;
}

QString GameFalloutNV::identifyGamePath() const
{
  auto result = GameGamebryo::identifyGamePath();  // Default registry path
  // EPIC Game Store
  if (result.isEmpty()) {
    /**
     * Basegame: 5daeb974a22a435988892319b3a4f476
     * Dead Money: b290229eb58045cbab9501640f3278f3
     * Honest Hearts: 562d4a2c1b3147b089a7c453e3ddbcbe
     * Old World Blues: c8dae1ab0570475a8b38a9041e614840
     * Lonesome Road: 4fa3d8d9b2cb4714a19a38d1a598be8f
     * Gun Runners' Arsenal: 7dcfb9cd9d134728b2646466c34c7b3b
     * Courier's Stash: ee9a44b4530942499ef1c8c390731fce
     */
    result = parseEpicGamesLocation({u"5daeb974a22a435988892319b3a4f476"_s});
    if (QFileInfo(result).isDir()) {
      QDir startPath = QDir(result);
      auto subDirs   = startPath.entryList({u"Fallout New Vegas*"_s},
                                           QDir::Dirs | QDir::NoDotAndDotDot);
      if (!subDirs.isEmpty())
        result = startPath.absoluteFilePath(subDirs.first());
    }
  }
  return result;
}

void GameFalloutNV::setGamePath(const QString& path)
{
  m_GamePath = path;
  checkVariants();
  m_MyGamesPath = determineMyGamesPath(gameDirectoryName());
}

QDir GameFalloutNV::savesDirectory() const
{
  return QDir(m_MyGamesPath % u"/Saves"_s);
}

QString GameFalloutNV::myGamesPath() const
{
  return m_MyGamesPath;
}

bool GameFalloutNV::isInstalled() const
{
  return !m_GamePath.isEmpty();
}

QString GameFalloutNV::gameName() const
{
  return u"New Vegas"_s;
}

QString GameFalloutNV::gameDirectoryName() const
{
  if (selectedVariant() == u"Epic Games"_s)
    return u"FalloutNV_Epic"_s;
  else
    return u"FalloutNV"_s;
}

void GameFalloutNV::detectGame()
{
  m_GamePath = identifyGamePath();
  checkVariants();
  m_MyGamesPath = determineMyGamesPath(gameDirectoryName());
}

QList<ExecutableInfo> GameFalloutNV::executables() const
{
  ExecutableInfo game(u"New Vegas"_s, findInGameFolder(binaryName()));
  ExecutableInfo launcher(u"Fallout Launcher"_s, findInGameFolder(getLauncherName()));
  QList<ExecutableInfo> extraExecutables =
      QList<ExecutableInfo>() << ExecutableInfo(u"Fallout Mod Manager"_s,
                                                findInGameFolder(u"fomm/fomm.exe"_s))
                              << ExecutableInfo(u"BOSS"_s,
                                                findInGameFolder(u"BOSS/BOSS.exe"_s))
                              << ExecutableInfo(u"GECK"_s, findInGameFolder(u"geck.exe"_s))
                              << ExecutableInfo(u"LOOT"_s, QFileInfo(getLootPath()))
                                     .withArgument(u"--game=\"FalloutNV\""_s);
  if (selectedVariant() != u"Epic Games"_s) {
    extraExecutables.prepend(ExecutableInfo(
        u"NVSE"_s, findInGameFolder(m_Organizer->gameFeatures()
                                     ->gameFeature<MOBase::ScriptExtender>()
                                     ->loaderName())));
  } else {
    game.withArgument(u"-EpicPortal"_s);
    launcher.withArgument(u"-EpicPortal"_s);
  }
  QList<ExecutableInfo> executables = {game, launcher};
  executables += extraExecutables;
  return executables;
}

QList<ExecutableForcedLoadSetting> GameFalloutNV::executableForcedLoads() const
{
  return QList<ExecutableForcedLoadSetting>();
}

QString GameFalloutNV::name() const
{
  return u"Fallout NV Support Plugin"_s;
}

QString GameFalloutNV::localizedName() const
{
  return tr("Fallout NV Support Plugin");
}

QString GameFalloutNV::author() const
{
  return u"Tannin & MO2 Team"_s;
}

QString GameFalloutNV::description() const
{
  return tr("Adds support for the game Fallout New Vegas");
}

MOBase::VersionInfo GameFalloutNV::version() const
{
  return VersionInfo(1, 6, 0, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameFalloutNV::settings() const
{
  return QList<PluginSetting>()
         << PluginSetting(u"enable_loot_sorting"_s,
                          tr("While not recommended by the FNV modding community, "
                             "enables LOOT sorting"),
                          false);
}

void GameFalloutNV::initializeProfile(const QDir& path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() % '/' % gameDirectoryName(), path, u"plugins.txt"_s);
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS) ||
        !QFileInfo::exists(myGamesPath() % u"/fallout.ini"_s)) {
      copyToProfile(gameDirectory().absolutePath(), path, u"fallout_default.ini"_s,
                    u"fallout.ini"_s);
    } else {
      copyToProfile(myGamesPath(), path, u"fallout.ini"_s);
    }

    copyToProfile(myGamesPath(), path, u"falloutprefs.ini"_s);
    copyToProfile(myGamesPath(), path, u"falloutcustom.ini"_s);
    copyToProfile(myGamesPath(), path, u"GECKCustom.ini"_s);
    copyToProfile(myGamesPath(), path, u"GECKPrefs.ini"_s);
  }
}

QString GameFalloutNV::savegameExtension() const
{
  return u"fos"_s;
}

QString GameFalloutNV::savegameSEExtension() const
{
  return u"nvse"_s;
}

std::shared_ptr<const GamebryoSaveGame>
GameFalloutNV::makeSaveGame(QString filePath) const
{
  return std::make_shared<const FalloutNVSaveGame>(filePath, this);
}

QString GameFalloutNV::steamAPPId() const
{
  if (selectedVariant() == u"Steam"_s) {
    if (m_GamePath.endsWith(u"enplczru"_s)) {
      return u"22490"_s;
    } else {
      return u"22380"_s;
    }
  }
  return QString();
}

QStringList GameFalloutNV::primaryPlugins() const
{
  return {u"falloutnv.esm"_s};
}

QStringList GameFalloutNV::gameVariants() const
{
  return {u"Steam"_s, u"GOG"_s, u"Epic Games"_s};
}

QString GameFalloutNV::gameShortName() const
{
  return u"FalloutNV"_s;
}

QStringList GameFalloutNV::validShortNames() const
{
  return {u"Fallout3"_s};
}

QString GameFalloutNV::gameNexusName() const
{
  return u"newvegas"_s;
}

QStringList GameFalloutNV::iniFiles() const
{
  return {u"fallout.ini"_s, u"falloutprefs.ini"_s, u"falloutcustom.ini"_s, u"GECKCustom.ini"_s,
          u"GECKPrefs.ini"_s};
}

QStringList GameFalloutNV::DLCPlugins() const
{
  return {u"DeadMoney.esm"_s,    u"HonestHearts.esm"_s,      u"OldWorldBlues.esm"_s,
          u"LonesomeRoad.esm"_s, u"GunRunnersArsenal.esm"_s, u"CaravanPack.esm"_s,
          u"ClassicPack.esm"_s,  u"MercenaryPack.esm"_s,     u"TribalPack.esm"_s};
}

MOBase::IPluginGame::SortMechanism GameFalloutNV::sortMechanism() const
{
  if (m_Organizer->pluginSetting(name(), "enable_loot_sorting").toBool())
    return IPluginGame::SortMechanism::LOOT;
  return IPluginGame::SortMechanism::NONE;
}

int GameFalloutNV::nexusModOrganizerID() const
{
  return 42572;
}

int GameFalloutNV::nexusGameID() const
{
  return 130;
}

QDir GameFalloutNV::gameDirectory() const
{
  return QDir(m_GamePath);
}

MappingType GameFalloutNV::mappings() const
{
  MappingType result;

  for (const QString& profileFile : {u"plugins.txt"_s, u"loadorder.txt"_s}) {
    result.push_back({m_Organizer->profilePath() % '/' % profileFile,
                      localAppFolder() % '/' % gameShortName() % '/' % profileFile,
                      false});
    if (selectedVariant() == "Epic Games") {
      result.push_back(
          {m_Organizer->profilePath() % '/' % profileFile,
           localAppFolder() % '/' % gameDirectoryName() % '/' % profileFile, false});
    }
  }

  return result;
}
