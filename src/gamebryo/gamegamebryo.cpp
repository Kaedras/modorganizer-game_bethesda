#include "gamegamebryo.h"

#include "bsainvalidation.h"
#include "dataarchives.h"
#include "gamebryomoddatacontent.h"
#include "gamebryosavegame.h"
#include "gameplugins.h"
#include "iprofile.h"
#include "log.h"
#include "savegameinfo.h"
#include "scopeguard.h"
#include "scriptextender.h"
#include "utility.h"
#include "vdf_parser.h"

#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QIcon>
#include <QJsonDocument>
#include <QSettings>

#include <QtDebug>
#include <QtGlobal>

#include <optional>
#include <string>
#include <vector>

using namespace Qt::Literals::StringLiterals;

GameGamebryo::GameGamebryo() {}

void GameGamebryo::detectGame()
{
  m_GamePath    = identifyGamePath();
  m_MyGamesPath = determineMyGamesPath(gameName());
}

bool GameGamebryo::init(MOBase::IOrganizer* moInfo)
{
  m_Organizer = moInfo;
  m_Organizer->onAboutToRun([this](const auto& binary) {
    return prepareIni(binary);
  });
  return true;
}

bool GameGamebryo::isInstalled() const
{
  return !m_GamePath.isEmpty();
}

QIcon GameGamebryo::gameIcon() const
{
  return MOBase::iconForExecutable(gameDirectory().absoluteFilePath(binaryName()));
}

QDir GameGamebryo::gameDirectory() const
{
  return QDir(m_GamePath);
}

QDir GameGamebryo::dataDirectory() const
{
  return gameDirectory().absoluteFilePath(u"Data"_s);
}

void GameGamebryo::setGamePath(const QString& path)
{
  m_GamePath = path;
}

QDir GameGamebryo::documentsDirectory() const
{
  return m_MyGamesPath;
}

QDir GameGamebryo::savesDirectory() const
{
  return QDir(myGamesPath() % u"/Saves"_s);
}

std::vector<std::shared_ptr<const MOBase::ISaveGame>>
GameGamebryo::listSaves(QDir folder) const
{
  QStringList filters;
  filters << u"*."_s % savegameExtension();

  std::vector<std::shared_ptr<const MOBase::ISaveGame>> saves;
  for (const auto& info : folder.entryInfoList(filters, QDir::Files)) {
    try {
      saves.push_back(makeSaveGame(info.filePath()));
    } catch (std::exception& e) {
      MOBase::log::error("{}", e.what());
      continue;
    }
  }

  return saves;
}

void GameGamebryo::setGameVariant(const QString& variant)
{
  m_GameVariant = variant;
}

QString GameGamebryo::binaryName() const
{
  return gameShortName() % u".exe"_s;
}

MOBase::IPluginGame::LoadOrderMechanism GameGamebryo::loadOrderMechanism() const
{
  return LoadOrderMechanism::FileTime;
}

MOBase::IPluginGame::SortMechanism GameGamebryo::sortMechanism() const
{
  return SortMechanism::LOOT;
}

bool GameGamebryo::looksValid(QDir const& path) const
{
  // Check for <prog>.exe for now.
  return path.exists(binaryName());
}

QString GameGamebryo::gameVersion() const
{
  // We try the file version, but if it looks invalid (starts with the fallback
  // version), we look the product version instead. If the product version is
  // not empty, we use it.
  QString binaryAbsPath = gameDirectory().absoluteFilePath(binaryName());
  QString version       = MOBase::getFileVersion(binaryAbsPath);
  if (version.startsWith(FALLBACK_GAME_VERSION)) {
    QString pversion = MOBase::getProductVersion(binaryAbsPath);
    if (!pversion.isEmpty()) {
      version = pversion;
    }
  }
  return version;
}

QString GameGamebryo::getLauncherName() const
{
  return gameShortName() % u"Launcher.exe"_s;
}

QFileInfo GameGamebryo::findInGameFolder(const QString& relativePath) const
{
  return QFileInfo(m_GamePath % '/' % relativePath);
}

bool GameGamebryo::prepareIni(const QString&)
{
  MOBase::IProfile* profile = m_Organizer->profile();

  QString basePath = profile->localSettingsEnabled()
                         ? profile->absolutePath()
                         : documentsDirectory().absolutePath();

  if (!iniFiles().isEmpty()) {

    QString profileIni = basePath % '/' % iniFiles()[0];

    QSettings ini(profileIni, QSettings::IniFormat);

    if (ini.value(u"Launcher/bEnableFileSelection"_s, 0).toInt() != 1) {
      ini.setValue(u"Launcher/bEnableFileSelection"_s, 1);
    }
  }

  return true;
}

QString GameGamebryo::selectedVariant() const
{
  return m_GameVariant;
}

QString GameGamebryo::myGamesPath() const
{
  return m_MyGamesPath;
}

QString GameGamebryo::localAppFolder()
{
  // return values:
  // linux: ~/.config
  // os x: ~/Library/Preferences
  // windows: C:/Users/<USER>/AppData/Local
  return QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first();
}

void GameGamebryo::copyToProfile(QString const& sourcePath,
                                 QDir const& destinationDirectory,
                                 QString const& sourceFileName)
{
  copyToProfile(sourcePath, destinationDirectory, sourceFileName, sourceFileName);
}

void GameGamebryo::copyToProfile(QString const& sourcePath,
                                 QDir const& destinationDirectory,
                                 QString const& sourceFileName,
                                 QString const& destinationFileName)
{
  QString filePath = destinationDirectory.absoluteFilePath(destinationFileName);
  if (!QFileInfo::exists(filePath)) {
    if (!MOBase::shellCopy(sourcePath % '/' % sourceFileName, filePath)) {
      // if copy file fails, create the file empty
      QFile(filePath).open(QIODevice::WriteOnly);
    }
  }
}

MappingType GameGamebryo::mappings() const
{
  MappingType result;

  for (const auto& profileFile : {u"plugins.txt"_s, u"loadorder.txt"_s}) {
    result.push_back({m_Organizer->profilePath() % '/' % profileFile,
                      localAppFolder() % '/' % gameShortName() % '/' % profileFile,
                      false});
  }

  return result;
}

void GameGamebryo::registerFeature(std::shared_ptr<MOBase::GameFeature> feature)
{
  // priority does not matter, this is a game plugin so will get lowest priority in MO2
  m_Organizer->gameFeatures()->registerFeature(this, feature, 0, true);
}
