#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

// Part 1
//const size_t SZ = 50000, NRocks = 2022;
// Part 2
const size_t SZ = 1000000, NRocks = 1000000000000;

struct P {int x, y;};

enum class Shape : int {HLine = 0, Cross, BackL, VLine, Square};
std::map<Shape, int> height = {{Shape::HLine, 1},
  {Shape::Cross, 3},
  {Shape::BackL, 3},
  {Shape::VLine, 4},
  {Shape::Square, 2}};
  

void moveLeft(P& pos, const Shape shp, const std::vector<std::string>& field);

void moveRight(P& pos, const Shape shp, const std::vector<std::string>& field);

bool moveDown(P& pos, const Shape shp, const std::vector<std::string>& field);

void addShape(P& pos, const Shape shp, std::vector<std::string>& field, int& topmost);

int checkAndReset(std::vector<std::string>& field, int& topmost) {
  int n = 0;
  bool reset = topmost != 0;
  if (topmost != 0)
    for (auto& s: field) if (s[topmost] == '.')
      reset = false; 
  
  if (reset) {
    n = topmost;
    for (auto& s: field) for (auto& c: s) c = '.';
    for (auto& s: field) s[0] = '#';
    topmost = 0;
  }
  return n;
}

int main(int argc, char** argv) {
  std::ifstream file;
  file.open("input.txt");
  std::string moves;
  std::getline(file, moves);
  int nMoves = moves.size();
  
  std::vector<std::string> field(7, std::string(SZ, '.'));
  for (auto& s: field) s[0] = '#';
  
  // We basically simulate the falling rocks. When we find see we have a pattern
  // of stone which is covered by a horizontal stone line. Then we can reset the
  // board, and compute how many times this will repeat, and fast forward the
  // simulation. Not a foolproof solution but seems to work on the input.
  
  uint64_t tot = 0;
  int last1 = 0;
  size_t lastN = 0;
  int last2 = 0;
  int topmost = 0;
  
  int iMov = 0;
  for (size_t rn = 0; rn < NRocks; ++rn) {
    last2 = checkAndReset(field, topmost);
    if (last2) {
      if (last1 == last2) {
        size_t passedN = rn - lastN;
        size_t ncyclesrem = (NRocks - rn) / passedN;
        tot += (ncyclesrem+1)*(last2);
        rn += ncyclesrem*passedN;
      } else {
        last1 = last2;
        lastN = rn;
        tot += last2;
      }
    }
    
    Shape sh = (Shape)(rn%5);
    
    // point
    P pos = {2, topmost+height[sh]+3};
    
    while(true) {
      if (moves[iMov % nMoves] == '<') moveLeft(pos, sh, field); 
      else moveRight(pos, sh, field);
      iMov++;
      
      if (!moveDown(pos, sh, field)) {
        addShape(pos, sh, field, topmost);
        break;
      }
    }
  }
  tot += topmost;
  
  std::cout << "Height of the tower:\n" << tot << std::endl;
  
  for (int i = 10; i >= 0; --i) {
    for (int j= 0; j < 7; ++j)
       std::cout << field[j][i];
    std::cout << std::endl;
  }
}

void moveLeft(P& pos, const Shape shp, const std::vector<std::string>& field) {
  if (pos.x == 0) return;
  switch (shp) {
    case Shape::HLine:
    if (field[pos.x-1][pos.y] == '#') return;
    
    break;
    case Shape::Cross:
    if (field[pos.x][pos.y] == '#' ||
        field[pos.x-1][pos.y-1] == '#' ||
        field[pos.x][pos.y-2] == '#' ) return;
    
    break;
    case Shape::BackL:
    if (field[pos.x-1][pos.y-2] == '#' ||
        field[pos.x+1][pos.y-1] == '#' ||
        field[pos.x+1][pos.y] == '#' ) return;
    
    break;
    case Shape::VLine:
    for (int i = 0; i < 4; ++i)
      if (field[pos.x-1][pos.y-i] == '#') return;
    
    break;
    case Shape::Square:
    if (field[pos.x-1][pos.y] == '#' ||
        field[pos.x-1][pos.y-1] == '#' ) return;
    
    break;    
  }
  pos.x--;
}

void moveRight(P& pos, const Shape shp, const std::vector<std::string>& field) {
  switch (shp) {
    case Shape::HLine:
    if (pos.x == 3) return;
    if (field[pos.x+4][pos.y] == '#') return;
    
    break;
    case Shape::Cross:
    if (pos.x == 4) return;
    if (field[pos.x+2][pos.y] == '#' ||
        field[pos.x+3][pos.y-1] == '#' ||
        field[pos.x+2][pos.y-2] == '#' ) return;
    
    break;
    case Shape::BackL:
    if (pos.x == 4) return;
    if (field[pos.x+3][pos.y] == '#' ||
        field[pos.x+3][pos.y-1] == '#' ||
        field[pos.x+3][pos.y-2] == '#' ) return;
    
    break;
    case Shape::VLine:
    if (pos.x == 6) return;
    for (int i = 0; i < 4; ++i)
      if (field[pos.x+1][pos.y-i] == '#') return;
    
    break;
    case Shape::Square:
    if (pos.x == 5) return;
    if (field[pos.x+2][pos.y] == '#' ||
        field[pos.x+2][pos.y-1] == '#' ) return;
    
    break;    
  }
  pos.x++;
}

bool moveDown(P& pos, const Shape shp, const std::vector<std::string>& field) {
  switch (shp) {
    case Shape::HLine:
    for (int i = 0; i < 4; ++i)
      if (field[pos.x+i][pos.y-1] == '#')
        return false;
    
    break;
    case Shape::Cross:
    if (field[pos.x][pos.y-2] == '#' ||
        field[pos.x+1][pos.y-3] == '#' ||
        field[pos.x+2][pos.y-2] == '#')
      return false; 
    
    
    break;
    case Shape::BackL:
    for (int i = 0; i < 3; ++i)
      if (field[pos.x+i][pos.y-3] == '#')
        return false;
    
    break;
    case Shape::VLine:
    if (field[pos.x][pos.y-4] == '#')
      return false; 
    
    break;
    case Shape::Square:
    if (field[pos.x][pos.y-2] == '#' || field[pos.x+1][pos.y-2] == '#')
      return false; 
    
    break;    
  }
  pos.y--;
  return true;
}

void addPoint(const P pos, std::vector<std::string>& field, int& topmost) {
  field[pos.x][pos.y] = '#';
  if (pos.y > topmost) topmost = pos.y;
}

void addShape(P& pos, const Shape shp, std::vector<std::string>& field, int& topmost) {
  switch (shp) {
    case Shape::HLine:
    addPoint({pos.x, pos.y}, field, topmost);
    addPoint({pos.x+1, pos.y}, field, topmost);
    addPoint({pos.x+2, pos.y}, field, topmost);
    addPoint({pos.x+3, pos.y}, field, topmost);
    
    break;
    case Shape::Cross:
    addPoint({pos.x, pos.y-1}, field, topmost);
    addPoint({pos.x+1, pos.y}, field, topmost);
    addPoint({pos.x+1, pos.y-1}, field, topmost);
    addPoint({pos.x+1, pos.y-2}, field, topmost);
    addPoint({pos.x+2, pos.y-1}, field, topmost);
        
    break;
    case Shape::BackL:
    addPoint({pos.x+2, pos.y}, field, topmost);
    addPoint({pos.x+2, pos.y-1}, field, topmost);
    addPoint({pos.x+2, pos.y-2}, field, topmost);
    addPoint({pos.x, pos.y-2}, field, topmost);
    addPoint({pos.x+1, pos.y-2}, field, topmost);
        
    break;
    case Shape::VLine:
    addPoint({pos.x, pos.y}, field, topmost);
    addPoint({pos.x, pos.y-1}, field, topmost);
    addPoint({pos.x, pos.y-2}, field, topmost);
    addPoint({pos.x, pos.y-3}, field, topmost);
        
    break;
    case Shape::Square:
    addPoint({pos.x, pos.y}, field, topmost);
    addPoint({pos.x, pos.y-1}, field, topmost);
    addPoint({pos.x+1, pos.y}, field, topmost);
    addPoint({pos.x+1, pos.y-1}, field, topmost);
    
    break;    
  }
}
