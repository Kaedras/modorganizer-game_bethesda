#include "enderalsedataarchives.h"

#include "iprofile.h"
#include <utility.h>

using namespace Qt::Literals::StringLiterals;

QStringList EnderalSEDataArchives::vanillaArchives() const
{
  return {u"Skyrim - Textures0.bsa"_s,
          u"Skyrim - Textures1.bsa"_s,
          u"Skyrim - Textures2.bsa"_s,
          u"Skyrim - Textures3.bsa"_s,
          u"Skyrim - Textures4.bsa"_s,
          u"Skyrim - Textures5.bsa"_s,
          u"Skyrim - Textures6.bsa"_s,
          u"Skyrim - Textures7.bsa"_s,
          u"Skyrim - Textures8.bsa"_s,
          u"Skyrim - Meshes0.bsa"_s,
          u"Skyrim - Meshes1.bsa"_s,
          u"Skyrim - Voices_en0.bsa"_s,
          u"Skyrim - Sounds.bsa"_s,
          u"Skyrim - Interface.bsa"_s,
          u"Skyrim - Animations.bsa"_s,
          u"Skyrim - Shaders.bsa"_s,
          u"Skyrim - Misc.bsa"_s,
          u"E - Meshes.bsa"_s,
          u"E - SE.bsa"_s,
          u"E - Scripts.bsa"_s,
          u"E - Sounds.bsa"_s,
          u"E - Textures1.bsa"_s,
          u"E - Textures2.bsa"_s,
          u"E - Textures3.bsa"_s,
          u"L - Textures.bsa"_s,
          u"L - Voices.bsa"_s};
}

QStringList EnderalSEDataArchives::archives(const MOBase::IProfile* profile) const
{
  QStringList result;

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"Enderal.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"Enderal.ini"_s);
  result.append(getArchivesFromKey(iniFile, u"SResourceArchiveList"_s));
  result.append(getArchivesFromKey(iniFile, u"SResourceArchiveList2"_s));

  return result;
}

void EnderalSEDataArchives::writeArchiveList(MOBase::IProfile* profile,
                                             const QStringList& before)
{
  QString list = before.join(',');

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"Enderal.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"Enderal.ini"_s);
  if (list.length() > 511) {
    int splitIdx = list.lastIndexOf(',', 512);
    setArchivesToKey(iniFile, u"SResourceArchiveList"_s, list.mid(0, splitIdx));
    setArchivesToKey(iniFile, u"SResourceArchiveList2"_s, list.mid(splitIdx + 2));
  } else {
    setArchivesToKey(iniFile, u"SResourceArchiveList"_s, list);
  }
}
