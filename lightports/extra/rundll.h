#ifndef RUNDLL_H
#define RUNDLL_H

#include <cpp-utils/strings/string_view.h>

#include "process.h"

namespace Windows {

class RunDll
{
  RunDll();

public:
  static Process execute64BitDll(cpp::wstring_view dll_name, cpp::wstring_view entry, cpp::wstring_view cmdln_args);
};

} // namespace Windows

#endif // RUNDLL_H
