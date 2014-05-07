#include <iostream>
#include <vector>
#include "Csv_io.h"
#include <string>
#include "adjacency_list.h"

int main()
{
  Csv_io csv;
  std::vector<std::string> result = csv.deserialize("test_ast.csv", ';');
  for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it)
  {
    std::cout << ' ' << *it << std::endl;
  }
  csv.serialize(result, "test2.csv", ',', 3);
  return 0;
}
