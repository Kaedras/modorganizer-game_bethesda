#include "starfieldbsainvalidation.h"

#include "dummybsa.h"
#include "iplugingame.h"
#include "iprofile.h"
#include "registry.h"

#include <QSettings>
#include <imoinfo.h>
#include <utility.h>

using namespace Qt::Literals::StringLiterals;

StarfieldBSAInvalidation::StarfieldBSAInvalidation(MOBase::DataArchives* dataArchives,
                                                   MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, "StarfieldCustom.ini", game)
{
  m_IniFileName = u"StarfieldCustom.ini"_s;
  m_Game        = game;
}

bool StarfieldBSAInvalidation::isInvalidationBSA(const QString& bsaName)
{
  return false;
}

QString StarfieldBSAInvalidation::invalidationBSAName() const
{
  return "";
}

unsigned long StarfieldBSAInvalidation::bsaVersion() const
{
  return 0x68;
}

bool StarfieldBSAInvalidation::prepareProfile(MOBase::IProfile* profile)
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
    if (ini.value(u"Archive/sResourceDataDirsFinal"_s, u"STRINGS\\"_s).toString() != "0") {
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
