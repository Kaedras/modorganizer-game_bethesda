#include "gamemorrowind.h"

#include "morrowindbsainvalidation.h"
#include "morrowinddataarchives.h"
#include "morrowindgameplugins.h"
#include "morrowindlocalsavegames.h"
#include "morrowindmoddatachecker.h"
#include "morrowindmoddatacontent.h"
#include "morrowindsavegame.h"
#include "morrowindsavegameinfo.h"

#include "executableinfo.h"
#include "pluginsetting.h"
#include "steamutility.h"

#include <gamebryounmanagedmods.h>

#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>

#include <QtDebug>

#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace MOBase;
using namespace Qt::Literals::StringLiterals;

GameMorrowind::GameMorrowind() {}

bool GameMorrowind::init(IOrganizer* moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  auto dataArchives = std::make_shared<MorrowindDataArchives>(this);
  registerFeature(dataArchives);
  registerFeature(std::make_shared<MorrowindBSAInvalidation>(dataArchives.get(), this));
  registerFeature(std::make_shared<MorrowindSaveGameInfo>(this));
  registerFeature(std::make_shared<MorrowindLocalSavegames>(this));
  registerFeature(std::make_shared<MorrowindModDataChecker>(this));
  registerFeature(
      std::make_shared<MorrowindModDataContent>(m_Organizer->gameFeatures()));
  registerFeature(std::make_shared<MorrowindGamePlugins>(moInfo));
  registerFeature(std::make_shared<GamebryoUnmangedMods>(this));

  return true;
}

QString GameMorrowind::gameName() const
{
  return u"Morrowind"_s;
}

QString GameMorrowind::getLauncherName() const
{
  return u"Morrowind Launcher.exe"_s;
}

QDir GameMorrowind::dataDirectory() const
{
  return gameDirectory().absoluteFilePath(u"Data Files"_s);
}

QDir GameMorrowind::savesDirectory() const
{
  return QDir(gameDirectory().absoluteFilePath(u"Saves"_s));
}

QDir GameMorrowind::documentsDirectory() const
{
  return gameDirectory();
}

QList<ExecutableInfo> GameMorrowind::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo(u"MWSE (Launcher Method)"_s,
                           findInGameFolder(u"MWSE Launcher.exe"_s))
         << ExecutableInfo(u"Morrowind"_s, findInGameFolder(binaryName()))
         << ExecutableInfo(u"Morrowind Launcher"_s, findInGameFolder(getLauncherName()))
         << ExecutableInfo(u"MGE XE"_s, findInGameFolder(u"MGEXEgui.exe"_s))
         << ExecutableInfo(u"LOOT"_s, QFileInfo(getLootPath()))
                .withArgument(u"--game=\"Morrowind\""_s);
}

QList<ExecutableForcedLoadSetting> GameMorrowind::executableForcedLoads() const
{
  return QList<ExecutableForcedLoadSetting>();
}

QString GameMorrowind::name() const
{
  return u"Morrowind Support Plugin"_s;
}

QString GameMorrowind::localizedName() const
{
  return tr("Morrowind Support Plugin");
}

QString GameMorrowind::author() const
{
  return u"Schilduin & MO2 Team"_s;
}

QString GameMorrowind::description() const
{
  return tr("Adds support for the game Morrowind.\n"
            "Splash by %1")
      .arg(u"AnyOldName3"_s);
}

MOBase::VersionInfo GameMorrowind::version() const
{
  return VersionInfo(1, 5, 1, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameMorrowind::settings() const
{
  return QList<PluginSetting>();
}

void GameMorrowind::initializeProfile(const QDir& path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() % u"/Morrowind"_s, path, u"plugins.txt"_s);
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    copyToProfile(gameDirectory().absolutePath(), path, u"Morrowind.ini"_s);
  }
}

QString GameMorrowind::savegameExtension() const
{
  return u"ess"_s;
}

QString GameMorrowind::savegameSEExtension() const
{
  return u"mwse"_s;
}

std::shared_ptr<const GamebryoSaveGame>
GameMorrowind::makeSaveGame(QString filePath) const
{
  return std::make_shared<const MorrowindSaveGame>(filePath, this);
}

QString GameMorrowind::steamAPPId() const
{
  return u"22320"_s;
}

QStringList GameMorrowind::primaryPlugins() const
{
  return {u"morrowind.esm"_s};
}

QString GameMorrowind::binaryName() const
{
  return u"Morrowind.exe"_s;
}

QString GameMorrowind::gameShortName() const
{
  return u"Morrowind"_s;
}

QString GameMorrowind::gameNexusName() const
{
  return u"Morrowind"_s;
}

QStringList GameMorrowind::iniFiles() const
{
  return {u"morrowind.ini"_s};
}

QStringList GameMorrowind::DLCPlugins() const
{
  return {u"Tribunal.esm"_s, u"Bloodmoon.esm"_s};
}

MOBase::IPluginGame::SortMechanism GameMorrowind::sortMechanism() const
{
  return SortMechanism::LOOT;
}

int GameMorrowind::nexusModOrganizerID() const
{
  return 1334;
}

int GameMorrowind::nexusGameID() const
{
  return 100;
}

QString GameMorrowind::identifyGamePath() const
{
  QString path = GameGamebryo::identifyGamePath();
  if (path.isEmpty())
    path = MOBase::findSteamGame(u"Morrowind"_s, u"Data Files/Morrowind.esm"_s);
  return path;
}
