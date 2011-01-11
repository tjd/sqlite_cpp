// topswop_dfs2.cpp

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#define NDEBUG
#include "topswop.h"

const bool show_n = false;

const string dbname = "best_so_far.db";

class perm_score {
public:
  perm_score(const perm& pp):  
    p(pp), score(do_all_top_swops_copy(p)) {}

  perm_score(const perm& pp, int s_score):
    p(pp), score(s_score) {}

  perm get_perm() const {return p;}
  int get_score() const {return score;}

private:
  perm p;
  int score;
};

inline bool cmp_perm_score(const perm_score& a, const perm_score& b) {
  return a.get_score() < b.get_score();
}


// Assumes perm[0] == 1 and that the 
// no depth limit if cutoff_score == -1
perm_score dfs(const perm& root, int cutoff_score) {
  assert(perm[0] == 1);
  const int n = root.size();
  int overall_best_score_for_n = get_current_score(n, dbname);
  vector<perm_score> stack;
  perm_score best(root);
  stack.push_back(best);
  int count = 0;
  while (!stack.empty()) {
    ++count;
    perm_score ps = stack.back();
    stack.pop_back();
    const perm p = ps.get_perm();
    const int ps_score = ps.get_score();
    const int new_score = ps_score + 1;
    if (ps_score > best.get_score()) {
      best = ps;
      const int best_score = best.get_score();
      if (show_n)
        cout << "n=" << n << ", best (" << best_score
             <<  "/" << overall_best_score_for_n << "): " 
             << best.get_perm() 
             << endl;
      if (best_score > overall_best_score_for_n) {
        set_current_perm(n, best.get_perm());
        overall_best_score_for_n = best_score;
      }
    }

    // add p's children to the stack
    if (new_score < cutoff_score || cutoff_score == -1) {
      for(int i = 1; i < n; ++i) {
        const int pi = p[i];
        if (pi == i + 1) { // does p[i] holds its home value?
          perm cpy(p);
          reverse(cpy.begin(), cpy.begin() + pi);
          stack.push_back(perm_score(cpy, new_score));
        } // if
      } // for
    } // if
  } // while
  return best;
}

/*
void test1() { 
  const int n = 97;
  perm best(*get_current_perm(n));
  perm root(best);
  for(int i = 0; i < n; ++i) {
    for(int j = i + 1; j < n; ++j) {
      ping('.');   
      reverse(root.begin() + i, root.begin() + j);
      dfs(root, -1);  
      reverse(root.begin() + i, root.begin() + j);
    }
  }
}

void test2() { 
  const int n = 97;
  perm best(*get_current_perm(n));
  perm root(best);
  for(int i = 0; i < 10000; ++i) {
    next_permutation(root.begin(), root.end());
    dfs(root, -1);  
    ping('.');   
  }
}

void test3() { 
  const int n = 97;
  perm best(*get_current_perm(n));
  perm root(best);
  for(int i = 0; i < 10000; ++i) {
    ping('.');   
    rotate(root.begin(), root.begin() + 1, root.end());
    for(int j = 0; j < n; ++j) {
      for(int k = j + 2; k < n; ++k) {
        reverse(root.begin() + j, root.begin() + k);
        dfs(root, -1);  
        reverse(root.begin() + j, root.begin() + k);
      }
    }
  }
}

void test4() {
  const int n = 97;
  int count = 0;
  while (true) {
    if (count > 1000000) {
      ping('.');
      count = 0;
    }
    perm root(*range(n));
    random_shuffle(root.begin() + 1, root.end());
    dfs(root, -1);
  }
}

void test5() {
  const int n = 97;
  int count = 0;
  perm root(*range(n));
  int a = 0, b = 0;
  while (a == b) {
    a = randint(1, n);
    b = randint(1, n);
  }
  swap(root[a], root[b]);  
  dfs(root, -1);
}

void test6() {
  const int n = 53;
  perm best(*get_current_perm(n));
  perm root(best);
  do_all_top_swops(root);
  cout << "root (n=" << n << "): " << root << endl;
  dfs(root, -1);
}
*/

// Idea: Start with a known good permutation, and do forward
// topswops. For each new permutation generated in this way, do
// backwards dfs to try to find better scores.
void test7() {
  const int n = 83;
  perm p(*get_current_perm(n));
  int count = 0;
  while (p[0] != 1) {
    ++count;
    const int first = p[0];
    reverse(p.begin(), p.begin() + first);
    cout << "(n=" << n << ";" << count << ")" << flush;
    dfs(p, -1);
  }
}

void back_dfs(int n = 97) {
  perm p(*get_current_perm(n));
  int count = 0;
  while (p[0] != 1) {
    ++count;
    const int first = p[0];
    reverse(p.begin(), p.begin() + first);
    cout << "(n=" << n << ";" << count << ")" << flush;
    dfs(p, -1);
  }
}

int main(int argc, char* argv[]) {
  int n = 97;
  if (argc > 1) {
    n = atoi(argv[1]);
    if (n < 1 || n > 97) 
      error("n must be in range [2, 97]");
    back_dfs(n);
  }
}
