// topswop.h

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm


#ifndef TOP_SWOP_H
#define TOP_SWOP_H 201012L

#include "util.h"
#include "/usr/include/soci/soci.h"
#include "/usr/include/soci/soci-sqlite3.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <exception>
#include <algorithm>

using namespace soci;
using namespace std;

typedef vector<int> perm;

// Note: Optimal values for 2, 3, 5, 7, 11, 13 have been found, so
// need to do any computation on those.
const int max_optimal_val = 13;
const int n_vals_size = 25;
const int n_vals[n_vals_size] = { 
  2,  3,  5,  7, 11, 
  13, 17, 19, 23, 29, 
  31, 37, 41, 43, 47, 
  53, 59, 61, 67, 71,
  73, 79, 83, 89, 97
};

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

inline int do_all_top_swops_copy(perm deck) {
  return do_all_top_swops(deck);
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


int get_current_score(int n, const string& dbname = "best_so_far.db") {
  try {
    session sql(sqlite3, dbname);
    int score = -1;
    sql << "SELECT score FROM topswops WHERE n = " << n, into(score);
    return score;
  } catch (exception const &e) {
    cerr << "Error: " << e.what() << '\n';
  }
}

perm* get_current_perm(int n, const string& dbname = "best_so_far.db") {
  try {
    session sql(sqlite3, dbname);
    string s;
    sql << "SELECT perm FROM topswops WHERE n = " << n, into(s);

    // replace every ',' with a space
    for(int i = 0; i < s.size(); ++i) {
      if (s[i] == ',') s[i] = ' ';
    }
    
    // convert all the numbers on s to ints
    perm* p = new perm(n);
    istringstream iss(s);
    int num = -1;
    int i = 0;
    while (iss >> num) {
      (*p)[i] = num;
      ++i;
    }
    
    return p;
  } catch (exception const &e) {
    cerr << "Error: " << e.what() << '\n';
  }
}

bool set_current_perm(int n, const perm& v, 
                      const string& dbname = "best_so_far.db") {
  //cout << "set_current_perm called ..." << endl;
  bool result;
  try {
    assert(n == v.size());
    assert(is_perm(v));
    session sql(sqlite3, dbname);
    sql << "begin exclusive transaction";
    //    const int curr_best_score = get_current_score(n, dbname);
    int curr_best_score;
    sql << "SELECT score FROM topswops WHERE n = " << n, into(curr_best_score);
    const int curr_score = do_all_top_swops_copy(v);
    if (curr_score > curr_best_score) {
      cout << "n = " << n << " improvement = " 
           << (curr_score - curr_best_score) << ", " << dbname << endl;
      cout << "score = " << curr_score << endl;
      cout << "perm = " << v << endl << endl;
      
      int score = -1;
      sql << "insert or replace into topswops(n, score, perm, date) "
          << "values(" << n << ", " 
          << curr_score << ", '" 
          << v << "', date('now'))";
      result = true;
    }
    result = false;
    sql << "commit transaction";
  } catch (exception const &e) {
    cerr << "Error: " << e.what() << '\n';
    result = false;
  }
  return result;
}


/*

// check if this is better the currently saved solution for n
void check_score(int n, int new_score, const perm& new_perm) {
  int curr = get_current_score(n);
  if (new_score > curr) {
    set_current_score(n, new_score, new_perm);
  }
}
*/

#endif
