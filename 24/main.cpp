#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

// This is solved by just keepin track of all possible positions we can reach at
// a given time. Then when one such position reaches the exit, we have found the
// minimum time. On each iteration, we just note any position which we can reach
// from current positions, and remove the ones covered by blizzard.

std::vector<std::string> copyKeepOnlyDir(const std::vector<std::string>& mapIn, char dir) {
  std::vector<std::string> map = mapIn;
  for (auto& s: map) for (auto& c: s) if (c != dir && c != '#') c = '.';
  return map;  
}

void updateWithCovered(std::vector<std::string>& map, const std::vector<std::string>& mapRef) {
  for (int y = 1; y < int(map.size())-1; ++y)
    for (int x = 1; x < int(map[0].size())-1; ++x)
      if (mapRef[y][x] != '.')
        map[y][x] = '$';
}

void makeMove(std::vector<std::string>& map, const std::vector<std::string>& mapOld, char dir) {
  for (int y = 1; y < int(map.size())-1; ++y)
    for (int x = 1; x < int(map[0].size())-1; ++x)
      switch(dir) {
      case '^':
        if (mapOld[y][x] == '^') {
          if (y == 1)
            map[map.size()-2][x] = '^';
          else
            map[y-1][x] = '^';
        }
        break;
      case 'v':
        if (mapOld[y][x] == 'v') {
          if (y == int(map.size())-2)
            map[1][x] = 'v';
          else
            map[y+1][x] = 'v';
        }
        break;
      case '<':
        if (mapOld[y][x] == '<') {
          if (x == 1)
            map[y][map[0].size()-2] = '<';
          else
            map[y][x-1] = '<';
        }
        break;
      case '>':
        if (mapOld[y][x] == '>') {
          if (x == int(map[0].size())-2)
            map[y][1] = '>';
          else
            map[y][x+1] = '>';
        }
        break;        
      }  
}

void updateComb(std::vector<std::string>& map,
                const std::vector<std::string>& mapU,
                const std::vector<std::string>& mapD,
                const std::vector<std::string>& mapL,
                const std::vector<std::string>& mapR,
                const std::vector<std::string>& mapOld) {
  // Write $ for occupied, X for those where we can be based on old map
  updateWithCovered(map, mapU);
  updateWithCovered(map, mapD);
  updateWithCovered(map, mapL);
  updateWithCovered(map, mapR);
  
  for (int y = 1; y < int(map.size())-1; ++y)
    for (int x = 1; x < int(map[0].size()-1); ++x)
      if (map[y][x] == '.')
        if (mapOld[y][x] == 'X' ||
            mapOld[y+1][x] == 'X' ||
            mapOld[y-1][x] == 'X' ||
            mapOld[y][x+1] == 'X' ||
            mapOld[y][x-1] == 'X')
           map[y][x] = 'X';  
           
  if (mapOld[mapOld.size()-2][mapOld[0].size()-2] == 'X')
    map.back()[map[0].size()-2] = 'X';
  if (mapOld[mapOld.size()-1][mapOld[0].size()-2] == 'X')
    map.back()[map[0].size()-2] = 'X';
  if (mapOld[0][1] == 'X') map[0][1] = 'X';
  if (mapOld[1][1] == 'X') map[0][1] = 'X';
}

void prntMap(const std::vector<std::string>& map) {
  std::cout << std::endl;
  for (const auto& s: map) std::cout << s << std::endl;
}

int main(int argc, char** argv) {
  std::ifstream file;
  file.open("input.txt");
  std::string line;
  
  std::vector<std::string> map;
  while(std::getline(file, line)) map.push_back(line);
    
  std::vector<std::string> mapU = copyKeepOnlyDir(map, '^');
  std::vector<std::string> mapD = copyKeepOnlyDir(map, 'v');
  std::vector<std::string> mapL = copyKeepOnlyDir(map, '<');
  std::vector<std::string> mapR = copyKeepOnlyDir(map, '>');
  std::vector<std::string> mapClean = copyKeepOnlyDir(map, 'Y');
  
  std::vector<std::string> mapUOld = mapU, mapDOld = mapD, mapLOld = mapL, mapROld = mapR, mapOld = map; 
  
  size_t count = 0;
  mapOld[0][1] = 'X';
  while(++count) {
    map = mapClean; mapU = mapClean; mapD = mapClean; mapL = mapClean; mapR = mapClean;
    
    makeMove(mapU, mapUOld, '^');
    makeMove(mapD, mapDOld, 'v');
    makeMove(mapL, mapLOld, '<');
    makeMove(mapR, mapROld, '>');
        
    updateComb(map, mapU, mapD, mapL, mapR, mapOld);
    
    if (map.back()[map[0].size()-2] == 'X') break;
    
    mapUOld = mapU; mapDOld = mapD; mapROld = mapR; mapLOld = mapL; mapOld = map; 
  }
  std::cout << "Rounds needed to reach exit:\n" << count << std::endl;
  
  // part 2
  // back
  mapOld = mapClean;
  mapOld.back()[map[0].size()-2] = 'X';
  while(++count) {
    map = mapClean; mapU = mapClean; mapD = mapClean; mapL = mapClean; mapR = mapClean;
    
    makeMove(mapU, mapUOld, '^');
    makeMove(mapD, mapDOld, 'v');
    makeMove(mapL, mapLOld, '<');
    makeMove(mapR, mapROld, '>');
        
    updateComb(map, mapU, mapD, mapL, mapR, mapOld);
    
    if (map[0][1] == 'X') break;
    
    mapUOld = mapU; mapDOld = mapD; mapROld = mapR; mapLOld = mapL; mapOld = map; 
  }
  std::cout << "Rounds needed to reach back again:\n" << count-1 << std::endl;
  // to dest again
  mapOld = mapClean;
  mapOld[0][1] = 'X';
  while(++count) {
    map = mapClean; mapU = mapClean; mapD = mapClean; mapL = mapClean; mapR = mapClean;
    
    makeMove(mapU, mapUOld, '^');
    makeMove(mapD, mapDOld, 'v');
    makeMove(mapL, mapLOld, '<');
    makeMove(mapR, mapROld, '>');
        
    updateComb(map, mapU, mapD, mapL, mapR, mapOld);
    
    if (map.back()[map[0].size()-2] == 'X') break;
    
    mapUOld = mapU; mapDOld = mapD; mapROld = mapR; mapLOld = mapL; mapOld = map; 
  }
  
  std::cout << "Rounds needed to reach exit after round trip:\n" << count-2 << std::endl;
}
