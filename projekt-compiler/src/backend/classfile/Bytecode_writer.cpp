#include "Bytecode_writer.h"

Bytecode_writer::Bytecode_writer(std::ostream* file) : filestream(*file) {
  //ctor
}

Bytecode_writer::~Bytecode_writer() {
  filestream.clear();
  //dtor
}

void Bytecode_writer::write(int value, uint8_t size) {
  uint8_t bsize = size / 8;
  std::vector<uint8_t> bytes(bsize);

  for (int i = 0; i < bsize; i++) {
    uint8_t shift =  (size - ((i+1) * 8));
    bytes[i] = (value >> shift) & 0xFF;
  }

  filestream.write((char*) &bytes[0], bsize);
}

void Bytecode_writer::writeU8(uint8_t value) {
  write(value, 8);
}

void Bytecode_writer::writeU16(uint16_t value) {
  write(value, 16);
  /*
    uint8_t bytes[2];
    bytes[0] = (value >> 8) & 0xFF; //high byte
    bytes[1] = (value) & 0xFF; // low byte

    filestream.write((char *) bytes, 2);*/
}


void Bytecode_writer::writeU32(uint32_t value) {
  write(value, 32);
  /*
    uint8_t bytes[4];
    bytes[0] = (value >> 24) & 0xFF; //high byte
    bytes[1] = (value >> 16) & 0xFF; //second high
    bytes[2] = (value >> 8) & 0xFF; //second low
    bytes[3] = (value) & 0xFF; //low byte

    filestream.write((char*) bytes, 4);*/
}

void Bytecode_writer::writeVector(std::vector<unsigned char> values) {
  filestream.write(reinterpret_cast<char*> (&values[0]) , values.size());
}



void Bytecode_writer::write_array(size_t len, const unsigned char arr[])
{
  for (size_t i = 0; i < len; i++)
    writeU8((uint8_t) arr[i]);
}


void Bytecode_writer::write_array(size_t len, const char arr[])
{
  for (size_t i = 0; i < len; i++)
    writeU8((uint8_t) arr[i]);
}
