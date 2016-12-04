/*
 * resources.h
 *
 *  Created on: 09.08.2015
 *      Author: Richard
 */

#ifndef LIBS_WINDOWS_BASE_RESOURCES_H_
#define LIBS_WINDOWS_BASE_RESOURCES_H_

#include <windows.h>
#include <string>

#include <lightports/core/memory.h>
#include <lightports/user/geometry.h>
#include <lightports/user/icon.h>

namespace Windows {

class Resources
{
public:
  static std::wstring getString(HINSTANCE hinstance, unsigned id);

  static IconView getIcon(HINSTANCE hinstance, int id);

  static IconView getIcon(HINSTANCE hinstance, int id, Size size);

//  static IconHandle getIconMetric(HINSTANCE hinstance, int id, int lim);
};

} /* namespace Windows */

#endif /* LIBS_WINDOWS_BASE_RESOURCES_H_ */
