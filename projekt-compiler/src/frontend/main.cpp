#include <iostream>
#include <vector>
#include "Csv_io.h"
#include <string>
#include <regex>
#include "adjacency_list.h"
#include "Invalid_Format_Exception.h"

void checkVec(std::vector<std::string> result)
{
  std::regex reg("\[[a-zA-z0-9]+\]"); //THE FUNCTION NAMES

  for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it)
  {
    if (std::regex_match((*it).begin(), (*it).end(), reg))
    {
      std::cout << "FUNCTION: " << *it << std::endl;
    }
  }
}

Command getCommand(std::string& cmd)
{
  Command c;
  std::regex regex_push("[0-9]|\[[0-9]\]|\['[a-zA-z0-9]+'\]");
  std::regex regex_add("a");
  if (std::regex_match(cmd.begin(), cmd.end(), regex_push))
  {
    c.type = Command::Type::PUSH_CONST;
  }
  else if (std::regex_match(cmd.begin(), cmd.end(), regex_add))
  {
    c.type = Command::Type::ADD;
  }
  else
  {
    c.type = Command::Type::OUTPUT;
  }

  size_t pos = cmd.find("[");
  if (pos != std::string::npos)
  {
    cmd.erase(pos); //remove [
    cmd.erase(cmd.end()-1); //remove ]
  }
  c.arg = &cmd;
  return c;
}
void des(const std::string& file, char delimiter)
{
  std::regex reg("\[[a-zA-z0-9]+\]"); //THE FUNCTION NAMES
  std::ifstream infile(file);
  std::string line;
  std::getline(infile, line); //must contain function name
  if (std::regex_match(line.begin(), line.end(), reg))
  {
    Adjacency_list adj(line);
    std::cout << "Graph: " << adj.name() << std::endl;
    bool hasNextLine = std::getline(infile, line);
    while(!std::regex_match(line.begin(), line.end(), reg) && hasNextLine)
    {
      Node n;
      std::stringstream lineStream(line);
      std::string cell;
      if (std::getline(lineStream, cell, delimiter)) // id
        n.id = std::stoi(cell);
      if (std::getline(lineStream, cell, delimiter)) // arg
        n.command = getCommand(cell);
      std::cout << "Node: " << n.id << " Cmd Type:" << n.command.type << " Arg: " << *n.command.arg << std::endl;
    // if (std::getline(lineStream, cell, delimiter)) // adj
      //todo
      hasNextLine = std::getline(infile, line);
    }
    infile.close();
  }
  else
  {
    throw ife;
  }
}

int main()
{
  Csv_io csv;
  std::vector<std::string> result = csv.deserialize("src/frontend/test_ast.csv", ';');
  checkVec(result);
  des("src/frontend/test_ast.csv", ';');
  csv.serialize(result, "test2.csv", ',', 3);
  return 0;
}
