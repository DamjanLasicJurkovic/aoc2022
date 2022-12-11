#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>

const int NRounds = 10000, NDiv = 1;

struct M {
  std::vector<int64_t> items;
  int op, val, div, t1, t2;
  uint64_t insp = 0;
};

int main(int argc, char** argv) {
  std::ifstream input_file;
  input_file.open("input.txt");
  std::string line;
  std::vector<M> ms;
  
  // get data
  while (std::getline(input_file, line)) {
    if (line == "") {}
    else if (line[0] == 'M') ms.push_back(M());
    else if (line[2] == 'T') ms.back().div = std::stoi(line.substr(21));
    else if (line[8] == 'r') ms.back().t1 = std::stoi(line.substr(29));
    else if (line[8] == 'a') ms.back().t2 = std::stoi(line.substr(30));
    else if (line[2] == 'O') {
      ms.back().op = line[23] == '*';
      ms.back().val = line[25] == 'o' ? -1 : std::stoi(line.substr(25));
    } else if (line[2] == 'S') {
      std::string l = line.substr(18);
      size_t i1 = 0, i2 = l.find(' ');
      while (true) {
        if (i2 == std::string::npos) i2 = l.size()+1;
        ms.back().items.push_back(std::stoi(l.substr(i1, i2-i1-1)));
        if (i2 == l.size()+1) break;
        i1 = i2+1;
        i2 = l.find(' ', i1);
      }
    }
  }

  // run simulation
  int64_t commonDiv = 1;
  for (auto& m: ms) commonDiv *= m.div;
  for (int t = 0; t < NRounds; ++t) {
    for (auto& m: ms) {
      for (auto x: m.items) {
        int v = m.val == -1 ? x : m.val;
        x = ((m.op == 1 ? x * v : x + v) / NDiv) % commonDiv;
        int t = x % m.div == 0 ? m.t1 : m.t2;
        ms[t].items.push_back(x);
        m.insp++;
      }
      m.items.resize(0);
    }
  }
  
  std::vector<uint64_t> best;
  for (auto&m : ms) best.push_back(m.insp);
  std::sort(best.begin(), best.end());
  std::cout << "Monkey business value:\n" <<
    best.back()*best[best.size()-2] << std::endl;
}
