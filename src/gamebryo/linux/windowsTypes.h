#pragma once

#include <cstdint>

using BYTE = uint8_t;
using WORD = uint16_t;
using DWORD = uint32_t;
using ULONGLONG = uint64_t;
using LPCWSTR = const wchar_t*;
using HKEY = int;
using REFKNOWNFOLDERID = int;

enum keys
{
  HKEY_LOCAL_MACHINE,
  HKEY_CURRENT_USER
};

// https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-dtyp/2fefe8dd-ab48-4e33-a7d5-7171455a9289
typedef struct _SYSTEMTIME {
  WORD wYear;
  WORD wMonth;
  WORD wDayOfWeek;
  WORD wDay;
  WORD wHour;
  WORD wMinute;
  WORD wSecond;
  WORD wMilliseconds;
} SYSTEMTIME,
 *PSYSTEMTIME;


// https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-filetime
typedef struct _FILETIME {
  DWORD dwLowDateTime;
  DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
