#ifndef CHARCODECS_H
#define CHARCODECS_H

#include <string>

#include <cpp-utils/strings/string_view.h>

namespace Windows {

std::wstring to_wstring(cpp::string_view src);
std::string to_string(cpp::wstring_view src);

} // namespace Windows

#endif // CHARCODECS_H
