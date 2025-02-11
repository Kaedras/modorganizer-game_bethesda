#include "fallout76dataarchives.h"

#include "iprofile.h"
#include <utility.h>

#include <QRegularExpression>

using namespace Qt::Literals::StringLiterals;

QStringList Fallout76DataArchives::vanillaArchives() const
{
  return {u"SeventySix - Animations.ba2"_s,
          u"SeventySix - ATX_Main.ba2"_s,
          u"SeventySix - ATX_Textures.ba2"_s,
          u"SeventySix - EnlightenExteriors01.ba2"_s,
          u"SeventySix - EnlightenExteriors02.ba2"_s,
          u"SeventySix - EnlightenInteriors.ba2"_s,
          u"SeventySix - GeneratedMeshes.ba2"_s,
          u"SeventySix - GeneratedTextures.ba2"_s,
          u"SeventySix - Interface.ba2"_s,
          u"SeventySix - Localization.ba2"_s,
          u"SeventySix - Materials.ba2"_s,
          u"SeventySix - Meshes01.ba2"_s,
          u"SeventySix - Meshes02.ba2"_s,
          u"SeventySix - MeshesExtra.ba2"_s,
          u"SeventySix - MiscClient.ba2"_s,
          u"SeventySix - Shaders.ba2"_s,
          u"SeventySix - Sounds01.ba2"_s,
          u"SeventySix - Sounds02.ba2"_s,
          u"SeventySix - Startup.ba2"_s,
          u"SeventySix - Textures01.ba2"_s,
          u"SeventySix - Textures02.ba2"_s,
          u"SeventySix - Textures03.ba2"_s,
          u"SeventySix - Textures04.ba2"_s,
          u"SeventySix - Textures05.ba2"_s,
          u"SeventySix - Textures06.ba2"_s,
          u"SeventySix - Voices.ba2"_s};
}

QStringList Fallout76DataArchives::sResourceIndexFileList() const
{
  return {u"SeventySix - Textures01.ba2"_s, u"SeventySix - Textures02.ba2"_s,
          u"SeventySix - Textures03.ba2"_s, u"SeventySix - Textures04.ba2"_s,
          u"SeventySix - Textures05.ba2"_s, u"SeventySix - Textures06.ba2"_s};
}

QStringList Fallout76DataArchives::sResourceStartUpArchiveList() const
{
  return {u"SeventySix - Interface.ba2"_s, u"SeventySix - Localization.ba2"_s,
          u"SeventySix - Shaders.ba2"_s, u"SeventySix - Startup.ba2"_s};
}

QStringList Fallout76DataArchives::SResourceArchiveMemoryCacheList() const
{
  return {u"SeventySix - Interface.ba2"_s, u"SeventySix - Materials.ba2"_s,
          u"SeventySix - MiscClient.ba2"_s, u"SeventySix - Shaders.ba2"_s};
}

QStringList Fallout76DataArchives::SResourceArchiveList() const
{
  return {u"SeventySix - GeneratedMeshes.ba2"_s, u"SeventySix - Materials.ba2"_s,
          u"SeventySix - Meshes01.ba2"_s,        u"SeventySix - Meshes02.ba2"_s,
          u"SeventySix - MeshesExtra.ba2"_s,     u"SeventySix - MiscClient.ba2"_s,
          u"SeventySix - Sounds01.ba2"_s,        u"SeventySix - Sounds02.ba2"_s,
          u"SeventySix - Startup.ba2"_s,         u"SeventySix - Voices.ba2"_s};
}

QStringList Fallout76DataArchives::SResourceArchiveList2() const
{
  return {u"SeventySix - Animations.ba2"_s, u"SeventySix - EnlightenInteriors.ba2"_s,
          u"SeventySix - GeneratedTextures.ba2"_s, u"SeventySix - EnlightenExteriors01.ba2"_s,
          u"SeventySix - EnlightenExteriors02.ba2"_s};
}

QStringList Fallout76DataArchives::sResourceArchive2List() const
{
  return {u"SeventySix - ATX_Main.ba2"_s, u"SeventySix - ATX_Textures.ba2"_s};
}

QStringList Fallout76DataArchives::archives(const MOBase::IProfile* profile) const
{
  QStringList result;

  QString iniFile =
      profile->localSettingsEnabled()
          ? QDir(profile->absolutePath()).absoluteFilePath(u"Fallout76.ini"_s)
          : localGameDirectory().absoluteFilePath(u"Fallout76.ini"_s);

  result.append(getArchivesFromKey(iniFile, u"sResourceIndexFileList"_s));
  result.append(getArchivesFromKey(iniFile, u"sResourceStartUpArchiveList"_s));
  result.append(getArchivesFromKey(iniFile, u"SResourceArchiveMemoryCacheList"_s));
  result.append(getArchivesFromKey(iniFile, u"SResourceArchiveList"_s));
  result.append(getArchivesFromKey(iniFile, u"SResourceArchiveList2"_s));
  result.append(getArchivesFromKey(iniFile, u"sResourceArchive2List"_s));

  return result;
}

void Fallout76DataArchives::writeArchiveList(MOBase::IProfile* profile,
                                             const QStringList& before)
{
  QString iniFile =
      profile->localSettingsEnabled()
          ? QDir(profile->absolutePath()).absoluteFilePath(u"Fallout76.ini"_s)
          : localGameDirectory().absoluteFilePath(u"Fallout76.ini"_s);

  QStringList sResourceIndexFileList          = {};
  QStringList sResourceStartUpArchiveList     = {};
  QStringList SResourceArchiveMemoryCacheList = {};
  QStringList SResourceArchiveList            = {};
  QStringList SResourceArchiveList2           = {};
  QStringList sResourceArchive2List           = {};

  for (int i = 0; i < before.size(); ++i) {
    const QString& archive = before[i];
    if (archive.contains(QRegularExpression(u" - Textures(\\d{2})\\.ba2$"_s))) {
      sResourceIndexFileList.append(archive);
    } else if (archive.contains(QRegularExpression(
                   u" - (Interface|Localization|Shaders|Startup)\\.ba2$"_s))) {
      sResourceStartUpArchiveList.append(archive);
    } else if (archive.contains(QRegularExpression(
                   u" - (Interface|Materials|MiscClient|Shaders)\\.ba2$"_s))) {
      SResourceArchiveMemoryCacheList.append(archive);
    } else if (archive.contains(QRegularExpression(
                   u" - "
                   "(GeneratedMeshes|Materials|Meshes(\\d{2}|\\w+)?|MiscClient|"
                   "Sounds\\d{2}|Startup|Voices)\\.ba2$"_s))) {
      SResourceArchiveList.append(archive);
    } else if (archive.contains(
                   QRegularExpression(u" - "
                                      "(Animations|Enlighten(Interiors|Exteriors\\d{2})"
                                      "|GeneratedTextures)\\.ba2$"_s))) {
      SResourceArchiveList2.append(archive);
    } else if (archive.contains(QRegularExpression(u" - ATX_.*\\.ba2$"_s))) {
      // if it is named after DLC, it has to go here
      sResourceArchive2List.append(archive);
    } else {
      // if it did not fit any description above, it gets tacked on at the end
      sResourceArchive2List.append(archive);
    }
  }

  setArchivesToKey(iniFile, u"sResourceIndexFileList"_s,
                   sResourceIndexFileList.join(u", "_s));
  setArchivesToKey(iniFile, u"sResourceStartUpArchiveList"_s,
                   sResourceStartUpArchiveList.join(u", "_s));
  setArchivesToKey(iniFile, u"SResourceArchiveMemoryCacheList"_s,
                   SResourceArchiveMemoryCacheList.join(u", "_s));
  setArchivesToKey(iniFile, u"SResourceArchiveList"_s, SResourceArchiveList.join(u", "_s));
  setArchivesToKey(iniFile, u"SResourceArchiveList2"_s, SResourceArchiveList2.join(u", "_s));
  setArchivesToKey(iniFile, u"sResourceArchive2List"_s, sResourceArchive2List.join(u", "_s));
}
