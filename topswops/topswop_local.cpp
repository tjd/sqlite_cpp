// topswop_local.cpp

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#define NDEBUG
#include "topswop.h"

const bool show_n = true;

const string dbname = "best_so_far.db";

class perm_score {
public:
  perm_score(const perm& pp, int d_depth) : 
    p(pp), score(do_all_top_swops_copy(p)), depth(d_depth) {}

  perm get_perm() const {return p;}
  int get_score() const {return score;}
  int get_depth() const {return depth;}
private:
  perm p;
  int score;
  int depth;
};

inline bool cmp_perm_score(const perm_score& a, const perm_score& b) {
  return a.get_score() < b.get_score();
}

inline void swap_rand_pair(perm& p) {
  swap(p[randint(p.size())], p[randint(p.size())]);
}

// assume perm[0] == 1
// no depth limit if max_depth == -1
perm_score dfs(const perm& root, int max_depth) {
  assert(perm[0] == 1);
  const int n = root.size();
  int overall_best_for_n = get_current_score(n, dbname);
  vector<perm_score> stack;
  perm_score best(root, 0);
  stack.push_back(best);
  while (!stack.empty()) {
    perm_score ps = stack.back();
    stack.pop_back();
    const perm p = ps.get_perm();
    const int new_depth = ps.get_depth() + 1;
    if (ps.get_score() > best.get_score()) {
      best = ps;
      cout << "n=" << n << ", best (" << best.get_score() 
           <<  "/" << overall_best_for_n << "): " 
           << best.get_perm() 
           << endl;
      set_current_perm(n, best.get_perm());
    }

    // add p's children to the stack
    if (new_depth < max_depth || max_depth == -1) {
      for(int i = 1; i < n; ++i) {
        if (p[i] == i + 1) { // does p[i] holds its home value?
          perm cpy(p);
          reverse(cpy.begin(), cpy.begin() + p[i]);
          stack.push_back(perm_score(cpy, new_depth));
        } // if
      } // for
    } // if
  } // while
  return best;
}


perm_score dfs_rand(const perm& root, int max_depth) {
  assert(perm[0] == 1);
  const int n = root.size();
  int overall_best_for_n = get_current_score(n, dbname);
  vector<perm_score> stack;
  perm_score best(root, 0);
  stack.push_back(best);
  int count = 0;
  while (!stack.empty()) {
    ++count;
    if (count > 100000000) {
      random_shuffle(stack.begin(), stack.end());
      count = 0;
      ping('!');
    }
    perm_score ps = stack.back();
    stack.pop_back();
    const perm p = ps.get_perm();
    const int new_depth = ps.get_depth() + 1;
    if (ps.get_score() > best.get_score()) {
      best = ps;
      cout << "n=" << n << ", best (" << best.get_score() 
           <<  "/" << overall_best_for_n << "): " 
           << best.get_perm() 
           << endl;
      set_current_perm(n, best.get_perm());
    }

    // add p's children to the stack
    if (new_depth < max_depth || max_depth == -1) {
      for(int i = 1; i < n; ++i) {
        if (p[i] == i + 1) { // does p[i] holds its home value?
          perm cpy(p);
          reverse(cpy.begin(), cpy.begin() + p[i]);
          stack.push_back(perm_score(cpy, new_depth));
        } // if
      } // for
    } // if
  } // while
  return best;
}

int main() {  
  perm root(*range(97));
  dfs_rand(root, -1);
  /*
  for(int i = 0; i < 16; ++i) {
    cout << "i=" << i << endl;
    perm root(*get_current_perm(15));
    root.insert(root.begin()+i, 16);
    dfs(root, -1);
  }
  */
}
