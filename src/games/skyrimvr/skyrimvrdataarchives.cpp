#include "skyrimvrdataarchives.h"

#include "iprofile.h"
#include <utility.h>

using namespace Qt::Literals::StringLiterals;

QStringList SkyrimVRDataArchives::vanillaArchives() const
{
  return {u"Skyrim - Textures0.bsa"_s, u"Skyrim - Textures1.bsa"_s, u"Skyrim - Textures2.bsa"_s,
          u"Skyrim - Textures3.bsa"_s, u"Skyrim - Textures4.bsa"_s, u"Skyrim - Textures5.bsa"_s,
          u"Skyrim - Textures6.bsa"_s, u"Skyrim - Textures7.bsa"_s, u"Skyrim - Textures8.bsa"_s,
          u"Skyrim - Meshes0.bsa"_s,   u"Skyrim - Meshes1.bsa"_s,   u"Skyrim - Voices_en0.bsa"_s,
          u"Skyrim - Sounds.bsa"_s,    u"Skyrim - Interface.bsa"_s, u"Skyrim - Animations.bsa"_s,
          u"Skyrim - Shaders.bsa"_s,   u"Skyrim - Misc.bsa"_s,      u"Skyrim - Patch.bsa"_s,
          u"Skyrim_VR - Main.bsa"_s};
}

QStringList SkyrimVRDataArchives::archives(const MOBase::IProfile* profile) const
{
  QStringList result;

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"SkyrimVR.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"SkyrimVR.ini"_s);
  result.append(getArchivesFromKey(iniFile, u"SResourceArchiveList"_s));
  result.append(getArchivesFromKey(iniFile, u"SResourceArchiveList2"_s));

  return result;
}

void SkyrimVRDataArchives::writeArchiveList(MOBase::IProfile* profile,
                                            const QStringList& before)
{
  QString list = before.join(u", "_s);

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"SkyrimVR.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"SkyrimVR.ini"_s);
  if (list.length() > 255) {
    int splitIdx = list.lastIndexOf(',', 256);
    setArchivesToKey(iniFile, u"SResourceArchiveList"_s, list.mid(0, splitIdx));
    setArchivesToKey(iniFile, u"SResourceArchiveList2"_s, list.mid(splitIdx + 2));
  } else {
    setArchivesToKey(iniFile, u"SResourceArchiveList"_s, list);
  }
}
