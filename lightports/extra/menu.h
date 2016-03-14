#pragma once

#include <windows.h>
#include <memory>
#include <utility>

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
    MenuItemInfo() : data_()
    {
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
  Menu(const Menu&);
  Menu& operator=(const Menu& other);
public:
  Menu() :
    hmenu_()
  { }

  Menu(HMENU handle) :
    hmenu_(handle)
  { }

  Menu(Menu&& other) :
    hmenu_(std::move(other.hmenu_))
  { }
  Menu& operator=(Menu&& other)
  {
    hmenu_ = std::move(other.hmenu_);
    return *this;
  }

  ~Menu()
  { }

  static bool isMenu(HMENU handle) { return IsMenu(handle); }
  static Menu getMainMenu(HWND hwnd) { return Menu(GetMenu(hwnd)); }
  static Menu createMenu() { return Menu(CreateMenu()); }
  static Menu createPopupMenu() { return Menu(CreatePopupMenu()); }
  
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

  HMENU getHMENU() const { return hmenu_.get(); }

private:
  struct HMenuDeleter {
    typedef HMENU pointer;
    void operator()(HMENU ptr) { DestroyMenu(ptr); }
  };
  typedef std::unique_ptr<HMENU, HMenuDeleter> HMenu;

  HMenu hmenu_;
};

} // namespace Windows

