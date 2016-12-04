#ifndef DLL_H
#define DLL_H

#include <windows.h>

namespace Windows {

#define WINDOWS_DLL_SHARED __attribute__((section ("shared"), shared))

extern "C" {
BOOL APIENTRY DllMain(HINSTANCE, DWORD, LPVOID);
}

class Dll {
  Dll();
public:

  static Dll& get();

  static HINSTANCE getHINSTANCE() { return get().instance_; }

private:
  HINSTANCE instance_;

  friend BOOL APIENTRY DllMain(HINSTANCE, DWORD, LPVOID);
};

} // namespace Windows

#endif // DLL_H
