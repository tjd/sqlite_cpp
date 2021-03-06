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

int get_current_score(int n, const string& dbname);

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

void ensure_increasing_scores();

int get_current_score(int n, const string& dbname = "best_so_far.db") {
  try {
    session sql(sqlite3, dbname);
    int score = -1;
    sql << "SELECT score FROM topswops WHERE n = " << n, into(score);
    return score;
  } catch (exception const &e) {
    cerr << "Error accessing DB in get_current_score: " << e.what() << '\n';
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

perm* get_current_perm(int n, const string& dbname = "best_so_far.db") {
  try {
    session sql(sqlite3, dbname);
    string s;
    sql << "SELECT perm FROM topswops WHERE n = " << n, into(s);

    return convert_to_perm(n, s);
  } catch (exception const &e) {
    cerr << "Error accessing DB in get_current_perm: " << e.what() << '\n';
  }
}

bool set_current_perm_no_improve(int n, const perm& v, 
                                 const string& dbname = "best_so_far.db") {
  bool result = false;
  try {
    assert(n == v.size());
    assert(is_perm(v));
    session sql(sqlite3, dbname);
    int best_score;
    sql << "SELECT score FROM topswops WHERE n = " << n, into(best_score);
    const int curr_score = do_all_top_swops_copy(v);
    if (curr_score > best_score) {
      cout << "n = " << n << ": improvement = " 
           << (curr_score - best_score) << ", " << dbname << endl;
      cout << "score = " << curr_score << endl;
      cout << "perm = " << v << endl << endl;

      ofstream ofs("best.log", ios::app);
      ofs << v << endl;
      
      int score = -1;
      sql << "insert or replace into topswops(n, score, perm, date) "
          << "values(" 
          << n << ", " 
          << curr_score << ", '" 
          << v << "', date('now'))";

      result = true;
    } 
  } catch (exception const &e) {
    cerr << "Error accessing DB in set_current_perm_no_improve: " 
         << e.what() << '\n';
    result = false;
  }
  return result;
}

void improve_current();

bool set_current_perm(int n, const perm& v, 
                      const string& dbname = "best_so_far.db") {
  bool result = set_current_perm_no_improve(n, v, dbname);
  if (result) {
    ensure_increasing_scores();
    //improve_current();
  }
  return result;
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

// Add all successors of v to stack.
int add_kids(perm* v, vector<perm*>& stack) {
  assert(is_perm(*v));
#ifndef NDEBUG
  perm vc(*v);
#endif
  int count = 0;
  int i = 2;
  for(perm::iterator vp = v->begin()+1; vp < v->end(); ++vp) {
    if (*vp == i) {
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

// Generate a new permutation by searching backwards from p.
//
// TODO: probably has memory leaks; indeed failed with a std:bad_alloc
// on one run
// TODO: add a cut-off to abort the search after certain number of tries
// TODO: decrease file I/O by passing in best n-val so far
// TODO: avoid new/delete by using pre-defined static storage?
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

bool search_back_heuristic_cutoff(perm p, int give_up_after, 
                                  const string& dbname = "best_so_far.db") {
  assert(is_perm(p));
  const int n = p.size();
  int n1_score = get_current_score(n-1, dbname);
  const int p_score = do_all_top_swops_copy(p);
  perm best(p);
  int best_score = p_score;
  vector<perm*> stack;
  stack.push_back(&p);
  int iter = 0;
  while (!stack.empty()) {
    perm* curr_perm = stack.back();
    stack.pop_back();

    ++iter;
    if (iter > give_up_after) {
      delete curr_perm;
      for(int i = 0; i < stack.size(); ++i) {
        delete stack[i];
      }
      return false;
    }

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

    // heuristic cut-off check
    if (p.back() == n && n1_score + score < best_score) {
      // don't add descendants
    } else {
      add_kids(curr_perm, stack);
    }
    assert(is_perm(*curr_perm));
    if (curr_perm != &p)
      delete curr_perm;
  } // while
  return false;
}

void improve_current_best_perms() {
  for(int i = 2; i < 98; ++i) {
    const int n = i; 
    perm_ptr p(get_current_perm(n));
    search_back(*p);
  }
}

// Each score from the 2nd onwards should be greater than the one just
// before it. If not, construct a new permutation consisting of the
// reverse of the (higher scoring) previous one with n inserted on the
// front.
void ensure_increasing_scores() {
  int score_prev = get_current_score(2);
  for(int n = 3; n < 98; ++n) {
    int score = get_current_score(n);
    if (score <= score_prev) {
      perm_ptr perm_prev(get_current_perm(n-1));// get the previous perm
      perm perm_n = *perm_prev;                 // copy it
      reverse(perm_n.begin(), perm_n.end());    // reverse
      perm_n.insert(perm_n.begin(), n);         // insert n at beginning
      search_back(perm_n);                      // any more improvement?
      set_current_perm_no_improve(n, perm_n);   // save to DB
      score_prev = get_current_score(n);
    } else {
      score_prev = score;
    }
  } // for
  //improve_current_best_perms();
}

// permute all triples of p and keep the one with the best improvement
bool next_perm_all_pair_perm3(perm& p) {
  assert(is_perm(p));
  const int n = p.size();
  int i_best = -1;
  int j_best = -1;
  int k_best = -1;
  int best_score = do_all_top_swops_copy(p);
  perm best_perm(p);
  for(int i = 0; i < n; ++i) {
    for(int j = i + 1; j < n; ++j) {
      for(int k = j + 1; k < n; ++k) {
        perm cpy(p);
        rotate_left(cpy[i], cpy[j], cpy[k]);
        search_back(cpy);
        assert(is_perm(cpy));
        int score = do_all_top_swops_copy(cpy);
        if (score > best_score) {
          i_best = i;
          j_best = j;
          k_best = k;
          best_score = score;
          best_perm = cpy;
          ping('?');
        }
        rotate_left(cpy[i], cpy[j], cpy[k]);
        search_back(cpy);
        assert(is_perm(cpy));
        score = do_all_top_swops_copy(cpy);
        if (score > best_score) {
          i_best = i;
          j_best = j;
          k_best = k;
          best_score = score;
          best_perm = cpy;
          ping('?');
        }
        swap(cpy[i], cpy[j]);
        search_back(cpy);
        assert(is_perm(cpy));
        score = do_all_top_swops_copy(cpy);
        if (score > best_score) {
          i_best = i;
          j_best = j;
          k_best = k;
          best_score = score;
          best_perm = cpy;
          ping('?');
        }
        rotate_left(cpy[i], cpy[j], cpy[k]);
        search_back(cpy);
        assert(is_perm(cpy));
        score = do_all_top_swops_copy(cpy);
        if (score > best_score) {
          i_best = i;
          j_best = j;
          k_best = k;
          best_score = score;
          best_perm = cpy;
          ping('?');
        }
        rotate_left(cpy[i], cpy[j], cpy[k]);
        search_back(cpy);
        assert(is_perm(cpy));
        score = do_all_top_swops_copy(cpy);
        if (score > best_score) {
          i_best = i;
          j_best = j;
          k_best = k;
          best_score = score;
          best_perm = cpy;
          ping('?');
        }      
      }
    }
  }
  
  if (i_best == -1) {
    return false;
  } else {
    p = best_perm;
    assert(is_perm(p));
    return true;
  } 
}

// rotate all (p[i-i], p[i]) and keep the one with the best improvement
bool next_perm_all_pair_rot(perm& p) {
  const int n = p.size();
  int i_best = -1;
  int j_best = -1;
  int k_best = -1;
  int best_score = do_all_top_swops_copy(p);
  for(int i = 0; i < n; ++i) {
    for(int j = i + 1; j < n; ++j) {
      for(int k = i; k < j; ++k) {
        perm cpy(p);
        rotate(cpy.begin() + i, cpy.begin() + k, cpy.begin() + j);
        search_back(cpy);
        const int score = do_all_top_swops_copy(cpy);
        if (score > best_score) {
          i_best = i;
          j_best = j;
          k_best = k;
          best_score = score;
        }
      }
    }
  }
  if (i_best == -1) {
    //    ping('-');
    return false;
  } else {
    rotate(p.begin() + i_best, p.begin() + k_best, p.begin() + j_best);
    //    ping('+');
    return true;
  } 
}

// reverse all (p[i-i], p[i]) and keep the one with the best improvement
bool next_perm_all_pair_rev(perm& p) {
  const int n = p.size();
  int i_best = -1;
  int j_best = -1;
  int best_score = do_all_top_swops_copy(p);
  for(int i = 0; i < n; ++i) {
    for(int j = i + 1; j < n; ++j) {
      reverse(p.begin() + i, p.begin() + j);
      search_back(p);
      const int score = do_all_top_swops_copy(p);
      if (score > best_score) {
        i_best = i;
        j_best = j;
        best_score = score;
        //        search_back_heuristic_cutoff(best_perm, 1000);
        ping('?');
      }
      reverse(p.begin() + i, p.begin() + j);
    }
  }
  if (i_best == -1) {
    return false;
  } else {
    reverse(p.begin() + i_best, p.begin() + j_best);
    return true;
  } 
}

// swap all (p[i-i], p[i]) and keep the one with the best improvement
bool next_perm_all_pair_swap(perm& p) {
  const int n = p.size();
  int i_best = -1;
  int j_best = -1;
  int best_score = do_all_top_swops_copy(p);
  for(int i = 0; i < n; ++i) {
    for(int j = i + 1; j < n; ++j) {
      swap(p[i], p[j]);
      search_back(p);
      const int score = do_all_top_swops_copy(p);
      if (score > best_score) {
        i_best = i;
        j_best = j;
        best_score = score;
      }
      swap(p[i], p[j]);
    }
  }
  if (i_best == -1) 
    return false;
  else {
    swap(p[i_best], p[j_best]);
    return true;
  } 
}

// swap all (p[i-i], p[i]) and keep the one with the best improvement
bool next_perm_seq_pair(perm& p) {
  const int n = p.size();
  int i_best = -1;
  int best_score = do_all_top_swops_copy(p);
  for(int i = 1; i < n; ++i) {
    swap(p[i-1], p[i]);
    const int score = do_all_top_swops_copy(p);
    if (score > best_score) {
      i_best = i;
      best_score = score;
    }
    swap(p[i-1], p[i]);
  }
  if (i_best == -1) 
    return false;
  else {
    swap(p[i_best - 1], p[i_best]);
    return true;
  } 
}

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

void improve_current_all_pair_rot() {
  cout << "improve_current_all_pair_rot() ..." << endl;
  //#pragma omp parallel for schedule(static)
  for(int n = 14; n < 98; ++n) {
    ping_n(n);
    perm_ptr p(get_current_perm(n));
    perm p_orig(*p);
    bool improved = false;
    while (next_perm_all_pair_rot(*p)) improved = true;
    if (improved) {
      int diff = do_all_top_swops_copy(*p) - do_all_top_swops_copy(p_orig);
      cout << "Improvement of " << diff << " for n = " << n << endl;
      set_current_perm(n, *p);
    }
  }
  cout << "... improve_current_all_pair_rot() done." << endl;
}

void improve_current_all_pair_perm3() {
  cout << "improve_current_all_pair_perm3() ..." << endl;
  //#pragma omp parallel for schedule(static)
  for(int n = 14; n < 98; ++n) {
    ping_n(n);
    perm_ptr p(get_current_perm(n));
    perm p_orig(*p);
    bool improved = false;
    while (next_perm_all_pair_perm3(*p)) improved = true;
    if (improved) {
      int diff = do_all_top_swops_copy(*p) - do_all_top_swops_copy(p_orig);
      cout << "Improvement of " << diff << " for n = " << n << endl;
      set_current_perm(n, *p);
    }
  }
  cout << "... improve_current_all_pair_perm3() done." << endl;
}

void improve_current_all_pair_rev() {
  cout << "improve_current_all_pair_rev() ..." << endl;
  //#pragma omp parallel for schedule(dynamic, 10)
  for(int n = 14; n < 98; ++n) {
    ping_n(n);
    perm_ptr p(get_current_perm(n));
    perm p_orig(*p);
    bool improved = false;
    while (next_perm_all_pair_rev(*p)) improved = true;
    if (improved) {
      int diff = do_all_top_swops_copy(*p) - do_all_top_swops_copy(p_orig);
      cout << "Improvement of " << diff << " for n = " << n << endl;
      set_current_perm(n, *p);
    }
  }
  cout << "... improve_current_all_pair_rev() done." << endl;
}

void improve_current_all_pair_swap() {
  cout << "improve_current_all_pair_swap() ..." << endl;
  //#pragma omp parallel for schedule(dynamic, 10)
  for(int n = 14; n < 98; ++n) {
    ping_n(n);
    perm_ptr p(get_current_perm(n));
    perm p_orig(*p);
    bool improved = false;
    while (next_perm_all_pair_swap(*p)) improved = true;
    if (improved) {
      int diff = do_all_top_swops_copy(*p) - do_all_top_swops_copy(p_orig);
      cout << "Improvement of " << diff << " for n = " << n << endl;
      set_current_perm(n, *p);
    }
  }
  cout << "... improve_current_all_pair_swap() done." << endl;
}

void improve_current() {
  cout << "improve_current() ..." << endl;
  #pragma omp parallel sections
  {
     improve_current_all_pair_rot();
     #pragma omp section
     improve_current_all_pair_perm3();
     #pragma omp section
     improve_current_all_pair_swap();
     #pragma omp section
     improve_current_all_pair_rev();   
  }
  cout << "... improve_current() done." << endl;
}

#endif
