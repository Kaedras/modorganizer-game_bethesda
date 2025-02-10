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

#include <uibase/peextractor.h>

#include <QBuffer>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QIcon>
#include <QJsonDocument>
#include <QSettings>
#include <QStandardPaths>

#include <QtDebug>
#include <QtGlobal>

#include <optional>
#include <string>
#include <vector>

#include "stub.h"

WORD GameGamebryo::getArch(QString const& program) const
{
  STUB();
  return 0;
}

QString GameGamebryo::identifyGamePath() const
{
  // perhaps try to look into prefix registry?
  // we could get the prefix path from steam/config/libraryfolders.vdf
  // but appid would be required
  // todo: check if steamAPPId() can be called at this point
  //  if not, get appid from gameShortName() instead?
  STUB();
  return {};
}

/*static*/ QString GameGamebryo::getLootPath()
{
  STUB();
  return {};
}

std::unique_ptr<BYTE[]> GameGamebryo::getRegValue(HKEY key, LPCWSTR path, LPCWSTR value,
                                                  DWORD flags, LPDWORD type = nullptr)
{
  STUB();
  return {};
}

QString GameGamebryo::findInRegistry(HKEY baseKey, LPCWSTR path, LPCWSTR value)
{
  STUB();
  return {};
}

QString GameGamebryo::getKnownFolderPath(REFKNOWNFOLDERID folderId, bool useDefault)
{
  // use folderId as QStandardPaths::StandardLocation
  return QStandardPaths::standardLocations(
             static_cast<QStandardPaths::StandardLocation>(folderId)).first();
}

QString GameGamebryo::getSpecialPath(const QString& name)
{
  STUB();
  return {};
}

QString GameGamebryo::determineMyGamesPath(const QString& gameName)
{
  const QString pattern = "%1/My Games/" + gameName;

  auto tryDir = [&](const QString& dir) -> std::optional<QString> {
    if (dir.isEmpty()) {
      return {};
    }

    const auto path = pattern.arg(dir);
    if (!QFileInfo(path).exists()) {
      return {};
    }

    return path;
  };

  // check inside user directory
  if (auto d = tryDir(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first())) {
    return *d;
  }

  // TODO: check inside wine prefix

  return {};
}

QString GameGamebryo::parseEpicGamesLocation(const QStringList& manifests)
{
  STUB();
  return {};
}

QString GameGamebryo::parseSteamLocation(const QString& appid,
                                         const QString& directoryName)
{
  QString steamLibraryLocation;
  QString steamLibraries =
      QDir::homePath() + QStringLiteral("/.steam/steam/config/libraryfolders.vdf");
  if (QFile(steamLibraries).exists()) {
    std::ifstream file(steamLibraries.toStdString());
    auto root = tyti::vdf::read(file);
    for (auto child : root.childs) {
      tyti::vdf::object* library = child.second.get();
      auto apps                  = library->childs["apps"];
      if (apps->attribs.contains(appid.toStdString())) {
        steamLibraryLocation = QString::fromStdString(library->attribs["path"]);
        break;
      }
    }
  }
  if (!steamLibraryLocation.isEmpty()) {
    QString gameLocation = steamLibraryLocation + "/steamapps/common/" + directoryName;
    if (QDir(gameLocation).exists())
      return gameLocation;
  }

  return {};
}

QString GameGamebryo::getFileVersion(const QString& fileName)
{
  QFile f(fileName);
  QBuffer buf;
  if (!f.open(QIODeviceBase::ReadOnly) || !buf.open(QIODeviceBase::ReadWrite)) {
    return {};
  }
  if (!PeExtractor::loadVersionData(&f, &buf)) {
    return {};
  }
  buf.seek(0);
  QDataStream stream(&buf);
  QString fileVersion, productVersion;
  stream >> fileVersion >> productVersion;
  return fileVersion;
}