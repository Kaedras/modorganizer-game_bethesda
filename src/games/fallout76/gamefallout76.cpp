#include "gamefallout76.h"

#include "fallout76dataarchives.h"
#include "fallout76moddatachecker.h"
#include "fallout76moddatacontent.h"
#include "fallout76unmanagedmods.h"

#include "versioninfo.h"
#include <creationgameplugins.h>
#include <executableinfo.h>
#include <pluginsetting.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

#include <memory>

#include "scopeguard.h"

using namespace MOBase;
using namespace Qt::Literals::StringLiterals;

GameFallout76::GameFallout76() {}

bool GameFallout76::init(IOrganizer* moInfo)
{
  if (!GameGamebryo::init(moInfo)) {
    return false;
  }

  registerFeature(std::make_shared<Fallout76DataArchives>(this));
  registerFeature(std::make_shared<Fallout76ModDataChecker>(this));
  registerFeature(
      std::make_shared<Fallout76ModDataContent>(m_Organizer->gameFeatures()));
  registerFeature(std::make_shared<CreationGamePlugins>(moInfo));
  registerFeature(std::make_shared<Fallout76UnmangedMods>(this));

  return true;
}

QString GameFallout76::gameName() const
{
  return u"Fallout 76"_s;
}

void GameFallout76::detectGame()
{
  m_GamePath    = identifyGamePath();
  m_MyGamesPath = determineMyGamesPath(gameName());
}

QList<ExecutableInfo> GameFallout76::executables() const
{
  return QList<ExecutableInfo>()
         << ExecutableInfo(u"Fallout 76"_s, findInGameFolder(binaryName()));
}

QList<ExecutableForcedLoadSetting> GameFallout76::executableForcedLoads() const
{
  return {};
}

QString GameFallout76::name() const
{
  return u"Fallout 76 Support Plugin"_s;
}

QString GameFallout76::author() const
{
  return u"Mod Organizer Team; EntranceJew"_s;
}

QString GameFallout76::description() const
{
  return tr("Adds support for the game Fallout 76.\n"
            "Splash by %1")
      .arg(u"nekoyoubi"_s);
}

MOBase::VersionInfo GameFallout76::version() const
{
  return VersionInfo(1, 0, 0, VersionInfo::RELEASE_FINAL);
}

QList<PluginSetting> GameFallout76::settings() const
{
  return QList<PluginSetting>();
}

void GameFallout76::initializeProfile(const QDir& path, ProfileSettings settings) const
{
  if (settings.testFlag(IPluginGame::MODS)) {
    copyToProfile(localAppFolder() + u"/Fallout76"_s, path, u"plugins.txt"_s);
    copyToProfile(localAppFolder() + u"/Fallout76"_s, path, u"loadorder.txt"_s);
  }

  if (settings.testFlag(IPluginGame::CONFIGURATION)) {
    if (settings.testFlag(IPluginGame::PREFER_DEFAULTS) ||
        !QFileInfo::exists(myGamesPath() % u"/Fallout76.ini"_s)) {
      copyToProfile(gameDirectory().absolutePath(), path, u"Fallout76_default.ini"_s,
                    u"Fallout76.ini"_s);
    } else {
      copyToProfile(myGamesPath(), path, u"Fallout76.ini"_s);
    }

    copyToProfile(myGamesPath(), path, u"Fallout76Prefs.ini"_s);
    copyToProfile(myGamesPath(), path, u"Fallout76Custom.ini"_s);
  }
}

QString GameFallout76::savegameExtension() const
{
  return "";
}

QString GameFallout76::savegameSEExtension() const
{
  return "";
}

std::vector<std::shared_ptr<const MOBase::ISaveGame>>
GameFallout76::listSaves([[maybe_unused]] QDir folder) const
{
  return {};
}

QString GameFallout76::identifyGamePath() const
{
  return parseSteamLocation(steamAPPId(), gameShortName());
}

std::shared_ptr<const GamebryoSaveGame> GameFallout76::makeSaveGame(QString) const
{
  return nullptr;
}

QString GameFallout76::steamAPPId() const
{
  return u"1151340"_s;
}

QStringList GameFallout76::primaryPlugins() const
{
  QStringList plugins = {u"SeventySix.esm"_s};

  plugins.append(CCPlugins());

  return plugins;
}

QStringList GameFallout76::gameVariants() const
{
  return {u"Regular"_s};
}

QString GameFallout76::gameShortName() const
{
  return u"Fallout76"_s;
}

QString GameFallout76::gameNexusName() const
{
  return u"Fallout76"_s;
}

QStringList GameFallout76::iniFiles() const
{
  return {u"Fallout76.ini"_s, u"Fallout76Prefs.ini"_s, u"Fallout76Custom.ini"_s};
}

QStringList GameFallout76::DLCPlugins() const
{
  return {};
}

QStringList GameFallout76::CCPlugins() const
{
  QStringList plugins = {};
  QFile file(gameDirectory().absoluteFilePath(u"Fallout76.ccc"_s));
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

IPluginGame::SortMechanism GameFallout76::sortMechanism() const
{
  return IPluginGame::SortMechanism::NONE;
}

IPluginGame::LoadOrderMechanism GameFallout76::loadOrderMechanism() const
{
  return IPluginGame::LoadOrderMechanism::PluginsTxt;
}

int GameFallout76::nexusModOrganizerID() const
{
  return -1;
}

int GameFallout76::nexusGameID() const
{
  return 2590;
}
