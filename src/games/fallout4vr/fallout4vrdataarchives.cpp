#include "fallout4vrdataarchives.h"

#include "iprofile.h"
#include <utility.h>

using namespace Qt::Literals::StringLiterals;

QStringList Fallout4VRDataArchives::vanillaArchives() const
{
  return {u"Fallout4 - Textures1.ba2"_s,   u"Fallout4 - Textures2.ba2"_s,
          u"Fallout4 - Textures3.ba2"_s,   u"Fallout4 - Textures4.ba2"_s,
          u"Fallout4 - Textures5.ba2"_s,   u"Fallout4 - Textures6.ba2"_s,
          u"Fallout4 - Textures7.ba2"_s,   u"Fallout4 - Textures8.ba2"_s,
          u"Fallout4 - Textures9.ba2"_s,   u"Fallout4 - Meshes.ba2"_s,
          u"Fallout4 - MeshesExtra.ba2"_s, u"Fallout4 - Voices.ba2"_s,
          u"Fallout4 - Sounds.ba2"_s,      u"Fallout4 - Interface.ba2"_s,
          u"Fallout4 - Animations.ba2"_s,  u"Fallout4 - Materials.ba2"_s,
          u"Fallout4 - Shaders.ba2"_s,     u"Fallout4 - Startup.ba2"_s,
          u"Fallout4 - Misc.ba2"_s,        u"Fallout4_VR - Main.ba2"_s,
          u"Fallout4_VR - Shaders.ba2"_s,  u"Fallout4_VR - Textures.ba2"_s};
}

QStringList Fallout4VRDataArchives::archives(const MOBase::IProfile* profile) const
{
  QStringList result;

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"fallout4.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"fallout4.ini"_s);
  result.append(getArchivesFromKey(iniFile, u"SResourceArchiveList"_s));
  result.append(getArchivesFromKey(iniFile, u"SResourceArchiveList2"_s));

  return result;
}

void Fallout4VRDataArchives::writeArchiveList(MOBase::IProfile* profile,
                                              const QStringList& before)
{
  QString list = before.join(u", "_s);

  QString iniFile = profile->localSettingsEnabled()
                        ? QDir(profile->absolutePath()).absoluteFilePath(u"fallout4.ini"_s)
                        : localGameDirectory().absoluteFilePath(u"fallout4.ini"_s);
  if (list.length() > 255) {
    int splitIdx = list.lastIndexOf(',', 256);
    setArchivesToKey(iniFile, u"SResourceArchiveList"_s, list.mid(0, splitIdx));
    setArchivesToKey(iniFile, u"SResourceArchiveList2"_s, list.mid(splitIdx + 2));
  } else {
    setArchivesToKey(iniFile, u"SResourceArchiveList"_s, list);
  }
}
