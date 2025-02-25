#pragma once

#ifdef __unix__
#include "linux/windowsTypes.h"
#else
#include <Windows.h>
#endif

inline QDateTime fileTimeToQDateTime(FILETIME fileTime)
{
  static constexpr int64_t WINDOWS_TICK      = 10000000;
  static constexpr int64_t SEC_TO_UNIX_EPOCH = 11644473600LL;

  int64_t timeInt =
      static_cast<int64_t>(fileTime.dwHighDateTime) << 32 | fileTime.dwLowDateTime;
  time_t unixTime = timeInt / WINDOWS_TICK - SEC_TO_UNIX_EPOCH;

  return QDateTime::fromSecsSinceEpoch(unixTime);
}
