#include "charcodecs.h"

#include <windows.h>
#include <string>

namespace Windows {

// convert UTF-8 string to wstring
std::wstring to_wstring(cpp::string_view str)
{
  std::wstring result(str.length(), L'\0');

  int chars = ::MultiByteToWideChar(
      CP_UTF8, 0, str.data(), str.length(),
      &result[0], result.length() + 1);
  if (chars == 0)
  {
    return std::wstring();
  }
  result.resize(chars);
  return result;
}

// convert wstring to UTF-8 string
std::string to_string(cpp::wstring_view str)
{
  std::string result(str.length(), L'\0');

  int chars = ::WideCharToMultiByte(
      CP_UTF8, 0, str.data(), str.length(),
      &result[0], result.length() + 1, nullptr, nullptr);
  if (chars == 0)
  {
    if (::GetLastError() != ERROR_INSUFFICIENT_BUFFER)
      return std::string();

    chars = ::WideCharToMultiByte(
        CP_UTF8, 0, str.data(), str.length(),
        nullptr, 0, nullptr, nullptr);
    if (chars == 0)
      return std::string();

    result.resize(chars);
    chars = ::WideCharToMultiByte(
          CP_UTF8, 0, str.data(), str.length(),
          &result[0], result.length() + 1, nullptr, nullptr);
    if (chars == 0)
      return std::string();
  }

  return result;
}

} // namespace Windows
