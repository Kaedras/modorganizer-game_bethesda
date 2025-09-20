#include "util.h"

#include <QDir>
#include <QFileInfo>
#include <QString>

#if (QT_VERSION < QT_VERSION_CHECK(6, 8, 0))
#include <QDirIterator>
#else
#include <QDirListing>
#endif

QString findFileCaseInsensitive(const QString& path) noexcept
{
  QFileInfo info(path);
  QString fileName = info.fileName();
  QDir parentDir(info.dir());

  return parentDir.absoluteFilePath(findFileNameCaseInsensitive(parentDir, fileName));
}

QString findFileNameCaseInsensitive(const QDir& path, const QString& fileName) noexcept
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 8, 0))
  QDirIterator it(path);

  while (it.hasNext()) {
    QFileInfo info(it.nextFileInfo());
    if (info.isFile()) {
      if (info.fileName().compare(fileName, Qt::CaseInsensitive) == 0) {
        return info.fileName();
      }
    }
  }
#else
  for (const auto& dirEntry :
       QDirListing(path.absolutePath(), QDirListing::IteratorFlag::FilesOnly)) {
    if (dirEntry.fileName().compare(fileName, Qt::CaseInsensitive) == 0) {
      return dirEntry.fileName();
    }
  }
#endif
  return fileName;
}
