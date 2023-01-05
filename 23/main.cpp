#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

// Just keep a set of elf positions and a map of proposed moves (positions and
// vect of elves who want to move there). That way the size of the board doesn't
// affect anything and lookups are relatively fast.

struct P {int x, y;
  bool operator <(const P& p) const {return x<p.x ? true : x==p.x && y<p.y; }
  bool operator ==(const P& p) const {return x==p.x && y==p.y; }
  };

void moveInDir(P& p, const int dir) {
  switch (dir) {
  case 0:
    p.y--;
    break;
  case 1:
    p.y++;
    break;
  case 2:
    p.x--;
    break;
  case 3:
    p.x++;
    break;
  }
}

void proposeMoves(const std::set<P>& elves, std::map<P, std::vector<P>>& prop, const int firstDir) {
  prop.clear();
  
  for (const P& elf: elves) {
    bool dirClean[4] = {true, true, true, true}; // 0 - north, 1 - south, 2 - west, 3 - east
    // check all 8 adjacent pos and mark if they taint one side
    if (elves.count(P{elf.x, elf.y-1})) dirClean[0] = false;
    if (elves.count(P{elf.x, elf.y+1})) dirClean[1] = false;
    if (elves.count(P{elf.x-1, elf.y})) dirClean[2] = false;
    if (elves.count(P{elf.x+1, elf.y})) dirClean[3] = false;
    if (elves.count(P{elf.x-1, elf.y-1})) { dirClean[0] = false; dirClean[2] = false; }
    if (elves.count(P{elf.x+1, elf.y-1})) { dirClean[0] = false; dirClean[3] = false; }
    if (elves.count(P{elf.x-1, elf.y+1})) { dirClean[1] = false; dirClean[2] = false; }
    if (elves.count(P{elf.x+1, elf.y+1})) { dirClean[1] = false; dirClean[3] = false; }
    
    if (!(dirClean[0] && dirClean[1] && dirClean[2] && dirClean[3])) {
      P propMove = elf;
      for (int d = 0; d < 4; ++d) {
        int dir = (firstDir+d) % 4;
        if (dirClean[dir]) {
          moveInDir(propMove, dir);
          break;
        }
      }
      
      if (prop.count(propMove) == 1) {
        prop[propMove].push_back(elf);
      } else {
        prop[propMove] = {elf};
      }
    }
  }  
}

void doMoves(std::set<P>& elves, const std::map<P, std::vector<P>>& prop) {
  for (const auto& [p, vect]: prop)
    if (vect.size() == 1) {
      elves.erase(vect[0]);
      elves.insert(p);
    }
}

size_t computeRectSize(const std::set<P>& elves) {
  P someElf = *elves.begin();
  int xmin = someElf.x, xmax = xmin, ymin = someElf.y, ymax = ymin;
  for (const P& p: elves) {
    xmin = std::min(p.x, xmin);
    xmax = std::max(p.x, xmax);
    ymin = std::min(p.y, ymin);
    ymax = std::max(p.y, ymax);
  }
  
  return (ymax - ymin + 1) * (xmax - xmin + 1);
}

void printBoard(const std::set<P>& elves) {
  P someElf = *elves.begin();
  int xmin = someElf.x, xmax = xmin, ymin = someElf.y, ymax = ymin;
  for (const P& p: elves) {
    xmin = std::min(p.x, xmin);
    xmax = std::max(p.x, xmax);
    ymin = std::min(p.y, ymin);
    ymax = std::max(p.y, ymax);
  }
  
  std::vector<std::string> map;
  for (int i = 0; i < ymax-ymin+1; ++i) map.push_back(std::string(xmax-xmin+1, '.'));
  for (const P& p: elves) map[p.y-ymin][p.x-xmin] = '#';
  for (const auto& s: map) std::cout << s << std::endl;
  
  std::cout << std::endl;
}

int main(int argc, char** argv) {
  std::ifstream file;
  file.open("input.txt");
  std::string line;
  
  // generate sparse map of elves
  std::set<P> elves;
  int y = 0;
  while(std::getline(file, line)) {
    for (int x = 0; x < int(line.size()); ++x)
      if (line[x] == '#')
        elves.insert({x,y});
    ++y;
  }
  
  // run simulation
  std::map<P, std::vector<P>> prop;
  int firstDir = 0; // 0 - north, 1 - south, 2 - west, 3 - east
  std::set<P> last = elves;
  for (int N = 0;; ++N) {
    proposeMoves(elves, prop, firstDir);
    doMoves(elves, prop); 
    firstDir = (firstDir + 1) % 4;
    
    if (N==9) { // part 1
      size_t rectSize = computeRectSize(elves);
      
      std::cout << "Number of empty tiles in bounding rect after 10 rounds:\n"
        << rectSize - elves.size() << std::endl;
    }
    
    if (last == elves) {
      std::cout << "Round where no elf moved:\n" << N+1 << std::endl;
      break;
    }
    last = elves;
  }
}
