#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int64_t snafuToDec(const std::string& code) {
  int64_t x = 1, num = 0;
  for (int i = code.size()-1; i >= 0; --i) {
    int64_t mult = 0;
    switch (code[i]) {
      case '-':
        mult = -1;
        break;
      case '=':
        mult = -2;
        break;
      default:
        mult = std::stoi(std::string(1,code[i]));
        break;
    }
    num += mult*x;
    x*=5;
  } 
  return num;
}

std::string decToPent(int64_t dec) {
  std::string res;
  while(dec != 0) { res = "01234"[dec % 5] + res; dec /= 5; }
  return res;
}

std::string pentToSnafu(const std::string& pent) {
  std::vector<int> snafu = {0};
  for (auto& c: pent) snafu.push_back(std::stoi(std::string(1,c)));
  
  for (int i = snafu.size()-1; i >= 0; --i) {
    if (snafu[i] == 3 || snafu[i] == 4) {
      snafu[i-1]++;
      snafu[i] = snafu[i] == 3 ? -2 : -1;
    }
    if (snafu[i] == 5) {
      snafu[i-1]++;
      snafu[i] = 0;      
    }
  }
  
  std::string snafuStr = "";
  for (auto x: snafu) snafuStr.push_back(x == -1 ? '-' : x == -2 ? '=' : '0'+x);
  if (snafuStr[0] == '0') snafuStr = snafuStr.substr(1);
  return snafuStr;
}

std::string decToSnafu(int64_t num) {  
  std::string pent = decToPent(num);
  return pentToSnafu(pent);
}

int main(int argc, char** argv) {
  std::ifstream file;
  file.open("input.txt");
  std::string line;
  
  std::vector<std::string> codes;
  while(std::getline(file, line)) codes.push_back(line);
    
  std::vector<int64_t> dec;  
  for (const auto& s: codes) dec.push_back(snafuToDec(s));
    
  int64_t sum = 0;
  for (int64_t x: dec) sum += x;
  std::string sumSnafu = decToSnafu(sum);
  
  std::cout << "sum(dec): " << sum << ", sum(snafu): " << sumSnafu << std::endl;  
}
