#include "charcodecs.h"

#include <stdexcept>

#include <windows.h>
#include <locale>
#include <codecvt>
#include <string>

namespace Windows {

// convert UTF-8 string to wstring
std::wstring to_wstring(cpp::string_view str)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
  return myconv.from_bytes(str.begin(), str.end());
}

// convert wstring to UTF-8 string
std::string to_string(cpp::wstring_view str)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
  return myconv.to_bytes(str.begin(), str.end());
}

} // namespace Windows
