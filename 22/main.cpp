#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

bool part1 = false;

// For part1, lazy solution find teleport location by scanning everytime. For
// part2, I hardcoded the teleport locations based on input geometry like a
// scrub.

// 0 - right, 1 - down, 2 - left, 3 - up
void tryTeleportRight(int& x, int& y, int& dir, const std::vector<std::string>& map) {
  int nX=0, nY=0, nD=0;
  if (part1) {
    nD = dir;
    nY = y;
    while(map[y][nX] == ' ') ++nX;
  } else {
    int a = (map.size()-2)/4;
    int quarter = (y-1)/a;
    switch (quarter) {
      case 0:
        nX = 2*a;
        nY = 3*a - (y - 1);
        nD = 2;
        break;
      case 1:
        nX = 2*a+1 + (y - a - 1);
        nY = a;
        nD = 3;
        break;
      case 2:
        nX = 3*a;
        nY = a - (y - 2*a - 1);
        nD = 2;
        break;
      case 3:
        nX = a+1 + (y - 3*a - 1);
        nY = 3*a;
        nD = 3;
        break;
    }
  }
  if (map[nY][nX] == '.') { x = nX; y = nY; dir = nD; }
}

// 0 - right, 1 - down, 2 - left, 3 - up
void tryTeleportLeft(int& x, int& y, int& dir, const std::vector<std::string>& map) {
  int nX=0, nY=0, nD=0;
  if (part1) {
    nD = dir;
    nX = map[y].size() - 1;
    nY = y;
    while(map[y][nX] == ' ') --nX;
  } else {
    int a = (map.size()-2)/4;
    int quarter = (y-1)/a;
    switch (quarter) {
      case 0:
        nX = 1;
        nY = 3*a - (y - 1);
        nD = 0;
        break;
      case 1:
        nX = 1 + (y - a - 1);
        nY = 2*a+1;
        nD = 1;
        break;
      case 2:
        nX = a+1;
        nY = a - (y - 2*a - 1);
        nD = 0;
        break;
      case 3:
        nX = a+1 + (y - 3*a - 1);
        nY = 1;
        nD = 1;
        break;
    }
  }
  if (map[nY][nX] == '.') { x = nX; y = nY; dir = nD; }
}

void tryTeleportUp(int& x, int& y, int& dir, const std::vector<std::string>& map) {
  int nX=0, nY=0, nD=0;
  if (part1) {
    nD = dir;
    nY = map.size() - 1;
    nX = x;
    while(map[nY][x] == ' ') --nY;
  } else {
    int a = (map[0].size()-2)/3;
    int quarter = (x-1)/a;
    switch (quarter) {
      case 0:
        nX = a+1;
        nY = a+1 + (x-1);
        nD = 0;
        break;
      case 1:
        nX = 1;
        nY = 3*a+1 + (x-a-1);
        nD = 0;
        break;
      case 2:
        nX = 1 + (x-2*a-1);
        nY = 4*a;
        nD = 3;
        break;
    }
  }
  if (map[nY][nX] == '.') { x = nX; y = nY; dir = nD; }
}

void tryTeleportDown(int& x, int& y, int& dir, const std::vector<std::string>& map) {
  int nX=0, nY=0, nD=0;
  if (part1) {
    nD = dir;
    nX = x;
    while(map[nY][x] == ' ') ++nY;
  } else {
    int a = (map[0].size()-2)/3;
    int quarter = (x-1)/a;
    switch (quarter) {
      case 0:
        nX = 2*a+1 + (x-1);
        nY = 1;
        nD = 1;
        break;
      case 1:
        nX = a;
        nY = 3*a+1 + (x - a - 1);
        nD = 2;
        break;
      case 2:
        nX = 2*a;
        nY = a+1 + (x - 2*a - 1);
        nD = 2;
        break;
    }
  }
  if (map[nY][nX] == '.') { x = nX; y = nY; dir = nD; }
}

void moveRight(int& x, int& y, int& dir, const std::vector<std::string>& map) {
  const char c = map[y][x+1];
  if (c == '.') ++x;
  if (c == ' ') tryTeleportRight(x,y,dir,map);
}

void moveDown(int& x, int& y, int& dir, const std::vector<std::string>& map) {
  const char c = map[y+1][x];
  if (c == '.') ++y;
  if (c == ' ') tryTeleportDown(x,y,dir,map);
}

void moveLeft(int& x, int& y, int& dir, const std::vector<std::string>& map) {
  const char c = map[y][x-1];
  if (c == '.') --x;
  if (c == ' ') tryTeleportLeft(x,y,dir,map);
}

void moveUp(int& x, int& y, int& dir, const std::vector<std::string>& map) {
  const char c = map[y-1][x];
  if (c == '.') --y;
  if (c == ' ') tryTeleportUp(x,y,dir,map);
}

void move(int& x, int& y, int& dir,
  const std::vector<std::string>& map, const int n) {
  for (int i = 0; i < n; ++i) {
    switch (dir) {
    case 0:
      moveRight(x, y, dir, map);
      break;
    case 1:
      moveDown(x, y, dir, map);
      break;
    case 2:
      moveLeft(x, y, dir, map);
      break;
    case 3:
      moveUp(x, y, dir, map);
      break;      
    }
  }
}

void prntMap(int& x, int& y, const int dir,
  const std::vector<std::string>& map) {
    const char c = dir == 0 ? '>' : dir == 1 ? 'V' : dir == 2 ? '<' : '^';
    for (int i = 0; i < y; ++i) std::cout << map[i] << std::endl;
    std::cout << map[y].substr(0, x) << c << map[y].substr(x+1) << std::endl;
    for (int i = y+1; i < int(map.size()); ++i) std::cout << map[i] << std::endl;
    std::cout << std::endl;
  }

int main(int argc, char** argv) {
  std::ifstream file;
  file.open("input.txt");
  std::string line;
  
  // parse map into a vect of strings
  std::vector<std::string> map;
  map.push_back(" ");
  while(std::getline(file, line)) {
    if (line == "") break;
    map.push_back(" " + line + " ");
  }
  size_t maxSize = 0;
  for (const auto& s: map) maxSize = std::max(maxSize, s.size());
  for (auto& s: map)
    if (s.size() != maxSize)
      s += std::string(maxSize - s.size(), ' ');
  
  map.push_back(std::string(maxSize, ' '));
  
  // parse instructions to a vect of L, R or number string
  std::getline(file, line);
  std::vector<std::string> instr;
  std::string tmpNr;
  for (auto c: line) {
    if (c == 'R' || c == 'L') {
      if (tmpNr.size() > 0) instr.push_back(tmpNr);
      tmpNr = "";
      instr.push_back(std::string(1, c));
    } else {
      tmpNr.push_back(c);      
    } 
  }
  if (tmpNr.size() > 0) instr.push_back(tmpNr);
  
  // run simulation
  int dir = 0; // 0 - right, 1 - down, 2 - left, 3 - up
  int x = map[1].find('.'), y = 1; // we have an empty row

  for (const auto& s: instr) {
    if (s == "R") dir = dir == 3 ? 0 : dir + 1;
    else if (s == "L") dir = dir == 0 ? 3 : dir - 1;
    else move(x,y,dir,map,std::stoi(s));   
  }
    
  // print result (rows and cols start at 1 for this calc, but we extended the
  // map so they are already ++)
  std::cout << "y: " << y << ", x: " << x << ", dir: " << dir << std::endl;
  const int res = y * 1000 + 4 * x + dir;
  std::cout << "Result for final pos and dir:\n" << res << std::endl;
}
