#include "obliviondataarchives.h"
#include <utility.h>

using namespace Qt::Literals::StringLiterals;

QStringList OblivionDataArchives::vanillaArchives() const
{
  return {u"Oblivion - Misc.bsa"_s,    u"Oblivion - Textures - Compressed.bsa"_s,
          u"Oblivion - Meshes.bsa"_s,  u"Oblivion - Sounds.bsa"_s,
          u"Oblivion - Voices1.bsa"_s, u"Oblivion - Voices2.bsa"_s};
}

QStringList OblivionDataArchives::archives(const MOBase::IProfile* profile) const
{
  QStringList result;

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"oblivion.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"oblivion.ini"_s);
  result.append(getArchivesFromKey(iniFile, u"SArchiveList"_s));

  return result;
}

void OblivionDataArchives::writeArchiveList(MOBase::IProfile* profile,
                                            const QStringList& before)
{
  QString list = before.join(u", "_s);

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"oblivion.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"oblivion.ini"_s);
  setArchivesToKey(iniFile, u"SArchiveList"_s, list);
}
