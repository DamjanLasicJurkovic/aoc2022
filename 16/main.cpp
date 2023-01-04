#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

struct NodeStr {int fr; std::vector<std::string> conn;};
struct Node {int fr; std::vector<int> conn; std::vector<int> dist; };

struct NodeDist{size_t i; int dist; std::vector<int> conn;
  bool operator <(const NodeDist&n) const { return dist<n.dist; }};

int run(uint64_t visited, int ci, const std::vector<Node>& g3, int tmax);

Node shortestToAllWithFr(const std::vector<Node>& graph, size_t index) {
  Node res;
  res.fr = graph[index].fr;
  std::map<int, NodeDist> rem;
  for (size_t i = 0; i < graph.size(); ++i) {
    NodeDist x;
    x.i = i;
    x.dist = std::numeric_limits<int>::max();
    x.conn = graph[i].conn;
    rem[i] = x;
  }
  rem[index].dist = 0;
  std::set<int> conn;
  for (auto i: graph[index].conn) conn.insert(i);
  
  // run dijkstra
  std::vector<NodeDist> srt;
  while (rem.size() != 0) {
    srt.resize(0);
    for (auto& [i, n]: rem) {
      srt.push_back(n);      
    }
    std::sort(srt.begin(), srt.end());
    
    // update distances
    for (auto& i: srt[0].conn) {
      if (rem.count(i) == 1)
      rem[i].dist = rem[i].dist <= srt[0].dist ? rem[i].dist : srt[0].dist + 1;
    }
        
    if (graph[srt[0].i].fr != 0 && srt[0].i != index) {
      // add this to conn distance
      res.conn.push_back(srt[0].i);
      res.dist.push_back(srt[0].dist);
    }
    
    // remove from map
    rem.erase(srt[0].i);
  }
  
  return res;
}

int main(int argc, char** argv) {
  std::ifstream file;
  file.open("input.txt");
  std::string line;
    
  //// Parse input into a graph via a map of nodes
  std::vector<Node> graph;
  int indexAA = 0;
  {
    std::vector<std::string> names;
    std::map<std::string, NodeStr> map;
    while (std::getline(file, line)) {
      NodeStr x;
      x.fr = std::stoi(line.substr(line.find('=')+1, line.find(';') - line.find('=')));
      size_t i1 = line.find(';') + 24;
      if (line[i1] == ' ') ++i1;
      while (true) {
        size_t i2 = line.find(',', i1);
        if (i2 == std::string::npos) {
          x.conn.push_back(line.substr(i1));
          break; 
        } else {
          x.conn.push_back(line.substr(i1, i2-i1));
          i1 = i2+2;
        }
      }
      names.push_back(line.substr(6,2));
      map[names.back()] = x;
    }
    for (auto& [k,n]: map) {
      std::cout << k << ", fr: " << n.fr << ", conn:";
      for (auto& s: n.conn) std::cout << s << ",";
      std::cout << std::endl;    
    }
    std::cout << "-------\n" << std::endl;    
    
    // create an int graph
    std::map<std::string, size_t> indexes;
    for (size_t i = 0; i < names.size(); ++i) indexes[names[i]] = i;
    graph.resize(names.size());
    for (auto& [name, node]: map) {
      Node& n = graph[indexes[name]];
      n.fr = node.fr;
      for (auto& s: node.conn) n.conn.push_back(indexes[s]);
    }
    for (size_t i = 0; i < graph.size(); ++i) {
      std::cout << i << ", fr: " << graph[i].fr << ", conn: ";
      for (auto& c: graph[i].conn) std::cout << c << ",";
      std::cout << std::endl;    
    }
    std::cout << "-------\n" << std::endl;  
    auto itAA = std::find(names.begin(), names.end(), "AA");
    indexAA = itAA - names.begin();   
  }
  graph[indexAA].fr = 100000; // Just so it is included in final graph
  
  
  //// Create a new graph of only nodes with fr > 0 with min distances to all
  std::vector<Node> g2;
  {
    // Create graph by computing dijkstra from each point to each point
    // (skip 0 fr points here)
    for (size_t i = 0; i < graph.size(); ++i) {
      if (graph[i].fr == 0) { g2.push_back(graph[i]); continue; }
      
      Node res = shortestToAllWithFr(graph, i);
      
      g2.push_back(res);      
    }    
  }
  for (size_t i = 0; i < g2.size(); ++i) {
      if(g2[i].fr != 0) {
      std::cout << i << ", fr: " << g2[i].fr << ", conn: ";
      for (size_t j = 0; j < g2[i].conn.size(); ++j) {
        std::cout << g2[i].conn[j] << ",d=" << g2[i].dist[j] << "; ";
      }
      std::cout << std::endl;    
    }
  }
  std::cout << "-------\n" << std::endl;  
  
  
  //// Update graph by removing all 0 fr nodes
  //// - scan vector, add those with fr to new vect, note how many 0 skipped,
  ////   adust next indexes
  std::vector<Node> g3;
  {
    for (auto& n: g2) if (n.fr != 0) g3.push_back(n);
    int nRem = 0;
    for (int i = 0; i < int(g2.size()); ++i) {
      if (g2[i].fr == 0) {
        if (indexAA > i - nRem)
          --indexAA;
        for (auto &n: g3)
          for (auto &ind: n.conn) 
             if (int(ind) > i - nRem)
                --ind;
             
        ++nRem; 
      }      
    }
    g3[indexAA].fr = 0;
  }
  for (size_t i = 0; i < g3.size(); ++i) {
      std::cout << i << ", fr: " << g3[i].fr << ", conn: ";
      for (size_t j = 0; j < g3[i].conn.size(); ++j) {
        std::cout << g3[i].conn[j] << ",d=" << g3[i].dist[j] << "; ";
      }
      std::cout << std::endl;   
  }
  std::cout << "-------\n" << std::endl;  
  
  // add 1 to all distances for turning the lever
  for (auto& n: g3) for (auto& d: n.dist) d++;
  
  
  //// Part1
  //// Run graph traversal on this final graph
  //// - always pull lever
  //// - note if time >= 30 if best score
  //// - never visit already visited (keep set)
  uint64_t visited = 0;
  int best = run(visited, indexAA, g3, 30);
  
  std::cout << "Total pressure released:\n" << best << std::endl;
  
  
  //// Part2
  //// Repeat same as above but split nodes into two groups, one for you, one
  //// for elephant. Cover all permutations by using a bitfield, true go to you,
  //// false to elephant. Then sum resulting pressure. N permutations is
  //// 2^N nodes, so probably not that much for example set. Node AA is always
  //// in both sets, but we wont complicate here for 2x speed increase.
  uint64_t perm = 0;
  size_t nNodes = g3.size();
  uint64_t mask = 0;
  best = 0;
  for (size_t i = 0; i < nNodes; ++i) mask += (1 << i);
  uint64_t maskAA = mask;
  maskAA &= ~(1 << indexAA);
  do {
    if (perm % 100 == 0) std::cout << perm << std::endl;
    uint64_t vis1 = 0, vis2 = 0;
    for (size_t i = 0; i < nNodes; ++i) {
      if (perm & (1 << i)) vis1 += 1 << i;
      else vis2 += 1 << i;
    }
    // Remove AA node
    vis1 &= maskAA;
    vis2 &= maskAA;
    
    int b1 = run(vis1, indexAA, g3, 26);
    int b2 = run(vis2, indexAA, g3, 26);
    best = std::max(best, b1+b2);
    
    perm++;
  } while((perm & mask) != 0);
  
  std::cout << "Total pressure released part2:\n" << best << std::endl;
  
}

int run(uint64_t visited, int ci, const std::vector<Node>& g3, int tmax) {
  int best = 0, press = 0, time = 0, ni = 0;
  std::vector<int> path = {};
  std::vector<size_t> next = {0};
  while(true) {
    #if 0
    std::cout << "\n---\nCI: " << ci << "\npath: " << std::endl;
    for (auto x: path) std::cout << x << ", ";
    std::cout << "\nsz: " << path.size() << std::endl;
    #endif
    
    const Node& n = g3[ci];
    
    // check if exhausted all options or advance
    best = std::max(best, press);
    if (time >= tmax) { goto moveBack; }
    
    if (next.back() == n.conn.size()) goto moveBack;
    
    // try move
    ni = n.conn[next.back()];
    if (visited & (1 << ni)) {
      goto advanceNext;
    } else {
      goto moveNext;
    }
    continue;
    
advanceNext:
    next.back()++;
    continue;
    
moveNext:
    // add pressure of next node
    time += n.dist[next.back()];
    visited |= (1 << ci);
    path.push_back(ci);
    next.push_back(0);
    ci = ni;
    press += std::max(tmax - time, 0) * g3[ci].fr;
    continue;
    
moveBack:
    press -= std::max(tmax - time, 0) * g3[ci].fr;
    if (path.size() == 0) break;
    visited &= ~(1 << ci);
    ci = path.back();
    path.pop_back();
    next.pop_back();
    time -= g3[ci].dist[next.back()];
    next.back()++;
    continue;
  }
   
  return best;
}

