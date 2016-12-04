#ifndef WINSTRING_REF_H
#define WINSTRING_REF_H

#include <memory>
#include <type_traits>
#include <windows.h>
#include <cpp-utils/memory/unique_array.h>

namespace Windows {


//
// Local memory

namespace Detail {
struct LocalDeleter {
  void operator()(void* ptr) {
    ::LocalFree(ptr);
  }
};
} // namespace Detail

template<typename T>
using LocalPtr = std::unique_ptr<T, Detail::LocalDeleter>;

template<typename T>
using LocalArray = cpp::unique_array<T, Detail::LocalDeleter>;

//
// GeneralHandle

template<typename Deleter>
using ManagedHandle = std::unique_ptr<typename Deleter::pointer, Deleter>;

#define WINDOWS_DEFINE_HANDLE_DELETER(func) \
  template<typename PointerT> \
  struct func##Functor { \
    typedef PointerT pointer; \
    void operator()(PointerT handle) { \
      func(handle); \
    } \
  }

#define WINDOWS_HANDLE_DELETER(func) func##Functor

#define WINDOWS_DEFINE_GENERIC_HANDLE_TYPE(name, deleter) \
  WINDOWS_DEFINE_HANDLE_DELETER(deleter); \
  template<typename T> \
  using name = ::Windows::GeneralHandle<T, WINDOWS_HANDLE_DELETER(deleter)>;

#define WINDOWS_DEFINE_HANDLE_TYPE(name, type, deleter) \
  WINDOWS_DEFINE_HANDLE_DELETER(deleter); \
  using name = ::Windows::GeneralHandle<type, WINDOWS_HANDLE_DELETER(deleter)>;


template<
  typename PointerT,
  template<typename> class DeleterFuncT
>
using GeneralHandle = std::unique_ptr<PointerT, DeleterFuncT<PointerT> >;

//
// Handle

struct HandleDeleter {
  typedef HANDLE pointer;

  void operator()(HANDLE ptr)
  { ::CloseHandle(ptr); }
};

using Handle = ManagedHandle<HandleDeleter>;

struct HandleExDeleter {
  typedef HANDLE pointer;

  void operator()(HANDLE ptr)
  { if (ptr != INVALID_HANDLE_VALUE) ::CloseHandle(ptr); }
};

using HandleEx = ManagedHandle<HandleExDeleter>;

struct FindHandleDeleter {
  typedef HANDLE pointer;

  void operator()(HANDLE ptr)
  { if (ptr != INVALID_HANDLE_VALUE) ::FindClose(ptr); }
};

using FindHandle = ManagedHandle<FindHandleDeleter>;

} // namespace Windows

#endif // WINSTRING_REF_H
