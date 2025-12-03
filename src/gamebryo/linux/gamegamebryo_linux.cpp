#include "../gamegamebryo.h"
#include "util.h"
#include "vdf_parser.h"

#include <steamutility.h>

#include <QRegularExpression>
#include <QString>
#include <iostream>
#include <pwd.h>
#include <sys/types.h>
#include <utility.h>

#define STUB() std::cout << __FUNCTION__ << ": STUB\n"

using namespace MOBase;
using namespace Qt::StringLiterals;

// set the default variant to steam to enable getting the appid earlier
GameGamebryo::GameGamebryo() : m_GameVariant(u"Steam"_s) {}

void GameGamebryo::setPrefixPath(const QString& path)
{
  m_PrefixPath = path;

  // determine whether this prefix is for wine or proton
  // this is required because of minor differences, e.g. username
  static const QRegularExpression re(u"/steamapps/compatdata/[0-9]+"_s);
  if (re.match(m_PrefixPath).hasMatch()) {
    // assume proton if path contains "/steamapps/compatdata/<appid>"
    m_isProton = true;
    m_User     = u"steamuser"_s;

    // append /pfx to the prefix path
    if (!m_PrefixPath.endsWith("pfx"_L1) && !m_PrefixPath.endsWith("pfx/"_L1)) {
      m_PrefixPath += "/pfx"_L1;
    }
  } else {
    m_isProton = false;

    // there are usually only two user dirs: Public and the normal user
    QDir usersDir(m_PrefixPath % "/drive_c/users");
    for (const QString& user : usersDir.entryList(QDir::Dirs)) {
      if (user != "Public") {
        m_User = user;
        break;
      }
    }

    // check if getpwuid() returns something else
    // TODO: remove this later as it is mainly for testing
    const passwd* pwd = getpwuid(getuid());
    if (pwd != nullptr) {
      if (m_User != pwd->pw_name) {
        log::warn("Username mismatch: got {} from wine prefix and {} from getpwuid()",
                  m_User, pwd->pw_name);
      }
    }
  }

  // update m_MyGamesPath
  m_MyGamesPath = m_PrefixPath % "/drive_c/users/" % m_User % "/Documents/My Games";
}

QString GameGamebryo::identifyGamePath() const
{
  return parseSteamLocation(steamAPPId(), gameName());
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
  QString src = findFileNameCaseInsensitive(sourcePath, sourceFileName);
  copyToProfile(sourcePath, destinationDirectory, src, src);
}

void GameGamebryo::copyToProfile(QString const& sourcePath,
                                 QDir const& destinationDirectory,
                                 QString const& sourceFileName,
                                 QString const& destinationFileName)
{
  QString srcName  = findFileNameCaseInsensitive(sourcePath, sourceFileName);
  QString filePath = destinationDirectory.absoluteFilePath(destinationFileName);
  if (!QFileInfo::exists(filePath)) {
    if (!MOBase::shellCopy(sourcePath % "/"_L1 % srcName, filePath)) {
      // if copy file fails, create the file empty
      QFile(filePath).open(QIODevice::WriteOnly);
    }
  }
}

QString GameGamebryo::localAppFolder() const
{
  return QStringLiteral("%1/drive_c/users/%2/AppData/Local").arg(m_PrefixPath, m_User);
}

std::unique_ptr<BYTE[]> GameGamebryo::getRegValue(HKEY, LPCWSTR, LPCWSTR, DWORD,
                                                  LPDWORD)
{
  // no-op
  return {};
}

QString GameGamebryo::findInRegistry(HKEY, LPCWSTR, LPCWSTR)
{
  // no-op
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
  // no-op
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
