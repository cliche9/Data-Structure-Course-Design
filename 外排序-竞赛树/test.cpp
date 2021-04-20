#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

int main () {

  std::ifstream is("data/input/example.txt");   // open file

  int c;
  std::cout << is.tellg() << std::endl;
  while (is >> c)                  // loop getting single characters
    std::cout << "pre_index: " << is.tellg() << std::endl << "value: " << c << std::endl << "index: " << is.tellg() << std::endl;
  
  std::cout << c << std::endl;
  if (is.eof())                      // check for EOF
    std::cout << is.tellg() << "[EoF reached]\n";
  else
    std::cout << is.tellg() << "[error reading]\n";

  is.close();                        // close file

  return 0;
}