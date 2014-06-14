#ifndef BYTECODE_WRITER_H
#define BYTECODE_WRITER_H
#include <iostream>
#include <vector>

class Bytecode_writer
{
  public:
    Bytecode_writer(std::ostream& file);
    virtual ~Bytecode_writer();

    void writeU8(uint8_t value);
    void writeU16(uint16_t value);
    void writeU32(uint32_t value);
    void writeVector(std::vector<unsigned char> values);
  protected:
  private:
    std::ostream& filestream;
    void write(int value, uint8_t size);
};

#endif // BYTECODE_WRITER_H
