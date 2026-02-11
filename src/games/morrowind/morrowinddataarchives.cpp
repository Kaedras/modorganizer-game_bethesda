#include "morrowinddataarchives.h"
#include "registry.h"
#include <utility.h>

#include "gamegamebryo.h"

using namespace MOBase;
using namespace Qt::StringLiterals;

QStringList MorrowindDataArchives::vanillaArchives() const
{
  return {"Morrowind.bsa"};
}

QStringList MorrowindDataArchives::getArchives(const QString& iniFile) const
{
  QStringList result;

  QString key = "Archive ";
  int i       = 0;
  QString value;
  while ((value = ReadRegistryValue(u"Archives"_s, key % QString::number(i), u""_s,
                                    iniFile)) != ""_L1) {
    result.append(value.trimmed());
    i++;
  }

  return result;
}

void MorrowindDataArchives::setArchives(const QString& iniFile, const QStringList& list)
{
  WriteRegistryValue(u"Archives"_s, QString(), QString(), iniFile);

  QString key      = "Archive ";
  int writtenCount = 0;
  for (const QString& value : list) {
    if (!WriteRegistryValue(u"Archives"_s, key % QString::number(writtenCount), value,
                            iniFile)) {
      qWarning("failed to set archives in \"%s\"", qUtf8Printable(iniFile));
    }
    ++writtenCount;
  }
}

QStringList MorrowindDataArchives::archives(const MOBase::IProfile* profile) const
{
  QStringList result;

  QString iniFile =
      profile->localSettingsEnabled()
          ? QDir(profile->absolutePath()).absoluteFilePath("morrowind.ini")
          : gameDirectory().absoluteFilePath("morrowind.ini");
  result.append(getArchives(iniFile));

  return result;
}

void MorrowindDataArchives::writeArchiveList(MOBase::IProfile* profile,
                                             const QStringList& before)
{
  QString iniFile =
      profile->localSettingsEnabled()
          ? QDir(profile->absolutePath()).absoluteFilePath("morrowind.ini")
          : gameDirectory().absoluteFilePath("morrowind.ini");
  setArchives(iniFile, before);
}
