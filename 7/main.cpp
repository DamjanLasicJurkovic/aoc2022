#include <iostream>
#include <fstream>
#include <string>
#include <map>

void cd(std::string& p, const std::string& dst) {
  if (dst == "/")
    p = "/";
  else if (dst == "..")
    p = p.rfind('/') == 0 ? "/" : p.substr(0, p.rfind('/'));
  else
    p += p == "/" ? dst : "/" + dst;
}

void addToParents(std::map<std::string, size_t>& m, const std::string& path, const size_t sz) {
  std::string p = path;
  while (true) {
    if (p == "/") return;
    cd(p, "..");
    m[p] += sz;
  }
}

int main(int argc, char** argv) {
  std::ifstream file;
  file.open("input.txt");
  std::string line, path;
  std::map<std::string, size_t> self, all;

  // Construct map of sums of sizes of only files in paths
  size_t sum = 0;
  bool ls = false;
  while (std::getline(file, line)) {
    if (line[0] != '$') { // we are in ls output, add file sz to sum
       if (line[0] != 'd')
          sum += std::stoi(line.substr(0, line.find(' ')));
    } else { // command
      if (ls) { // we just exited ls output, add sum to map
        ls = false;
        if (self.find(path) == self.end()) {
          self[path] = sum;
          all[path] = sum;
        }
      }
      if (line == "$ ls") { // ls command
        ls = true;
        sum = 0;
      } else { // cd command
        cd(path, line.substr(5));
      }
    }
  }
  // if ls was called last we do one last write
  if (ls && self.find(path) == self.end()) {
    self[path] = sum;
    all[path] = sum;
  }

  // now traverse and add sum of fileizes to all parent dirs
  for (auto& [p, sz]: self) addToParents(all, p, sz);

  // compute and print results
  size_t sum_part_1 = 0;
  size_t sz_part_2_least = 30000000 + all["/"] - 70000000;
  size_t sz_part_2 = 70000000;
  for (auto& [p, sz]: all) {
    if (sz <= 100000) sum_part_1 += sz;
    if (sz >= sz_part_2_least && sz < sz_part_2) sz_part_2 = sz;
  }
  std::cout << "Combined sum of all dir sizes below 100000:\n" << sum_part_1 << std::endl;
  std::cout << "Size of folder to delete for part 2:\n" << sz_part_2 << std::endl;
}
