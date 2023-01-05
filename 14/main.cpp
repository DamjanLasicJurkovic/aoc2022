#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

// Just simulate the sand for both parts. For part2, make
// the map wide enough, so bascially add height to both x
// directions. For very high and narrow maps one could
// also go for better performance by not using a larger map
// but implicitly assuming the sand pattern in each off-limit
// direction. For this example the first option is fast enough.

const bool part2 = true;

struct P { int x, y; };

int main(int argc, char** argv) {
  std::ifstream file;
  file.open("input.txt");
  std::string line;
  P start = {500, 0};
  // Get all point sequences from input
  std::vector<std::vector<P>> paths;
  while (std::getline(file, line)) {
    int i = 0, j = 0;
    P p;
    paths.push_back({});
    while(true) {
      j = line.find(',', i);
      p.x = std::stoi(line.substr(i,j-i));
      i = j + 1;
      j = line.find('>', i);
      if (j == std::string::npos) {
        p.y = std::stoi(line.substr(i));
        paths.back().push_back(p);
        break;
      } else {
        p.y = std::stoi(line.substr(i,j-i-2));
        paths.back().push_back(p);
        i = j + 2;
      }
    }
  }

  // Compute min point x y values, and normalize all points
  P min = start, max = start;
  for (auto& v: paths) for (auto& p: v) {
    min.x = std::min(min.x, p.x); min.y = std::min(min.y, p.y);
    max.x = std::max(max.x, p.x); max.y = std::max(max.y, p.y);  }

  if (part2) {
    min.x -= max.y - min.y;
    max.x += max.y - min.y;
  }

  for (auto& v: paths) for (auto& p: v) { p.x -= min.x; p.y -= min.y; }

  start.x -= min.x; start.y -= min.y; max.x -= min.x; max.y -= min.y;

  // Generate map
  std::vector<std::string> map(max.y+1, std::string(max.x+1, '.'));
  if (part2) {
    map.push_back(std::string(map[0].size(), '.'));
    map.push_back(std::string(map[0].size(), '#'));
  }
  for (auto& v: paths) {
    P prev = v[0];
    for (int i = 1; i < v.size(); ++i) {
      P p = v[i];
      if (p.x == prev.x) {
        int start = std::min(p.y, prev.y);
        for (int j = 0; j <= std::abs(p.y-prev.y); ++j) {
           map[start+j][p.x] = '#';
        }
      } else {
        int start = std::min(p.x, prev.x);
        for (int j = 0; j <= std::abs(p.x-prev.x); ++j) {
           map[p.y][start+j] = '#';
        }
      }
      prev = p;
    }
  }
  map[start.y][start.x] = '+';

  // Run simulation
  int n = 0;
  bool run = true;
  while (run) {
    P a = start;
    while(true) {
      if (a.y == map.size() - 1) { run = false; break; }
      if (map[a.y+1][a.x] == '.') {a.y++; continue;}
      if (a.x == 0) { run = false; break; }
      if (map[a.y+1][a.x-1] == '.') {a.y++; a.x--; continue;}
      if (a.x == map[0].size() - 1) { run = false; break; }
      if (map[a.y+1][a.x+1] == '.') {a.y++; a.x++; continue;}
      map[a.y][a.x] = 'o';
      ++n;
      if (a.x == start.x && a.y == start.y) run = false;
      break;
    }
  }

  std::cout << "Nuber of sand units on map:\n" << n << std::endl;
}
