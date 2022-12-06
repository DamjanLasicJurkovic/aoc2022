#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

const int N = 14;

int main(int argc, char** argv) {
  std::ifstream file;
  file.open("input.txt");
  std::string line, buf, tmp;
  for (int i = 0; i < N; ++i) buf.push_back(file.get());
  for (int i = N;; ++i) {
    // Insertion into sorted or hashtable preferable for large inputs
    tmp = buf;
    std::sort(tmp.begin(), tmp.end());
    if (std::unique(tmp.begin(), tmp.end()) == tmp.end()) {
      std::cout << "N chars read to get marker:" << i << std::endl;
      break;
    }
    buf[i % N] = file.get();
  }
}
