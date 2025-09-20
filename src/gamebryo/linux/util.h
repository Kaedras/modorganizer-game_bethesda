#pragma once

#include <QDir>
#include <QString>

/**
 * @brief Find a file in the specified path, matching its name case-insensitively, and
 * returns its absolute file path if found. If no matching file is found, the original
 * path is returned.
 * @param path The path to the file being searched, including the file name.
 * @return The absolute file path of the matching file if found, or the original path if
 * no matching file exists.
 */
QString findFileCaseInsensitive(const QString& path) noexcept;

/**
 * @brief Find a file in the specified path, matching its name case-insensitively, and
 * return its name if found. If no matching file is found, the original file name is
 * returned.
 * @param path The path to the file being searched.
 * @param  fileName The file name to search for.
 * @return The file name of the matching file if found, or the original name if no
 * matching file exists.
 */
QString findFileNameCaseInsensitive(const QDir& path, const QString& fileName) noexcept;
