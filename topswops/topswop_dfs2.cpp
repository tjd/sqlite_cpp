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
  set<perm> good_perms;
  int prev_good_perms_size = 0;
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
    } else if (good_logging && good_pct * overall_best_score_for_n <= ps_score) {
      //      good_perms.insert(good_perms.begin(), ps.get_perm());
      //      if (good_perms.size() > prev_good_perms_size) {
      prev_good_perms_size = good_perms.size();
      ofstream fout(good_file(n).c_str(), ios::app);
      fout << ps.get_perm() << '\n';
      ping('=');
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

void back_dfs(const perm& raw_perm, int stop_after = -1) {
  const int n = raw_perm.size();
  perm p(raw_perm);
  int count = 0;
  while (p[0] != 1) {
    ++count;
    if (stop_after != -1 && count >= stop_after) return;
    const int first = p[0];
    reverse(p.begin(), p.begin() + first);
    cout << "(n=" << n << ";" << count << ")" << flush;
    dfs(p, -1);
  }
}

void back_dfs(int n = 97) {
  back_dfs(*get_current_perm(n));
}


// read perms from the "good" log files and try back_dfs on them
void test8() {
  const int n = 97;
  ifstream ifs(good_file(n).c_str());
  string line;
  int count = 0;
  while (getline(ifs, line)) {
    perm_ptr p(convert_to_perm(n, line));
    cout << "\n[" << count << "]" << endl;
    back_dfs(*p, 100);
    ++count;
  }
  cout << count << " good permutations read\n";
}

// read perms from the "good" log files and try back_dfs on them
void test_good_perm(int n) {
  ifstream ifs(good_file(n).c_str());
  string line;
  int count = 0;
  while (getline(ifs, line)) {
    perm_ptr p(convert_to_perm(n, line));
    cout << "\n[n=" << n << ";" << count << "]" << endl;
    back_dfs(*p, 50);
    ++count;
  }
  cout << count << " good permutations read\n";
}

int main() {
  //  test8();
  for(int n = 23; n < 98; ++n) {
    test_good_perm(n);
  }
}

/*
int main(int argc, char* argv[]) {
  int n = 97;
  if (argc > 1) {
    n = atoi(argv[1]);
    if (n < 1 || n > 97) 
      error("n must be in range [2, 97]");
    back_dfs(n);
  }
}
*/
