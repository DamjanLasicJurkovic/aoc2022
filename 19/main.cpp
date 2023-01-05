#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <chrono>

// Algorithm is a depth first search on a graph. Each node represents the robot
// to buy next. Branches for which the next robot cannot be bought are prunned.
// In order to reduced the search space, only first three robots are included in
// the graph, the fourth one is considered to be implicitly bought if resources
// are available before the next robot in the chain can be bought. I believe
// this is not exactly correct, but it works on the provided inputs. Still, this
// solution takes a long time to compute, so probably better algos exist.

const bool part2 = true;
const int tend = part2 ? 32 : 24;

void addCostRobotMult(int rob, int* ores, const std::vector<int>& bp, int mult) {
  switch (rob) {
  case 0:
    ores[0] += mult*bp[0];
    break;  
  case 1:
    ores[0] += mult*bp[1];
    break;  
  case 2:
    ores[0] += mult*bp[2];
    ores[1] += mult*bp[3];
    break;  
  case 3:
    ores[0] += mult*bp[4];
    ores[2] += mult*bp[5];
    break;  
  }
}

// Each step ends with robot being built, so it doesnt contribute yet to ores
void stepBackOresRobs(int bought, int tstep, int* robs, int* ores, const std::vector<int>& bp) {
  robs[bought]--;  
  // read cost for robot
  addCostRobotMult(bought, ores, bp, +1);
  // Remove all produced ores in this timestep
  for (int i = 0; i < 4; ++i) ores[i] -= robs[i] * tstep;
}


// Each step ends with robot being built, so it doesnt contribute yet to ores
int tryBuyNext(int rob, int time, int* robs, int* ores, const std::vector<int>& bp, bool& wasGeode) {
  bool canBuyGeode = false, canBuyNormal = false;
  int oresReq[4] = {0,0,0,0};
  addCostRobotMult(3, oresReq, bp, +1);
  int tNeeded = 0, tNeededG = 0;
  if (robs[2] != 0) {
    for (int i = 0; i < 4; ++i) {
      if (ores[i] >= oresReq[i]) continue;
      int diff = oresReq[i] - ores[i];
      int needed = diff%robs[i] == 0 ? diff/robs[i] : diff/robs[i]+1;
      tNeededG = std::max(tNeededG, needed);
    }
    if (!(tNeededG + time > tend-1)) {
      canBuyGeode = true;
    }
  }
  
  for (int& x: oresReq) x = 0;
  addCostRobotMult(rob, oresReq, bp, +1);
  tNeeded = 0;
  for (int i = 0; i < 4; ++i) {
    if (ores[i] >= oresReq[i]) continue;
    int diff = oresReq[i] - ores[i];
    int needed = diff%robs[i] == 0 ? diff/robs[i] : diff/robs[i]+1;
    tNeeded = std::max(tNeeded, needed);
  }
    
  if (!(tNeeded + time > tend-1)) canBuyNormal = true;
  
  if (canBuyGeode && canBuyNormal) {
    if (tNeededG <= tNeeded) {
      rob = 3;
      wasGeode = true;
      tNeeded = tNeededG;
    }
  } else if (canBuyGeode || canBuyNormal) {
    if (!canBuyNormal) {
      rob = 3;
      wasGeode = true;
      tNeeded = tNeededG;
    }
  } else {
    return -1;
  }
  
  int tStep = tNeeded + 1;
  
  // Add ores from all robots, remove ores from cost
  for (int i = 0; i < 4; ++i) ores[i] += robs[i] * tStep;
  addCostRobotMult(rob, ores, bp, -1);
  robs[rob]++;
  return tStep;
}

int findBestForBP(const std::vector<int>& bp) {
  size_t N = 0;
  std::vector<int> buyOrder = {0}; // 0 - ore, 1 - clay, 2 - obs, 3 - geode
  std::vector<int> times = {0};
  int best = -10;
  
  int robs[4] = {1,0,0,0};
  int ores[4] = {0,0,0,0};
  
  while(true) {
    if (false) {
      ++N;
      if (N%100000000 == 0) {
        for (auto& x: buyOrder) std::cout << x << ", ";
        std::cout << std::endl << std::endl;
      }
    }
    int score = -10;
    int tstep = 0;
    bool wasGeode = false;
    
    if (buyOrder.back() >= (2 + (robs[1] > 0))) goto Back;

    tstep = tryBuyNext(buyOrder.back(), times.back(), robs, ores, bp, wasGeode);
    
    if (tstep == -1) { // we ran out of time
      score = ores[3] + robs[3]*(tend-times.back());
      if (score > best) { best = score;
      }
      goto Next;
    }
    times.push_back(times.back()+tstep);
    if (wasGeode) buyOrder.back() = 3;
    buyOrder.push_back(0);
    continue;
Next:
    buyOrder.back()++;
    continue;
Back:
    if (buyOrder.size() == 1) break;
    buyOrder.pop_back();
    int taft = times.back();
    times.pop_back();
    int tbef = times.back();
    int tdiff = taft-tbef;
    stepBackOresRobs(buyOrder.back(), tdiff, robs, ores, bp);    
    buyOrder.back()++;
  }
    
  return best; 
}


int main(int argc, char** argv) {  
  std::ifstream file;
  file.open("input.txt");
  std::string line;
  
   // 0 - ore r cost, 1 - clay r cost, 2,3 - obs r cost ore,clay, 4,5 - geode r cost ore,obs
  std::vector<std::vector<int>> BPs;
    
  while(std::getline(file, line)) {
    std::vector<int> tmp;
    int is = 0;
    for (int i = 0; i < 4; ++i) {
      is = line.find("costs", is) + 6;
      int ie = line.find(' ', is);
      tmp.push_back(std::stoi(line.substr(is,ie)));
      if (i>1) {
        is = line.find("and", is) + 4;
        ie = line.find(' ', is);
        tmp.push_back(std::stoi(line.substr(is,ie)));        
      }
    }
    BPs.push_back(tmp);
  }
  if (part2) BPs.resize(3);

  // Traverse graph, each new node is next robot type to buy
  std::vector<int> bestAll;
  for (size_t ibp = 0; ibp < BPs.size(); ++ibp) {  
    std::cout << "Solving BP nr. " << ibp << std::endl;
    bestAll.push_back(findBestForBP(BPs[ibp]));
  }
  
  size_t result = !part2;
  for (size_t ibp = 0; ibp < BPs.size(); ++ibp) {
    std::cout << "\n--- bp:" << ibp;
    std::cout << "\nscore: " << bestAll[ibp] << std::endl;
    if (part2) result *= bestAll[ibp];
    else result += bestAll[ibp]*(ibp+1);
  }
  std::cout << "\nResulting score of all BPs:\n" << result << std::endl;
}
