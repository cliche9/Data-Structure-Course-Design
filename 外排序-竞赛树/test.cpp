#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

int main () {

  std::ifstream is("data/example.txt");   // open file

  char c;
  while (is.get(c))                  // loop getting single characters
    std::cout << is.tellg() << std::endl << c << std::endl;

  if (is.eof())                      // check for EOF
    std::cout << is.tellg() << "[EoF reached]\n";
  else
    std::cout << is.tellg() << "[error reading]\n";

  is.close();                        // close file

  return 0;
}