#include "skyrimdataarchives.h"
#include <utility.h>

using namespace Qt::Literals::StringLiterals;

QStringList SkyrimDataArchives::vanillaArchives() const
{
  return {u"Skyrim - Misc.bsa"_s,        u"Skyrim - Shaders.bsa"_s,
          u"Skyrim - Textures.bsa"_s,    u"HighResTexturePack01.bsa"_s,
          u"HighResTexturePack02.bsa"_s, u"HighResTexturePack03.bsa"_s,
          u"Skyrim - Interface.bsa"_s,   u"Skyrim - Animations.bsa"_s,
          u"Skyrim - Meshes.bsa"_s,      u"Skyrim - Sounds.bsa"_s,
          u"Skyrim - Voices.bsa"_s,      u"Skyrim - VoicesExtra.bsa"_s};
}

QStringList SkyrimDataArchives::archives(const MOBase::IProfile* profile) const
{
  QStringList result;

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"skyrim.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"skyrim.ini"_s);
  result.append(getArchivesFromKey(iniFile, u"SResourceArchiveList"_s));
  result.append(getArchivesFromKey(iniFile, u"SResourceArchiveList2"_s));

  return result;
}

void SkyrimDataArchives::writeArchiveList(MOBase::IProfile* profile,
                                          const QStringList& before)
{
  QString list = before.join(u", "_s);

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"skyrim.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"skyrim.ini"_s);
  if (list.length() > 255) {
    int splitIdx = list.lastIndexOf(',', 256);
    setArchivesToKey(iniFile, u"SResourceArchiveList"_s, list.mid(0, splitIdx));
    setArchivesToKey(iniFile, u"SResourceArchiveList2"_s, list.mid(splitIdx + 2));
  } else {
    setArchivesToKey(iniFile, u"SResourceArchiveList"_s, list);
  }
}
