#ifndef DEBUG_H
#define DEBUG_H

#include <string>

#include <cpp-utils/strings/string_view.h>
#include <cpp-utils/preprocessor.h>
#include <windows.h>

namespace Windows {

enum LogLevel {
  LL_MESSAGE,
  LL_WARNING,
  LL_CRITICAL,
  LL_ERROR,

  LL_MAX
};

//
// Print debug message

#ifdef NDEBUG
# define WIN_MESSAGE(fmt, ...)
#else
# define WIN_MESSAGE(fmt, ...) ::Windows::printMessage(::Windows::LL_MESSAGE, fmt L"\n", ##__VA_ARGS__)
#endif

#define WIN_WARNING(fmt, ...) ::Windows::printMessage(::Windows::LL_WARNING, fmt L"\n", ##__VA_ARGS__)
#define WIN_CRITICAL(fmt, ...) ::Windows::printMessage(::Windows::LL_CRITICAL, fmt L"\n", ##__VA_ARGS__)
#define WIN_ERROR(fmt, ...) ::Windows::printMessage(::Windows::LL_ERROR, fmt L"\n", ##__VA_ARGS__)

void printMessage(LogLevel level, const wchar_t* format, ...);

//
// Windows API error

std::wstring getWindowsError(DWORD code);
std::string getAsciiWindowsError(DWORD code);

inline
std::wstring getLastWindowsError()
{
  return getWindowsError(GetLastError());
}

struct PrintWindowsFail
{
  static void call(const char* func, const char* error_message, HRESULT error_code);
};

struct ThrowWindowsFail
{
  static void call(const char* func, const char* error_message, HRESULT error_code);
};

//
// Check return value
#define win_throw_fail() win_throw_on_fail(false)
#define win_print_fail() win_print_on_fail(false)
#define win_throw_on_fail(expr) (::Windows::checkReturnValue<::Windows::ThrowWindowsFail>((expr), __PRETTY_FUNCTION__, "[" __FILE__ ":" CPP_STRINGIFY(__LINE__) "]"))
#define win_print_on_fail(expr) (::Windows::checkReturnValue<::Windows::PrintWindowsFail>((expr), __PRETTY_FUNCTION__, "[" __FILE__ ":" CPP_STRINGIFY(__LINE__) "]"))
#define win_return_if_failed(expr,value) \
  WIN_BEGIN_MACRO_BLOCK \
    if (!expr) { \
      return value; \
    } \
  WIN_END_MACRO_BLOCK

template<typename FailureFunc, typename ReturnValue>
inline ReturnValue checkReturnValue(ReturnValue return_value, const char* func, const char* fail_message) {
  if (!return_value) {
    FailureFunc::call(func, fail_message, HRESULT_FROM_WIN32(GetLastError()));
  }
  return return_value;
}

template<typename FailureFunc>
inline HRESULT checkReturnValue(HRESULT return_value, const char* func, const char* fail_message) {
  if (return_value != S_OK) {
    FailureFunc::call(func, fail_message, return_value);
  }
  return return_value;
}

} // namespace Windows

void print(const wchar_t* format, ...);

#endif // DEBUG_H
