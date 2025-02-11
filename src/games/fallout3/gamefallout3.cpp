#include "gamefallout3.h"

#include "fallout3bsainvalidation.h"
#include "fallout3dataarchives.h"
#include "fallout3moddatachecker.h"
#include "fallout3moddatacontent.h"
#include "fallout3savegame.h"
#include "fallout3scriptextender.h"

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

GameFallout3::GameFallout3() {}

bool GameFallout3::init(IOrganizer* moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  auto dataArchives = std::make_shared<Fallout3DataArchives>(this);
  registerFeature(std::make_shared<Fallout3ScriptExtender>(this));
  registerFeature(dataArchives);
  registerFeature(std::make_shared<Fallout3BSAInvalidation>(dataArchives.get(), this));
  registerFeature(std::make_shared<GamebryoSaveGameInfo>(this));
  registerFeature(std::make_shared<GamebryoLocalSavegames>(this, u"fallout.ini"_s));
  registerFeature(std::make_shared<Fallout3ModDataChecker>(this));
  registerFeature(
      std::make_shared<Fallout3ModDataContent>(m_Organizer->gameFeatures()));
  registerFeature(std::make_shared<GamebryoGamePlugins>(moInfo));
  registerFeature(std::make_shared<GamebryoUnmangedMods>(this));
  return true;
}

QString GameFallout3::identifyVariant() const
{
  if (QFile::exists(m_GamePath + u"/Fallout3ng.exe"_s)) {
    return u"Low Violence"_s;
  } else if (QFile::exists(m_GamePath + u"/Galaxy.dll"_s)) {
    return u"GOG"_s;
  } else if (QFile::exists(m_GamePath + "/FalloutLauncherEpic.exe")) {
    return u"Epic Games"_s;
  } else if (m_GamePath.endsWith(u"Fallout 3 goty"_s)) {
    return u"Steam (Game of the Year)"_s;
  } else {
    return u"Steam (Regular)"_s;
  }
}

QString GameFallout3::identifyGamePath() const
{
  // Steam (Regular)
  auto result = parseSteamLocation(u"22300"_s, u"Fallout 3"_s);

  // Steam (Game of the Year)
  if (result.isEmpty()) {
    result = parseSteamLocation(u"22370"_s, u"Fallout 3 goty"_s);
  }

  // Epic Games
  if (result.isEmpty()) {
    // Fallout 3: Game of the Year Edition: adeae8bbfc94427db57c7dfecce3f1d4
    result = parseEpicGamesLocation({u"adeae8bbfc94427db57c7dfecce3f1d4"_s});
    if (QFileInfo(result).isDir()) {
      QDir startPath = QDir(result);
      auto subDirs =
          startPath.entryList({u"Fallout 3 GOTY*"_s}, QDir::Dirs | QDir::NoDotAndDotDot);
      if (!subDirs.isEmpty())
        result = startPath.absoluteFilePath(subDirs.first());
    }
  }

  // GOG (and Steam)
  if (result.isEmpty()) {
    result = GameGamebryo::identifyGamePath();
  }

  return result;
}

QString GameFallout3::gameName() const
{
  return "Fallout 3";
}

void GameFallout3::detectGame()
{
  m_GamePath = identifyGamePath();
  setGameVariant(identifyVariant());
  m_MyGamesPath = determineMyGamesPath(u"Fallout3"_s);
}

QList<ExecutableInfo> GameFallout3::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo(u"FOSE"_s,
                           findInGameFolder(m_Organizer->gameFeatures()
                                                ->gameFeature<MOBase::ScriptExtender>()
                                                ->loaderName()))
         << ExecutableInfo(u"Fallout 3"_s, findInGameFolder(binaryName()))
         << ExecutableInfo(u"Fallout Mod Manager"_s, findInGameFolder(u"fomm/fomm.exe"_s))
         << ExecutableInfo(u"Construction Kit"_s, findInGameFolder(u"geck.exe"_s))
         << ExecutableInfo(u"Fallout Launcher"_s, findInGameFolder(getLauncherName()))
         << ExecutableInfo(u"BOSS"_s, findInGameFolder(u"BOSS/BOSS.exe"_s))
         << ExecutableInfo(u"LOOT"_s, QFileInfo(getLootPath()))
                .withArgument(u"--game=\"Fallout3\""_s);
}

QList<ExecutableForcedLoadSetting> GameFallout3::executableForcedLoads() const
{
  return QList<ExecutableForcedLoadSetting>();
}

QString GameFallout3::name() const
{
  return u"Fallout 3 Support Plugin"_s;
}

QString GameFallout3::localizedName() const
{
  return tr("Fallout 3 Support Plugin");
}

QString GameFallout3::author() const
{
  return u"Tannin"_s;
}

QString GameFallout3::description() const
{
  return tr("Adds support for the game Fallout 3.");
}

MOBase::VersionInfo GameFallout3::version() const
{
  return VersionInfo(1, 4, 1, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameFallout3::settings() const
{
  return QList<PluginSetting>();
}

void GameFallout3::initializeProfile(const QDir& path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + u"/Fallout3"_s, path, u"plugins.txt"_s);
    copyToProfile(localAppFolder() + u"/Fallout3"_s, path, u"loadorder.txt"_s);
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS) ||
        !QFileInfo(myGamesPath(), u"fallout.ini"_s).exists()) {
      copyToProfile(gameDirectory().absolutePath(), path, u"fallout_default.ini"_s,
                    u"fallout.ini"_s);
    } else {
      copyToProfile(myGamesPath(), path, u"fallout.ini"_s);
    }

    copyToProfile(myGamesPath(), path, u"falloutprefs.ini"_s);
    copyToProfile(myGamesPath(), path, u"FalloutCustom.ini"_s);
    copyToProfile(myGamesPath(), path, u"custom.ini"_s);
    copyToProfile(myGamesPath(), path, u"GECKCustom.ini"_s);
    copyToProfile(myGamesPath(), path, u"GECKPrefs.ini"_s);
  }
}

QString GameFallout3::savegameExtension() const
{
  return u"fos"_s;
}

QString GameFallout3::savegameSEExtension() const
{
  return "";
}

std::shared_ptr<const GamebryoSaveGame>
GameFallout3::makeSaveGame(QString filePath) const
{
  return std::make_shared<const Fallout3SaveGame>(filePath, this);
}

QString GameFallout3::steamAPPId() const
{
  if (selectedVariant() == u"Steam (Game Of The Year)"_s) {
    return u"22370"_s;
  } else if (selectedVariant() == u"Steam (Regular)"_s) {
    return u"22300"_s;
  } else {
    return "";
  }
}

QStringList GameFallout3::primaryPlugins() const
{
  return {u"fallout3.esm"_s};
}

QStringList GameFallout3::gameVariants() const
{
  return {u"Steam (Regular)"_s, u"Steam (Game Of The Year)"_s, u"Epic Games"_s, u"GOG"_s,
          u"Low Violence"_s};
}

QString GameFallout3::binaryName() const
{
  if (selectedVariant() == u"Low Violence"_s) {
    return u"Fallout3ng.exe"_s;
  } else {
    return GameGamebryo::binaryName();
  }
}

QString GameFallout3::gameShortName() const
{
  return u"Fallout3"_s;
}

QStringList GameFallout3::validShortNames() const
{
  return {u"FalloutNV"_s};
}

QString GameFallout3::gameNexusName() const
{
  return u"fallout3"_s;
}

QStringList GameFallout3::iniFiles() const
{
  return {u"fallout.ini"_s, u"falloutprefs.ini"_s, u"FalloutCustom.ini"_s, u"GECKCustom.ini"_s,
          u"GECKPrefs.ini"_s};
}

QStringList GameFallout3::DLCPlugins() const
{
  return {u"ThePitt.esm"_s, u"Anchorage.esm"_s, u"BrokenSteel.esm"_s, u"PointLookout.esm"_s,
          u"Zeta.esm"_s};
}

int GameFallout3::nexusModOrganizerID() const
{
  return 16348;
}

int GameFallout3::nexusGameID() const
{
  return 120;
}

QString GameFallout3::getLauncherName() const
{
  const QMap<QString, QString> names = {
      {u"Steam (Regular)"_s, u"Fallout3Launcher.exe"_s},
      {u"Steam (Game of the Year)"_s, u"Fallout3Launcher.exe"_s},
      {u"Epic Games"_s, u"FalloutLauncherEpic.exe"_s},
      {u"GOG"_s, u"FalloutLauncher.exe"_s},
      {u"Low Violence"_s, u"Fallout3Launcher.exe"_s}};

  return names.value(selectedVariant());
}
