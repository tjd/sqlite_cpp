// topswop_perm.cpp

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#define NDEBUG
#include "topswop.h"


int try_all_perm_random_restart(int n, int restart_at, int stop_after) {
  int curr = get_current_score(n);
  vector<int> deck(n), start(n), best(n);
  init_deck(deck);
  shuffle_no_fix(deck);
  int highest_count_so_far = -1;
  int count = 0;
  int iterations = 0;
  while (next_permutation(deck.begin(), deck.end())) {
    ++iterations;
    if (iterations >= stop_after) return highest_count_so_far;

    if (deck[0] == 1) {
      shuffle_no_fix(deck);
      cout << '1' << flush;
    }
    ++count;
    if (count >= restart_at) {
      count = 0;
      shuffle(deck);
      cout << '.' << flush;
    }
    copy(deck.begin(), deck.end(), start.begin());
    int count = do_all_top_swops(start);

    if (count > highest_count_so_far) {
      highest_count_so_far = count;
      copy(deck.begin(), deck.end(), best.begin());
      if (highest_count_so_far > curr) {
        int diff = highest_count_so_far - curr;
        curr = highest_count_so_far;
        cout << "\nn = " << n << ", score = " << curr 
             << " (" << diff << " improvement)" << endl
             << best << endl;
        set_current_perm(n, best);
        //        perturb_2rot(n);
        //        perturb_3rot(n);
      }
    }
  }
  cout << best << " (n=" << n << ", optimal_count=" << highest_count_so_far 
       << ")" << endl;
  return highest_count_so_far;
}

void forever() {
  const int restart = 10000000;
  cout << "\nTopswop Random Restart Permuter, all n" 
       << ", restart = " << restart << endl;
  while (true) {
    for(int i = 0; i < n_vals_size; ++i) {
      if (n_vals[i] > max_optimal_val) {
        cout << "(n=" << n_vals[i] << ")" << flush;
        try_all_perm_random_restart(n_vals[i], restart, 1000000000);
      }
    }
  }
}

void test_perm() {
  vector<int> deck(4);
  init_deck(deck);
  while (next_permutation(deck.begin(), deck.end())) {
    cout << deck << endl;
  }
  cout << "done\n";
}

// void test_one() {
//   const int n = 97;
//   const int restart = 10000000;
//   cout << "\nTopswop Random Restart Permuter, n = " << n 
//        << ", restart = " << restart << endl;
//   try_all_perm_random_restart(n, restart, 1000000000);
// }

int main() {
  forever();
}
