#include "application.h"

#include <time.h>
#include <shlobj.h>

#include "../core/debug.h"
#include "../core/macros.h"
#include "../base/charcodecs.h"

namespace Windows {

//static WindowsVersion Application_getWindowsVersion() {
//  OSVERSIONINFO wininfo;
//  wininfo.dwOSVersionInfoSize = sizeof(wininfo);
//  GetVersionEx(&wininfo);
//  return (wininfo.dwMajorVersion << 8) | (wininfo.dwMinorVersion);
//}

static void Application_newfailed() {
  _putws(L"not enough memory!");
  abort();
}

static void Application_terminate() {
  _putws(L"uncatched exception!");
  abort();
}

static void Application_unexpected() {
  throw;
}

Application* Application::instance_ = nullptr;

Application::Application(cpp::wstring_view name, HINSTANCE instance) :
  appinstance_(instance), name_(name.to_string())
{
  if (instance_ != nullptr) {
    is_running_ = true;
    return;
  }
  instance_ = this;

  setlocale(LC_ALL, "");
  _tzset();
  std::set_new_handler(Application_newfailed);
  std::set_unexpected(Application_unexpected);
  std::set_terminate(Application_terminate);

  mutex_.reset(CreateMutex(nullptr, false, name.data()));
  DWORD error = GetLastError();
  if (!mutex_) {
    WIN_WARNING(L"cannot create application mutex: %s",
                getWindowsError(error).c_str());
  }

  is_running_ = (error == ERROR_ALREADY_EXISTS);
}

int Application::run(ExecuteFunc entry) {
  if (is_running_) return 0;
  is_running_ = true;

  try {
    return entry();
  } catch (const std::exception& e) {
    WIN_ERROR(L"uncatched exception: %ls", to_wstring(e.what()).c_str());
  } catch (...) {
    WIN_ERROR(L"uncatched exception!");
  }

  return 0;
}

int Application::processMessages()
{
  // main message loop
  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return (int) msg.wParam;
}

bool Application::is64BitWindows()
{
#if CPUBITSET == 32
  // We can check if the operating system is 64-bit by checking whether
  // we are running under Wow64 (we are 32-bit code). We must check if this
  // function is implemented before we call it, because some older 32-bit
  // versions of kernel32.dll (eg. Windows 2000) don't know about it.
  // See "IsWow64Process", http://msdn.microsoft.com/en-us/library/ms684139.aspx

  typedef BOOL (WINAPI *IsWow64Process_t) (HANDLE, BOOL*);

  auto IsWow64Process = (IsWow64Process_t)
      GetProcAddress(GetModuleHandle(L"kernel32"), "IsWow64Process");
  if (IsWow64Process != nullptr) {
    BOOL isWow64 = false;
    if (IsWow64Process(GetCurrentProcess(), &isWow64)) {
      return isWow64;
    }
  }

  return false;
#else
	return true;
#endif
}


// Utils

std::wstring Application::getExecutablePath() {
  wchar_t result[MAX_PATH+1];
  GetModuleFileNameW(nullptr, result, sizeof(result));
  return result;
}

std::wstring Application::getConfigPath() {
  wchar_t path[MAX_PATH+1];
  HRESULT hr = SHGetFolderPathW(nullptr, CSIDL_APPDATA, nullptr, 0, path);
  return (hr == S_OK) ? path : nullptr;
}

} // namespace Windows
