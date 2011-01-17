// test.cpp

#include "topswop.h"

int main() {
  srand(2);
  cout << "Running test ..." << endl;
  Topswop p(10);
  cout << "p.size() == " << p.size() << endl;
  cout << "p.score() == " << p.score() << endl;
  for(int i = 0; i < p.size(); ++i) {
    cout << "p[" << i << "] == " << p[i] << endl;
  }
  p.randomize();
  cout << '\n';
  cout << p << endl;

  for(int n = 2; n < 98; ++n) {
    best[n] = p;
  }

  best.print();
  best.save();
  cout << "test done\n";
}
