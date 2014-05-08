#include "frontend/Csv_io.h"

Csv_io::Csv_io()
{
    //ctor
}

Csv_io::~Csv_io()
{
    //dtor
}


Csv_io::vec Csv_io::deserialize(Csv_io::str file, char delimiter)
{
  std::ifstream infile(file);
  Csv_io::vec result;
  std::string line;
  while(std::getline(infile, line))
  {
    std::stringstream lineStream(line);
    std::string cell;
    while(std::getline(lineStream, cell, delimiter))
    {
      result.push_back(cell);
    }
  }
  infile.close();
  return result;
}

void Csv_io::serialize(const Csv_io::vec& values, Csv_io::str file, char delimiter, int columns)
{
  std::ofstream outfile(file);
  int count = 1;
  for (Csv_io::vec::const_iterator it = values.begin(); it != values.end(); it++)
  {
    outfile << *it ;
    if ((count % columns) == 0)
      outfile << std::endl;
    else
      outfile << delimiter;
    count++;
  }
  outfile.close();
}
