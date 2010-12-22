// topswop_hc.cpp

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#define NDEBUG
#include "topswop.h"

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
  while (next_permutation(deck.begin(), deck.end())) {
    ++iterations;
    if (iterations >= stop_after) return highest_count_so_far;

    if (deck[0] == 1) {
      shuffle_no_fix(deck);
      ping('1');
    }
    ++count;
    if (count >= restart_at) {
      count = 0;
      shuffle_no_fix(deck);
      ping('.');
    }

    int score = do_all_top_swops_copy(deck);

    // Heuristic cut-off: if the last number is home, then re-start if
    // the previous perm's best score plus the current score is less
    // than the highest score so far.
    if (deck.back() == n && n1_score + score < highest_count_so_far) {
      shuffle_no_fix(deck);
      ping('c');
    }

    if (score > highest_count_so_far) {
      highest_count_so_far = score;
      best = deck;
      if (highest_count_so_far > curr_best) {
        ping('!');
        int diff = highest_count_so_far - curr_best;
        curr_best = highest_count_so_far;
        set_current_perm(n, best, dbname);
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

void test1() {
  const int n = 71;
  const int tries = 10000 * 1000;
  const int give_up_after = 10000000;
  cout << "Topswop HC test n = " << n << ", tries = " << tries 
       << ", give_up_after = " << give_up_after << endl;
  for(int i = 0; i < tries; ++i) {
    perm_ptr p(range(n));
    shuffle(*p);
    search_back_heuristic_cutoff(*p, give_up_after);
    //ping('.');
  }
}

int main() {
  //forever();
  test1();
}
