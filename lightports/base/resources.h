/*
 * resources.h
 *
 *  Created on: 09.08.2015
 *      Author: Richard
 */

#ifndef LIBS_WINDOWS_BASE_RESOURCES_H_
#define LIBS_WINDOWS_BASE_RESOURCES_H_

#include <string>
#include "module.h"
#include <lightports/core/memory.h>
#include <lightports/core/geometry.h>
#include <windows.h>
#include <lightports/extra/icon.h>

namespace Windows {

class Resources
{
public:
  static std::wstring getString(unsigned id) { return getString(Module::getInstance(), id); }
  static std::wstring getString(HINSTANCE hinstance, unsigned id);

  static HICON getIcon(int id) { return getIcon(Module::getInstance(), id); }
  static HICON getIcon(HINSTANCE hinstance, int id);

  static HICON getIcon(int id, Size size) { return getIcon(Module::getInstance(), id, size); }
  static HICON getIcon(HINSTANCE hinstance, int id, Size size);

//  static Icon getIconMetric(int id, int lim) { return getIconMetric(Module::getInstance(), id, lim); }
//  static Icon getIconMetric(HINSTANCE hinstance, int id, int lim);
};

} /* namespace Windows */

#endif /* LIBS_WINDOWS_BASE_RESOURCES_H_ */
