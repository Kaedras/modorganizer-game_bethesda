#include "morrowinddataarchives.h"
#include <utility.h>

#include "gamegamebryo.h"

#include <QSettings>

QStringList MorrowindDataArchives::vanillaArchives() const
{
  return {"Morrowind.bsa"};
}

QStringList MorrowindDataArchives::getArchives(const QString& iniFile) const
{
  wchar_t buffer[256];
  QStringList result;
  std::wstring iniFileW = QDir::toNativeSeparators(iniFile).toStdWString();

  errno = 0;

  QString key = "Archive ";
  int i       = 0;

  QSettings ini(iniFile, QSettings::IniFormat);

  while (ini.contains(QString("Archives/%1%2").arg(key, i))) {
    result.append(QString::fromStdWString(buffer).trimmed());
    i++;
  }

  return result;
}

void MorrowindDataArchives::setArchives(const QString& iniFile, const QStringList& list)
{
  QSettings ini(iniFile, QSettings::IniFormat);

  QString key      = "Archive ";

  for (int i = 0; i < list.size(); i++) {
    ini.setValue(QString("Archives/%1%2").arg(key, i), list.at(i));
    if (ini.status() != QSettings::NoError) {
      qWarning("failed to set archives in \"%s\"", qUtf8Printable(iniFile));
    }
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
