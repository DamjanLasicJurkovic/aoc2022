#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>

int main(int argc, char** argv) {
  std::ifstream input_file;
  input_file.open("input.txt");
  std::string line("");
  size_t sum = 0;
  size_t sum_badge = 0;
  std::string triplet = "";
  size_t triplet_n = 0;
  while(std::getline(input_file, line)) {
    // Handle second part
    ++triplet_n;
    // Add line charset to triplet line
    {
    std::string tmp(line);
    std::sort(tmp.begin(), tmp.end());
    char last = '#';
    for (char c : tmp) {
      if (c != last) {
        triplet += c;
        last = c;
      }
    }
    }
    if (triplet_n == 3) {
      std::sort(triplet.begin(), triplet.end());
      size_t char_count = 1;
      char last = '#';
      for (auto c : triplet) {
        if (c == last) {
          ++char_count;
        } else {
          char_count = 1;
          last = c;
        }
        if (char_count == 3) {
          sum_badge += std::isupper(c) ? c - 'A' + 27 : c - 'a' + 1;
          break;
        }
      }
      
      triplet_n = 0;
      triplet = "";
    }
        
    
    // Handle first part
    size_t n = line.size() / 2;
    std::sort(line.begin(), line.begin() + n);
    std::sort(line.begin() + n, line.end());
    
    // Loop 0-n and see which duplicated
    size_t i1 = 0, i2 = n;
    while (true) {
      char c1 = line[i1], c2 = line[i2];        
      if (c1 == c2) {
        sum += std::isupper(c1) ? c1 - 'A' + 27 : c1 - 'a' + 1;
        break;
      } else if (c1 > c2) {
        ++i2;
      } else { // c1 < c2
        ++i1;
      }
    }
  }

  std::cout << "Sum of misplaced priorities:\n" << sum << std::endl;
  std::cout << "Sum of group badges:\n" << sum_badge << std::endl;
}
