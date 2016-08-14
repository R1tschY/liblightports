#ifndef CONTROL_H
#define CONTROL_H

#include <cpp-utils/assert.h>
#include <cpp-utils/iterator_range.h>
#include <cpp-utils/strings/string_view.h>
#include <lightports/core/debug.h>
#include <lightports/core/debugstream.h>
#include <lightports/core/exception.h>
#include <lightports/core/geometry.h>
#include <lightports/core/macros.h>
#include <windows.h>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>

namespace Windows {

struct FindWindowIteratorSentinel { };

class FindWindowIterator : public std::iterator<std::forward_iterator_tag, HWND>
{
public:
  FindWindowIterator(HWND parent, cpp::wstring_view window_class, cpp::wstring_view window_title)
  : parent_(parent), class_(window_class.data()), window_(window_title.data())
  {
    ++(*this);
  }

  bool operator==(FindWindowIteratorSentinel) { return hwnd_ == nullptr; }
  bool operator==(FindWindowIterator& iter) { return hwnd_ == iter.hwnd_; }
  bool operator!=(FindWindowIteratorSentinel) { return hwnd_ != nullptr; }
  bool operator!=(FindWindowIterator& iter) { return hwnd_ != iter.hwnd_; }

  FindWindowIterator& operator++()
  {
    hwnd_ = ::FindWindowExW(parent_, hwnd_, window_, class_);
    if (!hwnd_)
      throw Exception(GetLastError());
    return *this;
  }

  HWND operator*() const { return hwnd_; }

private:
  HWND parent_ = nullptr;
  const wchar_t* class_ = nullptr;
  const wchar_t* window_ = nullptr;

  HWND hwnd_ = nullptr;
};


class Control {
  DISALLOW_COPY_AND_ASSIGN(Control);
public:

  // enums

  enum class AnimateMode {
    Blend = AW_BLEND,
    SlideRightToLeft = AW_HOR_NEGATIVE | AW_SLIDE,
    SlideLeftToRight = AW_HOR_POSITIVE | AW_SLIDE,
    SlideTopToBottom = AW_VER_POSITIVE | AW_SLIDE,
    SlideBottomToTop = AW_VER_NEGATIVE | AW_SLIDE,
    RollRightToLeft = AW_HOR_NEGATIVE,
    RollLeftToRight = AW_HOR_POSITIVE,
    RollTopToBottom = AW_VER_NEGATIVE,
    RollBottmToTop = AW_VER_POSITIVE,
    ExpandCollapse = AW_CENTER
  };

  // ctors/dtor
  explicit Control() :
          class_(MAKEINTATOM(getWindowClass()))
  {
  }

  explicit Control(DWORD style, DWORD exstyle = 0) :
          class_(MAKEINTATOM(getWindowClass())),
          style_(style),
          exstyle_(exstyle)
  {
  }

  explicit Control(const wchar_t* window_class, DWORD style, DWORD exstyle) :
          class_(window_class),
          style_(style),
          exstyle_(exstyle)
  {
  }

  explicit Control(ATOM window_class, DWORD style, DWORD exstyle) :
          class_(MAKEINTATOM(window_class)),
          style_(style),
          exstyle_(exstyle)
  {
  }

  Control(Control&& other);
  Control& operator=(Control&& other);

  virtual ~Control();

  // properties

  HWND getNativeHandle() const {
    return handle_.get();
  }

  Rectangle getClientRect() const {
    RECT rc;
    cpp_assert(handle_);
    win_print_on_fail(GetClientRect(handle_.get(), &rc));
    return rc;
  }

  bool okay() const {
    return bool(handle_);
  }
  explicit operator bool() const {
    return okay();
  }

  // base functions
  void create(HWND parent, cpp::wstring_view title, const Rectangle& rect) {
    create(parent, title, rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());
  }
  void create(HWND parent, cpp::wstring_view title, int x, int y, int width, int height);

  void destroy();

  void show(int show_command = SW_SHOW);
  void showAnimated(AnimateMode mode, unsigned time) { AnimateWindow(getNativeHandle(), time, (int)mode); }

  void hide() { show(SW_HIDE); }
  void hideAnimated(AnimateMode mode, unsigned time) { AnimateWindow(getNativeHandle(), time, (int)mode | AW_HIDE); }

  // properties

  void setParent(HWND value) { win_print_on_fail(SetParent(getNativeHandle(), value)); }
  void setParent(Control value) { win_print_on_fail(SetParent(getNativeHandle(), value.getNativeHandle())); }
  HWND getParent() const { return GetParent(getNativeHandle()); }

  void setTopmost();

  void setPosition(const Rectangle& rect);

  static ATOM registerClass(
      cpp::wstring_view class_name,
      UINT style,
      HICON icon,
      HCURSOR cursor,
      HBRUSH background,
      cpp::wstring_view menu,
      HICON small_icon);

  static HWND find(HWND parent, cpp::wstring_view class_name, cpp::wstring_view window_name)
  {
    return FindWindowExW(parent, nullptr, class_name.data(), window_name.data());
  }

  static
  cpp::iterator_range<FindWindowIterator, FindWindowIteratorSentinel>
  findAll(HWND parent, cpp::wstring_view class_name, cpp::wstring_view window_name)
  {
    return cpp::iterator_range<FindWindowIterator, FindWindowIteratorSentinel>(
        FindWindowIterator(parent, class_name, window_name),
        FindWindowIteratorSentinel()
      );
  }

  static std::wstring getClassName(HWND hwnd);

private:

  struct ControlDeleter {
    typedef HWND pointer;
    void operator()(HWND ptr) {
      DestroyWindow(ptr);
    }
  };
  using Hwnd = std::unique_ptr<HWND, ControlDeleter>;
  using HwndRef = HWND;

  Hwnd handle_ = nullptr;

  HwndRef parent_ = nullptr;
  const wchar_t* class_ = nullptr;
  DWORD style_ = 0;
  DWORD exstyle_ = 0;

  static ATOM getWindowClass();
  static const std::wstring& getWindowClassName();

  static LRESULT CALLBACK MessageEntry(
      HWND handle,
      UINT msg,
      WPARAM wparam,
      LPARAM lparam);

protected:
  // hooks
  virtual void onCreate();
  virtual void onDestroy();
  virtual LRESULT onMessage(UINT msg, WPARAM wparam, LPARAM lparam);
};

} // namespace Windows

#endif // CONTROL_H
