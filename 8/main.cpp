#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
  std::ifstream file;
  file.open("input.txt");
  std::string line, data;

  // generate flattened array of chars, and same for bool
  std::getline(file, line);
  size_t nc = line.size(), nr = 0;
  do {
    nr++;
    data+=line;
  } while (std::getline(file, line));
  std::vector<bool> visible(nr*nc, false);

  // for each of four directions, scan all chars and set bool to true if visible from that dir
  char last;
  for (int i = 0; i < nr; ++i) for (int j = last = 0; j < nc; ++j) if (data[i*nc+j] > last) {last = data[i*nc+j]; visible[i*nc+j] = true; }
  for (int i = 0; i < nr; ++i) for (int j = (last = 0) + nc - 1; j >= 0; --j) if (data[i*nc+j] > last) {last = data[i*nc+j]; visible[i*nc+j] = true; }
  for (int i = 0; i < nc; ++i) for (int j = last = 0; j < nr; ++j) if (data[j*nc+i] > last) {last = data[j*nc+i]; visible[j*nc+i] = true; }
  for (int i = 0; i < nc; ++i) for (int j = (last = 0) + nr - 1; j >= 0; --j) if (data[j*nc+i] > last) {last = data[j*nc+i]; visible[j*nc+i] = true; }
  size_t sum = 0;
  for (auto v: visible) sum += v;
  std::cout << "Number of visible trees:\n" << sum << std::endl;

  // second part
  std::vector<size_t> scores(nr*nc, 0);
  for (int i = 0; i < nr; ++i) for (int j = 0; j < nc; ++j) {
    size_t l = 0, r = 0, u = 0, d = 0;
    char c = data[i*nc+j];
    for (int k = j - 1; k >= 0; --k) {++l; if (data[i*nc+k] >= c) break;};
    for (int k = j + 1; k < nc; ++k) {++r; if (data[i*nc+k] >= c) break;};
    for (int k = i - 1; k >= 0; --k) {++d; if (data[k*nc+j] >= c) break;};
    for (int k = i + 1; k < nr; ++k) {++u; if (data[k*nc+j] >= c) break;};
    scores[i*nc+j] = l * r * u * d;
  }
  size_t best = 0;
  for (auto x: scores) best = x > best ? x : best;
  std::cout << "Tree with best score:\n" << best << std::endl;
}
