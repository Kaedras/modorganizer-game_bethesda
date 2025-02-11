#include "morrowinddataarchives.h"
#include <utility.h>

#include "gamegamebryo.h"

#include <QSettings>

using namespace Qt::Literals::StringLiterals;

QStringList MorrowindDataArchives::vanillaArchives() const
{
  return {u"Morrowind.bsa"_s};
}

QStringList MorrowindDataArchives::getArchives(const QString& iniFile) const
{
  wchar_t buffer[256];
  QStringList result;
  std::wstring iniFileW = QDir::toNativeSeparators(iniFile).toStdWString();

  errno = 0;

  QString key = u"Archive "_s;
  int i       = 0;

  QSettings ini(iniFile, QSettings::IniFormat);

  while (ini.contains(QString(u"Archives/%1%2"_s).arg(key, i))) {
    result.append(QString::fromStdWString(buffer).trimmed());
    i++;
  }

  return result;
}

void MorrowindDataArchives::setArchives(const QString& iniFile, const QStringList& list)
{
  QSettings ini(iniFile, QSettings::IniFormat);

  QString key      = u"Archive "_s;

  for (int i = 0; i < list.size(); i++) {
    ini.setValue(QString(u"Archives/%1%2"_s).arg(key, i), list.at(i));
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
          ? QDir(profile->absolutePath()).absoluteFilePath(u"morrowind.ini"_s)
          : gameDirectory().absoluteFilePath(u"morrowind.ini"_s);
  result.append(getArchives(iniFile));

  return result;
}

void MorrowindDataArchives::writeArchiveList(MOBase::IProfile* profile,
                                             const QStringList& before)
{
  QString iniFile =
      profile->localSettingsEnabled()
          ? QDir(profile->absolutePath()).absoluteFilePath(u"morrowind.ini"_s)
          : gameDirectory().absoluteFilePath(u"morrowind.ini"_s);
  setArchives(iniFile, before);
}
