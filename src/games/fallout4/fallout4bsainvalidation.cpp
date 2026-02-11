#include "fallout4bsainvalidation.h"

#include "dummybsa.h"
#include "iplugingame.h"
#include "iprofile.h"
#include "registry.h"
#include <imoinfo.h>
#include <utility.h>

using namespace MOBase;
using namespace Qt::StringLiterals;

Fallout4BSAInvalidation::Fallout4BSAInvalidation(MOBase::DataArchives* dataArchives,
                                                 MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, "Fallout4Custom.ini", game)
{
  m_IniFileName = "Fallout4Custom.ini";
  m_Game        = game;
}

bool Fallout4BSAInvalidation::isInvalidationBSA(const QString& bsaName)
{
  return false;
}

QString Fallout4BSAInvalidation::invalidationBSAName() const
{
  return "";
}

uint32_t Fallout4BSAInvalidation::bsaVersion() const
{
  return 0x68;
}

bool Fallout4BSAInvalidation::prepareProfile(MOBase::IProfile* profile)
{
  bool dirty          = false;
  QString basePath    = profile->localSettingsEnabled()
                            ? profile->absolutePath()
                            : m_Game->documentsDirectory().absolutePath();
  QString iniFilePath = basePath + "/" + m_IniFileName;

  if (profile->invalidationActive(nullptr)) {
    // write bInvalidateOlderFiles = 1, if needed
    QString setting = ReadRegistryValue(u"Archive"_s, u"bInvalidateOlderFiles"_s,
                                        u"0"_s, iniFilePath);
    if (setting != "1"_L1) {
      dirty = true;
      if (!WriteRegistryValue(u"Archive"_s, u"bInvalidateOlderFiles"_s, u"1"_s,
                              iniFilePath)) {
        qWarning("failed to override data directory in \"%s\"",
                 qUtf8Printable(m_IniFileName));
      }
    }
    setting = ReadRegistryValue(u"Archive"_s, u"sResourceDataDirsFinal"_s,
                                u"STRINGS\\"_s, iniFilePath);
    if (setting != "0"_L1) {
      dirty = true;
      if (!WriteRegistryValue(u"Archive"_s, u"sResourceDataDirsFinal"_s, u""_s,
                              iniFilePath)) {
        qWarning("failed to override data directory in \"%s\"",
                 qUtf8Printable(m_IniFileName));
      }
    }
  }

  return dirty;
}
