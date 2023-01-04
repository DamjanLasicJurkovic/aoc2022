#include <iostream>
#include <fstream>
#include <string>
#include <map>

// first part
std::map<std::string, size_t> scoring = {
  {"A X", 1 + 3},
  {"A Y", 2 + 6},
  {"A Z", 3 + 0},
  {"B X", 1 + 0},
  {"B Y", 2 + 3},
  {"B Z", 3 + 6},
  {"C X", 1 + 6},
  {"C Y", 2 + 0},
  {"C Z", 3 + 3}
};

// second part
std::map<std::string, size_t> scoringSecond = {
  {"A X", 3 + 0},
  {"A Y", 1 + 3},
  {"A Z", 2 + 6},
  {"B X", 1 + 0},
  {"B Y", 2 + 3},
  {"B Z", 3 + 6},
  {"C X", 2 + 0},
  {"C Y", 3 + 3},
  {"C Z", 1 + 6}
};


int main(int argc, char** argv) {
  std::ifstream input_file;
  input_file.open("input.txt");
  std::string line("");
  size_t scoreFirst = 0;
  size_t scoreSecond = 0;
  while(std::getline(input_file, line)) {
    scoreFirst += scoring[line];
    scoreSecond += scoringSecond[line];
  }

  std::cout << "Final score (first part):\n" << scoreFirst << std::endl;
  std::cout << "Final score (second part):\n" << scoreSecond << std::endl;
}
