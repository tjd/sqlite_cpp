// topswop_sqlite_test.cpp

#include "topswop.h"

int main() {
  cout << "Topswop Sqlite Test" << endl;
  for(int i = 0; i < n_vals_size; ++i) {
    int n = n_vals[i];
    int score = get_current_score(n);
    cout << "n = " << n << " score = " << score << endl;
  }

  for(int i = 0; i < n_vals_size; ++i) {
    int n = n_vals[i];
    perm p = *get_current_perm(n);
    cout << "n = " << n << " perm = " << p << endl;
  }

  perm p = *range(51);
  set_current_perm(51, p);
}
