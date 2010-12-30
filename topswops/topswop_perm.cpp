// topswop_perm.cpp

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#define NDEBUG
#include "topswop.h"

const string dbname = "best_so_far.db";

bool next_perm_rand_swap1(perm& p) {
  if (p[0] == 1) return false;
  int a = randint(p.size());
  int b = randint(p.size());
  swap(p[a], p[b]);
  return true;
}

bool next_perm_rand_swap2(perm& p) {
  int a = randint(p.size());
  if (p[a] == 1) return false;
  swap(p[a], p[0]);
  return true;
}

bool next_perm_swap1(perm& p) {
  assert(is_perm(p));
  const int n = p.size();

  if (p[0] == n) return false;

  // find the index of 1 in p
  int one_loc = -1;
  for(int i = 0; i < n; ++i) {
    if (p[i] == 1) {
      one_loc = i;
      break;
    }
  }

  if (one_loc == n - 1)    // if the 1 is at the end that return false
    return false;
  else if (one_loc <= p[0]) {  
    swap(p[one_loc], p[p[0] - 1]);
    return true;
  } else {
    swap(p[one_loc], p[one_loc + 1]);
    return true;
  }
}

int try_all_perm_random_restart(int n, int restart_at, int stop_after) {
  int curr_best = get_current_score(n, dbname);
  int n1_score = get_current_score(n-1, dbname);
  perm deck(n), best(n);
  init_deck(deck);
  shuffle_no_fix(deck);
  int highest_count_so_far = 0;
  int count = 0;
  int iterations = 0;
  //  while (next_permutation(deck.begin(), deck.end())) {
  //  while (next_perm_swap1(deck)) {
  while (next_perm_rand_swap2(deck)) {
    ++iterations;
    if (iterations >= stop_after) return highest_count_so_far;

    ++count;

    if (deck[0] == 1) {
      shuffle_no_fix(deck);
      count = 0;
      //ping('1');
    }

    if (count >= restart_at) {
      count = 0;
      shuffle_no_fix(deck);
      ping('.');
    }

    const int score = do_all_top_swops_copy(deck);

    // Heuristic cut-off: if the last number is home, then re-start if
    // the previous perm's best score plus the current score is less
    // than the highest score so far.
    if (deck.back() == n && n1_score + score < highest_count_so_far) {
      count = 0;
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
  //  cout << best << " (n=" << n << ", optimal_count=" << highest_count_so_far 
  //       << ")" << endl;
  return highest_count_so_far;
}

int try_all_perm_hc_random_restart(int n, int restart_at, int stop_after) {
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

    ++count;

    if (deck[0] == 1) {
      shuffle_no_fix(deck);
      count = 0;
      ping('1');
    }

//     if (count >= restart_at) {
//       count = 0;
//       shuffle_no_fix(deck);
//       ping('.');
//     }

    const int score = do_all_top_swops_hc_copy(deck, 2*n1_score);

    // Heuristic cut-off: if the last number is home, then re-start if
    // the previous perm's best score plus the current score is less
    // than the highest score so far.
    if (deck.back() == n && n1_score + score < highest_count_so_far) {
      count = 0;
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
  int count = 1;
  while (true) {
    if (count % 10000 == 0) cout << "(" << count << ")" << flush;
    ++count;
    for(int i = 0; i < n_vals_size; ++i) {
      const int n = n_vals[i];

      if (n > max_optimal_val) {
        //cout << "(n=" << n-1 << ")" << flush;
        try_all_perm_random_restart(n-1, restart, 1000000000);
        //cout << "(n=" << n << ")" << flush;
        try_all_perm_random_restart(n, restart, 1000000000);
      }
    }
  }
}

void forever_hc() {
  const int restart = 10000000;
  cout << "\nTopswop Random Restart Permuter, all n" 
       << ", restart = " << restart << endl;
  int count = 1;
  while (true) {
    if (count % 10000 == 0) cout << "(" << count << ")" << flush;
    ++count;
    for(int i = 0; i < n_vals_size; ++i) {
      const int n = n_vals[i];

      if (n > max_optimal_val) {
        //cout << "(n=" << n-1 << ")" << flush;
        try_all_perm_hc_random_restart(n-1, restart, 1000000000);
        //cout << "(n=" << n << ")" << flush;
        try_all_perm_hc_random_restart(n, restart, 1000000000);
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
   try_all_perm_hc_random_restart(n, restart, 15); // 1000000000);
 }

int main() {
  forever();
  //test_one();
}
