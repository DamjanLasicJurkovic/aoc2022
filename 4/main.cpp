#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>

int main(int argc, char** argv) {
  std::ifstream input_file;
  input_file.open("input.txt");
  std::string line("");
  size_t n_contained = 0;
  size_t n_overlapped = 0;
  size_t a[4] = {0,0,0,0};
  while(std::getline(input_file, line)) {
    size_t i_comma = line.find(',');
    size_t i_del1 = line.find('-');
    size_t i_del2 = line.find('-', i_del1+1);
    a[0] = std::stoi(line.substr(0, i_del1));
    a[1] = std::stoi(line.substr(i_del1+1, i_comma-i_del1-1));
    a[2] = std::stoi(line.substr(i_comma+1, i_del2-i_comma-1));
    a[3] = std::stoi(line.substr(i_del2+1, line.size()-i_del2-1));
        
    if ( (a[0] <= a[2] && a[1] >= a[3]) ||
         (a[0] >= a[2] && a[1] <= a[3]) ) {
         ++n_contained;
        }
        
    if ( (a[0] >= a[2] && a[0] <= a[3]) ||
         (a[1] >= a[2] && a[1] <= a[3]) ||
         (a[2] >= a[0] && a[2] <= a[1]) ||
         (a[3] >= a[0] && a[3] <= a[1]) ) {
         ++n_overlapped;
        }       
  }

  std::cout << "Nr of fully contained ranges:\n" << n_contained << std::endl;
  std::cout << "Nr of overlapped ranges:\n" << n_overlapped << std::endl;
   
  return 0;
}
