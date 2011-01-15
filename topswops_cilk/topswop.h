// topswop.h

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#ifndef TOP_SWOP_H
#define TOP_SWOP_H 201012L

#include "util.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <exception>
#include <algorithm>

using namespace std;

typedef vector<int> perm;
typedef const auto_ptr<perm> perm_ptr;

// Note: Optimal values for 2, 3, 5, 7, 11, 13, 17 have been found, so
// need to do any computation on those.
const int max_optimal_val = 17;
const int n_vals_size = 25;
const int n_vals[n_vals_size] = { 
  2,  3,  5,  7, 11, 
  13, 17, 19, 23, 29, 
  31, 37, 41, 43, 47, 
  53, 59, 61, 67, 71,
  73, 79, 83, 89, 97
};

const string dbname = "best_so_far.db";

// print the current stack contents; used for debugging
void print_stack(const vector<perm*>& stack) {
  if (stack.empty()) cout << "   <stack empty>" << endl;
  else if (stack.size() == 1) cout << "   0: " << *stack[0] << endl;
  else {
    cout << "   0: " << *stack[0];
    for(int i = 1; i < stack.size(); ++i) 
      cout << "\n   " << i << ": " << *stack[i];
    cout << endl;
  }
}

const double good_pct = 0.999;
const bool good_logging = false;

inline string good_file(int n) {
  return "good/" + int_to_str(n) + ".log";
}

// The STL reverse is pretty good: a by-hand reverse function was
// slightly slower.
int do_all_top_swops(perm& deck) {
  int count = 0;
  while (deck[0] != 1) {
    reverse(deck.begin(), deck.begin() + deck[0]);  // do 1 top-swop
    ++count;
  }
  return count;
}


int do_all_top_swops_hc(perm& deck, int n1_score) {
  const int n = deck.size();
  int count = 0;
  while (deck[0] != 1) {
    if (deck.back() == n) return count + n1_score; 
    reverse(deck.begin(), deck.begin() + deck[0]);  // do 1 top-swop
    ++count;
  }
  return count;
}

inline int do_all_top_swops_copy(perm deck) {
  return do_all_top_swops(deck);
}

inline int do_all_top_swops_hc_copy(perm deck, int n1_score) {
  return do_all_top_swops_hc(deck, n1_score);
}

void fix_shuffle(perm& v) {
  if (v[0] == 1) {
    for(int i = 1; i < v.size(); ++i) {
      if (v[i] != i) {
        swap(v[0], v[i]);
        return;
      }
    }
  }
}

inline void shuffle(perm& v) {
  random_shuffle(v.begin(), v.end());
  fix_shuffle(v);
}

inline void shuffle_no_fix(perm& v) {
  random_shuffle(v.begin(), v.end());
}

// returns a pointer to a new vector<int> initialized to 1, 2, ..., n
perm* range(int n) {
  perm* v = new perm(n);
  int i = 1;
  for(perm::iterator p = v->begin(); p < v->end(); ++p) {
    *p = i;
    ++i;
  }
  return v;
}

bool is_perm(perm v) {
  sort(v.begin(), v.end());
  for(int i = 0; i < v.size(); ++i) {
    if (v[i] != i + 1)
      return false;
  }
  return true;
}

void init_deck(perm& deck) {
  srand(time(NULL));
  for(int i = 0; i < deck.size(); ++i) {
    deck[i] = i+1;
  }
}

// convert a comma-separated n-permutation string to a perm
perm* convert_to_perm(int n, string s) {
    // replace every ',' with a space
    for(int i = 0; i < s.size(); ++i) {
      if (s[i] == ',') s[i] = ' ';
    }
    perm* p = new perm(n);
    istringstream iss(s);
    int num = -1;
    int i = 0;
    while (iss >> num) {
      (*p)[i] = num;
      ++i;
    }
    
    return p;
}

// Returns count of how many items in *p are in their home position.
int count_home(perm* p, int start_index = 0) {
  int i = start_index + 1;
  int count = 0;
  for(perm::iterator vp = p->begin() + start_index; vp < p->end(); ++vp) {
    if (*vp == i) ++count;
    ++i;
  }
  return count;
}

// Starting at index 1, returns count of how many items in *p are in
// their home position.
inline int count_home_skip_first(perm* p) {
  return count_home(p, 1);
}

int has_some_home_num(perm* p, int start_index = 0) {
  int i = start_index + 1;
  for(perm::iterator vp = p->begin() + start_index; vp < p->end(); ++vp) {
    if (*vp == i) return true;
    ++i;
  }
  return false;
}

inline int has_some_home_num_skip_first(perm* p) {
  return has_some_home_num(p, 1);
}




#endif
