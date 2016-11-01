/*
 * cursor.h
 *
 *  Created on: 02.09.2016
 *      Author: Richard
 */

#ifndef LIBS_LIGHTPORTS_LIGHTPORTS_EXTRA_CURSOR_H_
#define LIBS_LIGHTPORTS_LIGHTPORTS_EXTRA_CURSOR_H_

namespace Windows {

Point getCursorPosition()
{
  POINT pt;
  GetCursorPos(&pt);
  return pt;
}

}  // namespace Windows

#endif /* LIBS_LIGHTPORTS_LIGHTPORTS_EXTRA_CURSOR_H_ */
