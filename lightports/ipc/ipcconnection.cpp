/*
 * ipcconnection.cpp
 *
 *  Created on: 31.07.2015
 *      Author: richard
 */

#include "ipcconnection.h"

#include <cpp-utils/assert.h>
#include "../core/debug.h"
#include "ipcdata.h"

namespace Windows {

IPCConnection::IPCConnection(const std::wstring& path)
{
  file_ = HandleEx(CreateFileW(
      path.c_str(),
      GENERIC_WRITE,
      FILE_SHARE_READ,
      nullptr,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      nullptr));
  win_throw_on_fail(file_.get() != INVALID_HANDLE_VALUE);
}

void IPCConnection::callFunction(const std::string& func_name,
    const IPCData& data)
{
  cpp_assert(file_);
  write(func_name.c_str(), func_name.size());
}

void IPCConnection::write(const char* data, std::size_t datalen)
{
  cpp_assert(file_);
  DWORD bytes_written;
  bool success = WriteFile(
      file_.get(),
      data,
      DWORD(datalen),
      &bytes_written,
      nullptr);
  win_throw_on_fail(success == true);
  // TODO bytes_written != datalen
}

} /* namespace Windows */
