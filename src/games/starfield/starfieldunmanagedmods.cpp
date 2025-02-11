#include "starfieldunmanagedmods.h"

#include "log.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

using namespace Qt::Literals::StringLiterals;

StarfieldUnmanagedMods::StarfieldUnmanagedMods(const GameStarfield* game,
                                               const QString& appDataFolder)
    : GamebryoUnmangedMods(game), m_AppDataFolder(appDataFolder)
{}

StarfieldUnmanagedMods::~StarfieldUnmanagedMods() {}

QStringList StarfieldUnmanagedMods::mods(bool onlyOfficial) const
{
  QStringList result;

  QStringList pluginList   = game()->primaryPlugins() + game()->enabledPlugins();
  QStringList otherPlugins = game()->DLCPlugins();
  otherPlugins.append(game()->CCPlugins());
  for (const QString& plugin : otherPlugins) {
    pluginList.removeAll(plugin);
  }
  QMap<QString, QDir> directories = {{u"data"_s, game()->dataDirectory()}};
  directories.insert(game()->secondaryDataDirectories());
  for (const QDir& directory : directories) {
    for (const QString& fileName : directory.entryList({u"*.esp"_s, u"*.esl"_s, u"*.esm"_s})) {
      if (!pluginList.contains(fileName, Qt::CaseInsensitive)) {
        if (!onlyOfficial || pluginList.contains(fileName, Qt::CaseInsensitive)) {
          result.append(fileName.chopped(4));  // trims the extension off
        }
      }
    }
  }

  return result;
}

QFileInfo StarfieldUnmanagedMods::referenceFile(const QString& modName) const
{
  QFileInfoList files;
  QMap<QString, QDir> directories = {{u"data"_s, game()->dataDirectory()}};
  directories.insert(game()->secondaryDataDirectories());
  for (const QDir& directory : directories) {
    files += directory.entryInfoList({modName % u".es*"_s});
  }
  if (files.size() > 0) {
    return files.at(0);
  } else {
    return QFileInfo();
  }
}

std::map<QString, StarfieldUnmanagedMods::ContentCatalog>
StarfieldUnmanagedMods::parseContentCatalog() const
{
  QFile content(m_AppDataFolder % '/' % game()->gameShortName() %
                u"/ContentCatalog.txt"_s);
  std::map<QString, StarfieldUnmanagedMods::ContentCatalog> contentCatalog;
  if (content.open(QIODevice::OpenModeFlag::ReadOnly)) {
    auto contentData      = content.readAll();
    QString convertedData = QString::fromLatin1(contentData);
    contentData           = convertedData.toUtf8();
    QJsonParseError jsonError;
    QJsonDocument contentDoc = QJsonDocument::fromJson(contentData, &jsonError);
    if (jsonError.error) {
      MOBase::log::warn(QObject::tr("ContentCatalog.txt appears to be corrupt: %1")
                            .arg(jsonError.errorString()));
    } else {
      QJsonObject contentObj = contentDoc.object();
      for (const auto& mod : contentObj.keys()) {
        if (mod == u"ContentCatalog"_s)
          continue;
        auto modInfo = contentObj.value(mod).toObject();
        QStringList pluginList;
        QStringList files;
        for (const auto& file : modInfo.value(u"Files"_s).toArray()) {
          QString fileName = file.toString();
          files.append(fileName);
          if (fileName.endsWith(u".esm"_s, Qt::CaseInsensitive) ||
              fileName.endsWith(u".esl"_s, Qt::CaseInsensitive) ||
              fileName.endsWith(u".esp"_s, Qt::CaseInsensitive)) {
            pluginList.append(fileName);
          }
        }
        for (const auto& plugin : pluginList) {
          contentCatalog[plugin]       = ContentCatalog();
          contentCatalog[plugin].files = files;
          contentCatalog[plugin].name  = modInfo.value(u"Title"_s).toString();
        }
      }
    }
  }
  return contentCatalog;
}

QStringList StarfieldUnmanagedMods::secondaryFiles(const QString& modName) const
{
  QStringList files;
  auto contentCatalog = parseContentCatalog();
  for (const auto& mod : contentCatalog) {
    if (mod.first.startsWith(modName, Qt::CaseInsensitive)) {
      files += mod.second.files;
      break;
    }
  }
  // file extension in FO4 is .ba2 instead of bsa
  QMap<QString, QDir> directories = {{u"data"_s, game()->dataDirectory()}};
  directories.insert(game()->secondaryDataDirectories());
  for (const QDir& directory : directories) {
    for (const QString& archiveName : directory.entryList({modName % u"*.ba2"_s})) {
      files.append(directory.absoluteFilePath(archiveName));
    }
  }
  return files;
}

QString StarfieldUnmanagedMods::displayName(const QString& modName) const
{
  auto contentCatalog = parseContentCatalog();
  for (const auto& mod : contentCatalog) {
    if (mod.first.startsWith(modName, Qt::CaseInsensitive)) {
      return mod.second.name;
    }
  }
  // unlike in earlier games, in fallout 4 the file name doesn't correspond to
  // the public name
  return modName;
}
