// perf.cpp

#include "topswop.h"

int main() {
  cout << "(initializing)" << endl;
  srand(time(NULL));
  vector<Topswop> pop;
  for(int n = 2; n < 98; ++n) {
    Topswop p(n);
    pop.push_back(p);
    p.randomize();
    best[n] = p;
  }

  cout << "(running)" << endl;
  for(int i = 0; i < 100000; ++i) {
    for(int n = 2; n < 98; ++n) {
      pop[n-2].randomize();
      if (pop[n-2].score() > best[n].score()) {
        best[n] = pop[n-2];
      }
    }
  }

  print_best();
}
