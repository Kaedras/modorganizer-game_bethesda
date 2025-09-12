#include "../gamegamebryo.h"
#include "vdf_parser.h"

#if (QT_VERSION < QT_VERSION_CHECK(6, 8, 0))
#include <QDirIterator>
#else
#include <QDirListing>
#endif

#include <steamutility.h>

#include <QStandardPaths>
#include <QString>
#include <iostream>
#include <utility.h>

#define STUB() std::cout << __FUNCTION__ << ": STUB\n"

using namespace Qt::StringLiterals;

namespace
{

QString findFileName(QString const& path, QString const& fileName)
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 8, 0))
  QDirIterator it(path);

  while (it.hasNext()) {
    QFileInfo info(it.nextFileInfo());
    if (info.isFile()) {
      if (info.fileName().compare(fileName, Qt::CaseInsensitive) == 0) {
        return info.fileName();
      }
    }
  }
#else
  for (const auto& dirEntry : QDirListing(path, QDirListing::IteratorFlag::FilesOnly)) {
    if (dirEntry.fileName().compare(fileName, Qt::CaseInsensitive) == 0) {
      return dirEntry.fileName();
    }
  }
#endif
  return fileName;
}

}  // namespace

QString GameGamebryo::identifyGamePath() const
{
  return parseSteamLocation(steamAPPId(), gameShortName());
}

QString GameGamebryo::getLootPath()
{
  STUB();
  return {};
}

void GameGamebryo::copyToProfile(QString const& sourcePath,
                                 QDir const& destinationDirectory,
                                 QString const& sourceFileName)
{
  QString src = findFileName(sourcePath, sourceFileName);
  copyToProfile(sourcePath, destinationDirectory, src, src);
}

void GameGamebryo::copyToProfile(QString const& sourcePath,
                                 QDir const& destinationDirectory,
                                 QString const& sourceFileName,
                                 QString const& destinationFileName)
{
  QString srcName  = findFileName(sourcePath, sourceFileName);
  QString filePath = destinationDirectory.absoluteFilePath(destinationFileName);
  if (!QFileInfo(filePath).exists()) {
    if (!MOBase::shellCopy(sourcePath + "/" + srcName, filePath)) {
      // if copy file fails, create the file empty
      QFile(filePath).open(QIODevice::WriteOnly);
    }
  }
}

QString GameGamebryo::localAppFolder(const QString& appID)
{
  QString compatData = MOBase::findCompatDataByAppID(appID);

  QString path =
      compatData % QStringLiteral("/pfx/drive_c/users/steamuser/AppData/Local/");
  if (QFile::exists(path)) {
    return path;
  }

  return {};
}

std::unique_ptr<BYTE[]> GameGamebryo::getRegValue(HKEY, LPCWSTR, LPCWSTR, DWORD,
                                                  LPDWORD)
{
  // this function is probably not needed
  return {};
}

QString GameGamebryo::findInRegistry(HKEY, LPCWSTR, LPCWSTR)
{
  // this function is probably not needed
  return {};
}

QString GameGamebryo::getKnownFolderPath(REFKNOWNFOLDERID folderId, bool)
{
  // use folderId as QStandardPaths::StandardLocation
  return QStandardPaths::standardLocations(
             static_cast<QStandardPaths::StandardLocation>(folderId))
      .first();
}

QString GameGamebryo::getSpecialPath(const QString&)
{
  // this function is probably not needed
  return {};
}

QString GameGamebryo::determineMyGamesPath(const QString& gameName)
{
  const QString pattern = "%1/My Games/"_L1 % gameName;

  auto tryDir = [&](const QString& dir) -> std::optional<QString> {
    if (dir.isEmpty()) {
      return {};
    }

    auto path = pattern.arg(dir);
    if (!QFileInfo::exists(path)) {
      return {};
    }

    return path;
  };

  // check inside user directory
  if (auto d =
          tryDir(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
                     .first())) {
    return *d;
  }

  return {};
}
QString GameGamebryo::determineMyGamesPath(const QString& gameName,
                                           const QString& appID)
{
  const QString pattern = "%1/My Games/"_L1 % gameName;

  auto tryDir = [&](const QString& dir) -> std::optional<QString> {
    if (dir.isEmpty()) {
      return {};
    }

    auto path = pattern.arg(dir);
    if (!QFileInfo::exists(path)) {
      return {};
    }

    return path;
  };

  // check inside wine prefix
  QString steamLocation = parseSteamLocation(appID, gameName);

  QDir steamuserDocumentsDir = QDir(steamLocation);
  if (!steamuserDocumentsDir.cd(
          QStringLiteral("../../compatdata/%1/pfx/drive_c/users/steamuser/Documents")
              .arg(appID))) {
    return {};
  }

  if (auto d = tryDir(steamuserDocumentsDir.absolutePath())) {
    return *d;
  }

  // check inside user directory
  if (auto d =
          tryDir(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
                     .first())) {
    return *d;
  }
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
      QDir::homePath() % QStringLiteral("/.steam/steam/config/libraryfolders.vdf");
  if (QFile(steamLibraries).exists()) {
    std::ifstream file(steamLibraries.toStdString());
    auto root = tyti::vdf::read(file);
    for (const auto& child : root.childs) {
      tyti::vdf::object* library = child.second.get();
      auto apps                  = library->childs["apps"];
      if (apps->attribs.contains(appid.toStdString())) {
        steamLibraryLocation = QString::fromStdString(library->attribs["path"]);
        break;
      }
    }
  }
  if (!steamLibraryLocation.isEmpty()) {
    QString gameLocation =
        steamLibraryLocation % u"/steamapps/common/"_s % directoryName;
    if (QDir(gameLocation).exists())
      return gameLocation;
  }

  return {};
}
