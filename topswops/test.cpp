// test.cpp

#include "topswop.h"

int main() {
  srand(5);
  cout << "Running test ..." << endl;
  Topswop p(10);
  cout << "p.size() == " << p.size() << endl;
  cout << "p.score() == " << p.score() << endl;
  for(int i = 0; i < p.size(); ++i) {
    cout << "p[" << i << "] == " << p[i] << endl;
  }
  p.randomize();
  cout << '\n';
  cout << p << " (" << p.score() << ")" << endl;
  p.do_swap_home(2);
  p.do_swap_home(3);
  p.do_swap_home(4);
  cout << dfs(p) << endl;

  //  cout << p << " (" << p.score() << ")" << endl;

  /*
  for(int n = 2; n < 98; ++n) {
    best[n] = p;
  }

  best.print();

  best.print();
  //best.save();
  */
  cout << "test done\n";
}
