// perf.cpp

#include "topswop.h"

int main() {
  cout << "(perf initializing)" << endl;
  srand(time(NULL));
  vector<Topswop> pop;
  for(int n = 2; n < 98; ++n) {
    //cout << "n=" << n << endl;
    Topswop p(n);
    pop.push_back(p);
    p.randomize();
    best[n] = p;
  }

  cout << "(perf running)" << endl;
  for(int iter = 0; iter < 10000; ++iter) {
    for(int n = 2; n < 98; ++n) {
      Topswop& p = pop[n-2];
      p.randomize();
      if (p.score() > best[n].score()) {
        best[n] = p;
      }
    }
  }

  best.print();
}
