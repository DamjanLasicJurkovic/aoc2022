#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

const bool CrateMover9000 = false;

int main(int argc, char** argv) {
  std::ifstream file;
  file.open("input.txt");
  std::string line;

  // get starting state
  std::getline(file, line);
  int n = (line.size() + 1) / 4;
  std::vector<std::string> state(n, "");
  do {
    if (line == "") break;
    if (line[1] == '1') continue;

    for (int i = 0; i < n; ++i) {
      char c = line[i*4 + 1];
      if (c != ' ') state[i].push_back(c);
    }
  } while (std::getline(file, line));
  for (auto& s: state) std::reverse(s.begin(), s.end());

  // iterate over commands and run the simulation
  int comm[3] = {0};
  while (std::getline(file, line)) {
    for (int tmp = line.find(' '), i = 0; i < 3; ++i) {
      int tmp2 = line.find(' ', tmp + 1);
      comm[i] = std::stoi(line.substr(tmp+1,tmp2-tmp-1));
      tmp = line.find(' ', tmp2+1);
    }
    if (CrateMover9000) {
      for (int i = 0; i < comm[0]; ++i) {
        state[comm[2] - 1].push_back(state[comm[1] - 1].back());
        state[comm[1] - 1].pop_back();
      }
    } else { //second task
      std::string& s2 = state[comm[1]-1];
      state[comm[2]-1].append(s2.substr(s2.size() - comm[0]));
      s2.erase(s2.size() - comm[0]);
    }
  }

  // print results
  std::cout << "Top chars in each column:" << std::endl;
  for (auto& s: state) std::cout << s.back();
  std::cout << std::endl;
}
