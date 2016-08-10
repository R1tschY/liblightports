#include "debug.h"

#include <stdarg.h>
#include <cerrno>  // für errno
#include <cstdio>  // für printf
#include <sstream>
#include <codecvt>
#include <locale>
#include <wchar.h>
#include <string>
#include <cpp-utils/algorithm/length.h>
#include <cpp-utils/assert.h>
#include <cpp-utils/iostreams/small_stringstream.h>

#include "memory.h"
#include "exception.h"

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

//static
//void getErrorString(DWORD code, DWORD language, std::wstring& result)
//{
//  if (result.capacity() < 64)
//    result.append(64 - result.size(), L'\0');
//  else
//    result.append(result.capacity() - result.size(), L'\0');
//
//  DWORD chars = ::FormatMessageW(
//      FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
//      nullptr,
//      code,
//      language,
//      &(*result.begin()),
//      result.size(),
//      nullptr);
//
//  if (chars > 0) {
//    // FormatMessageW succeeded
//    result.erase(chars + 1);
//  }
//  else
//  {
//    result = std::to_wstring(code);
//
//    DWORD error = ::GetLastError();
//    if (error != 0)
//      print(L"Lightports: Failed to format error message %d with error code %d", code, error);
//  }
//}

//static
//std::wstring getErrorString(DWORD code, DWORD language)
//{
//  wchar_t buffer[256];
//
//  DWORD chars = ::FormatMessageW(
//      FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
//      nullptr,
//      code,
//      language,
//      &buffer,
//      cpp::length(buffer),
//      nullptr);
//
//  if (chars > 0)
//    // FormatMessageW succeeded
//    return std::wstring(buffer, chars);
//
//  DWORD error = ::GetLastError();
//  if (error == 0)
//    // empty message!
//    return std::to_wstring(code);
//
//  print(L"Lightports: Failed to format error message %d with error code %d", code, error);
//
//  return std::to_wstring(code);
//}

std::wstring getWindowsError(DWORD code)
{
  auto error_string = getErrorString(code, Language::Neutral);
  if (!error_string) {
    wchar_t buffer[64];
    cpp::small_owstringstream<64> stream;
    stream << "Error code: " << code;
    return stream.str();
  }

  return std::wstring(error_string.data(), error_string.size());
}

std::string getAsciiWindowsError(DWORD code)
{
  auto error_string = getErrorString(code, Language::Neutral);
  if (!error_string) {
    cpp::small_ostringstream<64> stream;
    stream << "Error code: " << code;
    return stream.str();
  }

  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.to_bytes(error_string.begin(), error_string.end());
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
    L"--- MESSAGE: ", // LL_MESSAGE
    L"??? WARNING: ", // LL_WARNING
    L"!!! CRITIAL: ", // LL_CRITICAL
    L"!!! ERROR: ", // LL_ERROR
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

void printError(cpp::wstring_view error_message, DWORD error_code) {
  auto error_string = getErrorString(error_code, Language::Neutral);
  if (error_string) {
    WIN_CRITICAL(L"Error while call of %ls: %ls", error_message.data(), error_string.data());
  }
  else
  {
    WIN_CRITICAL(L"Error while call of %ls: error code %d.", error_message.data(), error_code);
  }
}

void printWindowsFail(const char* func, const char* error_message, DWORD error_code) {
  auto error_string = getErrorString(error_code, Language::Neutral);
  if (error_string) {
    WIN_CRITICAL(L"Error while call of %s %s: %ls", func, error_message, error_string.data());
  }
  else
  {
    WIN_CRITICAL(L"Error while call of %s %s: error code %d.", func, error_message, error_code);
  }
}

void throwWindowsFail(const char* func, const char* error_message, DWORD error_code) {
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
