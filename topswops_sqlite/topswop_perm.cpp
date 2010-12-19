// topswop_perm.cpp

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#define NDEBUG
#include "topswop.h"

const string dbname = "best_so_far.db";

int add_kids(perm* v, vector<perm*>& stack) {
  assert(is_perm(*v));
#ifndef NDEBUG
  perm vc(*v);
#endif
  int count = 0;
  int i = 1;
  for(perm::iterator vp = v->begin(); vp < v->end(); ++vp) {
    if (*vp == i && i != 1) {
      perm* next_perm = new perm(*v);
      assert(is_perm(*next_perm));
      reverse(next_perm->begin(), next_perm->begin() + *vp);
      assert(is_perm(*next_perm));
      stack.push_back(next_perm);
      ++count;
    }
    ++i;
  }
  assert(count == count_home_skip_first(&vc));
  return count;
}

// generate a new permutation by searching backwards from p 
//
// TODO: probably has memory leaks; indeed failed with a std:bad_alloc
// on one run
// TODO: add a cut-off to abort the search after certain number of tries
// TODO: decrease file I/O by passing in best n-val so far
bool search_back(perm p) {
  assert(is_perm(p));
  const int n = p.size();
  const int p_score = do_all_top_swops_copy(p);
  perm best(p);
  int best_score = p_score;
  vector<perm*> stack;
  stack.push_back(&p);
  while (!stack.empty()) {
    perm* curr_perm = stack.back();
    stack.pop_back();

    assert(is_perm(*curr_perm));
    int score = do_all_top_swops_copy(*curr_perm);
    if (score > best_score) {
      int diff = score - best_score;
      best = *curr_perm;
      best_score = score;
      if (set_current_perm(n, best)) {
        cout << "\nn = " << n << " improvement of " << diff << endl;
        cout << "improved score = " << best_score << endl;
        cout << " improved perm = " << best << endl;
      }
    }
    add_kids(curr_perm, stack);
    assert(is_perm(*curr_perm));
    if (curr_perm != &p)
      delete curr_perm;
  } // while
  return false;
}

void improve_current_best_perms() {
  for(int i = 2; i < 98; ++i) {
    const int n = i; 
    perm* p = get_current_perm(n);
    search_back(*p);
    delete p;
  }
}

// Each score from the 2nd onwards should be greater than the one just
// before it. If not, construct a new permutation consisting of the
// (higher scoring) previous one with n appended to the end.
void ensure_increasing_scores() {
  int score_prev = get_current_score(2);
  for(int n = 3; n < 98; ++n) {
    int score = get_current_score(n);
    if (score < score_prev) {
      perm* perm_prev = get_current_perm(n-1);  // get the previous perm
      perm perm_n = *perm_prev;                 // copy it
      perm_n.push_back(n);                      // append n
      search_back(perm_n);                      // any more improvement?
      set_current_perm(n, perm_n);              // save to DB
      score_prev = get_current_score(n);
      delete perm_prev;
    } else {
      score_prev = score;
    }
  } // for
  improve_current_best_perms();
}

int try_all_perm_random_restart(int n, int restart_at, int stop_after) {
  int curr_best = get_current_score(n, dbname);
  //cout << "curr_best = " << curr_best << endl;
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
    //cout << "score(" << deck << ") = " << count << endl;

    if (count > highest_count_so_far) {
      highest_count_so_far = count;
      best = deck;
      //      cout << "highest_count_so_far = " << highest_count_so_far << endl;
      //      cout << best << endl;
      if (highest_count_so_far > curr_best) {
        cout << '!' << flush;
        int diff = highest_count_so_far - curr_best;
        curr_best = highest_count_so_far;
//         cout << "\nn = " << n << ", score = " << curr_best 
//              << " (" << diff << " improvement)" << endl
//              << best << endl;
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
