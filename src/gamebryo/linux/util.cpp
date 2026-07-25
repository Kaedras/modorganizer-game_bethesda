#include "util.h"

#include <QDir>
#include <QDirListing>
#include <QFileInfo>
#include <QString>

QString findFileCaseInsensitive(const QString& path) noexcept
{
  QFileInfo info(path);
  QString fileName = info.fileName();
  QDir parentDir(info.dir());

  return parentDir.absoluteFilePath(findFileNameCaseInsensitive(parentDir, fileName));
}

QString findFileNameCaseInsensitive(const QDir& path, const QString& fileName) noexcept
{
  if (QFileInfo::exists(path.absoluteFilePath(fileName))) {
    return fileName;
  }

  for (const auto& dirEntry :
       QDirListing(path.absolutePath(), QDirListing::IteratorFlag::FilesOnly)) {
    if (dirEntry.fileName().compare(fileName, Qt::CaseInsensitive) == 0) {
      return dirEntry.fileName();
    }
  }

  return fileName;
}
