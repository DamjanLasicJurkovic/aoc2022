#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

// For part2 idea is to check the diff between one and the other side of the
// comparison values for various humn values, and first find, by doubling, two
// diffs with opposite signs. Then this means the right value is inbetween those
// humn values. Then we can just binary search to find the right value. Since I
// was too lazy to code it, I just ran a few numbers by hand with some prints,
// until I found the solution.

const int64_t MinMax = 3296135418600;

struct M {std::string name, m1, m2; char op; };

int main(int argc, char** argv) {
  std::ifstream file;
  file.open("input.txt");
  std::string line;
  
  std::map<std::string, int64_t> solvedInit, solved;
  std::vector<M> unsolved;
  
  while(std::getline(file, line)) {
    std::string nm = line.substr(0, line.find(':'));
    std::string expr = line.substr(line.find(':')+2);
        
    if (std::isdigit(expr[0])) {
      solvedInit[nm] = std::stoi(expr);
      continue;      
    }
    
    M tmp;
    tmp.name = nm;
    tmp.op = expr[5];
    tmp.m1 = expr.substr(0, 4);
    tmp.m2 = expr.substr(7);
    unsolved.push_back(tmp);
  }
  
  // part1
  solved = solvedInit;
  while(true) {
    for (const auto& m: unsolved) {
      // could have map instead of vect and remove from map but dont bother
      if (solved.count(m.name) == 0) {
        if (solved.count(m.m1) == 1 && solved.count(m.m2) == 1) {
          int64_t val = 0;
          switch (m.op) {
            case '+':
              val = solved[m.m1] + solved[m.m2];
              break;
            case '-':
              val = solved[m.m1] - solved[m.m2];
              break;
            case '/':
              val = solved[m.m1] / solved[m.m2];
              break;
            case '*':
              val = solved[m.m1] * solved[m.m2];
              break;
          }
          solved[m.name] = val;
          if (m.name=="root") {
            std::cout << "Value of root monkey:\n" << val << std::endl;
            goto finished;           
          }
        }
      }  
    }
  }
finished:
  
  // part2 (should have found the oppo sign diff interval, then binary search,
  // but I found a good guess by hand then the loop finds it by incrementing).
  for (int64_t i = MinMax; i < MinMax+100000; ++i) {
    solved = solvedInit;
    solved["humn"] = i;
    
    while(true) {
      for (const auto& m: unsolved) {
        if (solved.count(m.name) == 0) {
          if (solved.count(m.m1) == 1 && solved.count(m.m2) == 1) {
            int64_t val = 0;
            switch (m.op) {
              case '+':
                val = solved[m.m1] + solved[m.m2];
                break;
              case '-':
                val = solved[m.m1] - solved[m.m2];
                break;
              case '/':
                val = solved[m.m1] / solved[m.m2];
                break;
              case '*':
                val = solved[m.m1] * solved[m.m2];
                break;
            }
            solved[m.name] = val;
            if (m.name == "root") {
              if (i%25 == 0) {
                std::cout << "diff: " << solved[m.m1] - solved[m.m2] << std::endl;
              }
              if (solved[m.m1] == solved[m.m2]) {
                std::cout << "Solution found for humn:\n" << i << std::endl;
                goto finished2;
              } else {
                goto restart_loop;
              }
            }
          }
        }  
      }
    }
restart_loop:
    if (i%25 == 0) std::cout << "humn val: " << i << std::endl;
  }
  
  std::cout << "No solution found" << std::endl;
  return 0;
  
finished2:  
  return 0;
}
