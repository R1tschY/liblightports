#include "rundll.h"

#include "../core/debug.h"
#include "../base/path.h"
#include <cpp-utils/strings/string_literal.h>

namespace Windows {

Process RunDll::execute64BitDll(cpp::wstring_view dll_name, cpp::wstring_view entry, cpp::wstring_view cmdln_args)
{
  cpp::wstring_view rundll32_exe = wstring_literal("C:\\Windows\\Sysnative\\rundll32.exe");

  std::wstring cmdln;
  cmdln.append(rundll32_exe.begin(), rundll32_exe.end());
  cmdln += L' ';
  cmdln.append(dll_name.begin(), dll_name.end());
  cmdln += L',';
  cmdln.append(entry.begin(), entry.end());
  cmdln += L' ';
  cmdln.append(cmdln_args.begin(), cmdln_args.end());

  print(L"%ls\n", cmdln.c_str());

  if (!Path::exists(dll_name)) {
    throw std::runtime_error("RunDll64Bit: dll does not exist.\n");
  }

  return Process::runCmdln(std::move(cmdln));
}

} // namespace Windows
