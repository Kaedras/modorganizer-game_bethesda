#include "gamebryobsainvalidation.h"

#include "dummybsa.h"
#include "iplugingame.h"
#include "iprofile.h"
#include "registry.h"
#include <imoinfo.h>
#include <utility.h>

#include <QDir>
#include <QStringList>

using namespace MOBase;
using namespace Qt::StringLiterals;

GamebryoBSAInvalidation::GamebryoBSAInvalidation(MOBase::DataArchives* dataArchives,
                                                 const QString& iniFilename,
                                                 MOBase::IPluginGame const* game)
    : m_DataArchives(dataArchives), m_IniFileName(iniFilename), m_Game(game)
{}

bool GamebryoBSAInvalidation::isInvalidationBSA(const QString& bsaName)
{
  static QStringList invalidation{invalidationBSAName()};

  for (const QString& file : invalidation) {
    if (file.compare(bsaName, Qt::CaseInsensitive) == 0) {
      return true;
    }
  }
  return false;
}

void GamebryoBSAInvalidation::deactivate(MOBase::IProfile* profile)
{
  prepareProfile(profile);
}

void GamebryoBSAInvalidation::activate(MOBase::IProfile* profile)
{
  prepareProfile(profile);
}

bool GamebryoBSAInvalidation::prepareProfile(MOBase::IProfile* profile)
{
  bool dirty          = false;
  QString basePath    = profile->localSettingsEnabled()
                            ? profile->absolutePath()
                            : m_Game->documentsDirectory().absolutePath();
  QString iniFilePath = basePath + "/" + m_IniFileName;

  // write bInvalidateOlderFiles = 1, if needed
  QString setting =
      ReadRegistryValue(u"Archive"_s, u"bInvalidateOlderFiles"_s, u"0"_s, iniFilePath);
  if (setting != "1"_L1) {
    dirty = true;
    if (!WriteRegistryValue(u"Archive"_s, u"bInvalidateOlderFiles"_s, u"1"_s,
                            iniFilePath)) {
      qWarning("failed to activate BSA invalidation in \"%s\"",
               qUtf8Printable(m_IniFileName));
    }
  }

  if (profile->invalidationActive(nullptr)) {

    // add the dummy bsa to the archive string, if needed
    QStringList archives = m_DataArchives->archives(profile);
    bool bsaInstalled    = false;
    for (const QString& archive : archives) {
      if (isInvalidationBSA(archive)) {
        bsaInstalled = true;
        break;
      }
    }
    if (!bsaInstalled) {
      m_DataArchives->addArchive(profile, 0, invalidationBSAName());
      dirty = true;
    }

    // create the dummy bsa if necessary
    QString bsaFile = m_Game->dataDirectory().absoluteFilePath(invalidationBSAName());
    if (!QFile::exists(bsaFile)) {
      DummyBSA bsa(bsaVersion());
      bsa.write(bsaFile);
      dirty = true;
    }

    // write SInvalidationFile = "", if needed
    setting = ReadRegistryValue(u"Archive"_s, u"SInvalidationFile"_s,
                                u"ArchiveInvalidation.txt"_s, iniFilePath);
    if (!setting.isEmpty()) {
      dirty = true;
      if (!WriteRegistryValue(u"Archive"_s, u"SInvalidationFile"_s, u""_s,
                              iniFilePath)) {
        qWarning("failed to activate BSA invalidation in \"%s\"",
                 qUtf8Printable(m_IniFileName));
      }
    }
  } else {

    // remove the dummy bsa from the archive string, if needed
    QStringList archivesBefore = m_DataArchives->archives(profile);
    for (const QString& archive : archivesBefore) {
      if (isInvalidationBSA(archive)) {
        m_DataArchives->removeArchive(profile, archive);
        dirty = true;
      }
    }

    // delete the dummy bsa, if needed
    QString bsaFile = m_Game->dataDirectory().absoluteFilePath(invalidationBSAName());
    if (QFile::exists(bsaFile)) {
      MOBase::shellDeleteQuiet(bsaFile);
      dirty = true;
    }

    // write SInvalidationFile = "ArchiveInvalidation.txt", if needed
    setting =
        ReadRegistryValue(u"Archive"_s, u"SInvalidationFile"_s, u""_s, iniFilePath);
    if (setting != "ArchiveInvalidation.txt"_L1) {
      dirty = true;
      if (!WriteRegistryValue(u"Archive"_s, u"SInvalidationFile"_s,
                              u"ArchiveInvalidation.txt"_s, iniFilePath)) {
        qWarning("failed to activate BSA invalidation in \"%s\"",
                 qUtf8Printable(m_IniFileName));
      }
    }
  }

  return dirty;
}
