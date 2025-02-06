#include "fo4londonbsainvalidation.h"

#include "dummybsa.h"
#include "iplugingame.h"
#include "iprofile.h"

#include <QSettings>
#include <imoinfo.h>
#include <utility.h>

Fallout4LondonBSAInvalidation::Fallout4LondonBSAInvalidation(
    MOBase::DataArchives* dataArchives, MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, "Fallout4Custom.ini", game)
{
  m_IniFileName = "Fallout4Custom.ini";
  m_Game        = game;
}

bool Fallout4LondonBSAInvalidation::isInvalidationBSA(const QString& bsaName)
{
  return false;
}

QString Fallout4LondonBSAInvalidation::invalidationBSAName() const
{
  return "";
}

unsigned long Fallout4LondonBSAInvalidation::bsaVersion() const
{
  return 0x68;
}

bool Fallout4LondonBSAInvalidation::prepareProfile(MOBase::IProfile* profile)
{
  bool dirty          = false;
  QString basePath    = profile->localSettingsEnabled()
                            ? profile->absolutePath()
                            : m_Game->documentsDirectory().absolutePath();
  QString iniFilePath = basePath + "/" + m_IniFileName;

  QSettings ini(iniFilePath, QSettings::IniFormat);

  QString bInvalidateOlderFiles = QStringLiteral("Archive/bInvalidateOlderFiles");
  QString sResourceDataDirsFinal = QStringLiteral("Archive/sResourceDataDirsFinal");

  if (profile->invalidationActive(nullptr)) {
    // write bInvalidateOlderFiles = 1, if needed
    if (ini.value(bInvalidateOlderFiles, "0").toString() != "1") {
      dirty = true;
      ini.setValue(bInvalidateOlderFiles, "1");
      if (ini.status() != QSettings::NoError) {
        qWarning("failed to override data directory in \"%s\"",
                 qUtf8Printable(m_IniFileName));
      }
    }
    if (ini.value(sResourceDataDirsFinal, "STRINGS\\").toString() != "0") {
      dirty = true;
      ini.setValue(sResourceDataDirsFinal, "");
      if (ini.status() != QSettings::NoError) {
        qWarning("failed to override data directory in \"%s\"",
                 qUtf8Printable(m_IniFileName));
      }
    }
  }

  return dirty;
}
