#include "gamebryounmanagedmods.h"
#include "gamegamebryo.h"
#include <pluginsetting.h>

using namespace Qt::Literals::StringLiterals;

GamebryoUnmangedMods::GamebryoUnmangedMods(const GameGamebryo* game) : m_Game(game) {}

GamebryoUnmangedMods::~GamebryoUnmangedMods() {}

QStringList GamebryoUnmangedMods::mods(bool onlyOfficial) const
{
  QStringList result;

  QStringList dlcPlugins  = m_Game->DLCPlugins();
  QStringList mainPlugins = m_Game->primaryPlugins();

  QDir dataDir(m_Game->dataDirectory());
  for (const QString& fileName :
       dataDir.entryList({u"*.esp"_s, u"*.esl"_s, u"*.esm"_s})) {
    if (!mainPlugins.contains(fileName, Qt::CaseInsensitive) &&
        (!onlyOfficial || dlcPlugins.contains(fileName, Qt::CaseInsensitive))) {
      result.append(fileName.chopped(4));  // trims the extension off
    }
  }

  return result;
}

QString GamebryoUnmangedMods::displayName(const QString& modName) const
{
  return modName;
}

QFileInfo GamebryoUnmangedMods::referenceFile(const QString& modName) const
{
  QFileInfoList files = m_Game->dataDirectory().entryInfoList({modName % u".es*"_s});
  if (files.size() > 0) {
    return files.at(0);
  } else {
    return QFileInfo();
  }
}

QStringList GamebryoUnmangedMods::secondaryFiles(const QString& modName) const
{
  QStringList archives;
  QDir dataDir = m_Game->dataDirectory();
  for (const QString& archiveName : dataDir.entryList({modName % u"*.bsa"_s})) {
    archives.append(dataDir.absoluteFilePath(archiveName));
  }
  return archives;
}
