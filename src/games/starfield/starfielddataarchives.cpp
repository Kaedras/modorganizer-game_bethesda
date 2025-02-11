#include "starfielddataarchives.h"

#include "iprofile.h"
#include <utility.h>

using namespace Qt::Literals::StringLiterals;

QStringList StarfieldDataArchives::vanillaArchives() const
{
  return {u"Starfield - Animations.ba2"_s,
          u"Starfield - DensityMaps.ba2"_s,
          u"Starfield - FaceAnimation01.ba2"_s,
          u"Starfield - FaceAnimation02.ba2"_s,
          u"Starfield - FaceAnimation03.ba2"_s,
          u"Starfield - FaceAnimation04.ba2"_s,
          u"Starfield - FaceAnimationPatch.ba2"_s,
          u"Starfield - FaceMeshes.ba2"_s,
          u"Starfield - GeneratedTextures.ba2"_s,
          u"Starfield - Interface.ba2"_s,
          u"Starfield - Localization.ba2"_s,
          u"Starfield - LODMeshes.ba2"_s,
          u"Starfield - LODMeshesPatch.ba2"_s,
          u"Starfield - LODTextures.ba2"_s,
          u"Starfield - Materials.ba2"_s,
          u"Starfield - Meshes01.ba2"_s,
          u"Starfield - Meshes02.ba2"_s,
          u"Starfield - MeshesPatch.ba2"_s,
          u"Starfield - Misc.ba2"_s,
          u"Starfield - Particles.ba2"_s,
          u"Starfield - ParticlesTestData.ba2"_s,
          u"Starfield - PlanetData.ba2"_s,
          u"Starfield - Shaders.ba2"_s,
          u"Starfield - ShadersBeta.ba2"_s,
          u"Starfield - Terrain01.ba2"_s,
          u"Starfield - Terrain02.ba2"_s,
          u"Starfield - Terrain03.ba2"_s,
          u"Starfield - Terrain04.ba2"_s,
          u"Starfield - TerrainPatch.ba2"_s,
          u"Starfield - Textures01.ba2"_s,
          u"Starfield - Textures02.ba2"_s,
          u"Starfield - Textures03.ba2"_s,
          u"Starfield - Textures04.ba2"_s,
          u"Starfield - Textures05.ba2"_s,
          u"Starfield - Textures06.ba2"_s,
          u"Starfield - Textures07.ba2"_s,
          u"Starfield - Textures08.ba2"_s,
          u"Starfield - Textures09.ba2"_s,
          u"Starfield - Textures10.ba2"_s,
          u"Starfield - Textures11.ba2"_s,
          u"Starfield - TexturesPatch.ba2"_s,
          u"Starfield - Voices01.ba2"_s,
          u"Starfield - Voices02.ba2"_s,
          u"Starfield - VoicesPatch.ba2"_s,
          u"Starfield - WwiseSounds01.ba2"_s,
          u"Starfield - WwiseSounds02.ba2"_s,
          u"Starfield - WwiseSounds03.ba2"_s,
          u"Starfield - WwiseSounds04.ba2"_s,
          u"Starfield - WwiseSounds05.ba2"_s,
          u"Starfield - WwiseSoundsPatch.ba2"_s,
          u"Constellation - Localization.ba2"_s,
          u"Constellation - Textures.ba2"_s,
          u"OldMars - Localization.ba2"_s,
          u"OldMars - Textures.ba2"_s,
          u"BlueprintShips-Starfield - Localization.ba2"_s};
}

QStringList StarfieldDataArchives::archives(const MOBase::IProfile* profile) const
{
  QStringList result;

  QString defaultIniFile = gameDirectory().absoluteFilePath(u"Starfield.ini"_s);
  QString customIniFile =
      profile->localSettingsEnabled()
          ? QDir(profile->absolutePath()).absoluteFilePath(u"StarfieldCustom.ini"_s)
          : localGameDirectory().absoluteFilePath(u"StarfieldCustom.ini"_s);
  QStringList archiveSettings = {u"SResourceArchiveList"_s, u"sResourceIndexFileList"_s,
                                 u"SResourceArchiveMemoryCacheList"_s,
                                 u"sResourceStartUpArchiveList"_s,
                                 u"sResourceEnglishVoiceList"_s};
  for (const auto& setting : archiveSettings) {
    auto archives = getArchivesFromKey(customIniFile, setting, 4096);
    if (archives.isEmpty())
      archives = getArchivesFromKey(defaultIniFile, setting, 4096);
    result.append(archives);
  }

  return result;
}

void StarfieldDataArchives::writeArchiveList(MOBase::IProfile* profile,
                                             const QStringList& before)
{}

void StarfieldDataArchives::addArchive(MOBase::IProfile* profile, int index,
                                       const QString& archiveName)
{}

void StarfieldDataArchives::removeArchive(MOBase::IProfile* profile,
                                          const QString& archiveName)
{}
