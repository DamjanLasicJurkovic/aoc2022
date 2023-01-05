#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

// Relatively straightforward, just simulate the swaps, but skip repeated swaps
// by doing a modulo.

int64_t transMove(int64_t n, int64_t pos, int64_t size) {
  int64_t finalIndex = pos + n;
  
  if (finalIndex <= 0) {
    finalIndex %= (size-1);
    finalIndex = size+finalIndex;
    --finalIndex;
  }
  if (finalIndex > size - 1) {
    finalIndex %= (size-1);
  }
  return finalIndex;  
}

void doSwaps(std::vector<int64_t>& v, std::map<int64_t,int64_t>& mpos, int64_t iSrc, int64_t iTarget) {
  int tmp = v[iSrc];
  if (iTarget > iSrc) {
    for (int64_t i = iSrc; i < iTarget; ++i) {
      v[i] = v[i+1];
      mpos[v[i]] = mpos[v[i]] - 1;
    }
    v[iTarget] = tmp;
    mpos[tmp] += iTarget - iSrc;
  } else if (iTarget < iSrc) {
    for (int64_t i = iSrc; i > iTarget; --i) {
      v[i] = v[i-1];
      mpos[v[i]]++;
    }
    v[iTarget] = tmp;
    mpos[tmp] -= iSrc - iTarget;
  } 
}

void compute(const std::vector<int64_t>& init, int nRounds) {
  std::vector<int64_t> curr;
  std::map<int64_t,int64_t> mpos; // key = index in init, val = index in curr vector
  for (int64_t i = 0; i < int64_t(init.size()); ++i) { mpos[i] = i; curr.push_back(i); }
  
  for (int N = 0; N < nRounds; ++N) {
    std::cout << N << std::endl;
    for (int64_t i = 0; i < int64_t(init.size()); ++i) {
      int64_t pos = mpos[i];
      int64_t targetIndex = transMove(init[i], pos, curr.size());
      
      // do swaps in curr vector, also update positions in maps
      doSwaps(curr, mpos, pos, targetIndex);
    }
  }
  int64_t pos0i = std::distance(init.begin(), std::find(init.begin(), init.end(), 0));
  int64_t pos0 = std::distance(curr.begin(), std::find(curr.begin(), curr.end(), pos0i));
  int64_t n1 = init[curr[(pos0 + 1000) % curr.size()]];
  int64_t n2 = init[curr[(pos0 + 2000) % curr.size()]];
  int64_t n3 = init[curr[(pos0 + 3000) % curr.size()]];
  
  std::cout << "Numbers:\n" << n1 << ", " << n2 << ", " << n3 << std::endl;
  std::cout << "Sum:\n" << n1 + n2 + n3 << std::endl;
}

int main(int argc, char** argv) {
  std::ifstream file;
  file.open("input.txt");
  std::string line;
  std::vector<int64_t> init1, init2;
  
  while(std::getline(file, line)) init1.push_back(std::stoi(line));
  init2 = init1;
  for (auto& i: init2) i *= 811589153;
  
  // part 1
  std::cout << "\n--- Part1:" << std::endl;
  compute(init1, 1);
  
  // part 2
  std::cout << "\n--- Part2:" << std::endl;
  compute(init2, 10);

  return 0;
}
