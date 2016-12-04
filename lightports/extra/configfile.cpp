#include "configfile.h"

#include <string>
#include <windows.h>
#include <cpp-utils/strings/string_view.h>
#include <cpp-utils/strings/string_literal.h>

namespace Windows {

////////////////////////////////////////////////////////////////////////////////
//     ConfigFile
////////////////////////////////////////////////////////////////////////////////
bool ConfigFile::existsKey(cpp::wstring_view section, cpp::wstring_view key) const
{
  using namespace cpp::string_view_literals;

  constexpr auto test = L"?#ßüäö§$%&!"sv;
  return getString(section, key, test) != test;
}

std::wstring ConfigFile::getString(cpp::wstring_view section,
                                   cpp::wstring_view key,
                                   cpp::wstring_view default_value) const
{
  wchar_t buffer[255];
  DWORD bytes;

  bytes = GetPrivateProfileStringW(section.data(), key.data(), default_value.data(), buffer, sizeof(buffer), filename_.c_str());
  // TODO: Puffer zu klein -> bytes == sizeof(buffer) - 1

  return std::wstring(buffer);
}

int ConfigFile::getInteger(cpp::wstring_view section,
                           cpp::wstring_view key,
                           int default_value) const
{
  return GetPrivateProfileInt(section.data(), key.data(), default_value, filename_.c_str());
}

void ConfigFile::setString(cpp::wstring_view section,
                           cpp::wstring_view key,
                           cpp::wstring_view value)
{
  BOOL success = WritePrivateProfileStringW(section.data(), key.data(), value.data(), filename_.c_str());
  if (!success) {
//MESSAGE(L"Fehler bei Schreiben von [%s]->%s: %s", section, key, GetLastWindowsError().c_str());
  }
}

void ConfigFile::setInteger(cpp::wstring_view section,
                            cpp::wstring_view key,
                            int value)
{

 BOOL success = WritePrivateProfileStringW(section.data(), key.data(),
      std::to_wstring(value).c_str(), filename_.c_str());
  if (!success) {
//    MESSAGE(L"Fehler bei Schreiben von [%s]->%s: %s", section, key, GetLastWindowsError().c_str());
  }
}

std::vector<std::wstring> ConfigFile::getSections() const {
  wchar_t buffer[2048];
  DWORD bytes;

  bytes = GetPrivateProfileStringW(nullptr, nullptr, nullptr, buffer, sizeof(buffer), filename_.c_str());
  // TODO: Puffer zu klein -> bytes == sizeof(buffer)-2

  return splitStringArray(buffer);
}

std::vector<std::wstring> ConfigFile::getKeys(cpp::wstring_view section) const {
  wchar_t buffer[2048];

  DWORD bytes = GetPrivateProfileStringW(
    section.data(),
    nullptr,
    nullptr,
    buffer, sizeof(buffer),
    filename_.c_str()
  );
  // TODO: Puffer zu klein -> bytes == sizeof(buffer)-2

  return splitStringArray(buffer);
}

std::vector<std::wstring> ConfigFile::splitStringArray(const wchar_t* p) {
  std::vector<std::wstring> result;
  const wchar_t* section;

  while (*p != 0) {
    section = p;
    do p++; while (*p != 0);
    result.push_back(section);
    p++;
  }

  return std::move(result);
}

ConfigFile::ConfigFile()
{

}

void ConfigFile::loadFromFile(cpp::wstring_view filename) {
  filename_.assign(filename.begin(), filename.end());
}

} // namespace Windows

