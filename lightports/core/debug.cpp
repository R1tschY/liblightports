#include <cpp-utils/algorithm/length.h>
#include <cpp-utils/iostreams/small_stringstream.h>
#include <cpp-utils/memory/unique_array.h>
#include <debugapi.h>
#include <errhandlingapi.h>
#include <lightports/core/debug.h>
#include <lightports/core/debugstream.h>
#include <lightports/core/exception.h>
#include <lightports/core/memory.h>
#include <minwindef.h>
#include <processthreadsapi.h>
#include <stdarg.h>
#include <wchar.h>
#include <winbase.h>
#include <winnt.h>
#include <winuser.h>
#include <experimental/string_view>
#include <locale>
#include <sstream>
#include <comdef.h>

#include "charcodecs.h"

namespace Windows {

namespace Language {
enum : DWORD
{
  Neutral = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
  Invariant = MAKELANGID(LANG_INVARIANT, SUBLANG_NEUTRAL),
  SystemDefault = MAKELANGID(LANG_SYSTEM_DEFAULT, SUBLANG_SYS_DEFAULT),
  UserDefault = MAKELANGID(LANG_USER_DEFAULT, SUBLANG_DEFAULT),
};
} // namespace Language

static
LocalArray<wchar_t> getErrorString(DWORD code, DWORD language)
{
  wchar_t* buffer;
  DWORD bytes = FormatMessageW(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr,
      code,
      language,
      (wchar_t*)&buffer,
      0,
      nullptr);

  if (bytes > 0)
    // FormatMessageW succeeded
    return LocalArray<wchar_t>{ buffer, bytes };

  DWORD error = ::GetLastError();
  if (error == 0)
    // TODO: empty message ?!
    return LocalArray<wchar_t>();

  return LocalArray<wchar_t>();
}

std::wstring getWindowsError(DWORD code)
{
  auto error_string = getErrorString(code, Language::Neutral);
  if (!error_string) {
    cpp::small_owstringstream<64> stream;
    stream << L"Windows error code: " << std::hex << code << L'\0';
    return stream.str();
  }

  return std::wstring(error_string.data(), error_string.size());
}

std::string getAsciiWindowsError(DWORD code)
{
  auto error_string = getErrorString(code, Language::Neutral);
  if (!error_string) {
    cpp::small_ostringstream<64> stream;
    stream << "Windows error code: " << std::hex << code << '\0';
    return stream.str();
  }

  return to_string(cpp::wstring_view(error_string.data(), error_string.size()));
}

void printMessage(LogLevel level, const wchar_t* format, ...) {
  wchar_t buffer[1024];
  va_list vl;

  va_start(vl, format);
  int result = vsnwprintf(buffer, cpp::length(buffer) - 1, format, vl);
  va_end(vl);

  if (result < 0) {
   // TODO: Error
  }

  static const wchar_t* prefix[LL_MAX] = {
    L"[INFO] ", // LL_MESSAGE
    L"[WARN] ", // LL_WARNING
    L"[CRITICAL] ", // LL_CRITICAL
    L"[ERROR] ", // LL_ERROR
  };

  OutputDebugStringW(prefix[level]);
  OutputDebugStringW(buffer);

  switch (level) {
  case LL_CRITICAL:
//    MessageBoxW(NULL, buffer, L"critical error", MB_OK | MB_ICONWARNING);
    break;
  case LL_ERROR:
    MessageBoxW(NULL, buffer, L"Fatal error", MB_OK | MB_ICONERROR);
    ExitProcess(1);
  default:;
  }
}

void PrintWindowsFail::call(
  const char* func, const char* error_message, HRESULT error_code)
{
  auto error_string = getErrorString(error_code, Language::Neutral);
  if (error_string) {
    WIN_CRITICAL(L"Error while call of %s %s: %ls", func, error_message, error_string.data());
  }
  else
  {
    WIN_CRITICAL(L"Error while call of %s %s: error code %d.", func, error_message, error_code);
  }
}

void ThrowWindowsFail::call(
  const char* func, const char* error_message, HRESULT error_code)
{
  auto error_string = getErrorString(error_code, Language::Neutral);
  if (error_string) {
    WIN_CRITICAL(L"Error while call of %s %s: %ls", func, error_message, error_string.data());
  }
  else
  {
    WIN_CRITICAL(L"Error while call of %s %s: error code %d.", func, error_message, error_code);
  }

  throw Exception(error_code);
}

} // namespace Windows

void print(const wchar_t* format, ...) {
  wchar_t buffer[1024];
  va_list vl;

  va_start(vl, format);
  vsnwprintf(buffer, 1023, format, vl);
  va_end(vl);

  buffer[1023] = L'\0';

  OutputDebugStringW(buffer);
}
