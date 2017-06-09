#ifndef CONTROL_H
#define CONTROL_H

#include <cpp-utils/assert.h>
#include <cpp-utils/iterator_range.h>
#include <cpp-utils/strings/string_view.h>
#include <lightports/core/debug.h>
#include <lightports/core/debugstream.h>
#include <lightports/core/exception.h>
#include <lightports/user/geometry.h>
#include <lightports/core/macros.h>
#include <windows.h>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>

namespace Windows {

class Control;
class Window;

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

namespace Internal {

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

using FindWindowRange = cpp::iterator_range<FindWindowIterator, FindWindowIteratorSentinel>;

class ControlCommonBase
{
public:
  static void setPosition(HWND hwnd, const Rectangle& rect);
  static std::wstring getClassName(HWND hwnd);
  static std::wstring getWindowText(HWND hwnd);
};

template<typename Derived>
class ControlBase : protected ControlCommonBase
{
public:
  // properties

  Rectangle getClientRect() const
  {
    RECT rc;
    win_throw_on_fail(GetClientRect(getHWND(), &rc));
    return rc;
  }

  bool okay() const {
    return bool(getHWND());
  }
  explicit operator bool() const {
    return okay();
  }

  // base functions

  void show(int show_command = SW_SHOW)
  {
    win_throw_on_fail(::ShowWindow(getHWND(), show_command));
  }

  void showAnimated(AnimateMode mode, unsigned time)
  {
    win_throw_on_fail(::AnimateWindow(getHWND(), time, (int)mode));
  }

  void hide()
  {
    show(SW_HIDE);
  }

  void hideAnimated(AnimateMode mode, unsigned time)
  {
    win_throw_on_fail(::AnimateWindow(getHWND(), time, (int)mode | AW_HIDE));
  }

  // properties

  void setParent(HWND value)
  {
    win_throw_on_fail(::SetParent(getHWND(), value));
  }

  void setParent(const Control& value);

  HWND getParent() const
  {
    return win_throw_on_fail(::GetParent(getHWND()));
  }

  void setTopmost()
  {
    win_throw_on_fail(
      ::SetWindowPos(getHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE)
    );
  }

  void setPosition(const Rectangle& rect)
  { ControlCommonBase::setPosition(getHWND(), rect); }

  std::wstring getClassName() const
  { return ControlCommonBase::getClassName(getHWND()); }

  std::wstring getWindowText() const
  { return ControlCommonBase::getWindowText(getHWND()); }

  Rectangle getWindowRect() const
  {
    RECT result;
    win_throw_on_fail(::GetWindowRect(getHWND(), &result));
    return result;
  }

  HWND findChild(cpp::wstring_view class_name, cpp::wstring_view window_name) const
  {
    return win_throw_on_fail(
      ::FindWindowExW(getHWND(), nullptr, class_name.data(), window_name.data())
    );
  }

  FindWindowRange findChilds(cpp::wstring_view class_name, cpp::wstring_view window_name) const
  {
    return cpp::make_range(
        FindWindowIterator(getHWND(), class_name, window_name),
        FindWindowIteratorSentinel()
      );
  }

  DWORD getProcessId() const
  {
    DWORD pid;
    ::GetWindowThreadProcessId(getHWND(), &pid);
    return pid;
  }

  DWORD getThreadId() const
  {
    return ::GetWindowThreadProcessId(getHWND(), nullptr);
  }

  bool operator==(HWND rhs) const
  {
    return getHWND() == rhs;
  }
  bool operator!=(HWND rhs) const
  {
    return getHWND() != rhs;
  }

private:
  HWND getHWND() const { return static_cast<const Derived*>(this)->getHWND(); }
};

} // namespace Internal

class Window : public Internal::ControlBase<Window>
{
public:
  constexpr Window() : hwnd_(nullptr) { }
  constexpr Window(HWND hwnd) : hwnd_(hwnd) { }
  Window(const Control& control);

  constexpr HWND getHWND() const { return hwnd_; }

  // enums

  static Window find(cpp::wstring_view class_name, cpp::wstring_view window_name)
  {
    return FindWindowExW(nullptr, nullptr, class_name.data(), window_name.data());
  }

  static
  Internal::FindWindowRange
  findAll(cpp::wstring_view class_name, cpp::wstring_view window_name)
  {
    return cpp::make_range(
      Internal::FindWindowIterator(nullptr, class_name, window_name),
      Internal::FindWindowIteratorSentinel()
    );
  }

  static Window at(Point pt)
  {
    return ::WindowFromPoint(pt.getNative());
  }

private:
  HWND hwnd_;
};

class Control : public Internal::ControlBase<Control>
{
public:
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

  void create(HWND parent, cpp::wstring_view title, const Rectangle& rect) {
    create(parent, title, rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());
  }
  void create(HWND parent, cpp::wstring_view title, int x, int y, int width, int height);

  void destroy();

  HWND getHWND() const {
    return handle_.get();
  }

  void setTopmost();

  static ATOM registerClass(
      cpp::wstring_view class_name,
      UINT style,
      HICON icon,
      HCURSOR cursor,
      HBRUSH background,
      cpp::wstring_view menu,
      HICON small_icon);

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

// impl's

inline
Window::Window(const Control& control)
: hwnd_(control.getHWND())
{ }

template<typename Derived> inline
void Internal::ControlBase<Derived>::setParent(const Control& value)
{
  win_throw_on_fail(::SetParent(getHWND(), value.getHWND()));
}

} // namespace Windows

#endif // CONTROL_H
