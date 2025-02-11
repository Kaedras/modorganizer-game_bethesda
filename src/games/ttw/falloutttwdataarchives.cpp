#include "falloutttwdataarchives.h"
#include <utility.h>

using namespace Qt::Literals::StringLiterals;

QStringList FalloutTTWDataArchives::vanillaArchives() const
{
  return {u"Fallout - Textures.bsa"_s, u"Fallout - Textures2.bsa"_s, u"Fallout - Meshes.bsa"_s,
          u"Fallout - Voices1.bsa"_s,  u"Fallout - Sound.bsa"_s,     u"Fallout - Misc.bsa"_s};
}

QStringList FalloutTTWDataArchives::archives(const MOBase::IProfile* profile) const
{
  QStringList result;

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"fallout.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"fallout.ini"_s);
  result.append(getArchivesFromKey(iniFile, u"SArchiveList"_s));

  return result;
}

void FalloutTTWDataArchives::writeArchiveList(MOBase::IProfile* profile,
                                              const QStringList& before)
{
  QString list = before.join(u", "_s);

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"fallout.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"fallout.ini"_s);
  setArchivesToKey(iniFile, u"SArchiveList"_s, list);
}
