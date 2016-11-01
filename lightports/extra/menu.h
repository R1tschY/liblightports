#pragma once

#include <windows.h>
#include <memory>
#include <utility>

#include <cpp-utils/algorithm/zero.h>
#include <cpp-utils/strings/string_view.h>

#include "../core/geometry.h"

namespace Windows {

enum class MenuEntryFlags {
  Checked = MF_CHECKED,
  Disabled = MF_DISABLED,
  Enabled = MF_ENABLED,
  Grayed = MF_GRAYED,
  Unchecked = MF_UNCHECKED
};

inline
MenuEntryFlags operator|(MenuEntryFlags a, MenuEntryFlags b)
{
    return static_cast<MenuEntryFlags>(static_cast<int>(a) | static_cast<int>(b));
}

class MenuItem
{
public:
  MenuItem()
  {
    data_.cbSize = sizeof(MENUITEMINFO);
  }

  MenuItem(const MENUITEMINFO& menu_item)
  : data_(menu_item)
  { }

  void setState(UINT value)
  {
    data_.fMask |= MIIM_STATE;
    data_.fState = value;
  }

  void setText(cpp::wstring_view text)
  {
    data_.fMask |= MIIM_FTYPE | MIIM_STRING;
    data_.fType |= MFT_STRING;

    text_.assign(text.begin(), text.end());
    data_.dwTypeData = const_cast<wchar_t*>(text_.c_str());
    data_.cch = text_.size();
  }

  const std::wstring& getText() const { return text_; }





  // TODO

  const MENUITEMINFO& getMENUITEMINFO() const { return data_; }
  MENUITEMINFO& getMENUITEMINFO() { return data_; }

private:
  MENUITEMINFO data_ = { };
  std::wstring text_;
};

struct HMenuDeleter {
  void operator()(HMENU ptr) { DestroyMenu(ptr); }
};

template<typename DataContainer>
class MenuBase {
public:
  MenuBase() :
    hmenu_()
  { }

  explicit MenuBase(HMENU handle) :
    hmenu_(handle)
  { }

  // properties

  bool okay() const { return getHMENU() != nullptr && ::IsMenu(getHMENU()); }
  explicit operator bool() const { return okay(); }

  // collection

  bool existsEntry(unsigned command) const
  {
    return ::GetMenuState(getHMENU(), command, MF_BYCOMMAND) != UINT(-1);
  }

  // modify menus

  void addMenu(cpp::wstring_view caption, const MenuBase& submenu, MenuEntryFlags flags = MenuEntryFlags::Enabled)
  {
    ::AppendMenuW(getHMENU(), MF_STRING | MF_POPUP | static_cast<int>(flags), reinterpret_cast<UINT_PTR>(submenu.hmenu_.get()), caption.data());
  }

  void addEntry(unsigned command, cpp::wstring_view caption, MenuEntryFlags flags = MenuEntryFlags::Enabled)
  {
    ::AppendMenuW(getHMENU(), MF_STRING | static_cast<int>(flags), command, caption.data());
  }

  void insertEntry(unsigned position, unsigned command, cpp::wstring_view caption, MenuEntryFlags flags = MenuEntryFlags::Enabled)
  {
    ::InsertMenuW(getHMENU(), position, MF_BYPOSITION | MF_STRING | static_cast<int>(flags), command, caption.data());
  }

  void insertEntryBefore(unsigned entry_command, unsigned command, cpp::wstring_view caption, MenuEntryFlags flags = MenuEntryFlags::Enabled)
  {
    ::InsertMenuW(getHMENU(), entry_command, MF_BYCOMMAND | MF_STRING | static_cast<int>(flags), command, caption.data());
  }

  void insertEntry(unsigned position, const MenuItem& item)
  {
    ::InsertMenuItemW(getHMENU(), position, true, &item.getMENUITEMINFO());
  }

  void insertEntryBefore(unsigned entry_command, const MenuItem& item)
  {
    ::InsertMenuItemW(getHMENU(), entry_command, false, &item.getMENUITEMINFO());
  }

  void addSeperator()
  {
    ::AppendMenuW(getHMENU(), MF_SEPARATOR, 0, nullptr);
  }

  void insertSeperator(unsigned position, unsigned sep_command = 0)
  {
    ::InsertMenuW(getHMENU(), position, MF_BYPOSITION | MF_SEPARATOR, sep_command, nullptr);
  }

  void insertSeperatorBefore(unsigned entry_command, unsigned sep_command = 0)
  {
    ::InsertMenuW(getHMENU(), entry_command, MF_BYCOMMAND | MF_SEPARATOR, sep_command, nullptr);
  }

  bool isEntryChecked(unsigned command) const
  {
    return GetMenuState(getHMENU(), command, MF_BYCOMMAND) & MF_CHECKED;
  }

  void modifyEntry(unsigned command, cpp::wstring_view caption, MenuEntryFlags flags = MenuEntryFlags::Enabled)
  {
    ::ModifyMenuW(getHMENU(), command, MF_BYCOMMAND | MF_STRING | static_cast<int>(flags), command, caption.data());
  }

  void modifyEntry(unsigned command, unsigned new_command, cpp::wstring_view caption, MenuEntryFlags flags = MenuEntryFlags::Enabled)
  {
    ::ModifyMenuW(getHMENU(), command, MF_BYCOMMAND | MF_STRING | static_cast<int>(flags), new_command, caption.data());
  }

  void modifyEntryAt(unsigned position, unsigned command, cpp::wstring_view caption, MenuEntryFlags flags = MenuEntryFlags::Enabled)
  {
    ::ModifyMenuW(getHMENU(), position, MF_BYPOSITION | MF_STRING | static_cast<int>(flags), command, caption.data());
  }

  void check(unsigned command, bool value = true)
  {
    CheckMenuItem(getHMENU(), command, MF_BYCOMMAND | (value ? MF_CHECKED : MF_UNCHECKED));
  }

  void checkAt(unsigned position, bool value = true)
  {
    CheckMenuItem(getHMENU(), position, MF_BYPOSITION | (value ? MF_CHECKED : MF_UNCHECKED));
  }

  void enable(unsigned command, bool value = true, bool grayed = false)
  {
    EnableMenuItem(getHMENU(), command, MF_BYCOMMAND | (value ? MF_ENABLED : (grayed) ? MF_GRAYED : MF_DISABLED));
  }

  void enableAt(unsigned position, bool value = true, bool grayed = false)
  {
    EnableMenuItem(getHMENU(), position, MF_BYPOSITION | (value ? MF_ENABLED : (grayed) ? MF_GRAYED : MF_DISABLED));
  }

  void deleteEntry(unsigned command)
  {
    ::DeleteMenu(getHMENU(), command, MF_BYCOMMAND);
  }

  void deleteEntryAt(unsigned position)
  {
    ::DeleteMenu(getHMENU(), position, MF_BYPOSITION);
  }

  // native handle

  HMENU getHMENU() const { return &*hmenu_; }

private:
  DataContainer hmenu_;
};

using Menu = MenuBase<std::unique_ptr<typename std::remove_pointer<HMENU>::type, HMenuDeleter>>;

class MenuView : public MenuBase<HMENU>
{
  using Base = MenuBase<HMENU>;
public:
  using Base::Base;

  MenuView(const Menu& menu) : MenuBase<HMENU>(menu.getHMENU()) { }
};

inline
bool isMenu(HMENU handle) { return IsMenu(handle); }

// get menus
inline
MenuView getMainMenu(HWND hwnd) { return MenuView(GetMenu(hwnd)); }

/// \brief gets a copy of the system menu of \p hwnd
inline
MenuView getSystemMenu(HWND hwnd) { return MenuView(GetSystemMenu(hwnd, FALSE)); }

// create menus
inline Menu createMenu() { return Menu(CreateMenu()); }
inline Menu createPopupMenu() { return Menu(CreatePopupMenu()); }

/// \brief resets the system menu to the default state
inline void resetSystemMenu(HWND hwnd) { GetSystemMenu(hwnd, true); }

inline void openPopupMenu(MenuView menu, Point pt, HWND window)
{
  ::TrackPopupMenu(menu.getHMENU(),
      TPM_LEFTALIGN | TPM_RIGHTBUTTON,
      pt.getX(), pt.getY(), 0, window, nullptr);
}

} // namespace Windows

