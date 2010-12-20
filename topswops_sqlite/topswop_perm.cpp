// topswop_perm.cpp

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#define NDEBUG
#include "topswop.h"

const string dbname = "best_so_far.db";

int try_all_perm_random_restart(int n, int restart_at, int stop_after) {
  int curr_best = get_current_score(n, dbname);
  perm deck(n), best(n);
  init_deck(deck);
  shuffle_no_fix(deck);
  int highest_count_so_far = 0;
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
      shuffle_no_fix(deck);
      cout << '.' << flush;
    }

    int count = do_all_top_swops_copy(deck);

    if (count > highest_count_so_far) {
      highest_count_so_far = count;
      best = deck;
      if (highest_count_so_far > curr_best) {
        cout << '!' << flush;
        int diff = highest_count_so_far - curr_best;
        curr_best = highest_count_so_far;
        set_current_perm(n, best, dbname);
        ensure_increasing_scores();
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
    for(int i = 2; i < 98; ++i) {
      const int n = i;
      if (n > max_optimal_val) {
        cout << "(n=" << n << ")" << flush;
        try_all_perm_random_restart(n, restart, 1000000000);
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

 void test_one() {
   const int n = 7;
   const int restart = 5; //10000000;
   cout << "\nTopswop Random Restart Permuter, n = " << n 
        << ", restart = " << restart << endl;
   try_all_perm_random_restart(n, restart, 15); // 1000000000);
 }

int main() {
  forever();
  //test_one();
}
