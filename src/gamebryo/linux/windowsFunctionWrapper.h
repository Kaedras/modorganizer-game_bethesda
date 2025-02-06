#pragma once

#include "windowsTypes.h"
#include "stub.h"

inline bool FileTimeToSystemTime(const FILETIME *lpFileTime, SYSTEMTIME* lpSystemTime)
{
  (void)lpFileTime;
  (void)lpSystemTime;
  STUB();
  return false;
}
