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

#ifndef PROJEKT_COMPILER_SRC_BACKEND_CLASSFILE_BYTECODE_WRITER_H_
#define PROJEKT_COMPILER_SRC_BACKEND_CLASSFILE_BYTECODE_WRITER_H_

#include <iostream>
#include <vector>

class Bytecode_writer {
 public:
    explicit Bytecode_writer(std::ostream* file);
    virtual ~Bytecode_writer();

    void writeU8(uint8_t value);
    void writeU16(uint16_t value);
    void writeU32(uint32_t value);
    void writeVector(std::vector<unsigned char> values);

 protected:
    std::ostream& filestream;
    void write(int value, uint8_t size);
};

#endif  // PROJEKT_COMPILER_SRC_BACKEND_CLASSFILE_BYTECODE_WRITER_H_
