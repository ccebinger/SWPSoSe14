/*[--**--]
Copyright (C) 2014  SWPSoSe14Cpp Group

This program is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this
program; if not, see <http://www.gnu.org/licenses/>.*/

#ifndef CONSTANT_POOL_H_
#define CONSTANT_POOL_H_

#include <iostream>
#include <vector>

/**
 * Repräsentiert einen Constant Pool einer .class-Datei, Version 7.
 * Kann auf einem std::ostream ausgegeben werden, als Teil einer .class-Datei.
 */
class ConstantPool {
 public:
  ConstantPool();
  virtual ~ConstantPool();

  /**
   * Schreibt den Pool im .class-Dateiformat (Version 7) in den stream.
   */
  unsigned addConstant();
  std::ostream getByteArray();
};

#endif /* CONSTANT_POOL_H_ */
