#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

int main(int argc, char** argv) {
  std::ifstream file;
  file.open("input.txt");
  std::string line;
  
  std::vector<std::vector<int>> pts;
  
  while(std::getline(file, line)) {
    size_t i1 = line.find(',');
    size_t i2 = line.find(',', i1+1);
    
    std::vector<int> tmp;
    tmp.push_back(std::stoi(line.substr(0, i1)));
    tmp.push_back(std::stoi(line.substr(i1+1, i2-i1)));
    tmp.push_back(std::stoi(line.substr(i2+1)));
    pts.push_back(tmp);
  }
  
  // part1
  std::set<std::vector<int>> lava;
  for (auto& v: pts) lava.insert(v);
  size_t sum = 0;
  std::vector<int> tmp = {0,0,0};
  for (auto& v: pts)
    for (int i = 0; i < 3; ++i)
       for (int j = -1; j < 2; j += 2) {
         tmp = v;
         tmp[i] += j;
         if (lava.count(tmp) == 0) sum++;
       }
  std::cout << "Sum of exposed areas:\n" << sum << std::endl;
  
  // part2
  const int N = 20; // derived from the input
  char map[N][N][N];
  for (int x = 0; x < N; ++x)
  for (int y = 0; y < N; ++y)
  for (int z = 0; z < N; ++z) map[x][y][z] = '.';
  for (auto& v: pts) map[v[0]][v[1]][v[2]] = '#';
  
  std::set<std::vector<int>> exposed;
  std::set<std::vector<int>> covered;
  // Add all non lava edge points to exposed
  for (int x = 0; x < N; ++x) for (int y = 0; y < N; ++y)
    for (int z = 0; z < N; z+=N-1) if(map[x][y][z] != '#')
      { exposed.insert({x,y,z}); covered.insert({x,y,z}); map[x][y][z] = '/';}
  for (int y = 0; y < N; ++y) for (int z = 0; z < N; ++z)
    for (int x = 0; x < N; x+=N-1) if(map[x][y][z] != '#')
      { exposed.insert({x,y,z}); covered.insert({x,y,z}); map[x][y][z] = '/';}
  for (int z = 0; z < N; ++z) for (int x = 0; x < N; ++x)
    for (int y = 0; y < N; y+=N-1) if(map[x][y][z] != '#')
      { exposed.insert({x,y,z}); covered.insert({x,y,z}); map[x][y][z] = '/';}
  
  // For each exposed point, expose neighbour points. Keep until stop changing.
  bool rep = true;
  while (rep) {
    rep = false;
    for (int x = 1; x < N-1; ++x)
    for (int y = 1; y < N-1; ++y)
    for (int z = 1; z < N-1; ++z)
    if (map[x][y][z] == '.') {
      tmp[0] = x; tmp[1] = y; tmp[2] = z;
      if (covered.count(tmp) == 1) break;
      for (int i = 0; i < 3; ++i)
         for (int j = -1; j < 2; j += 2) {
           tmp[0] = x; tmp[1] = y; tmp[2] = z;
           tmp[i] += j;
           if (exposed.count(tmp) == 1) {
             exposed.insert({x,y,z});
             map[x][y][z] = '/';
             rep = true;
           }
         }
    }
  }
  // map with exposed points finished, now count exposed surfaces
  size_t sum2 = 0;
  for (auto& v: pts)
    for (int i = 0; i < 3; ++i)
       for (int j = -1; j < 2; j += 2) {
         tmp = v;
         tmp[i] += j;
         if (tmp[i] == -1 || tmp[i] == N) sum2++;
         else if (map[tmp[0]][tmp[1]][tmp[2]] == '/') sum2++;
       }
  std::cout << "Sum of exposed areas part2:\n" << sum2 << std::endl;

  
}
