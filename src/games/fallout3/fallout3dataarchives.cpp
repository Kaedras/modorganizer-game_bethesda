#include "fallout3dataarchives.h"

#include "iprofile.h"
#include <utility.h>

using namespace Qt::Literals::StringLiterals;

QStringList Fallout3DataArchives::vanillaArchives() const
{
  return {u"Fallout - Textures.bsa"_s, u"Fallout - Meshes.bsa"_s,     u"Fallout - Voices.bsa"_s,
          u"Fallout - Sound.bsa"_s,    u"Fallout - MenuVoices.bsa"_s, u"Fallout - Misc.bsa"_s};
}

QStringList Fallout3DataArchives::archives(const MOBase::IProfile* profile) const
{
  QStringList result;

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"fallout.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"fallout.ini"_s);
  result.append(getArchivesFromKey(iniFile, u"SArchiveList"_s));

  return result;
}

void Fallout3DataArchives::writeArchiveList(MOBase::IProfile* profile,
                                            const QStringList& before)
{
  QString list = before.join(u", "_s);

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"fallout.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"fallout.ini"_s);
  setArchivesToKey(iniFile, u"SArchiveList"_s, list);
}
