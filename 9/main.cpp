#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <cmath>
#include <vector>

const int n_knots = 10; // total knots, so 2 for first part, 10 for second

struct P{int x=0,y=0;bool operator<(const P&p)const{return p.x<x?true:p.x==x&&p.y<y;}};

void update(P& p, const P& r) {
  if (std::abs(p.x-r.x) > 1) {
    p.x += p.x < r.x ? 1 : -1;
    p.y += p.y == r.y ? 0 : p.y < r.y ? 1 : -1;
  } else if (std::abs(p.y-r.y) > 1) {
    p.y += p.y < r.y ? 1 : -1;
    p.x += p.x == r.x ? 0 : p.x < r.x ? 1 : -1;
  }
}

int main(int argc, char** argv) {
  std::ifstream file;
  file.open("input.txt");
  std::string line;
  std::vector<P> p(n_knots);
  std::set<P> visited;
  while (std::getline(file, line)) {
    char d = line[0];
    int n = std::stoi(line.substr(2));
    for (int i = 0; i < n; ++i) {
      p[0].x += (d == 'L' || d == 'R') ? (d == 'L' ? -1 : 1) : 0;
      p[0].y += (d == 'D' || d == 'U') ? (d == 'D' ? -1 : 1) : 0;
      for (int j = 1; j < n_knots; ++j) update(p[j], p[j-1]);
      visited.insert(p.back());
    }
  }
  std::cout << "Number of points tail has visited:\n" << visited.size() << std::endl;
}
