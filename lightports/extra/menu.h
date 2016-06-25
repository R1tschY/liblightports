#pragma once

#include <windows.h>
#include <memory>
#include <utility>

#include <cpp-utils/algorithm/zero.h>

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

class MenuItemInfo
{
public:
  MenuItemInfo()
  {
    cpp::zero(data_);
    data_.cbSize = sizeof(MENUITEMINFO);
  }

  void setState(UINT value)
  {
    data_.fMask |= MIIM_STATE;
    data_.fState = value;
  }

  // TODO

  const MENUITEMINFO& getMENUITEMINFO() const { return data_; }
  MENUITEMINFO& getMENUITEMINFO() { return data_; }

private:
  MENUITEMINFO data_;
};

class Menu {
public:
  Menu() :
    hmenu_()
  { }

  Menu(HMENU handle) :
    hmenu_(handle)
  { }

  static bool isMenu(HMENU handle) { return IsMenu(handle); }

  // get menus
  static Menu getMainMenu(HWND hwnd) { return Menu(GetMenu(hwnd)); }

  /// \brief gets a copy of the system menu of \p hwnd
  static Menu getSystemMenu(HWND hwnd) { return Menu(GetSystemMenu(hwnd, FALSE)); }

  // create menus
  static Menu createMenu() { return Menu(CreateMenu()); }
  static Menu createPopupMenu() { return Menu(CreatePopupMenu()); }
  
  // properties

  bool okay() const { return IsMenu(hmenu_.get()); }
  explicit operator bool() const { return okay(); }

  // collection

  bool existsEntry(unsigned command) const
  {
    return GetMenuState(hmenu_.get(), command, MF_BYCOMMAND) != -1;
  }

  // modify menus

  void addMenu(const std::wstring& caption, const Menu& submenu, MenuEntryFlags flags = MenuEntryFlags::Enabled)
  {
    ::AppendMenuW(hmenu_.get(), MF_STRING | MF_POPUP | static_cast<int>(flags), reinterpret_cast<UINT_PTR>(submenu.hmenu_.get()), caption.c_str());
  }

  void addEntry(unsigned command, const std::wstring& caption, MenuEntryFlags flags = MenuEntryFlags::Enabled)
  {
    ::AppendMenuW(hmenu_.get(), MF_STRING | static_cast<int>(flags), command, caption.c_str());
  }

  void insertEntry(unsigned position, unsigned command, const std::wstring& caption, MenuEntryFlags flags = MenuEntryFlags::Enabled)
  {
    ::InsertMenuW(hmenu_.get(), position, MF_BYPOSITION | MF_STRING | static_cast<int>(flags), command, caption.c_str());
  }

  void insertEntryBefore(unsigned entry_command, unsigned command, const std::wstring& caption, MenuEntryFlags flags = MenuEntryFlags::Enabled)
  {
    ::InsertMenuW(hmenu_.get(), entry_command, MF_BYCOMMAND | MF_STRING | static_cast<int>(flags), command, caption.c_str());
  }

  void insertEntry(unsigned position, const MenuItemInfo& item)
  {
    ::InsertMenuItemW(hmenu_.get(), position, true, &item.getMENUITEMINFO());
  }

  void insertEntryBefore(unsigned entry_command, const MenuItemInfo& item)
  {
    ::InsertMenuItemW(hmenu_.get(), entry_command, false, &item.getMENUITEMINFO());
  }

  void addSeperator()
  {
    ::AppendMenuW(hmenu_.get(), MF_SEPARATOR, 0, nullptr);
  }

  void insertSeperator(unsigned position)
  {
    ::InsertMenuW(hmenu_.get(), position, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr);
  }

  void insertSeperatorBefore(unsigned entry_command)
  {
    ::InsertMenuW(hmenu_.get(), entry_command, MF_BYCOMMAND | MF_SEPARATOR, 0, nullptr);
  }

  bool isEntryChecked(unsigned command) const
  {
    return GetMenuState(hmenu_.get(), command, MF_BYCOMMAND) & MF_CHECKED;      
  }

  void modifyEntry(unsigned command, const std::wstring& caption, MenuEntryFlags flags = MenuEntryFlags::Enabled)
  {
    ::ModifyMenuW(hmenu_.get(), command, MF_BYCOMMAND | MF_STRING | static_cast<int>(flags), command, caption.c_str());
  }

  void modifyEntry(unsigned command, unsigned new_command, const std::wstring& caption, MenuEntryFlags flags = MenuEntryFlags::Enabled)
  {
    ::ModifyMenuW(hmenu_.get(), command, MF_BYCOMMAND | MF_STRING | static_cast<int>(flags), new_command, caption.c_str());
  }

  void modifyEntryAt(unsigned position, unsigned command, const std::wstring& caption, MenuEntryFlags flags = MenuEntryFlags::Enabled)
  {
    ::ModifyMenuW(hmenu_.get(), position, MF_BYPOSITION | MF_STRING | static_cast<int>(flags), command, caption.c_str());
  }

  void check(unsigned command, bool value)
  {
    CheckMenuItem(hmenu_.get(), command, MF_BYCOMMAND | (value ? MF_CHECKED : MF_UNCHECKED));
  }

  void checkAt(unsigned position, bool value)
  {
    CheckMenuItem(hmenu_.get(), position, MF_BYPOSITION | (value ? MF_CHECKED : MF_UNCHECKED));
  }

  void enable(unsigned command, bool value, bool grayed = false)
  {
    EnableMenuItem(hmenu_.get(), command, MF_BYCOMMAND | (value ? MF_ENABLED : (grayed) ? MF_GRAYED : MF_DISABLED));
  }

  void enableAt(unsigned position, bool value, bool grayed = false)
  {
    EnableMenuItem(hmenu_.get(), position, MF_BYPOSITION | (value ? MF_ENABLED : (grayed) ? MF_GRAYED : MF_DISABLED));
  }

  void deleteEntry(unsigned command)
  {
    ::DeleteMenu(hmenu_.get(), command, MF_BYCOMMAND);
  }

  void deleteEntryAt(unsigned position)
  {
    ::DeleteMenu(hmenu_.get(), position, MF_BYPOSITION);
  }

  // utils

  /// \brief resets the system menu to the default state
  static void resetSystemMenu(HWND hwnd) { GetSystemMenu(hwnd, TRUE); }

  // native handle

  HMENU getHMENU() const { return hmenu_.get(); }

private:
  struct HMenuDeleter {
    typedef HMENU pointer;
    void operator()(HMENU ptr) { DestroyMenu(ptr); }
  };

  std::unique_ptr<HMENU, HMenuDeleter> hmenu_;
};

} // namespace Windows

