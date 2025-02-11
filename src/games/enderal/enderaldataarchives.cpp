#include "enderaldataarchives.h"
#include <utility.h>

using namespace Qt::Literals::StringLiterals;

QStringList EnderalDataArchives::vanillaArchives() const
{
  return {u"Skyrim - Misc.bsa"_s,      u"Skyrim - Shaders.bsa"_s,    u"Skyrim - Textures.bsa"_s,
          u"Skyrim - Interface.bsa"_s, u"Skyrim - Animations.bsa"_s, u"Skyrim - Meshes.bsa"_s,
          u"Skyrim - Sounds.bsa"_s,    u"E - Meshes.bsa"_s,          u"E - Music.bsa"_s,
          u"E - Scripts.bsa"_s,        u"E - Sounds.bsa"_s,          u"E - Textures1.bsa"_s,
          u"E - Textures2.bsa"_s,      u"E - Textures3.bsa"_s,       u"L - Textures.bsa"_s,
          u"L - Voices.bsa"_s};
}

QStringList EnderalDataArchives::archives(const MOBase::IProfile* profile) const
{
  QStringList result;

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"Enderal.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"Enderal.ini"_s);
  result.append(getArchivesFromKey(iniFile, u"SResourceArchiveList"_s));
  result.append(getArchivesFromKey(iniFile, u"SResourceArchiveList2"_s));

  return result;
}

void EnderalDataArchives::writeArchiveList(MOBase::IProfile* profile,
                                           const QStringList& before)
{
  QString list = before.join(u", "_s);

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"Enderal.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"Enderal.ini"_s);
  if (list.length() > 255) {
    int splitIdx = list.lastIndexOf(',', 256);
    setArchivesToKey(iniFile, u"SResourceArchiveList"_s, list.mid(0, splitIdx));
    setArchivesToKey(iniFile, u"SResourceArchiveList2"_s, list.mid(splitIdx + 2));
  } else {
    setArchivesToKey(iniFile, u"SResourceArchiveList"_s, list);
  }
}
