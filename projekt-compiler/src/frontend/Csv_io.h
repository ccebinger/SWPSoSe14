#ifndef CSV_IO_H
#define CSV_IO_H

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <iostream>


class Csv_io
{
    public:
        typedef std::vector<std::string> vec;
        typedef const std::string& str;

        Csv_io();
        virtual ~Csv_io();
        vec deserialize(str file, char delimiter);
        void serialize(const vec& values, str file, char delimiter, int columns);
    private:
};

#endif // CSV_IO_H
