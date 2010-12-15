// perturb_test.cpp

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#include "topswap.h"

int main() {
  cout << "Perturbing Best Scores" << endl;
  for(int i = 0; i < n_vals_size; ++i) {
    const int n = n_vals[i];
    if (n > max_optimal_val) {
      cout << "Calling perturb_2rot(" << n << ") ..." << flush;
      perturb_2rot(n);
      cout << endl;
    }
  }

  for(int i = 0; i < n_vals_size; ++i) {
    const int n = n_vals[i];
    if (n > max_optimal_val) {
      cout << "Calling perturb_3rot(" << n << ") ..." << flush;
      perturb_3rot(n);
      cout << endl;
    }
  }
}
