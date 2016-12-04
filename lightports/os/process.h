#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <cpp-utils/strings/string_view.h>
#include "../core/memory.h"
#include <windows.h>

namespace Windows {

class Process
{
public:
  enum AccessRights
  {
    Create = PROCESS_CREATE_PROCESS,
    CreateThread = PROCESS_CREATE_THREAD,
    DupHandle = PROCESS_DUP_HANDLE,
    QueryInformation = PROCESS_QUERY_INFORMATION,
    QueryLimitedInformation = PROCESS_QUERY_LIMITED_INFORMATION,
    SetInformation = PROCESS_SET_INFORMATION,
    SetQuota = PROCESS_SET_QUOTA,
    SuspendResume = PROCESS_SUSPEND_RESUME,
    Terminate = PROCESS_TERMINATE,
    VmOperation = PROCESS_VM_OPERATION,
    VmRead = PROCESS_VM_READ,
    VmWrite = PROCESS_VM_WRITE,
    Synchronize = SYNCHRONIZE
  };

  explicit Process(HANDLE process) :
    process_(process)
  { }

  static Process runCmdln(
    cpp::wstring_view cmdln,
    cpp::wstring_view working_directory = cpp::wstring_view());

  static Process run(
    cpp::wstring_view exe_path,
    cpp::wstring_view args,
    cpp::wstring_view working_directory = cpp::wstring_view());

  static Process open(DWORD desiredAccess, DWORD pid);

  void terminate(DWORD exitCode);

  HANDLE getHANDLE() const { return process_.get(); }

private:
  Handle process_;
};

} // namespace Windows

#endif // PROCESS_H
