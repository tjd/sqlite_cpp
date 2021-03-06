// topswop_perm.cpp

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#define NDEBUG
#include "topswop.h"

const bool show_n = true;

const string dbname = "best_so_far.db";

int try_all_perm_random_restart(int n, int restart_at, int stop_after) {
  int curr_best = get_current_score(n, dbname);
  int n1_score = get_current_score(n-1, dbname);
  perm deck(n), best(n);
  init_deck(deck);
  shuffle_no_fix(deck);
  int highest_count_so_far = 0;
  int count = 0;
  int iterations = 0;
  // while (next_perm_swap1(deck)) {
  //while (next_perm_rand_swap2(deck)) {
  while (next_permutation(deck.begin(), deck.end())) {
  //while (next_perm_seq_pair(deck)) {
  //while (next_perm_all_pair(deck)) {
  //while (next_perm_all_pair_rev(deck)) {
    ++iterations;
    if (iterations >= stop_after) return highest_count_so_far;

    ++count;

    if (deck[0] == 1) {
      shuffle_no_fix(deck);
      count = 0;
      ping('1');
    }

    if (count >= restart_at) {
      count = 0;
      shuffle_no_fix(deck);
      ping('.');
    }

    const int score = do_all_top_swops_copy(deck);

    /*
    // Heuristic cut-off: if the last number is home, then re-start if
    // the previous perm's best score plus the current score is less
    // than the highest score so far.
    if (deck.back() == n && n1_score + score < highest_count_so_far) {
      count = 0;
      shuffle_no_fix(deck);
      ping('c');
    }
    */

    if (score > highest_count_so_far) {
      highest_count_so_far = score;
      best = deck;
      if (highest_count_so_far > curr_best) {
        ping('!');
        //int diff = highest_count_so_far - curr_best;
        curr_best = highest_count_so_far;
        set_current_perm(n, best, dbname);
      }
    }
  }
  //  cout << best << " (n=" << n << ", optimal_count=" << highest_count_so_far 
  //       << ")" << endl;
  return highest_count_so_far;
}


void forever() {
  const int restart = 10000000;
  cout << "\nTopswop Random Restart Permuter, all n" 
       << ", restart = " << restart << endl;
  int count = 1;
  while (true) {
    if (!show_n && count % 10 == 1) ping(count);
    //ping(count);
    ++count;
    for(int i = 0; i < n_vals_size; ++i) {
      const int n = n_vals[i];

      if (n > max_optimal_val) {
        //ping_n(n-1);
        //try_all_perm_random_restart(n-1, restart, 1000000000);
        if (show_n) ping_n(n);
        //ping_n(n);
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

/*
void test_one() {
  const int n = 7;
  const int restart = 5; //10000000;
  cout << "\nTopswop Random Restart Permuter, n = " << n 
       << ", restart = " << restart << endl;
  try_all_perm_hc_random_restart(n, restart, 15); // 1000000000);
}
*/

int main() {
  //improve_current();
  forever();
}
