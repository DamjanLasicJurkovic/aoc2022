#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

const bool part1 = false;

struct P { int x, y; bool operator <(const P& p) const {return x < p.x;} };

struct D { P pos; P bpos; int dist; P rangeOnRow; };

const int rowY = part1 ? 2000000 : 10;

const int maxXY = 4000000;

int main(int argc, char** argv) {
  std::ifstream file;
  file.open("input.txt");
  std::string line;

  // Parse all detector data
  std::vector<D> det;
  while (std::getline(file, line)) {
    D d;
    int i = line.find('=');
    d.pos.x = std::stoi(line.substr(i+1, line.find(',') - i));
    i = line.find('=', i + 1);
    d.pos.y = std::stoi(line.substr(i+1, line.find(':') - i));
    i = line.find('=', i + 1);
    d.bpos.x = std::stoi(line.substr(i+1, line.find(',') - i));
    i = line.find('=', i + 1);
    d.bpos.y = std::stoi(line.substr(i+1));
    d.dist = std::abs(d.pos.x - d.bpos.x) + std::abs(d.pos.y - d.bpos.y);
    det.push_back(d);
  }

  if (part1) {
  // Only find those affecting the row, compute range
  std::vector<D> eff;
  for (auto&d: det) {
    if (d.pos.y - d.dist <= rowY && d.pos.y + d.dist >= rowY) {
      int reach = d.dist - std::abs(d.pos.y - rowY);
      d.rangeOnRow.x = d.pos.x - reach;
      d.rangeOnRow.y = d.pos.x + reach + 1;
      eff.push_back(d);
    }
  }
  // Figure out needed row size, normalize ranges and pos, fill row
  P minmax = eff[0].rangeOnRow;
  for (auto& p: eff) {
    minmax.x = std::min(minmax.x, p.rangeOnRow.x);
    minmax.y = std::max(minmax.y, p.rangeOnRow.y);
  }
  std::string row(minmax.y - minmax.x, '.');
  for (auto& p: eff)
    for (int i = p.rangeOnRow.x - minmax.x; i < p.rangeOnRow.y - minmax.x; ++i)
      row[i] = '#';
  for (auto& p: eff) {
    if (p.pos.y == rowY) row[p.pos.x - minmax.x] = 'D';
    if (p.bpos.y == rowY) row[p.bpos.x - minmax.x] = 'B';
  }

  // Count
  int res = 0;
  for (char c: row) if (c=='#') ++res;
  std::cout << "Number of points that cannot be beacons on row " << rowY << ":\n" << res << std::endl;

  } else { // part2
  // Scan all points, if hit object can teleport to end, and we expect hit each time
  // So max n scanners pts checked per row
  std::vector<P> ranges;
  P res;
  bool finished = false;
  for (int y = 0; y < maxXY && !finished; ++y) {
    // Only find those affecting the row, compute range
    ranges.resize(0);
    for (auto& d: det) {
      if (d.pos.y - d.dist <= y && d.pos.y + d.dist >= y) {
        int reach = d.dist - std::abs(d.pos.y - y);
        P r;
        r.x = d.pos.x - reach;
        r.y = d.pos.x + reach;
        if (r.x < 0) r.x = 0;
        ranges.push_back(r);
      }
    }
    // sort the ranges by min, then chain to see if cover full row
    std::sort(ranges.begin(), ranges.end());
    int x = -1;
    for (auto& p: ranges) {
      if (x >= maxXY) break;
      if (p.x <= x+1) x = std::max(p.y, x);
      else {res.x = x+1; res.y = y; finished = true; break;}
    }
  }
    std::cout << "Distress location:" << res.x << "," << res.y << std::endl;
    size_t freq = size_t(res.x)*4000000 + res.y;
    std::cout << "Distress tuning frequency:" << freq << std::endl;
  }
}
