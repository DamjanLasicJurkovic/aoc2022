#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

int main(int argc, char** argv) {
  std::ifstream input_file;
  input_file.open("input.txt");
  std::string line("");
  std::vector<size_t> sums;
  sums.push_back(0);
  while(std::getline(input_file, line))
    if (line == "") sums.push_back(0);
    else sums.back() += std::stoi(line);
    
  std::sort(sums.begin(), sums.end(), std::greater<size_t>());
  size_t sum = 0;
  for (int i = 0; i < 3; ++i) sum += sums[i];
  
  std::cout << "Elf with most calories:\n" << sums[0] << std::endl;
  std::cout << "Elves with top three calories (sum):\n" << sum << std::endl;
   
  return 0;
}
