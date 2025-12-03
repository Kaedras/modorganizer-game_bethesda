#include "../gamegamebryo.h"

#include "scopeguard.h"
#include "utility.h"
#include "vdf_parser.h"
#include <Knownfolders.h>
#include <QDirIterator>
#include <QJsonDocument>
#include <QStandardPaths>
#include <Shlobj.h>
#include <Windows.h>
#include <fstream>
#include <winreg.h>
#include <winver.h>

GameGamebryo::GameGamebryo() {}

QString GameGamebryo::identifyGamePath() const
{
  QString path = "Software\\Bethesda Softworks\\" + gameShortName();
  return findInRegistry(HKEY_LOCAL_MACHINE, path.toStdWString().c_str(),
                        L"Installed Path");
}

/*static*/ QString GameGamebryo::getLootPath()
{
  return findInRegistry(HKEY_LOCAL_MACHINE, L"Software\\LOOT", L"Installed Path") +
         "/Loot.exe";
}

QString GameGamebryo::localAppFolder() const
{
  return QStandardPaths::standardLocations(QStandardPaths::GenericConfigLocation)
      .first();
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
  if (!QFileInfo(filePath).exists()) {
    if (!MOBase::shellCopy(sourcePath + "/" + sourceFileName, filePath)) {
      // if copy file fails, create the file empty
      QFile(filePath).open(QIODevice::WriteOnly);
    }
  }
}

std::unique_ptr<BYTE[]> GameGamebryo::getRegValue(HKEY key, LPCWSTR path, LPCWSTR value,
                                                  DWORD flags, LPDWORD type = nullptr)
{
  DWORD size = 0;
  HKEY subKey;
  LONG res = ::RegOpenKeyExW(key, path, 0, KEY_QUERY_VALUE | KEY_WOW64_32KEY, &subKey);
  if (res != ERROR_SUCCESS) {
    res = ::RegOpenKeyExW(key, path, 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &subKey);
    if (res != ERROR_SUCCESS)
      return std::unique_ptr<BYTE[]>();
  }
  res = ::RegGetValueW(subKey, L"", value, flags, type, nullptr, &size);
  if (res == ERROR_FILE_NOT_FOUND || res == ERROR_UNSUPPORTED_TYPE) {
    return std::unique_ptr<BYTE[]>();
  }
  if (res != ERROR_SUCCESS && res != ERROR_MORE_DATA) {
    throw MOBase::MyException(
        QObject::tr("failed to query registry path (preflight): %1").arg(res, 0, 16));
  }

  std::unique_ptr<BYTE[]> result(new BYTE[size]);
  res = ::RegGetValueW(subKey, L"", value, flags, type, result.get(), &size);

  if (res != ERROR_SUCCESS) {
    throw MOBase::MyException(
        QObject::tr("failed to query registry path (read): %1").arg(res, 0, 16));
  }

  return result;
}

QString GameGamebryo::findInRegistry(HKEY baseKey, LPCWSTR path, LPCWSTR value)
{
  std::unique_ptr<BYTE[]> buffer =
      getRegValue(baseKey, path, value, RRF_RT_REG_SZ | RRF_NOEXPAND);

  return QString::fromUtf16(reinterpret_cast<const char16_t*>(buffer.get()));
}

QString GameGamebryo::getKnownFolderPath(REFKNOWNFOLDERID folderId, bool useDefault)
{
  PWSTR path = nullptr;
  ON_BLOCK_EXIT([&]() {
    if (path != nullptr)
      ::CoTaskMemFree(path);
  });

  if (::SHGetKnownFolderPath(folderId, useDefault ? KF_FLAG_DEFAULT_PATH : 0, NULL,
                             &path) == S_OK) {
    return QDir::fromNativeSeparators(QString::fromWCharArray(path));
  } else {
    return QString();
  }
}

QString GameGamebryo::getSpecialPath(const QString& name)
{
  QString base = findInRegistry(
      HKEY_CURRENT_USER,
      L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders",
      name.toStdWString().c_str());

  WCHAR temp[MAX_PATH];
  if (::ExpandEnvironmentStringsW(base.toStdWString().c_str(), temp, MAX_PATH) != 0) {
    return QString::fromWCharArray(temp);
  } else {
    return base;
  }
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

  // a) this is the way it should work. get the configured My Documents directory
  if (auto d = tryDir(getKnownFolderPath(FOLDERID_Documents, false))) {
    return *d;
  }

  // b) if there is no <game> directory there, look in the default directory
  if (auto d = tryDir(getKnownFolderPath(FOLDERID_Documents, true))) {
    return *d;
  }

  // c) finally, look in the registry. This is discouraged
  if (auto d = tryDir(getSpecialPath("Personal"))) {
    return *d;
  }

  return {};
}

QString GameGamebryo::parseEpicGamesLocation(const QStringList& manifests)
{
  // Use the registry entry to find the EGL Data dir first, just in case something
  // changes
  QString manifestDir = findInRegistry(
      HKEY_LOCAL_MACHINE, L"Software\\Epic Games\\EpicGamesLauncher", L"AppDataPath");
  if (manifestDir.isEmpty())
    manifestDir = getKnownFolderPath(FOLDERID_ProgramData, false) +
                  "\\Epic\\EpicGamesLauncher\\Data\\";
  manifestDir += "Manifests";
  QDir epicManifests(manifestDir, "*.item",
                     QDir::SortFlags(QDir::Name | QDir::IgnoreCase), QDir::Files);
  if (epicManifests.exists()) {
    QDirIterator it(epicManifests);
    while (it.hasNext()) {
      QString manifestFile = it.next();
      QFile manifest(manifestFile);

      if (!manifest.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open Epic Games manifest file.");
        continue;
      }

      QByteArray manifestData = manifest.readAll();

      QJsonDocument manifestJson(QJsonDocument::fromJson(manifestData));

      if (manifests.contains(manifestJson["AppName"].toString())) {
        return manifestJson["InstallLocation"].toString();
      }
    }
  }
  return "";
}
