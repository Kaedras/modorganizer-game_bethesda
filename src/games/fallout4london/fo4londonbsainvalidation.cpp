#include "fo4londonbsainvalidation.h"

#include "dummybsa.h"
#include "iplugingame.h"
#include "iprofile.h"

#include <QSettings>
#include <imoinfo.h>
#include <utility.h>

using namespace Qt::Literals::StringLiterals;

Fallout4LondonBSAInvalidation::Fallout4LondonBSAInvalidation(
    MOBase::DataArchives* dataArchives, MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, u"Fallout4Custom.ini"_s, game)
{
  m_IniFileName = u"Fallout4Custom.ini"_s;
  m_Game        = game;
}

bool Fallout4LondonBSAInvalidation::isInvalidationBSA(const QString& bsaName)
{
  return false;
}

QString Fallout4LondonBSAInvalidation::invalidationBSAName() const
{
  return u""_s;
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
  QString iniFilePath = basePath % '/' % m_IniFileName;

  QSettings ini(iniFilePath, QSettings::IniFormat);

  if (profile->invalidationActive(nullptr)) {
    // write bInvalidateOlderFiles = 1, if needed
    if (ini.value(u"Archive/bInvalidateOlderFiles"_s, 0).toInt() != 1) {
      dirty = true;
      ini.setValue(u"Archive/bInvalidateOlderFiles"_s, 1);
      if (ini.status() != QSettings::NoError) {
        qWarning("failed to override data directory in \"%s\"",
                 qUtf8Printable(m_IniFileName));
      }
    }
    if (ini.value(u"Archive/sResourceDataDirsFinal"_s, u"STRINGS\\"_s).toString() != u"0"_s) {
      dirty = true;
      ini.setValue(u"Archive/sResourceDataDirsFinal"_s, u""_s);
      if (ini.status() != QSettings::NoError) {
        qWarning("failed to override data directory in \"%s\"",
                 qUtf8Printable(m_IniFileName));
      }
    }
  }

  return dirty;
}
