#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <map>

typedef std::pair<size_t, size_t> P;
typedef std::map<P, size_t> DistMap;

P getMin(DistMap& m) {
  // Inneficient AF, ain't got time for nicer solution 
  std::vector<size_t> d;
  std::vector<P> p;
  for (auto& [pt, dst]: m) {
    d.push_back(dst);
    p.push_back(pt);
  }
  auto it = std::min_element(d.begin(), d.end());
  return p[std::distance(d.begin(), it)];
}

void update(
  const P& s, const P& d, std::vector<std::string>& map, DistMap& dist)
{
  if(!dist.count(d)) return;
  char cs = map[s.first][s.second], cd = map[d.first][d.second];
  if (cs - cd <= 1)
    dist[d] = dist[d] <= dist[s] ? dist[d] : dist[s] + 1;
}

int main(int argc, char** argv) {
  std::ifstream input_file;
  input_file.open("input.txt");
  std::string line;
  
  // get data
  std::vector<std::string> map;
  P start, end;
  while (std::getline(input_file, line)) {
    map.push_back(line);
    if (line.find('S') != std::string::npos)
      start = {map.size() - 1, line.find('S')}, map.back()[line.find('S')] = 'a';
    if (line.find('E') != std::string::npos)
      end = {map.size() - 1, line.find('E')}, map.back()[line.find('E')] = 'z';
  }

  // run dijkstra
  DistMap dist;
  for (size_t i = 0; i < map.size(); ++i)
    for (size_t j = 0; j < map[0].size(); ++j)
      dist[{i, j}] = std::numeric_limits<size_t>::max();
  dist[end] = 0;
  
  size_t part1dist = 0;
  std::vector<size_t> part2cand;
  while(dist.size() > 0) {
    P min = getMin(dist);
    
    if (dist[min] == std::numeric_limits<size_t>::max()) break;
        
    if (min == start) part1dist = dist[min];
    
    P tmp = min;
    tmp.first += 1;
    update(min, tmp, map, dist);
    tmp.first -= 2;
    update(min, tmp, map, dist);
    tmp.first += 1;
    tmp.second += 1;
    update(min, tmp, map, dist);
    tmp.second -= 2;
    update(min, tmp, map, dist);
    
    if (map[min.first][min.second] == 'a') part2cand.push_back(dist[min]);
    
    dist.erase(min);
  }
  std::cout << "Smallest distance to end (part 1):\n" << part1dist << std::endl;
  std::cout << "Best route distance (part 2):\n" <<
    *std::min_element(part2cand.begin(), part2cand.end()) << std::endl;
}
