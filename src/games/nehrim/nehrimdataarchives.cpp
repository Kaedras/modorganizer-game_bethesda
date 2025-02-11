#include "nehrimdataarchives.h"
#include <utility.h>

using namespace Qt::Literals::StringLiterals;

QStringList NehrimDataArchives::vanillaArchives() const
{
  return {u"N - Meshes.bsa"_s, u"N - Textures1.bsa"_s, u"N - Textures2.bsa"_s, u"N - Misc.bsa"_s,
          u"N - Sounds.bsa"_s, u"L - Voices.bsa"_s,    u"L - Misc.bsa"_s};
}

QStringList NehrimDataArchives::archives(const MOBase::IProfile* profile) const
{
  QStringList result;

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"oblivion.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"oblivion.ini"_s);
  result.append(getArchivesFromKey(iniFile, u"SArchiveList"_s));

  return result;
}

void NehrimDataArchives::writeArchiveList(MOBase::IProfile* profile,
                                          const QStringList& before)
{
  QString list = before.join(u", "_s);

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"oblivion.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"oblivion.ini"_s);
  setArchivesToKey(iniFile, u"SArchiveList"_s, list);
}
