#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
  std::ifstream input_file;
  input_file.open("input.txt");
  std::string line;
  int X = 1, cycle = 0, v = 0, part1 = 0;
  std::vector<std::string> screen(6, std::string(40, '.'));
  while(++cycle) {
    if (cycle % 40 == 20) part1 += cycle * X;
    if ( std::abs(cycle % 40 - X % 40 - 1) < 2 )
      screen[cycle/40][cycle % 40] = '#';
    
    if (v == 0) {
      if (!std::getline(input_file, line)) break;
      if (line[0] == 'a') v = std::stoi(line.substr(5));
    } else { X += v; v = 0; }
  }
     
  std::cout << "Sum signal power on specified cycles:\n" << part1 << std::endl;
  std::cout << "Final drawn screen (part 2):\n";
  for (auto& s: screen) std::cout << s << std::endl;
}
